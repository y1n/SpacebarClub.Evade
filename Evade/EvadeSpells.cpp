#include "EvadeSpells.h"
#include "Evade.h"

int EvadingSpellData::MoveSpeedAmount()
{
	if (this->spell_type == kMovementSpeedBuff)
	{
		if (this->movespeed_delegate)
		{
			return this->movespeed_delegate(this);
		}
	}

	return 0;
}

bool EvadingSpellData::IsReady()
{
	if (this->is_ready_delegate)
	{
		return this->is_ready_delegate(this);
	}

	if (int(this->item_id) != 0)
	{
		return g_LocalPlayer->CanUseItem(this->item_id);
	}

	if (this->reqiured_spellbook_spellname.empty())
	{
		return g_LocalPlayer->CanUseSpell(this->SpellSlot());
	}

	return g_LocalPlayer->GetSpellbook()->GetSpell(this->SpellSlot())->SData().SpellName == reqiured_spellbook_spellname &&
		g_LocalPlayer->CanUseSpell(this->SpellSlot());
}

bool EvadingSpellData::IsEnabled()
{
	if (this->get_menu())
	{
		return this->get_menu()->GetElement(this->MenuName() + ".IsEnabled")->GetBool();
	}

	return TRUE;
}

bool EvadingSpellData::IsTargeted()
{
	return this->target_flags != 0;
}

SpellSlot EvadingSpellData::SpellSlot()
{
	if (this->spell_slot_delegate)
	{
		return this->spell_slot_delegate(this);
	}

	if (int(this->item_id) != 0)
	{
		int i = 0;
		for (auto& item : g_LocalPlayer->GetItems())
		{			
			if (item.ItemId == this->item_id)
				return ::SpellSlot(i + 6);

			i++;
		}

		return SpellSlot::Invalid;
	}

	return this->slot;
}

DangerLevel EvadingSpellData::DangerLevel()
{
	if (this->get_menu())
	{
		return ::DangerLevel(this->get_menu()->GetElement(this->MenuName() + ".DangerLevel")->GetInt());
	}

	return this->danger_level;
}

SpellValidTargets EvadingSpellData::TargetFlags()
{
	if (this->target_flags & kAllyWards)
	{
		if (this->get_menu())
		{
			auto const wardjump = this->get_menu()->GetElement(this->MenuName() + ".WardJump")->GetBool();
			if (!wardjump)
			{
				return SpellValidTargets(this->target_flags ^ kAllyWards);
			}
		}

	}

	return SpellValidTargets(this->target_flags);
}

std::string EvadingSpellData::MenuName()
{
	if (this->spell_name.empty())
	{
		std::string spell_slot_name;

		switch (this->SpellSlot())
		{
		case SpellSlot::Invalid: spell_slot_name = "Invalid"; break;
		case SpellSlot::Q: spell_slot_name = "Q"; break;
		case SpellSlot::W: spell_slot_name = "W"; break;
		case SpellSlot::E: spell_slot_name = "E"; break;
		case SpellSlot::R: spell_slot_name = "R"; break;
		case SpellSlot::Summoner1: spell_slot_name = "Summoner1"; break;
		case SpellSlot::Summoner2: spell_slot_name = "Summoner2"; break;
		case SpellSlot::Item_1: spell_slot_name = "Item_1"; break;
		case SpellSlot::Item_2: spell_slot_name = "Item_2"; break;
		case SpellSlot::Item_3: spell_slot_name = "Item_3"; break;
		case SpellSlot::Item_4: spell_slot_name = "Item_4";	break;
		case SpellSlot::Item_5: spell_slot_name = "Item_5";	break;
		case SpellSlot::Item_6: spell_slot_name = "Item_6";	break;
		case SpellSlot::Trinket: spell_slot_name = "Trinket"; break;
		case SpellSlot::Recall: spell_slot_name = "Recall"; break;
		default: spell_slot_name = "Unknown"; break;
		}

		return g_LocalPlayer->ChampionName() + "." + spell_slot_name;
	}

	return g_LocalPlayer->ChampionName() + "." + this->spell_name;
}

