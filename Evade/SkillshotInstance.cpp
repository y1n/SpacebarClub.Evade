#include "SkillshotInstance.h"
#include "Collision.h"

SkillshotInstance::SkillshotInstance(SkillshotData* sdata, Vector const& start, Vector const& end,
	unsigned detection_tick, unsigned end_tick, IGameObject* caster)
{
	this->skillshot_data = new SkillshotData(*sdata);
	this->start_position = start;
	this->end_position = end;
	this->start_tick = detection_tick;
	this->caster = caster;
	this->direction = (end - start).Normalized();

	this->start_position = this->start_position.To3DWorld();
	this->end_position.z = this->start_position.z;

	switch (sdata->skillshot_type)
	{
	case kLine:
	case kMissileLine:
		this->rectangle = new Geometry::Rectangle(start, this->CollisionPosition(), this->Radius());
		break;
	case kCircle:
		this->circle = new Geometry::Circle(this->CollisionPosition(), this->Radius());
		break;
	case kMissileCone:
	case kCone:
		this->sector = new Geometry::Sector(start, this->CollisionPosition() - start, this->skillshot_data->raw_radius * M_PI / 180.0f, sdata->Range());
		break;
	case kArc:
		this->arc = new Geometry::Arc(start, this->CollisionPosition(), this->Radius());
		break;
	case kRing:
		this->ring = new Geometry::Ring(this->CollisionPosition(), this->skillshot_data->raw_radius, this->skillshot_data->radius_ex);
		break;
	default: break;
	}

	if (Evade::spell_configs.find(sdata->MenuName()) != Evade::spell_configs.end())
	{
		this->menu_config = Evade::spell_configs[sdata->MenuName()];
	}

	this->end_tick = this->start_tick + end_tick;
	this->polygon = new Geometry::Polygon;
	this->evading_polygon = new Geometry::Polygon;
	this->drawing_polygon = new Geometry::Polygon;
	this->pathfinding_innerpolygon = new Geometry::Polygon;
	this->pathfinding_outerpolygon = new Geometry::Polygon;

	if (this->IsValid())
	{
		this->UpdatePolygons();
	}
}

SkillshotInstance::~SkillshotInstance()
{
	delete this->skillshot_data;
	delete this->polygon;
	delete this->evading_polygon;
	delete this->drawing_polygon;
	delete this->pathfinding_innerpolygon;
	delete this->pathfinding_outerpolygon;
	delete this->rectangle;
	delete this->ring;
	delete this->sector;
	delete this->arc;
	delete this->circle;
}

unsigned SkillshotInstance::Radius()
{
	unsigned int radius = Config::AdditionalRadius;

	if (!this->skillshot_data->dont_add_bounding_radius)
	{
		radius += g_LocalPlayer->BoundingRadius();
	}

	radius += this->skillshot_data->raw_radius;
	return radius;
}

bool SkillshotInstance::IsValid()
{
	if (this != nullptr && this->skillshot_data != nullptr && this->menu_config != nullptr &&
		this->polygon != nullptr && this->evading_polygon != nullptr && this->drawing_polygon)
	{
		if (this->end_tick + this->skillshot_data->extra_duration <= g_Common->TickCount())
		{
			return false;
		}

		if (!this->skillshot_data->spellbook_spell_name.empty() &&
			g_Common->TickCount() - this->start_tick > this->skillshot_data->delay + 200 + this->start_position.Distance(this->end_position) / this->skillshot_data->missile_speed * 1000)
		{
			if (this->caster && this->caster->IsVisible())
			{
				if (const auto spellbook = this->caster->GetSpellbook())
				{
					if (spellbook->GetSpell(this->skillshot_data->spell_slot)->SData().SpellName != this->skillshot_data->spellbook_spell_name)
					{
						return false;
					}
				}
			}
		}

		switch (this->skillshot_data->skillshot_type)
		{
		case kLine:
		case kMissileLine:  return this->rectangle != nullptr;
		case kCircle:       return this->circle != nullptr;
		case kCone:         return this->sector != nullptr;
		case kMissileCone:         return this->sector != nullptr;
		case kArc:          return this->arc != nullptr;
		case kRing:         return this->ring != nullptr;
		default:            break;
		}
	}

	return false;
}

