#pragma once

#include <vector>

#include "../SDK/PluginSDK.h"
#include "../SDK/EventHandler.h"
#include "SkillshotInstance.h"

namespace SpellDetector
{
	extern std::vector<SkillshotInstance*> detected_skillshots;
	extern std::vector<SkillshotInstance*> enabled_skillshots;
	extern std::vector<SkillshotInstance*> skillshots_in_danger;

	void Load();
	void Unload();

	void OnTick();
	void OnDraw();
	void OnCreate(IGameObject* sender);
	void OnDelete(IGameObject* sender);
	void OnProcessSpell(IGameObject* sender, OnProcessSpellEventArgs* args);
	void OnNewPath(IGameObject* sender, OnNewPathEventArgs* args);

	SkillshotInstance* AddSkillshot(IGameObject* caster, Vector const& start_pos, Vector const& end_pos, SkillshotData* skillshot_data, unsigned int start_time, IGameObject* missile_client = nullptr);
}