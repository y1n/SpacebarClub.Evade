#include "SkillshotData.h"

SkillshotData::SkillshotData()
{
	this->spell_slot = SpellSlot::Q;
	this->skillshot_type = kMissileLine;
	this->danger_level = kLow;
	this->delay = 250;
	this->missile_speed = INT_MAX;
	this->can_be_deleted = true;
}

std::string SkillshotData::MenuName() const
{
	if (this->menu_name.empty())
	{
		auto result = this->champion_name;

		result += ".";
		result += this->spell_name;

		return result;
	}

	return this->menu_name;
}

std::string SkillshotData::MenuDisplayName() const
{
	if (this->menu_display_name.empty())
	{
		auto result = this->spell_name;
		std::string spell_slot_name;

		switch (this->spell_slot)
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

	return this->menu_display_name;
}

unsigned int SkillshotData::Range() const
{
	if (this->skillshot_type == kMissileLine ||
		this->skillshot_type == kLine ||
		this->skillshot_type == kCone ||
		this->skillshot_type == kMissileCone)
	{
		return this->raw_range + Config::AdditionalRange;
	}

	return this->raw_range;
}

unsigned int SkillshotData::Radius() const
{
	if (!this->dont_add_bounding_radius)
	{
		return this->raw_radius + Config::AdditionalRadius;
	}

	return this->raw_radius + static_cast<unsigned int>(g_LocalPlayer->BoundingRadius()) + Config::AdditionalRadius;
}

bool SkillshotData::CollidesOnlyWithYasuoWall() const
{
	return (this->collision_flags ^ kYasuoWall) == kYasuoWall;
}

SkillshotData* InitializeSpell(std::string const& champion)
{
	auto result = new SkillshotData();
	result->_champion_name(champion);

	return result;
}