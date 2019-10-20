#pragma once

namespace Config
{
	const bool IsDebugEnabled = FALSE;

	const unsigned int AdditionalRange = 25;
	const unsigned int AdditionalRadius = 10;
	const unsigned int GridSize = 10;
	const unsigned int PathFindingInnerDistance = 30;
	const unsigned int PathFindingOuterDistance = 60;

	const unsigned int CrossingTime = 250;
	const unsigned int EvadePointChangeTime = 300;
	const unsigned int EvadingFirstTime = 200;
	const unsigned int EvadingRouteChangeTime = 250;
	const unsigned int EvadingSecondTime = 80;
	const unsigned int EvadePointChangeInterval = 300;

	const unsigned int DiagonalEvadePointsCount = 7;
	const unsigned int DiagonalEvadePointsStep = 20;
	const unsigned int EnemiesRangeScan = 650;
}

enum SpellType
{
	kLine,
	kMissileLine,
	kCircle,
	kCone,
	kMissileCone,
	kArc,
	kRing
};

enum DangerLevel
{
	kLow,
	kMedium,
	kHigh,
	kExtreme
};

enum SpellCollisionFlags
{
	kMinions = 1 << 0,
	kHeroes = 1 << 1,
	kYasuoWall = 1 << 2,
	kWall = 1 << 3
};

enum CrowdControllFlags
{
	kHardCc = 1,
	kSlow = 2
};

enum SpellValidTargets
{
	kAllyMinions = 1 << 0,
	kEnemyMinions = 1 << 1,

	kAllyWards = 1 << 2,
	kEnemyWards = 1 << 3,

	kAllyChampions = 1 << 4,
	kEnemyChampions = 1 << 5
};

enum EvadingSpellType
{
	kWindwall,
	kDash,
	kBlink,
	kInvulnerability,
	kMovementSpeedBuff,
	kShield
};