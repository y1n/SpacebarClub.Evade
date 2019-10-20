#pragma once

#include "../SDK/PluginSDK.h"
#include "../SDK/EventHandler.h"
#include "../SDK/Vector.h"

struct SpellMenuConfig
{
	IMenu* menu_instance;

	IMenuElement* is_enabled;
	IMenuElement* is_dangerous;
	IMenuElement* draw;
	IMenuElement* danger_level;

	SpellMenuConfig() : menu_instance(nullptr), is_enabled(nullptr), is_dangerous(nullptr), draw(nullptr),
		danger_level(nullptr)
	{
	}

	SpellMenuConfig(IMenu* menu_tab, IMenuElement* _is_enabled, IMenuElement* _is_dangerous, IMenuElement*  _draw, IMenuElement* _danger_level) :
		menu_instance(menu_tab),
		is_enabled(_is_enabled), is_dangerous(_is_dangerous), draw(_draw), danger_level(_danger_level)
	{
	}
};

namespace Evade
{
	extern IMenu*                              menu;
	extern IMenu*                              evading_spells_menu;
	extern IMenu*                              spells_menu;
	extern IMenu*                              collision_menu;
	extern IMenu*                              config_menu;
	extern IMenu*                              drawing_menu;
	extern IMenu*                              colors_menu;

	extern std::map<std::string, SpellMenuConfig*>  spell_configs;

	extern Vector player_position;
	extern int LastWardJumpTick;

	bool Load();
	void Unload();
	
	bool IsPlayerSpellShielded();
	bool IsPlayerImmobile();
	bool IsEnabled();
	bool DodgeOnlyDangerous();
	
	bool IsAboutToHit(int time, IGameObject* unit = nullptr);
	
	bool IsEvading();
	void SetEvading(bool b);
	
	void TryToEvade(Vector const& to);
	void OnUpdate();
	void OnIssueOrder(IGameObject* sender, OnIssueOrderEventArgs* args);
	void OnNewPath(IGameObject* sender, OnNewPathEventArgs* args);
	void OnCastSpell(IGameObject* sender, OnCastSpellEventArgs* args);
	void OnBeforeAttack(BeforeAttackOrbwalkerArgs* args);
}