#pragma once

#include "../SDK/PluginSDK.h"
#include "../SDK/Geometry.h"

namespace Pathfinder
{
	bool IsCross(Vector const& a, Vector const& b, Vector const& c, Vector const& d);
	bool IsConcave(std::vector<Vector> const& a, int id);
	bool CanReach(Vector const& start, Vector const& end, std::vector<Geometry::Polygon> const& polys);
	bool FindPath(Vector start, Vector end, std::vector<Vector>& out_result);
	Vector GetClosestOutsidePoint(Vector const& from, std::vector<Geometry::Polygon> const& polys);

	template <typename TP, typename TV>
	class PriorityQueue
	{
	public:
		size_t size() { return this->list.size(); }

		bool IsEmpty() { return this->list.empty(); }

		TV* Dequeue()
		{
			auto pair = *this->list.begin();

			auto deq = this->list[pair.first].front();
			this->list[pair.first].pop();

			if (this->list[pair.first].empty())
			{
				this->list.erase(pair.first);
			}

			return deq;
		}

		void Enqueue(TP const &p, TV * v)
		{
			std::queue<TV*> q;

			if (this->list.find(p) == this->list.end())
			{
				this->list[p] = q;
			}

			this->list.at(p).push(v);
		}

	private:
		std::map<TP, std::queue<TV*>> list;
	};

	class Node
	{
	public:
		std::vector<Node*> neightbours;
		Vector point;

		Node() { point = Vector(0, 0, 0); }
		Node(Vector const& p)
		{
			this->point = p;
		}
	};

	template<typename TNode>
	class Path
	{
	public:
		TNode * last_step;

		Path<TNode>* prev_steps;

		double total_cost{};

		explicit Path(TNode* start)
		{
			this->last_step = start;
			this->total_cost = 0;
			this->prev_steps = nullptr;
		}

		Path(TNode* start, Path<TNode>* previous_steps, double cost)
		{
			this->last_step = start;
			this->prev_steps = previous_steps;
			this->total_cost = cost;
		}

		Path<TNode>* AddStep(TNode* step, double cost)
		{
			return new Path<TNode>(step, this, this->total_cost + cost);
		}

		std::vector<TNode*> GetVectorPath()
		{
			std::vector<TNode*> nodes;
			auto p = this;

			do
			{
				nodes.push_back(p->last_step);
				p = p->prev_steps;
			} while (p != nullptr);

			return nodes;
		}
	};

	class PathFollow
	{
	public:
		static bool IsFollowing()
		{
			return !paths.empty();
		}


		static void KeepFollowPath();
		static void Start(std::vector<Vector> const& path)
		{
			paths = path;
		}

		static void Stop()
		{
			paths.clear();
		}

	private:
		static std::vector<Vector> paths;
	};
}