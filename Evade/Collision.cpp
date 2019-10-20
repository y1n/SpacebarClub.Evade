#include "Collision.h"

#include <regex>

#include "../SDK/EventHandler.h"

namespace Collision
{
	struct FastPredictionResult
	{
		bool is_moving;
		Vector current_position;
		Vector predicted_position;

		FastPredictionResult() : is_moving(false) {}
		FastPredictionResult(bool ismoving, Vector const& currentposition, Vector const& predictedposition) : is_moving(ismoving), current_position(currentposition), predicted_position(predictedposition) {}
	};

	int     wall_cast_time = 0;
	bool    is_yasuo_in_team = false;
	Vector  wall_cast_position;
	std::regex yasuo_wall_regex = std::regex("_w_windwall.\\.troy", std::regex_constants::icase);

	FastPredictionResult GetFastPrediction(Vector const& check_from, IGameObject* unit, float delay, int speed);
	void                 ProcessSpell(IGameObject* sender, OnProcessSpellEventArgs* args);

	void Initialize()
	{
		for (auto const& unit : g_ObjectManager->GetChampions())
		{
			if (unit->IsAlly() && unit->ChampionId() == ChampionId::Yasuo)
			{
				is_yasuo_in_team = true;
				break;
			}
		}

		if (is_yasuo_in_team)
			EventHandler<Events::OnProcessSpellCast>::AddEventHandler(ProcessSpell);
	}

	void Unload()
	{
		if (is_yasuo_in_team)
			EventHandler<Events::OnProcessSpellCast>::RemoveEventHandler(ProcessSpell);
	}