std::string EvadingSpellData::MenuDisplayName()
{
	if (this->spell_name.empty())
	{
		auto result = g_LocalPlayer->ChampionName();

		std::string spell_slot_name;

		switch (this->SpellSlot())
		{
		case SpellSlot::Invalid: spell_slot_name = "Invalid"; break;
		case SpellSlot::Q: spell_slot_name = "Q"; break;
		case SpellSlot::W: spell_slot_name = "W"; break;
		case SpellSlot::E: spell_slot_name = "E"; break;
		case SpellSlot::R: spell_slot_name = "R"; break;
		case SpellSlot::Summoner1: spell_slot_name = "Summoner1"; break;
		case SpellSlot::Summoner2: spell_slot_name = "Summoner2"; break;
		case SpellSlot::Item_1: spell_slot_name = "Item_1"; break;
		case SpellSlot::Item_2: spell_slot_name = "Item_2"; break;
		case SpellSlot::Item_3: spell_slot_name = "Item_3"; break;
		case SpellSlot::Item_4: spell_slot_name = "Item_4";	break;
		case SpellSlot::Item_5: spell_slot_name = "Item_5";	break;
		case SpellSlot::Item_6: spell_slot_name = "Item_6";	break;
		case SpellSlot::Trinket: spell_slot_name = "Trinket"; break;
		case SpellSlot::Recall: spell_slot_name = "Recall"; break;
		default: spell_slot_name = "Unknown"; break;
		}

		result += " [";
		result += spell_slot_name;
		result += "]";

		return result;
	}

	return this->spell_name;
}

IMenu* EvadingSpellData::get_menu()
{
	if (!this->menu_instance)
	{
		if (Evade::evading_spells_menu)
		{
			if (Evade::evading_spells_menu->ElementExists(this->MenuName()))
			{
				this->menu_instance = Evade::evading_spells_menu->GetSubMenu(this->MenuName());
			}
		}
	}

	return this->menu_instance;
}

namespace EvadeSpellsDb
{
	std::vector<EvadingSpellData*>  evading_spells;
	bool                            was_initialized = false;

