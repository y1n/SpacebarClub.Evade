#pragma once

#include "../SDK/PluginSDK.h"
#include "ISpecialSpell.h"

namespace SpecialSpells
{
	std::vector<ISpecialSpell*>& GetSpecialSpells(std::vector<ChampionId> const& champion_ids);
}