	Vector GetCollisionPoint(SkillshotInstance* skillshot, Vector const& check_from, bool check_menu_settings)
	{
		if (!skillshot->IsValid())
		{
			return {};
		}

		skillshot->force_disabled = false;
		auto from = check_from.IsValid() ? check_from : skillshot->GetMissilePosition(0);

		struct DetectedCollision
		{
			float distance;
			float difference;

			Vector position;
			SpellCollisionFlags collision_flags;
			IGameObject* unit;

			DetectedCollision() : distance(0), difference(0), collision_flags(), unit(nullptr) {}
			DetectedCollision(float dist, float diff, Vector const& pos, SpellCollisionFlags flags, IGameObject* unit) : distance(dist), difference(diff), position(pos), collision_flags(flags), unit(unit) {}
		};

		std::vector<DetectedCollision> detected_collisions;

		if (skillshot->skillshot_data->collision_flags & kWall)
		{
			auto const distance = from.Distance(skillshot->end_position);
			auto output = skillshot->end_position;
			auto temp = distance;

			for (int i = distance; i >= 0; i -= 25)
			{
				auto vec = from + (skillshot->end_position - from).Normalized() * i;

				if (vec.IsWall())
				{
					temp = vec.Distance(from);
					output = vec;
				}
			}

			detected_collisions.emplace_back(distance - temp, temp, output, kWall, nullptr);
		}

		if (check_menu_settings && !Evade::collision_menu->GetElement("enabled")->GetBool())
		{
			return {};
		}

		auto const team = skillshot->caster->Team();
		auto const tick_count = g_Common->TickCount();

		if (!skillshot->caster->IsValid())
		{
			goto FALLBACK;
		}

		if (skillshot->skillshot_data->collision_flags & kMinions)
		{
			if (!check_menu_settings || Evade::collision_menu->GetElement("minions")->GetBool())
			{
				for (auto unit : g_ObjectManager->GetMinionsAll())
				{
					if (unit->Team() == team && unit->IsValidTarget(1200, false, from))
					{
						auto prediction = GetFastPrediction(from, unit,
							fmax(0, skillshot->skillshot_data->delay - (tick_count - skillshot->start_tick)), skillshot->skillshot_data->missile_speed);

						auto const projection = prediction.predicted_position.ProjectOn(from, skillshot->end_position);
						if (projection.IsOnSegment)
						{
							auto const w = skillshot->skillshot_data->raw_radius + (!prediction.is_moving ? unit->BoundingRadius() - 15 : 0) -
								prediction.predicted_position.Distance(from, skillshot->end_position, true);

							if (w > 0)
							{
								detected_collisions.emplace_back(w, prediction.predicted_position.Distance(from), projection.LinePoint + skillshot->direction * 30, kMinions, unit);
							}
						}
					}
				}
			}
		}

		if (skillshot->skillshot_data->collision_flags & kHeroes)
		{
			if (!check_menu_settings || Evade::collision_menu->GetElement("champions")->GetBool())
			{
				for (auto unit : g_ObjectManager->GetChampions())
				{
					if (!unit->IsMe() && skillshot->caster != unit && unit->Team() == team && unit->IsValidTarget(1200, false, from))
					{
						auto prediction = GetFastPrediction(from, unit,
							fmax(0, skillshot->skillshot_data->delay - (tick_count - skillshot->start_tick)), skillshot->skillshot_data->missile_speed);

						auto const projection = prediction.predicted_position.ProjectOn(from, skillshot->end_position);
						if (projection.IsOnSegment)
						{
							auto const w = skillshot->skillshot_data->raw_radius + 30 -
								prediction.predicted_position.Distance(from, skillshot->end_position, true);

							if (w > 0)
							{
								detected_collisions.emplace_back(w, prediction.predicted_position.Distance(from), projection.LinePoint + skillshot->direction * 30, kHeroes, unit);
							}
						}
					}
				}
			}
		}

		if (skillshot->skillshot_data->collision_flags & kYasuoWall)
		{
			if (!check_menu_settings || Evade::collision_menu->GetElement("yasuo_wall")->GetBool())
			{
				IGameObject* wall = nullptr;

				for (auto entity : g_ObjectManager->GetByType(EntityType::obj_GeneralParticleEmitter))
				{
					if (entity->Team() == team && std::regex_match(entity->Name().c_str(), yasuo_wall_regex, std::regex_constants::match_flag_type::match_any))
					{
						wall = entity;
						break;
					}
				}

				if (wall != nullptr && wall->IsValid())
				{
					std::vector<Vector> intersections;

					try
					{
						auto const wall_name = wall->Name();
						auto const level = std::stoi(wall_name.substr(wall_name.length() - 6, 1));
						auto const width = 300 + 50 * level;
						auto const direction = (wall->Position() - wall_cast_position).Normalized().Perpendicular();
						auto const start = wall->Position() + direction * (width / 2.0f);
						auto const end = start - direction * width;
						auto const rectangle = Geometry::Rectangle(start, end, 75).ToPolygon(0, 0);

						for (size_t i = 0; i < rectangle.Points.size(); ++i)
						{
							auto const line_start = rectangle.Points[i];
							auto const line_end = rectangle.Points[i != rectangle.Points.size() - 1 ? i + 1 : 0];
							auto const intersection = line_start.Intersection(line_end, from, skillshot->end_position);

							if (intersection.Intersects)
								intersections.push_back(intersection.Point);
						}

						if (!intersections.empty())
						{
							std::sort(intersections.begin(), intersections.end(), [&from](Vector const& a, Vector const& b)
							{
								return a.DistanceSquared(from) < b.DistanceSquared(from);
							});

							auto closest_intersection = intersections.front();
							auto t = tick_count + fmax(0, skillshot->skillshot_data->delay - (tick_count - skillshot->start_tick)) + 100 +
								1000 * closest_intersection.Distance(from) / skillshot->skillshot_data->missile_speed;

							if (t - wall_cast_time < 4000)
							{
								if (skillshot->skillshot_data->skillshot_type == kMissileLine)
								{
									skillshot->force_disabled = true;
								}

								detected_collisions.emplace_back(t, closest_intersection.Distance(from), closest_intersection, kYasuoWall, nullptr);
							}
						}
					}
					catch (std::exception const&) {}
				}
			}
		}

	FALLBACK:

		if (!detected_collisions.empty())
		{
			std::sort(detected_collisions.begin(), detected_collisions.end(), [](DetectedCollision const& a, DetectedCollision const& b)
			{
				return a.distance < b.distance;
			});

			return detected_collisions.front().position;
		}

		return {};
	}

	FastPredictionResult GetFastPrediction(Vector const & check_from, IGameObject * unit, float delay, int speed)
	{
		if (unit->IsValid())
		{
			auto const position = unit->ServerPosition();
			auto const delay_t = delay / 1000.0f + check_from.Distance(position) / speed;
			auto const d = delay_t * unit->MoveSpeed();
			auto const path = unit->RealPath();

			if (Geometry::Geometry::PathLength(path) > d)
			{
				return FastPredictionResult(true, position, Geometry::Geometry::CutPath(path, d).front());
			}

			return FastPredictionResult(false, path[path.size() - 1], path[path.size() - 1]);
		}

		return {};
	}

	void ProcessSpell(IGameObject* sender, OnProcessSpellEventArgs* args)
	{
		if (sender->IsAIHero() && args->SpellData->SpellName == "YasuoWMovingWall")
		{
			wall_cast_time = g_Common->TickCount();
			wall_cast_position = args->EndPosition;
		}
	}
}