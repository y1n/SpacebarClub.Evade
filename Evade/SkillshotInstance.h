#pragma once

#include "../SDK/PluginSDK.h"
#include "../SDK/Geometry.h"
#include "SkillshotData.h"
#include "Evade.h"

struct IntersectionInfo
{
	Vector point;

	float distance;
	unsigned int time;
	bool is_valid;

	IntersectionInfo() : distance(0), time(0), is_valid(false) {}
	IntersectionInfo(Vector const& _point, float _distance, unsigned int _time, Vector const& from) : distance(_distance), time(_time), is_valid(this->point.IsValid())
	{
		this->point = _point + (from - _point).Normalized() * Config::GridSize;
	}
};

struct SafePath
{
	IntersectionInfo intersection;
	bool is_safe;

	SafePath() : is_safe(false) {}
	SafePath(IntersectionInfo _intersection, bool _is_safe) : intersection(std::move(_intersection)), is_safe(_is_safe) {}
};

class SkillshotInstance
{
public:
	SkillshotData*      skillshot_data = nullptr;
	SpellMenuConfig*    menu_config = nullptr;

	Vector start_position{};
	Vector end_position{};
	Vector direction{};

	int start_tick = 0;
	int end_tick = 0;

	bool force_disabled = FALSE;
	bool is_from_fow = FALSE;

	IGameObject* caster = nullptr;
	IGameObject* missile_client = nullptr;

	Geometry::Polygon*   polygon = nullptr;
	Geometry::Polygon*   evading_polygon = nullptr;
	Geometry::Polygon*   drawing_polygon = nullptr;
	Geometry::Polygon*   pathfinding_innerpolygon = nullptr;
	Geometry::Polygon*   pathfinding_outerpolygon = nullptr;
	Geometry::Rectangle* rectangle = nullptr;
	Geometry::Ring*      ring = nullptr;
	Geometry::Sector*    sector = nullptr;
	Geometry::Arc*       arc = nullptr;
	Geometry::Circle*    circle = nullptr;

	SkillshotInstance(SkillshotData* sdata, Vector const& start, Vector const& end, unsigned int detection_tick, unsigned end_tick, IGameObject* caster);
	~SkillshotInstance();

	unsigned int Radius();

	bool IsValid();
	bool IsEnabled();
	bool IsGlobal();
	bool IsSafePoint(Vector const& point);
	SafePath IsSafePath(std::vector<Vector> const& path, unsigned int time_ms, int speed, unsigned int delay);
	bool IsSafeToBlink(Vector const& point, unsigned int time_ms, unsigned int delay);
	bool IsAboutToHit(unsigned int time_ms, IGameObject* unit);

	void OnTick();
	void UpdatePolygons();
	void Draw(float thickness = 1.0f);

	Vector CollisionPosition();
	Vector GetMissilePosition(unsigned int time_ms);

private:
	Vector cached_collision_result{};
	int last_collision_calculation = 0;
};