bool SkillshotInstance::IsEnabled()
{
	if (!Evade::IsEnabled() || this->force_disabled)
	{
		return false;
	}

	if (!this->menu_config->is_enabled->GetBool())
	{
		return false;
	}

	if (!this->menu_config->is_dangerous->GetBool() && Evade::DodgeOnlyDangerous())
	{
		return false;
	}

	if (this->is_from_fow && !Evade::config_menu->GetElement("fow_dodge")->GetBool())
	{
		return false;
	}

	if (this->skillshot_data->skillshot_type == kLine ||
		this->skillshot_data->skillshot_type == kMissileLine)
	{
		if (!Evade::config_menu->GetElement("dodge_linear")->GetBool())
		{
			return false;
		}
	}

	if (this->skillshot_data->skillshot_type == kCircle)
	{
		if (!Evade::config_menu->GetElement("dodge_circular")->GetBool())
		{
			return false;
		}
	}

	return true;
}

bool SkillshotInstance::IsGlobal()
{
	return this->skillshot_data->Range() > 24000;
}

bool SkillshotInstance::IsSafePoint(Vector const& point)
{
	return this->polygon->IsOutside(point);
}

SafePath SkillshotInstance::IsSafePath(std::vector<Vector> const& path, unsigned time_ms, int speed, unsigned delay)
{
	float dist = 0;
	time_ms += g_Common->Ping() / 2;
	speed = speed == -1 ? g_LocalPlayer->MoveSpeed() : speed;

	std::vector<IntersectionInfo> intersections;

	for (size_t i = 0; i <= path.size() - 2; ++i)
	{
		auto const from = path[i];
		auto const to = path[i + 1];
		std::vector<IntersectionInfo> segments;

		for (size_t j = 0; j < this->polygon->Points.size(); ++j)
		{
			auto const segment_start = this->polygon->Points[j];
			auto const segment_end = this->polygon->Points[j == this->polygon->Points.size() - 1 ? 0 : j + 1];
			auto const intersection = from.Intersection(to, segment_start, segment_end);

			if (intersection.Intersects)
			{
				auto d = dist + intersection.Point.Distance(from);
				segments.emplace_back(intersection.Point, d, d / speed * 1000, from);
			}
		}

		dist += from.Distance(to);

		std::sort(segments.begin(), segments.end(), [](IntersectionInfo const& a, IntersectionInfo const& b) { return a.distance < b.distance; });

		for (auto const& segment : segments)
			intersections.push_back(segment);
	}

	if (this->skillshot_data->skillshot_type == kMissileLine || this->skillshot_data->skillshot_type == kMissileCone || this->skillshot_data->skillshot_type == kArc)
	{
		if (this->IsSafePoint(g_LocalPlayer->Position()))
		{
			if (intersections.empty())
			{
				return SafePath({}, true);
			}

			if (this->skillshot_data->dont_cross)
			{
				return SafePath(intersections.front(), false);
			}

			for (size_t i = 0; i < intersections.size(); i = i + 2)
			{
				auto enter_intersection = intersections[i];
				auto enter_intersection_segment = enter_intersection.point.ProjectOn(this->start_position, this->CollisionPosition()).SegmentPoint;

				if (i == intersections.size() - 1)
				{
					return SafePath(intersections.front(),
						this->CollisionPosition().Distance(this->GetMissilePosition(enter_intersection.time - time_ms)) + 50 <=
						this->CollisionPosition().Distance(enter_intersection_segment) &&
						g_LocalPlayer->MoveSpeed() < this->skillshot_data->missile_speed);
				}

				auto exit_intersection = intersections[i + 1];
				auto exit_intersection_segment = exit_intersection.point.ProjectOn(this->start_position, this->CollisionPosition()).SegmentPoint;

				if (this->GetMissilePosition(enter_intersection.time - time_ms).Distance(this->CollisionPosition()) + 50 >
					enter_intersection_segment.Distance(this->CollisionPosition()) && this->GetMissilePosition(exit_intersection.time + time_ms).Distance(this->CollisionPosition()) <=
					exit_intersection_segment.Distance(this->CollisionPosition()))
				{
					return SafePath(intersections.front(), false);
				}
			}

			return SafePath(intersections.front(), true);
		}

		if (intersections.empty())
		{
			return SafePath({}, false);
		}

		auto exit = intersections.front();
		auto exit_segment = exit.point.ProjectOn(this->start_position, this->CollisionPosition()).SegmentPoint;

		if (this->GetMissilePosition(exit.time + time_ms).Distance(this->CollisionPosition()) <= exit_segment.Distance(this->CollisionPosition()))
		{
			return SafePath(exit, false);
		}
	}

	if (intersections.empty())
	{
		return SafePath({}, this->IsSafePoint(g_LocalPlayer->Position()));
	}

	if (this->skillshot_data->dont_cross && this->IsSafePoint(g_LocalPlayer->Position()))
	{
		return SafePath(intersections.front(), false);
	}
		
	auto const end_time = (this->skillshot_data->dont_add_extra_duration ? 0 : this->skillshot_data->extra_duration) + (this->end_tick - this->start_tick) -
		(g_Common->TickCount() - this->start_tick);
	
	auto const position_after = Geometry::Geometry::PositionAfter(path, end_time, speed, delay);

	return SafePath(intersections.front(), this->IsSafePoint(position_after));
}

