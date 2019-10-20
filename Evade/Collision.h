#pragma once

#include "../SDK/PluginSDK.h"
#include "SkillshotInstance.h"

namespace Collision
{
	void Initialize();
	void Unload();
	
	Vector GetCollisionPoint(SkillshotInstance* skillshot, Vector const& check_from = {}, bool check_menu_settings = true);
}