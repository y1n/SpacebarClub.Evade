#include "PathFinding.h"
#include "Evade.h"
#include "SpellDetector.h"

namespace Pathfinder
{
	bool IsCross(Vector const& a, Vector const& b, Vector const& c, Vector const& d)
	{
		const auto f = (b.x - a.x) * (d.y - c.y) - (b.y - a.y) * (d.x - c.x);

		if (f == 0.0f)
		{
			return false;
		}

		const auto num1 = (a.y - c.y) * (d.x - c.x) - (a.x - c.x) * (d.y - c.y);
		const auto num2 = (a.y - c.y) * (b.x - a.x) - (a.x - c.x) * (b.y - a.y);

		if (num1 == 0.0f || num2 == 0.0f)
		{
			return false;
		}

		auto r = num1 / f;
		auto s = num2 / f;

		return r > 0 && r < 1 && s > 0 && s < 1;
	}

	bool IsConcave(std::vector<Vector> const& a, int id)
	{
		const auto cur = a[id];
		const auto next = a[(id + 1) % a.size()];
		const auto prev = a[id == 0 ? a.size() - 1 : id - 1];
		const auto left = Vector2(cur.x - prev.x, cur.y - prev.y);
		const auto right = Vector2(next.x - cur.x, next.y - cur.y);

		return left.x * right.y - left.y * right.x < 0;
	}

	bool CanReach(Vector const& start, Vector const& end, std::vector<Geometry::Polygon> const& polys)
	{
		if (start == end)
		{
			return false;
		}

		const int n_points = 2;
		const auto step = start.Distance(end) / n_points;
		const auto direction = (end - start).Normalized();

		for (int i = 0; i <= n_points; i++)
		{
			auto p = start + direction * (step * i);
			if (p.IsWall())
			{
				return false;
			}
		}

		for (auto const& poly : polys)
		{
			for (size_t i = 0; i < poly.Points.size(); ++i)
			{
				if (IsCross(start, end, poly.Points[i], poly.Points[i == poly.Points.size() - 1 ? 0 : i + 1]))
				{
					return false;
				}
			}

		}

		return true;
	}

	Vector GetClosestOutsidePoint(Vector const& from, std::vector<Geometry::Polygon> const& polys)
	{
		std::vector<Vector> result;

		for (auto const& poly : polys)
		{
			for (size_t i = 0; i < poly.Points.size(); ++i)
			{
				result.push_back(from.ProjectOn(poly.Points[i], poly.Points[i == poly.Points.size() - 1 ? 0 : i + 1]).SegmentPoint);
			}
		}

		if (result.empty())
		{
			return {};
		}

		std::sort(result.begin(), result.end(), [&](Vector const& a, Vector const& b) { return a.Distance(from) < b.Distance(from); });
		return result.front();
	}

	void PathFollow::KeepFollowPath()
	{
		if (!IsFollowing())
		{
			return;
		}

		while (IsFollowing() && Evade::player_position.Distance(paths[0]) < 80)
		{
			paths.erase(paths.begin());
		}

		if (IsFollowing())
		{
			g_LocalPlayer->IssueOrder(IssueOrderType::MoveTo, paths[0], false);
		}
	}