bool SkillshotInstance::IsSafeToBlink(Vector const& point, unsigned time_ms, unsigned delay)
{
	time_ms /= 2;

	if (this->IsSafePoint(point))
	{
		return true;
	}

	if (this->skillshot_data->skillshot_type == kMissileLine)
	{
		return point.Distance(this->GetMissilePosition(0), this->GetMissilePosition(delay + time_ms), true)
			< this->Radius();
	}

	return this->skillshot_data->extra_duration + (this->end_tick - this->start_tick) -
		(g_Common->TickCount() - this->start_tick) > int(time_ms + delay);
}

bool SkillshotInstance::IsAboutToHit(unsigned time_ms, IGameObject* unit)
{
	if (!unit->IsValid())
	{
		return false;
	}

	if (this->IsSafePoint(unit->ServerPosition()))
	{
		return false;
	}

	if (this->skillshot_data->skillshot_type == kMissileLine)
	{
		return unit->ServerPosition()
			.Distance(this->GetMissilePosition(0), this->GetMissilePosition(time_ms), true) < this->Radius();
	}

	return this->skillshot_data->extra_duration + (this->end_tick - this->start_tick)
		- (g_Common->TickCount() - this->start_tick) <= int(time_ms);
}

void SkillshotInstance::OnTick()
{
	if (this->skillshot_data->collision_flags)
	{
		if (g_Common->TickCount() - this->last_collision_calculation > 50)
		{
			if (this->skillshot_data->spell_name == "LucianR")
			{
				this->last_collision_calculation = g_Common->TickCount();
				this->cached_collision_result = Collision::GetCollisionPoint(this, {}, false);
			}
			else if (Evade::collision_menu->GetElement("enabled")->GetBool())
			{
				this->last_collision_calculation = g_Common->TickCount();
				this->cached_collision_result = Collision::GetCollisionPoint(this);
			}
		}
	}

	if (this->skillshot_data->skillshot_type == kLine || this->skillshot_data->skillshot_type == kMissileLine)
	{
		this->rectangle->RStart = this->GetMissilePosition(0);
		this->rectangle->REnd = this->CollisionPosition();
		this->rectangle->Direction = (this->rectangle->REnd - this->rectangle->RStart).Normalized();
		this->rectangle->Perpendicular = this->rectangle->Direction.Perpendicular();
		this->UpdatePolygons();
	}

	if (this->caster != nullptr && !this->caster->IsVisible())
	{
		return;
	}

	if (this->skillshot_data->missile_follows_unit)
	{
		this->end_position = this->caster->Position();
		this->direction = (this->end_position - this->start_position).Normalized();

		if (this->skillshot_data->skillshot_type == kRing)
		{
			this->ring->Center = this->end_position;
			this->UpdatePolygons();
		}
	}

	if (this->skillshot_data->missile_connected_to_unit)
	{
		this->start_position = this->caster->Position();
		this->end_position = this->start_position + this->direction * this->skillshot_data->Range();

		if (this->skillshot_data->skillshot_type == kLine || this->skillshot_data->skillshot_type == kMissileLine)
		{
			this->rectangle->RStart = this->start_position;
			this->rectangle->REnd = this->end_position;
		}

		this->UpdatePolygons();
	}

	if (this->skillshot_data->spell_name == "GalioR")
	{
		if (!this->caster->HasBuff(hash("GalioIdolOfDurand")))
		{
			if (g_Common->TickCount() - this->start_tick > this->skillshot_data->delay + 300)
			{
				this->end_tick = 0;
			}
		}
	}
}

