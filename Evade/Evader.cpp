#include "Evader.h"
#include "SpellDetector.h"
#include "Evade.h"
#include "EvadeSpells.h"

namespace Evader
{
	template<typename T>
	struct min_max
	{
		T min;
		T max;

		void emplace_value(T& val)
		{
			if (val > max)
				max = val;

			if (val < min)
				min = val;
		}
		float percentage(T& val) { return (float(val - min)) / float(max - min); }
	};


	std::vector<Vector> GetPossibleEvadePoints()
	{
		std::vector<Vector> result;
		std::vector<EvadingPosition> evading_positions;
		std::vector<Geometry::Polygon> polygon_list;

		for (auto const& skillshot_instance : SpellDetector::enabled_skillshots)
		{
			polygon_list.push_back(*skillshot_instance->evading_polygon);
		}

		auto danger_polygons = Geometry::Geometry::ToPolygons(Geometry::Geometry::ClipPolygons(polygon_list));
		auto my_position = Evade::player_position;

		//Scan the sides of each polygon to find the safe point.
		for (auto const& poly : danger_polygons)
		{
			for (size_t i = 0; i <= poly.Points.size() - 1; ++i)
			{
				auto side_start = poly.Points[i];
				auto side_end = poly.Points[(i == poly.Points.size() - 1) ? 0 : i + 1];

				auto original_candidate = my_position.ProjectOn(side_start, side_end).SegmentPoint;
				auto distance_to_evade_point = original_candidate.DistanceSquared(my_position);

				if (distance_to_evade_point < 600 * 600)
				{
					auto side_distance = side_end.DistanceSquared(side_start);
					auto direction = (side_end - side_start).Normalized();
					int s = (distance_to_evade_point < 200 * 200 && side_distance > 90 * 90) ? Config::DiagonalEvadePointsCount : 0;

					for (int j = -s; j <= s; j++)
					{
						auto candidate = original_candidate + direction * (j * Config::DiagonalEvadePointsStep);

						if (candidate.IsWall())
							continue;

						result.push_back(candidate);
					}
				}
			}
		}

		return result;
	}

	bool GetEvadingPosition(EvadingPosition& result, Vector const& position, int speed, int delay)
	{
		speed = speed == -1 ? g_LocalPlayer->MoveSpeed() : speed;

		std::vector<EvadingPosition> evading_positions;
		auto my_position = Evade::player_position;

		struct
		{
			min_max<float> path_length;
			min_max<float> distance;
			min_max<int> enemies;
		} scalars{};

		for (auto const& candidate : GetPossibleEvadePoints())
		{
			auto path_to_point = g_LocalPlayer->CreatePath(candidate);
			auto path = IsPathSafe(path_to_point, Config::EvadingFirstTime, speed, delay);

			if (IsPathSafe(path_to_point, Config::EvadingFirstTime, speed, delay).is_safe)
			{
				EvadingPosition evading_position(my_position, candidate, path.is_safe, candidate.CountEnemiesInRange(Config::EnemiesRangeScan));

				auto distance = candidate.Distance(position);
				auto path_length = evading_position.PathLength();

				scalars.distance.emplace_value(distance);
				scalars.path_length.emplace_value(path_length);
				scalars.enemies.emplace_value(evading_position.enemies_in_range);

				evading_positions.push_back(evading_position);
			}

			if (IsPathSafe(path_to_point, Config::EvadingSecondTime, speed, delay).is_safe)
			{
				EvadingPosition evading_position(my_position, candidate, false, candidate.CountEnemiesInRange(Config::EnemiesRangeScan));

				auto distance = candidate.Distance(position);
				auto path_length = evading_position.PathLength();

				scalars.distance.emplace_value(distance);
				scalars.path_length.emplace_value(path_length);
				scalars.enemies.emplace_value(evading_position.enemies_in_range);

				evading_positions.push_back(evading_position);
			}
		}

		if (evading_positions.empty())
			return false;

		//Calculate the best point
		for (auto& evading_position : evading_positions)
		{
			auto distance_to_position = position.Distance(evading_position.end_position);
			auto path_length = evading_position.PathLength();
			auto enemies = evading_position.enemies_in_range;

			evading_position.weight = 1;
			evading_position.weight += 1 - scalars.path_length.percentage(path_length);
			evading_position.weight += 1 - scalars.distance.percentage(distance_to_position);
			evading_position.weight += 1 - scalars.enemies.percentage(enemies);

			//if not evadable divide by 2
			if (!evading_position.is_evadable)
			{
				evading_position.weight *= 0.5f;
			}

			//if under turret divide by 2
			if (evading_position.end_position.IsUnderEnemyTurret())
			{
				evading_position.weight *= 0.5f;
			}
		}

		std::sort(evading_positions.begin(), evading_positions.end(), [](EvadingPosition const& a, EvadingPosition const& b)
		{
			return a.weight > b.weight;
		});

		result = evading_positions.front();
		return true;
	}

