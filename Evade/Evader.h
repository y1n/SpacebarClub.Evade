#pragma once

#include "../SDK/PluginSDK.h"

struct SafePath;
class EvadingSpellData;
class SkillshotInstance;

class EvadingPosition
{
public:
	Vector start_position;
	Vector end_position;

	int enemies_in_range = 0;
	float weight = 0;
	bool is_evadable = FALSE;

	EvadingPosition();
	EvadingPosition(Vector const& start_pos, Vector const& end_pos, bool can_evade, int enemies_count);

	float Distance();
	float PathLength();
	std::vector<Vector> Path();

private:
	float distance = 0;
	std::vector<Vector> path;
};

namespace Evader
{
	std::vector<Vector> GetPossibleEvadePoints();

	bool GetEvadingPosition(EvadingPosition& result, Vector const& position, int speed = -1, int delay = 0);
	bool GetDashPosition(EvadingPosition& result, Vector const& position, int speed, int delay, float range, bool fixed_range);
	bool GetBlinkPosition(EvadingPosition& result, Vector const& position, int delay, float range);

	IGameObject* GetBestTarget(Vector const& pos, EvadingSpellData* evadeSpell);

	bool IsPointBlinkSafe(Vector const& pos, int time, int delay);
	bool IsPointSafe(Vector const& pos);

	SafePath IsPathSafe(std::vector<Vector> const& paths, int time, int speed = -1, int delay = 0);
	SafePath IsPathSafe(Vector const& pos, int time, int speed = -1, int delay = 0);
}