void SkillshotInstance::UpdatePolygons()
{
	switch (this->skillshot_data->skillshot_type)
	{
	case kLine:
	case kMissileLine:
	{
		this->polygon->Points = this->rectangle->ToPolygon().Points;
		this->evading_polygon->Points = this->rectangle->ToPolygon(Config::AdditionalRadius).Points;
		this->drawing_polygon->Points = this->rectangle->ToPolygon(0, this->skillshot_data->raw_radius).Points;
		this->pathfinding_outerpolygon->Points = this->rectangle->ToPolygon(Config::PathFindingOuterDistance).Points;
		this->pathfinding_innerpolygon->Points = this->rectangle->ToPolygon(Config::PathFindingInnerDistance).Points;
		break;
	}
	case kCircle:
	{
		this->polygon->Points = this->circle->ToPolygon().Points;
		this->evading_polygon->Points = this->circle->ToPolygon(Config::AdditionalRadius).Points;
		this->drawing_polygon->Points = this->circle->ToPolygon(0, this->skillshot_data->raw_radius).Points;
		this->pathfinding_outerpolygon->Points = this->circle->ToPolygon(Config::PathFindingOuterDistance).Points;
		this->pathfinding_innerpolygon->Points = this->circle->ToPolygon(Config::PathFindingInnerDistance).Points;
		break;
	}
	case kMissileCone:
	case kCone:
	{
		this->polygon->Points = this->sector->ToPolygon().Points;
		this->evading_polygon->Points = this->sector->ToPolygon(Config::AdditionalRange).Points;
		this->drawing_polygon->Points = this->polygon->Points;
		this->pathfinding_outerpolygon->Points = this->sector->ToPolygon(Config::PathFindingOuterDistance).Points;
		this->pathfinding_innerpolygon->Points = this->sector->ToPolygon(Config::PathFindingInnerDistance).Points;
		break;
	}
	case kArc:
	{
		this->polygon->Points = this->arc->ToPolygon().Points;
		this->evading_polygon->Points = this->arc->ToPolygon(Config::AdditionalRange).Points;
		this->drawing_polygon->Points = this->polygon->Points;
		this->pathfinding_outerpolygon->Points = this->arc->ToPolygon(Config::PathFindingOuterDistance).Points;
		this->pathfinding_innerpolygon->Points = this->arc->ToPolygon(Config::PathFindingInnerDistance).Points;
		break;
	}
	case kRing:
	{
		this->polygon->Points = this->ring->ToPolygon().Points;
		this->evading_polygon->Points = this->ring->ToPolygon(Config::AdditionalRange).Points;
		this->drawing_polygon->Points = this->polygon->Points;
		this->pathfinding_outerpolygon->Points = this->ring->ToPolygon(Config::PathFindingOuterDistance).Points;
		this->pathfinding_innerpolygon->Points = this->ring->ToPolygon(Config::PathFindingInnerDistance).Points;
		break;
	}
	default: break;
	}
}