	std::vector<EvadingSpellData*>& GetEvadingSpells()
	{
		if (was_initialized)
		{
			return evading_spells;
		}

		was_initialized = true;
		auto const championid = g_LocalPlayer->ChampionId();

		evading_spells.push_back
		(
			InitializeEvadingSpell()->_spell_name("Walking")->_danger_level(kLow)
		);

#pragma region Spell Shields
		if (championid == ChampionId::Sivir)
		{
			evading_spells.push_back
			(
				InitializeEvadingSpell()->_spell_type(kShield)->_slot(SpellSlot::E)->_delay(100)->_danger_level(kLow)->_is_spellshield(TRUE)
			);
		}
		else if (championid == ChampionId::Nocturne)
		{
			evading_spells.push_back
			(
				InitializeEvadingSpell()->_spell_type(kShield)->_slot(SpellSlot::W)->_delay(100)->_danger_level(kLow)->_is_spellshield(TRUE)
			);
		}
#pragma endregion

#pragma region Move Speed Buffs
		else if (championid == ChampionId::Blitzcrank)
		{
			evading_spells.push_back
			(
				InitializeEvadingSpell()->_spell_type(kMovementSpeedBuff)->_slot(SpellSlot::W)->_delay(100)->_danger_level(kMedium)->_movespeed_delegate([](EvadingSpellData* inst)
			{
				return g_LocalPlayer->MoveSpeed() * (1.0f + 0.12f + 0.04f * g_LocalPlayer->GetSpellbook()->GetSpell(inst->SpellSlot())->Level());
			})
			);
		}
		else if (championid == ChampionId::Draven)
		{
			evading_spells.push_back
			(
				InitializeEvadingSpell()->_spell_type(kMovementSpeedBuff)->_slot(SpellSlot::W)->_delay(100)->_danger_level(kMedium)->_movespeed_delegate([](EvadingSpellData* inst)
			{
				return g_LocalPlayer->MoveSpeed() * (1.0f + 0.35f + 0.05f * g_LocalPlayer->GetSpellbook()->GetSpell(inst->SpellSlot())->Level());
			})
			);
		}
		else if (championid == ChampionId::Evelynn)
		{
			evading_spells.push_back
			(
				InitializeEvadingSpell()->_spell_type(kMovementSpeedBuff)->_slot(SpellSlot::W)->_delay(100)->_danger_level(kMedium)->_movespeed_delegate([](EvadingSpellData* inst)
			{
				return g_LocalPlayer->MoveSpeed() * (1.0f + 0.2f + 0.1f * g_LocalPlayer->GetSpellbook()->GetSpell(inst->SpellSlot())->Level());
			})
			);
		}
		else if (championid == ChampionId::Garen)
		{
			evading_spells.push_back
			(
				InitializeEvadingSpell()->_spell_type(kMovementSpeedBuff)->_slot(SpellSlot::Q)->_delay(100)->_danger_level(kMedium)->_movespeed_delegate([](EvadingSpellData* inst)
			{
				return g_LocalPlayer->MoveSpeed() * 1.35f;
			})
			);
		}
		else if (championid == ChampionId::Karma)
		{
			evading_spells.push_back
			(
				InitializeEvadingSpell()->_spell_type(kMovementSpeedBuff)->_slot(SpellSlot::E)->_delay(100)->_danger_level(kMedium)->_movespeed_delegate([](EvadingSpellData* inst)
			{
				return g_LocalPlayer->MoveSpeed() * (1.0f + 0.35f + 0.05f * g_LocalPlayer->GetSpellbook()->GetSpell(inst->SpellSlot())->Level());
			})
			);
		}
		else if (championid == ChampionId::Kennen)
		{
			evading_spells.push_back
			(
				InitializeEvadingSpell()->_spell_type(kMovementSpeedBuff)->_slot(SpellSlot::E)->_delay(100)->_danger_level(kMedium)->_movespeed_delegate([](EvadingSpellData* inst)
			{
				return g_LocalPlayer->MoveSpeed() + 200;
			})
			);
		}
		else if (championid == ChampionId::Khazix)
		{
			evading_spells.push_back
			(
				InitializeEvadingSpell()->_spell_type(kMovementSpeedBuff)->_slot(SpellSlot::R)->_delay(100)->_danger_level(kHigh)->_movespeed_delegate([](EvadingSpellData* inst)
			{
				return g_LocalPlayer->MoveSpeed() * 1.4f;
			})
			);
		}
		else if (championid == ChampionId::Lulu)
		{
			evading_spells.push_back
			(
				InitializeEvadingSpell()->_spell_type(kMovementSpeedBuff)->_slot(SpellSlot::W)->_delay(100)->_danger_level(kMedium)->_movespeed_delegate([](EvadingSpellData* inst)
			{
				return g_LocalPlayer->MoveSpeed() * (1.3f + g_LocalPlayer->TotalAbilityPower() / 100.0f * 0.1f);
			})
			);
		}
		else if (championid == ChampionId::Orianna)
		{
			evading_spells.push_back
			(
				InitializeEvadingSpell()->_spell_type(kMovementSpeedBuff)->_slot(SpellSlot::W)->_delay(100)->_danger_level(kMedium)->_movespeed_delegate([](EvadingSpellData* inst)
			{
				return g_LocalPlayer->MoveSpeed() * 1.2f;
			})->_is_ready_delegate([](EvadingSpellData* inst)
			{
				return g_LocalPlayer->CanUseSpell(inst->SpellSlot()) && g_LocalPlayer->HasBuff(hash("orianaghostself"));
			})
				);
		}
		else if (championid == ChampionId::Sivir)
		{
			evading_spells.push_back
			(
				InitializeEvadingSpell()->_spell_type(kMovementSpeedBuff)->_slot(SpellSlot::R)->_delay(100)->_danger_level(kHigh)->_movespeed_delegate([](EvadingSpellData* inst)
			{
				return g_LocalPlayer->MoveSpeed() * 1.6f;
			})
			);
		}
		else if (championid == ChampionId::Shyvana)
		{
			evading_spells.push_back
			(
				InitializeEvadingSpell()->_spell_type(kMovementSpeedBuff)->_slot(SpellSlot::W)->_delay(100)->_danger_level(kMedium)->_movespeed_delegate([](EvadingSpellData* inst)
			{
				return g_LocalPlayer->MoveSpeed() * (1.0f + 0.25f + 0.05f * g_LocalPlayer->GetSpellbook()->GetSpell(inst->SpellSlot())->Level());
			})
			);
		}
		else if (championid == ChampionId::Sona)
		{
			evading_spells.push_back
			(
				InitializeEvadingSpell()->_spell_type(kMovementSpeedBuff)->_slot(SpellSlot::E)->_delay(100)->_danger_level(kHigh)->_movespeed_delegate([](EvadingSpellData* inst)
			{
				auto const spellbook = g_LocalPlayer->GetSpellbook();
				return g_LocalPlayer->MoveSpeed() * (1.0f + 0.12f + 0.01f * spellbook->GetSpell(inst->SpellSlot())->Level()) +
					g_LocalPlayer->TotalAbilityPower() / 100.0f * 0.075f + 0.02f * spellbook->GetSpell(SpellSlot::R)->Level();
			})
			);
		}
		else if (championid == ChampionId::Teemo)
		{
			evading_spells.push_back
			(
				InitializeEvadingSpell()->_spell_type(kMovementSpeedBuff)->_slot(SpellSlot::W)->_delay(100)->_danger_level(kMedium)->_movespeed_delegate([](EvadingSpellData* inst)
			{
				return g_LocalPlayer->MoveSpeed() * (1.0f + 0.06f + 0.04f * g_LocalPlayer->GetSpellbook()->GetSpell(inst->SpellSlot())->Level());
			})
			);
		}
		else if (championid == ChampionId::Udyr)
		{
			evading_spells.push_back
			(
				InitializeEvadingSpell()->_spell_type(kMovementSpeedBuff)->_slot(SpellSlot::E)->_delay(100)->_danger_level(kMedium)->_movespeed_delegate([](EvadingSpellData* inst)
			{
				return g_LocalPlayer->MoveSpeed() * (1.0f + 0.1f + 0.05f * g_LocalPlayer->GetSpellbook()->GetSpell(inst->SpellSlot())->Level());
			})
			);
		}
		else if (championid == ChampionId::Zilean)
		{
			evading_spells.push_back
			(
				InitializeEvadingSpell()->_spell_type(kMovementSpeedBuff)->_slot(SpellSlot::E)->_delay(100)->_danger_level(kMedium)->_movespeed_delegate([](EvadingSpellData* inst)
			{
				return g_LocalPlayer->MoveSpeed() * 1.55f;
			})
			);
		}
#pragma endregion 

#pragma region Dashes
		else if (championid == ChampionId::Ahri)
		{
			evading_spells.push_back
			(
				InitializeEvadingSpell()->_spell_type(kDash)->_slot(SpellSlot::R)->_delay(0)->_range(450)->_speed(1500)->_danger_level(kExtreme)->_is_fixed_range(FALSE)
			);
		}
		else if (championid == ChampionId::Alistar)
		{
			evading_spells.push_back
			(
				InitializeEvadingSpell()->_spell_type(kDash)->_slot(SpellSlot::W)->_delay(100)->_range(650)->_speed(1900)->_danger_level(kHigh)->_is_fixed_range(FALSE)->
				_target_flags(kEnemyChampions | kEnemyMinions)
			);
		}
		else if (championid == ChampionId::Caitlyn)
		{
			evading_spells.push_back
			(
				InitializeEvadingSpell()->_spell_type(kDash)->_slot(SpellSlot::E)->_delay(250)->_range(390)->_speed(500)->_danger_level(kMedium)->_is_fixed_range(TRUE)->
				_invert(TRUE)
			);
		}
		else if (championid == ChampionId::Corki)
		{
			evading_spells.push_back
			(
				InitializeEvadingSpell()->_spell_type(kDash)->_slot(SpellSlot::W)->_delay(250)->_range(600)->_speed(1050)->_danger_level(kMedium)->_is_fixed_range(FALSE)
			);
		}
		else if (championid == ChampionId::Diana)
		{
			evading_spells.push_back
			(
				InitializeEvadingSpell()->_spell_type(kDash)->_slot(SpellSlot::R)->_delay(250)->_range(825)->_speed(2000)->_danger_level(kHigh)->_is_fixed_range(FALSE)->
				_target_flags(kEnemyChampions | kEnemyMinions)
			);
		}
		else if (championid == ChampionId::Fizz)
		{
			evading_spells.push_back
			(
				InitializeEvadingSpell()->_spell_type(kDash)->_slot(SpellSlot::Q)->_delay(100)->_range(550)->_speed(1400)->_danger_level(kMedium)->_is_fixed_range(FALSE)->
				_target_flags(kEnemyChampions | kEnemyMinions)
			);
		}
		else if (championid == ChampionId::Gragas)
		{
			evading_spells.push_back
			(
				InitializeEvadingSpell()->_spell_type(kDash)->_slot(SpellSlot::E)->_delay(250)->_range(600)->_speed(911)->_danger_level(kMedium)->_is_fixed_range(TRUE)
			);
		}
		else if (championid == ChampionId::Gnar)
		{
			evading_spells.push_back
			(
				InitializeEvadingSpell()->_spell_type(kDash)->_slot(SpellSlot::E)->_delay(100)->_range(475)->_speed(900)->_danger_level(kMedium)->_is_fixed_range(FALSE)->
				_reqiured_spellbook_spellname("GnarE")
			);
		}
		else if (championid == ChampionId::Graves)
		{
			evading_spells.push_back
			(
				InitializeEvadingSpell()->_spell_type(kDash)->_slot(SpellSlot::E)->_delay(100)->_range(425)->_speed(1250)->_danger_level(kMedium)->_is_fixed_range(TRUE)
			);
		}
		else if (championid == ChampionId::Jax)
		{
			evading_spells.push_back
			(
				InitializeEvadingSpell()->_spell_type(kDash)->_slot(SpellSlot::E)->_delay(100)->_range(700)->_speed(1400)->_danger_level(kMedium)->_is_fixed_range(FALSE)->
				_target_flags(0xFFFFFFFF)
			);
		}
		else if (championid == ChampionId::Kindred)
		{
			evading_spells.push_back
			(
				InitializeEvadingSpell()->_spell_type(kDash)->_slot(SpellSlot::Q)->_delay(250)->_range(340)->_speed(1250)->_danger_level(kMedium)->_is_fixed_range(TRUE)
			);
		}
		else if (championid == ChampionId::Leblanc)
		{
			evading_spells.push_back
			(
				InitializeEvadingSpell()->_spell_type(kDash)->_slot(SpellSlot::W)->_delay(100)->_range(600)->_speed(1600)->_danger_level(kMedium)->_is_fixed_range(FALSE)->
				_reqiured_spellbook_spellname("LeblancSlide")->_spell_name("Leblanc W")
			);

			evading_spells.push_back
			(
				InitializeEvadingSpell()->_spell_type(kDash)->_slot(SpellSlot::W)->_delay(100)->_range(600)->_speed(1600)->_danger_level(kMedium)->_is_fixed_range(FALSE)->
				_reqiured_spellbook_spellname("LeblancSlide")->_spell_name("Leblanc RW")
			);
		}
		else if (championid == ChampionId::LeeSin)
		{
			evading_spells.push_back
			(
				InitializeEvadingSpell()->_spell_type(kDash)->_slot(SpellSlot::W)->_delay(250)->_range(700)->_speed(2000)->_danger_level(kMedium)->_is_fixed_range(FALSE)->
				_reqiured_spellbook_spellname("BlindMonkWOne")->_target_flags(kAllyChampions | kAllyMinions | kAllyWards)
			);
		}
		else if (championid == ChampionId::Lucian)
		{
			evading_spells.push_back
			(
				InitializeEvadingSpell()->_spell_type(kDash)->_slot(SpellSlot::E)->_delay(100)->_range(425)->_speed(1350)->_danger_level(kMedium)->_is_fixed_range(FALSE)
			);
		}
		else if (championid == ChampionId::Nidalee)
		{
			evading_spells.push_back
			(
				InitializeEvadingSpell()->_spell_type(kDash)->_slot(SpellSlot::W)->_delay(250)->_range(375)->_speed(943)->_danger_level(kMedium)->_is_fixed_range(TRUE)->
				_reqiured_spellbook_spellname("Pounce")
			);
		}
		else if (championid == ChampionId::Pantheon)
		{
			evading_spells.push_back
			(
				InitializeEvadingSpell()->_spell_type(kDash)->_slot(SpellSlot::W)->_delay(250)->_range(600)->_speed(1000)->_danger_level(kMedium)->_is_fixed_range(FALSE)->
				_target_flags(kEnemyChampions | kEnemyMinions)
			);
		}
		else if (championid == ChampionId::Riven)
		{
			evading_spells.push_back
			(
				InitializeEvadingSpell()->_spell_type(kDash)->_slot(SpellSlot::Q)->_delay(250)->_range(275)->_speed(560)->_danger_level(kMedium)->_is_fixed_range(TRUE)->
				_requires_premove(TRUE)
			);

			evading_spells.push_back
			(
				InitializeEvadingSpell()->_spell_type(kDash)->_slot(SpellSlot::E)->_delay(250)->_range(250)->_speed(1200)->_danger_level(kMedium)->_is_fixed_range(FALSE)
			);
		}
		else if (championid == ChampionId::Tristana)
		{
			evading_spells.push_back
			(
				InitializeEvadingSpell()->_spell_type(kDash)->_slot(SpellSlot::W)->_delay(450)->_range(900)->_speed(800)->_danger_level(kMedium)->_is_fixed_range(FALSE)
			);
		}
		else if (championid == ChampionId::Tryndamere)
		{
			evading_spells.push_back
			(
				InitializeEvadingSpell()->_spell_type(kDash)->_slot(SpellSlot::E)->_delay(250)->_range(650)->_speed(900)->_danger_level(kMedium)->_is_fixed_range(FALSE)
			);
		}
		else if (championid == ChampionId::Vayne)
		{
			evading_spells.push_back
			(
				InitializeEvadingSpell()->_spell_type(kDash)->_slot(SpellSlot::Q)->_delay(250)->_range(300)->_speed(910)->_danger_level(kMedium)->_is_fixed_range(TRUE)
			);
		}
		else if (championid == ChampionId::MonkeyKing)
		{
			evading_spells.push_back
			(
				InitializeEvadingSpell()->_spell_type(kDash)->_slot(SpellSlot::E)->_delay(100)->_range(650)->_speed(1400)->_danger_level(kMedium)->_is_fixed_range(FALSE)->
				_target_flags(kEnemyChampions | kEnemyMinions)
			);
		}
		else if (championid == ChampionId::Yasuo)
		{
			evading_spells.push_back
			(
				InitializeEvadingSpell()->_spell_name("YasuoE")->_spell_type(kDash)->_slot(SpellSlot::E)->_delay(100)->_range(475)->_speed(1000)->_danger_level(kMedium)->_is_fixed_range(TRUE)->
				_target_flags(kEnemyChampions | kEnemyMinions)
			);
		}
#pragma endregion

#pragma region Blinks
		else if (championid == ChampionId::Ezreal)
		{
			evading_spells.push_back
			(
				InitializeEvadingSpell()->_spell_type(kBlink)->_slot(SpellSlot::E)->_delay(400)->_range(450)->_danger_level(kMedium)->_is_fixed_range(FALSE)
			);
		}
		else if (championid == ChampionId::Kassadin)
		{
			evading_spells.push_back
			(
				InitializeEvadingSpell()->_spell_type(kBlink)->_slot(SpellSlot::R)->_delay(200)->_range(700)->_danger_level(kMedium)->_is_fixed_range(FALSE)
			);
		}
		else if (championid == ChampionId::Shaco)
		{
			evading_spells.push_back
			(
				InitializeEvadingSpell()->_spell_type(kBlink)->_slot(SpellSlot::Q)->_delay(350)->_range(400)->_danger_level(kMedium)->_is_fixed_range(FALSE)
			);
		}
#pragma endregion

#pragma region Invulnerabilities
		else if (championid == ChampionId::Elise)
		{
			evading_spells.push_back
			(
				InitializeEvadingSpell()->_spell_type(kInvulnerability)->_slot(SpellSlot::E)->_delay(250)->_danger_level(kMedium)->
				_reqiured_spellbook_spellname("EliseSpiderEInitial")->
				_is_selfcast(TRUE)
			);
		}
		else if (championid == ChampionId::Vladimir)
		{
			evading_spells.push_back
			(
				InitializeEvadingSpell()->_spell_type(kInvulnerability)->_slot(SpellSlot::E)->_delay(250)->_danger_level(kHigh)->_is_selfcast(TRUE)
			);
		}
		else if (championid == ChampionId::Fizz)
		{
			evading_spells.push_back
			(
				InitializeEvadingSpell()->_spell_type(kInvulnerability)->_slot(SpellSlot::E)->_delay(250)->_danger_level(kHigh)
			);
		}
		else if (championid == ChampionId::MasterYi)
		{
			evading_spells.push_back
			(
				InitializeEvadingSpell()->_spell_type(kInvulnerability)->_slot(SpellSlot::Q)->_delay(250)->_range(600)->_danger_level(kMedium)->_target_flags(kEnemyChampions | kEnemyMinions)
			);
		}
		else if (championid == ChampionId::Xayah)
		{
			evading_spells.push_back
			(
				InitializeEvadingSpell()->_spell_type(kInvulnerability)->_slot(SpellSlot::R)->_delay(100)->_danger_level(kExtreme)->_try_cast_on_enemies(TRUE)
			);
		}
		else if (championid == ChampionId::Yasuo)
		{
			evading_spells.push_back
			(
				InitializeEvadingSpell()->_spell_type(kWindwall)->_slot(SpellSlot::W)->_delay(100)->_danger_level(kLow)
			);
		}

		auto const flash_spellslot = g_LocalPlayer->GetSpellbook()->GetSpellSlotFromName("SummonerFlash");
		if (flash_spellslot != SpellSlot::Invalid)
		{
			evading_spells.push_back
			(
				InitializeEvadingSpell()->_spell_name("Flash")->_spell_type(kInvulnerability)->_slot(flash_spellslot)->_delay(100)->_range(400)->_danger_level(kExtreme)->
				_is_fixed_range(FALSE)->_is_summonerspell(TRUE)
			);
		}
#pragma endregion

#pragma region Items
		evading_spells.push_back
		(
			InitializeEvadingSpell()->_spell_name("Blade of the Ruined King")->_spell_type(kMovementSpeedBuff)->_item_id(ItemId::Blade_of_the_Ruined_King)->
			_danger_level(kMedium)->_target_flags(kEnemyChampions)->_range(550)->_movespeed_delegate([](EvadingSpellData* inst)
		{
			return g_LocalPlayer->MoveSpeed() + 150;
		})
		);
		evading_spells.push_back
		(
			InitializeEvadingSpell()->_spell_name("Youmuu's Ghostblade")->_spell_type(kMovementSpeedBuff)->_item_id(ItemId::Youmuus_Ghostblade)->
			_danger_level(kMedium)->_movespeed_delegate([](EvadingSpellData* inst)
		{
			return g_LocalPlayer->MoveSpeed() * 1.2f;
		})
		);
		evading_spells.push_back
		(
			InitializeEvadingSpell()->_spell_name("Hextech Protobelt 01")->_spell_type(kDash)->_item_id(ItemId::Hextech_Protobelt_01)->
			_danger_level(kMedium)->_range(250)->_speed(1000)->_is_fixed_range(TRUE)
		);
		evading_spells.push_back
		(
			InitializeEvadingSpell()->_spell_name("Zhonya's Hourglass")->_spell_type(kInvulnerability)->_item_id(ItemId::Zhonyas_Hourglass)->
			_danger_level(kExtreme)
		);
#pragma endregion

#pragma region Shields
		if (championid == ChampionId::Karma)
		{
			evading_spells.push_back
			(
				InitializeEvadingSpell()->_spell_type(kShield)->_slot(SpellSlot::E)->_delay(100)->_range(800)->_danger_level(kLow)->_can_shield_allies(TRUE)
			);
		}
		else if (championid == ChampionId::Janna)
		{
			evading_spells.push_back
			(
				InitializeEvadingSpell()->_spell_type(kShield)->_slot(SpellSlot::E)->_delay(100)->_range(800)->_danger_level(kLow)->_can_shield_allies(TRUE)
			);
		}
		else if (championid == ChampionId::Morgana)
		{
			evading_spells.push_back
			(
				InitializeEvadingSpell()->_spell_type(kShield)->_slot(SpellSlot::E)->_delay(100)->_range(750)->_danger_level(kLow)->_can_shield_allies(TRUE)->_is_spellshield(TRUE)
			);
		}
#pragma  endregion

		return evading_spells;
	}

	EvadingSpellData* InitializeEvadingSpell()
	{
		return new EvadingSpellData;
	}
}