	bool GetDashPosition(EvadingPosition & result, Vector const & position, int speed, int delay, float range, bool fixed_range)
	{
		speed = speed == -1 ? g_LocalPlayer->MoveSpeed() : speed;

		std::vector<EvadingPosition> evading_positions;
		auto my_position = Evade::player_position;

		struct
		{
			min_max<float> path_length;
			min_max<float> distance;
			min_max<int> enemies;
		} scalars{};

		for (auto &candidate : GetPossibleEvadePoints())
		{
			if (fixed_range || my_position.Distance(candidate) > range)
			{
				candidate = my_position.Extend(candidate, range);
			}

			auto path_to_point = g_LocalPlayer->CreatePath(candidate);
			auto path = IsPathSafe(path_to_point, Config::EvadingFirstTime, speed, delay);

			if (IsPathSafe(path_to_point, Config::EvadingFirstTime, speed, delay).is_safe)
			{
				EvadingPosition evading_position(my_position, candidate, path.is_safe, candidate.CountEnemiesInRange(Config::EnemiesRangeScan));

				auto distance = candidate.Distance(position);
				auto path_length = evading_position.PathLength();

				scalars.distance.emplace_value(distance);
				scalars.path_length.emplace_value(path_length);
				scalars.enemies.emplace_value(evading_position.enemies_in_range);

				evading_positions.push_back(evading_position);
			}

			if (IsPathSafe(path_to_point, Config::EvadingSecondTime, speed, delay).is_safe)
			{
				EvadingPosition evading_position(my_position, candidate, false, candidate.CountEnemiesInRange(Config::EnemiesRangeScan));

				auto distance = candidate.Distance(position);
				auto path_length = evading_position.PathLength();

				scalars.distance.emplace_value(distance);
				scalars.path_length.emplace_value(path_length);
				scalars.enemies.emplace_value(evading_position.enemies_in_range);

				evading_positions.push_back(evading_position);
			}
		}

		if (evading_positions.empty())
			return false;

		//Calculate the best point
		for (auto& evading_position : evading_positions)
		{
			auto distance_to_position = position.Distance(evading_position.end_position);
			auto path_length = evading_position.PathLength();
			auto enemies = evading_position.enemies_in_range;

			evading_position.weight = 1;
			evading_position.weight += 1 - scalars.path_length.percentage(path_length);
			evading_position.weight += 1 - scalars.distance.percentage(distance_to_position);
			evading_position.weight += 1 - scalars.enemies.percentage(enemies);

			//if not evadable divide by 2
			if (!evading_position.is_evadable)
			{
				evading_position.weight *= 0.5f;
			}

			//if under turret divide by 2
			if (evading_position.end_position.IsUnderEnemyTurret())
			{
				evading_position.weight *= 0.5f;
			}
		}

		std::sort(evading_positions.begin(), evading_positions.end(), [](EvadingPosition const& a, EvadingPosition const& b)
		{
			return a.weight > b.weight;
		});

		result = evading_positions.front();
		return true;
	}