void SkillshotInstance::Draw(float thickness)
{
	if (this->menu_config->draw->GetBool())
	{
		auto color = RGBA(201, 204, 202, 255);

		if (this->IsEnabled())
		{
			switch (this->menu_config->danger_level->GetInt())
			{
			case kLow:
				color = Evade::colors_menu->GetElement("low_danger")->GetColor();
				break;
			case kMedium:
				color = Evade::colors_menu->GetElement("medium_danger")->GetColor();
				break;
			case kHigh:
				color = Evade::colors_menu->GetElement("high_danger")->GetColor();
				break;
			case kExtreme:
				color = Evade::colors_menu->GetElement("extreme_danger")->GetColor();
				break;
			}
		}

		if (this->skillshot_data->skillshot_type == kCircle)
		{
			g_Drawing->AddCircle
			(
				this->CollisionPosition().To3DWorld(),
				this->Radius(),
				color,
				thickness + 0.5f,
				20
			);
		}
		else
		{
			size_t const size = this->drawing_polygon->Points.size();

			for (size_t i = 0; i < size; ++i)
			{
				size_t const next_index = size - 1 == i ? 0 : i + 1;

				auto const start = this->drawing_polygon->Points[i];
				auto const end = this->drawing_polygon->Points[next_index];

				g_Drawing->AddLine(start, end, color, thickness);
			}

			if (this->skillshot_data->skillshot_type == kMissileLine)
			{
				g_Drawing->AddLine
				(
					this->rectangle->RStart + this->rectangle->Perpendicular * this->Radius(),
					this->rectangle->RStart - this->rectangle->Perpendicular * this->Radius(),
					this->IsEnabled() ? RGBA(50, 205, 50, 255) : RGBA(201, 204, 202, 255),
					thickness + 0.25f
				);
			}
		}
	}
}

Vector SkillshotInstance::CollisionPosition()
{
	if (this->cached_collision_result.IsValid())
	{
		return this->cached_collision_result;
	}

	if (this->IsGlobal())
	{
		float t = fmax<int, int>(0, g_Common->TickCount() - this->start_tick - this->skillshot_data->delay);
		t = fmax<int, float>(0, fmin<float, float>(this->end_position.Distance(this->start_position), t * this->skillshot_data->missile_speed / 1000.0f));

		auto const position = this->start_position + this->direction * t;
		return position + this->direction * this->skillshot_data->missile_speed * (0.5f + this->skillshot_data->Radius() * 2 / g_LocalPlayer->MoveSpeed());
	}

	return this->end_position;
}

Vector SkillshotInstance::GetMissilePosition(unsigned time_ms)
{
	if (this->skillshot_data->missile_speed == 0)
	{
		return this->start_position;
	}

	float x, t1;
	float const t = fmax<int, int>(0, g_Common->TickCount() + time_ms - this->start_tick - this->skillshot_data->delay);

	if (this->skillshot_data->missile_accel != 0)
	{
		if (this->skillshot_data->missile_accel > 0)
		{
			t1 = this->skillshot_data->missile_max_speed;
		}
		else
		{
			t1 = this->skillshot_data->missile_min_speed - this->skillshot_data->missile_speed;
		}

		t1 *= 1000.f / this->skillshot_data->missile_accel;

		if (t <= t1)
		{
			x = t * this->skillshot_data->missile_speed / 1000.f + 0.5f * this->skillshot_data->missile_accel * pow(t / 1000.f, 2);
		}
		else
		{
			x = t1 * this->skillshot_data->missile_speed / 1000.f + 0.5f * this->skillshot_data->missile_accel * pow(t1 / 1000.f, 2) +
				(t - t1) / 1000.f *	(this->skillshot_data->missile_accel < 0 ? this->skillshot_data->missile_max_speed : this->skillshot_data->missile_min_speed);
		}
	}
	else
	{
		x = t * this->skillshot_data->missile_speed / 1000.f;
	}

	return this->start_position + this->direction * fmax<float, float>(0, fmin<float, float>(this->CollisionPosition().Distance(this->start_position), x));
}