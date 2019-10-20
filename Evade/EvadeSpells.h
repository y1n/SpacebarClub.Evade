#pragma once

#include "../SDK/PluginSDK.h"
#include "EvadeConstants.h"

#define SET_VARE(TYPE, NAME) \
    EvadingSpellData* _##NAME(TYPE const& val) \
    { \
        this->NAME = val; \
        return this; \
    }

#define ADD_VARE(TYPE, NAME) \
    TYPE NAME = {}; \
    SET_VARE(TYPE, NAME)

class EvadingSpellData
{
public:
	ADD_VARE(EvadingSpellType, spell_type)
	ADD_VARE(DangerLevel, danger_level)
	ADD_VARE(ItemId, item_id)
	ADD_VARE(::SpellSlot, slot)
	
	ADD_VARE(std::string, spell_name)
	ADD_VARE(std::string, reqiured_spellbook_spellname)
	
	ADD_VARE(std::function<bool(EvadingSpellData*)>, is_ready_delegate)
	ADD_VARE(std::function<float(EvadingSpellData*)>, movespeed_delegate)
	ADD_VARE(std::function<::SpellSlot(EvadingSpellData*)>, spell_slot_delegate)
	
	ADD_VARE(int, delay)
	ADD_VARE(int, range)
	ADD_VARE(int, speed)
	ADD_VARE(int, target_flags)
	
	ADD_VARE(bool, is_disabled_by_default)
	ADD_VARE(bool, is_fixed_range)
	ADD_VARE(bool, invert)
	ADD_VARE(bool, is_summonerspell)
	ADD_VARE(bool, is_spellshield)
	ADD_VARE(bool, requires_premove)
	ADD_VARE(bool, is_selfcast)
	ADD_VARE(bool, is_skillshot)
	ADD_VARE(bool, can_shield_allies)
	ADD_VARE(bool, try_cast_on_enemies)

	int MoveSpeedAmount();

	bool IsReady();
	bool IsEnabled();
	bool IsTargeted();

	SpellSlot SpellSlot();
	DangerLevel DangerLevel();
	SpellValidTargets TargetFlags();

	std::string MenuName();
	std::string MenuDisplayName();

private:
	IMenu* menu_instance{};
	IMenu* get_menu();
};

namespace EvadeSpellsDb
{
	std::vector<EvadingSpellData*>& GetEvadingSpells();
	EvadingSpellData* InitializeEvadingSpell();
}