	bool GetBlinkPosition(EvadingPosition & result, Vector const & position, int delay, float range)
	{
		std::vector<EvadingPosition> evading_positions;
		auto my_position = Evade::player_position;

		struct
		{
			min_max<float> path_length;
			min_max<float> distance;
			min_max<int> enemies;
		} scalars{};

		for (auto const& candidate : GetPossibleEvadePoints())
		{
			auto path_to_point = g_LocalPlayer->CreatePath(candidate);

			if (IsPointBlinkSafe(path_to_point[path_to_point.size() - 1], Config::EvadingFirstTime, delay))
			{
				EvadingPosition evading_position(my_position, candidate, true, candidate.CountEnemiesInRange(Config::EnemiesRangeScan));

				auto distance = candidate.Distance(position);
				auto path_length = evading_position.PathLength();

				scalars.distance.emplace_value(distance);
				scalars.path_length.emplace_value(path_length);
				scalars.enemies.emplace_value(evading_position.enemies_in_range);

				evading_positions.push_back(evading_position);
			}

			if (IsPointBlinkSafe(path_to_point[path_to_point.size() - 1], Config::EvadingSecondTime, delay))
			{
				EvadingPosition evading_position(my_position, candidate, false, candidate.CountEnemiesInRange(Config::EnemiesRangeScan));

				auto distance = candidate.Distance(position);
				auto path_length = evading_position.PathLength();

				scalars.distance.emplace_value(distance);
				scalars.path_length.emplace_value(path_length);
				scalars.enemies.emplace_value(evading_position.enemies_in_range);

				evading_positions.push_back(evading_position);
			}
		}

		if (evading_positions.empty())
			return false;

		//Calculate the best point
		for (auto& evading_position : evading_positions)
		{
			auto distance_to_position = position.Distance(evading_position.end_position);
			auto path_length = evading_position.PathLength();
			auto enemies = evading_position.enemies_in_range;

			evading_position.weight = 1;
			evading_position.weight += 1 - scalars.path_length.percentage(path_length);
			evading_position.weight += 1 - scalars.distance.percentage(distance_to_position);
			evading_position.weight += 1 - scalars.enemies.percentage(enemies);

			//if not evadable divide by 2
			if (!evading_position.is_evadable)
			{
				evading_position.weight *= 0.5f;
			}

			//if under turret divide by 2
			if (evading_position.end_position.IsUnderEnemyTurret())
			{
				evading_position.weight *= 0.5f;
			}
		}

		std::sort(evading_positions.begin(), evading_positions.end(), [](EvadingPosition const& a, EvadingPosition const& b)
		{
			return a.weight > b.weight;
		});

		result = evading_positions.front();
		return true;
	}