	bool FindPath(Vector start, Vector end, std::vector<Vector>& out_result)
	{
		std::vector<Geometry::Polygon> outer;
		std::vector<Geometry::Polygon> inner;

		for (SkillshotInstance* const& spell : SpellDetector::detected_skillshots)
		{
			if (spell->IsValid())
			{
				outer.push_back(*spell->pathfinding_outerpolygon);
				inner.push_back(*spell->pathfinding_innerpolygon);
			}
		}

		auto outer_polygons = Geometry::Geometry::ToPolygons(Geometry::Geometry::ClipPolygons(outer));
		const auto inner_polygons = Geometry::Geometry::ToPolygons(Geometry::Geometry::ClipPolygons(inner));

		for (auto& poly : outer_polygons)
		{
			if (poly.IsInside(end))
				end = GetClosestOutsidePoint(end, outer_polygons);

			if (poly.IsInside(start))
				start = GetClosestOutsidePoint(end, outer_polygons);
		}

		if (CanReach(start, end, inner_polygons))
		{
			out_result.push_back(start);
			out_result.push_back(end);

			return true;
		}

		auto poly = Geometry::Polygon();
		poly.Points.emplace_back(start);
		poly.Points.emplace_back(end);

		outer_polygons.push_back(poly);

		std::vector<Vector> result;
		std::vector<Node*> nodes;
		std::vector<Node*> to_delete_nodes;
		std::vector<Path<Node>*> to_delete_path;

		for (auto const& poly1 : outer_polygons)
		{
			for (size_t i = 0; i < poly1.Points.size(); ++i)
			{
				if (poly1.Points.size() != 2 && IsConcave(poly1.Points, i))
				{
					continue;
				}

				Node* node1 = nullptr;
				const auto it = std::find_if(nodes.begin(), nodes.end(), [&](Node* a) { return a->point == poly1.Points[i]; });

				if (it != nodes.end())
				{
					node1 = *it;
				}
				else
				{
					node1 = new Node(poly1.Points[i]);
					to_delete_nodes.push_back(node1);
				}

				nodes.push_back(node1);

				for (auto const& poly2 : outer_polygons)
				{
					for (auto const& po : poly2.Points)
					{
						if (!CanReach(poly1.Points[i], po, inner_polygons))
						{
							continue;
						}

						Node* node2 = nullptr;
						const auto it2 = std::find_if(nodes.begin(), nodes.end(), [&](Node* a) { return a->point == po; });

						if (it2 != nodes.end())
						{
							node2 = *it2;
						}
						else
						{
							node2 = new Node(po);
							to_delete_nodes.push_back(node2);
						}

						nodes.push_back(node2);
						node1->neightbours.push_back(node2);
					}
				}
			}
		}

		const auto start_node_it = std::find_if(nodes.begin(), nodes.end(), [&](Node* a) { return a->point == start; });
		const auto end_node_it = std::find_if(nodes.begin(), nodes.end(), [&](Node* a) { return a->point == end; });

		if (end_node_it == nodes.end() || start_node_it == nodes.end())
		{
			return false;
		}

		const auto startNode = *start_node_it;
		const auto endNode = *end_node_it;

		std::vector<Vector> closed;
		PriorityQueue<double, Path<Node>> queue;

		const auto start_path = new Path<Node>(startNode);
		to_delete_path.push_back(start_path);

		queue.Enqueue(0, start_path);

		while (!queue.IsEmpty())
		{
			auto path = queue.Dequeue();

			if (std::find_if(closed.begin(), closed.end(), [&path](Vector const& a) { return a == path->last_step->point; }) != closed.end())
			{
				continue;
			}

			if (path->last_step == endNode)
			{
				auto vec_path = path->GetVectorPath();

				for (auto it = vec_path.rbegin(); it != vec_path.rend(); ++it)
				{
					result.push_back((*it)->point);
				}

				for (auto& e : to_delete_path)
					delete e;

				for (auto e : to_delete_nodes)
					delete e;

				out_result.clear();
				out_result.insert(out_result.end(), result.begin(), result.end());

				return true;
			}

			closed.push_back(path->last_step->point);

			for (auto node : path->last_step->neightbours)
			{
				auto new_path = path->AddStep(node, path->last_step->point.Distance(node->point));
				queue.Enqueue(new_path->total_cost + node->point.Distance(start), new_path);
				to_delete_path.push_back(new_path);
			}
		}

		for (auto& e : to_delete_path)
			delete e;

		for (auto e : to_delete_nodes)
			delete e;

		return false;
	}

	std::vector<Vector> PathFollow::paths;
}