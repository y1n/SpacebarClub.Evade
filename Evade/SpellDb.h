#pragma once

#include "../SDK/PluginSDK.h"
#include "SkillshotData.h"

class ISpecialSpell;

namespace SpellDb
{
	void Initialize();
	
	extern std::vector<SkillshotData*> spells;
	extern std::vector<ISpecialSpell*> special_spells;
}