	IGameObject* GetBestTarget(Vector const& pos, EvadingSpellData* evadeSpell)
	{
		std::vector<IGameObject*> targets;
		std::vector<std::pair<IGameObject*, Vector>> result;
		std::map<IGameObject*, EvadingPosition> evading_positions;

		auto my_pos = g_LocalPlayer->Position();
		auto delay = evadeSpell->spell_type == kInvulnerability ? 0 : evadeSpell->delay;

		struct
		{
			min_max<float> path_length;
			min_max<float> distance;
			min_max<int> enemies;
		} scalars{};

		if (evadeSpell->TargetFlags() & kAllyChampions)
		{
			for (auto const& ally : g_ObjectManager->GetChampions())
			{
				if (ally->IsAlly() && !ally->IsMe() && ally->IsValidTarget(evadeSpell->range, false, my_pos))
				{
					targets.push_back(ally);
				}
			}
		}

		if (evadeSpell->TargetFlags() & kAllyMinions)
		{
			for (auto const& ally : g_ObjectManager->GetMinionsAlly())
			{
				if (ally->IsValidTarget(evadeSpell->range, false, my_pos))
				{
					targets.push_back(ally);
				}
			}
		}

		if (evadeSpell->TargetFlags() & kAllyWards)
		{
			for (auto const& ally : g_ObjectManager->GetWards())
			{
				if (ally->IsValidTarget(evadeSpell->range, false, my_pos) && ally->IsAlly())
				{
					targets.push_back(ally);
				}
			}
		}

		if (evadeSpell->TargetFlags() & kEnemyChampions)
		{
			for (auto const& enemy : g_ObjectManager->GetChampions())
			{
				if (enemy->IsEnemy() && enemy->IsValidTarget(evadeSpell->range, true, my_pos))
				{
					targets.push_back(enemy);
				}
			}
		}

		if (evadeSpell->TargetFlags() & kEnemyMinions)
		{
			for (auto const& enemy : g_ObjectManager->GetMinionsEnemy())
			{
				if (enemy->IsValidTarget(evadeSpell->range, true, my_pos))
				{
					targets.push_back(enemy);
				}
			}

			for (auto const& enemy : g_ObjectManager->GetJungleMobs())
			{
				if (enemy->IsValidTarget(evadeSpell->range, true, my_pos))
				{
					targets.push_back(enemy);
				}
			}
		}

		if (evadeSpell->TargetFlags() & kEnemyWards)
		{
			for (auto const& enemy : g_ObjectManager->GetWards())
			{
				if (enemy->IsValidTarget(evadeSpell->range, true, my_pos))
				{
					targets.push_back(enemy);
				}
			}
		}

		for (auto const& target : targets)
		{
			auto end = target->ServerPosition();

			if (evadeSpell->spell_name == "YasuoE" && target->HasBuff(hash("YasuoDashWrapper")))
			{
				continue;
			}

			if (evadeSpell->is_fixed_range)
			{
				end = my_pos.Extend(end, evadeSpell->range);
			}

			if (!(evadeSpell->spell_type & kInvulnerability) && !IsPointSafe(end))
			{
				continue;
			}

			auto distance = end.Distance(Evade::player_position);

			if (g_Common->TickCount() - Evade::LastWardJumpTick < 250)
			{
				if (evading_positions.find(target) == evading_positions.end())
				{
					EvadingPosition evading_position(Evade::player_position, end, true, end.CountEnemiesInRange(Config::EnemiesRangeScan));
					evading_positions[target] = evading_position;

					auto path_length = evading_position.PathLength();

					scalars.distance.emplace_value(distance);
					scalars.path_length.emplace_value(path_length);
					scalars.enemies.emplace_value(evading_position.enemies_in_range);
				}

				result.emplace_back(target, end);
			}
			else if (evadeSpell->spell_type & kDash)
			{
				if (IsPathSafe(Config::EvadingFirstTime, evadeSpell->speed, delay).is_safe ||
					IsPathSafe(Config::EvadingSecondTime, evadeSpell->speed, delay).is_safe)
				{
					if (evading_positions.find(target) == evading_positions.end())
					{
						EvadingPosition evading_position(Evade::player_position, end, true, end.CountEnemiesInRange(Config::EnemiesRangeScan));
						evading_positions[target] = evading_position;

						auto path_length = evading_position.PathLength();

						scalars.distance.emplace_value(distance);
						scalars.path_length.emplace_value(path_length);
						scalars.enemies.emplace_value(evading_position.enemies_in_range);
					}
					result.emplace_back(target, end);
				}
			}
			else
			{
				if (IsPointBlinkSafe(end, Config::EvadingFirstTime, delay) ||
					IsPointBlinkSafe(end, Config::EvadingSecondTime, delay))
				{
					if (evading_positions.find(target) == evading_positions.end())
					{
						EvadingPosition evading_position(Evade::player_position, end, false, end.CountEnemiesInRange(Config::EnemiesRangeScan));
						evading_positions[target] = evading_position;

						auto path_length = evading_position.PathLength();

						scalars.distance.emplace_value(distance);
						scalars.path_length.emplace_value(path_length);
						scalars.enemies.emplace_value(evading_position.enemies_in_range);
					}

					result.emplace_back(target, end);
				}
			}
		}

		if (evading_positions.empty())
			return nullptr;

		float weight = 0;
		IGameObject* best_target = nullptr;

		//Calculate the best point
		for (auto& evading_position : evading_positions)
		{
			auto distance_to_position = pos.Distance(evading_position.second.end_position);
			auto path_length = evading_position.second.PathLength();
			auto enemies = evading_position.second.enemies_in_range;

			evading_position.second.weight = 1;
			evading_position.second.weight += 1 - scalars.path_length.percentage(path_length);
			evading_position.second.weight += 1 - scalars.distance.percentage(distance_to_position);
			evading_position.second.weight += 1 - scalars.enemies.percentage(enemies);

			//if not evadable divide by 2
			if (!evading_position.second.is_evadable)
			{
				evading_position.second.weight *= 0.5f;
			}

			//if under turret divide by 2
			if (evading_position.second.end_position.IsUnderEnemyTurret())
			{
				evading_position.second.weight *= 0.5f;
			}

			if (weight < evading_position.second.weight)
			{
				weight = evading_position.second.weight;
				best_target = evading_position.first;
			}
		}

		return best_target;
	}

	SafePath IsPathSafe(std::vector<Vector> const& paths, int time, int speed, int delay)
	{
		if (paths.size() < 2)
		{
			return { {}, false };
		}

		auto is_safe = true;
		std::vector<IntersectionInfo> intersects;

		for (auto const& spell_instance : SpellDetector::detected_skillshots)
		{
			if (!spell_instance->IsEnabled())
				continue;

			const auto check_path = spell_instance->IsSafePath(paths, time, speed, delay);
			is_safe = is_safe && check_path.is_safe;

			if (check_path.intersection.is_valid)
			{
				intersects.push_back(check_path.intersection);
			}
		}

		if (intersects.empty())
		{
			return SafePath({}, is_safe);
		}

		std::sort(intersects.begin(), intersects.end(), [](IntersectionInfo const& a, IntersectionInfo const& b)
		{
			return a.distance < b.distance;
		});

		return SafePath(intersects.front(), is_safe);
	}

	SafePath IsPathSafe(Vector const& pos, int time, int speed, int delay)
	{
		auto path = g_LocalPlayer->CreatePath(pos);

		if (path.size() == 1)
			path.push_back(pos);

		return IsPathSafe(path, time, speed, delay);
	}

	bool IsPointBlinkSafe(Vector const& pos, int time, int delay)
	{
		for (auto const& spell_instance : SpellDetector::detected_skillshots)
		{
			if (!spell_instance->IsSafeToBlink(pos, time, delay))
			{
				return false;
			}
		}

		return true;
	}

	bool IsPointSafe(Vector const& pos)
	{
		for (auto const& spell_instance : SpellDetector::detected_skillshots)
		{
			if (!spell_instance->IsSafePoint(pos))
			{
				return false;
			}
		}

		return true;
	}
}

std::vector<Vector> EvadingPosition::Path()
{
	if (this->path.empty())
	{
		this->path = g_LocalPlayer->CreatePath(this->start_position, this->end_position, true);
	}

	return this->path;
}

EvadingPosition::EvadingPosition()
{
}

EvadingPosition::EvadingPosition(Vector const& start_pos, Vector const& end_pos, bool can_evade, int enemies_count)
{
	this->start_position = start_pos;
	this->end_position = end_pos;
	this->is_evadable = can_evade;
	this->enemies_in_range = enemies_count;
}

float EvadingPosition::Distance()
{
	if (this->distance == 0)
	{
		this->distance = this->start_position.Distance(this->end_position);
	}

	return this->distance;
}

float EvadingPosition::PathLength()
{
	auto distance_delta = 0.0f;
	auto p = this->Path();

	if (p.size() > 1)
		for (size_t index = 0; index < p.size() - 1; ++index)
			distance_delta = distance_delta + p[index].Distance(p[index + 1]);

	return distance_delta;
}