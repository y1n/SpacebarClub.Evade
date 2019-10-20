#include "SpellDetector.h"
#include "SpellDb.h"
#include "ISpecialSpell.h"
#include <regex>

namespace SpellDetector
{
	std::vector<SkillshotInstance*> detected_skillshots;
	std::vector<SkillshotInstance*> enabled_skillshots;
	std::vector<SkillshotInstance*> skillshots_in_danger;

	void Load()
	{
		SpellDb::Initialize();
	}

	void Unload()
	{
		if (!SpellDb::spells.empty())
		{
			for (auto& x : SpellDb::spells)
				delete x;

			SpellDb::spells.clear();
		}

		if (!detected_skillshots.empty())
		{
			for (auto& x : detected_skillshots)
				delete x;

			detected_skillshots.clear();
		}

		if (!enabled_skillshots.empty())
		{
			enabled_skillshots.clear();
		}

		if (!skillshots_in_danger.empty())
		{
			skillshots_in_danger.clear();
		}

		if (!SpellDb::special_spells.empty())
		{
			for (auto& x : SpellDb::special_spells)
				delete x;

			SpellDb::special_spells.clear();
		}
	}

	void OnTick()
	{
		detected_skillshots.erase(std::remove_if(detected_skillshots.begin(), detected_skillshots.end(), [](SkillshotInstance* x)
		{
			if (!x->IsValid())
			{
				delete x;
				return true;
			}

			return false;
		}), detected_skillshots.end());

		if (!skillshots_in_danger.empty())
			skillshots_in_danger.clear();

		if (!enabled_skillshots.empty())
			enabled_skillshots.clear();

		for (auto const& skillshot : detected_skillshots)
		{
			if (skillshot->IsEnabled())
			{
				if (!skillshot->IsSafePoint(Evade::player_position))
					skillshots_in_danger.push_back(skillshot);

				enabled_skillshots.push_back(skillshot);
			}

			skillshot->OnTick();
		}
	}

	void OnDraw()
	{
		const auto draw_linear = Evade::drawing_menu->GetElement("draw_linear")->GetBool();
		const auto draw_circular = Evade::drawing_menu->GetElement("draw_circular")->GetBool();

		for (auto const& skillshot : detected_skillshots)
		{
			if (skillshot->rectangle && !draw_linear)
			{
				continue;
			}

			if (skillshot->circle && !draw_circular)
			{
				continue;
			}

			skillshot->Draw();
		}
	}

	void OnCreate(IGameObject* sender)
	{
		if (sender->IsMissileClient())
		{
			auto const caster = sender->MissileSender();
			auto spell_data = sender->MissileSpellData();

			if (caster->IsValid() && caster->IsAIHero() && (caster->IsEnemy() || Config::IsDebugEnabled))
			{
				for (auto const& special_spell : SpellDb::special_spells)
				{
					if (special_spell->OnCreateMissile(sender, caster, &spell_data))
					{
						return;
					}
				}

				for (auto const& skillshot_data : SpellDb::spells)
				{
					if (std::find_if(skillshot_data->missile_names.begin(), skillshot_data->missile_names.end(), [&spell_data](std::string const& x)
					{
						return StringEquals(spell_data.SpellName.c_str(), x.c_str(), TRUE);
					}) != skillshot_data->missile_names.end())
					{
						AddSkillshot(caster, sender->MissileStartPosition(), sender->MissileEndPosition(), skillshot_data, 0, sender);
						break;
					}
				}
			}
		}
		else if (sender->Type() == EntityType::obj_GeneralParticleEmitter)
		{
			for (auto const& special_spell : SpellDb::special_spells)
				special_spell->OnCreateParticle(sender);
		}
	}
	
	void OnDelete(IGameObject* sender)
	{
		if (sender->IsMissileClient())
		{
			auto const caster = sender->MissileSender();
			auto spell_data = sender->MissileSpellData();

			if (caster->IsValid() && caster->IsAIHero() && (caster->IsEnemy() || Config::IsDebugEnabled))
			{
				for (auto const& special_spell : SpellDb::special_spells)
				{
					if (special_spell->OnDeleteMissile(sender, caster, &spell_data))
					{
						return;
					}
				}

				detected_skillshots.erase(
					std::remove_if(detected_skillshots.begin(), detected_skillshots.end(), [&](SkillshotInstance* skillshot)
				{
					if (skillshot->IsValid() && skillshot->skillshot_data->can_be_deleted)
					{
						if (skillshot->missile_client != nullptr && skillshot->missile_client == sender)
						{
							if (skillshot->skillshot_data->toggle_name.empty() || skillshot->skillshot_data->skillshot_type != kCircle)
							{
								delete skillshot;
								return true;
							}
						}
					}
					return false;
				}), detected_skillshots.end());
			}
		}
		else if (sender->Type() == EntityType::obj_GeneralParticleEmitter)
		{
			for (auto const& special_spell : SpellDb::special_spells)
			{
				if (special_spell->OnDeleteParticle(sender))
				{
					return;
				}
			}

			detected_skillshots.erase(
				std::remove_if(detected_skillshots.begin(), detected_skillshots.end(), [&](SkillshotInstance* skillshot)
			{
				if (skillshot->IsValid())
				{
					if (!skillshot->skillshot_data->toggle_name.empty() && skillshot->end_position.Distance(sender->Position()) < 400)
					{
						const std::regex r(skillshot->skillshot_data->toggle_name.c_str());

						if (std::regex_match(sender->Name().c_str(), r, std::regex_constants::match_flag_type::match_default))
						{
							delete skillshot;
							return true;
						}
					}
				}
				return false;
			}), detected_skillshots.end());
		}
	}

	void OnProcessSpell(IGameObject* sender, OnProcessSpellEventArgs* args)
	{
		if ((sender->IsEnemy() || Config::IsDebugEnabled) && !args->IsAutoAttack && sender->IsAIHero())
		{
			for (auto const& special_spell : SpellDb::special_spells)
			{
				if (special_spell->OnProcessSpellCast(sender, args))
				{
					return;
				}
			}

			for (auto skillshot_data : SpellDb::spells)
			{
				if (skillshot_data->champion_name == sender->ChampionName())
				{
					if (StringEquals(skillshot_data->spell_name.c_str(), args->SpellData->SpellName.c_str(), TRUE) ||
						std::find_if(skillshot_data->extra_spell_names.begin(), skillshot_data->extra_spell_names.end(), [args](std::string const& x)
					{
						return StringEquals(x.c_str(), args->SpellData->SpellName.c_str(), TRUE);
					}) != skillshot_data->extra_spell_names.end())
					{
						auto start_position = args->StartPosition;
						auto end_position = args->EndPosition;

						if (!skillshot_data->start_from_particle.empty() || !skillshot_data->end_at_particle.empty())
						{
							g_ObjectManager->GetByType(EntityType::obj_GeneralParticleEmitter, [&](IGameObject* x)
							{
								if (!x->IsDead())
								{									
									if (!skillshot_data->start_from_particle.empty() && StringEquals(x->Name().c_str(), skillshot_data->start_from_particle.c_str(), TRUE))
									{
										start_position = x->Position();
									}

									if (!skillshot_data->end_at_particle.empty() && StringEquals(x->Name().c_str(), skillshot_data->end_at_particle.c_str(), TRUE))
									{
										end_position = x->Position();
									}
								}
								return false;
							});
						}

						if (!skillshot_data->start_from_object.empty() || !skillshot_data->end_at_object.empty())
						{
							g_ObjectManager->GetByType(EntityType::AIMinionClient, [&](IGameObject* x)
							{
								if (!x->IsDead() && x->Team() == sender->Team())
								{
									if (!skillshot_data->start_from_object.empty() && (StringEquals(x->Name().c_str(), skillshot_data->start_from_object.c_str(), TRUE) || StringEquals(x->BaseSkinName().c_str(), skillshot_data->start_from_object.c_str(), TRUE)))
									{
										start_position = x->Position();
									}
									if (!skillshot_data->end_at_object.empty() && (StringEquals(x->Name().c_str(), skillshot_data->end_at_object.c_str(), TRUE) || StringEquals(x->BaseSkinName().c_str(), skillshot_data->end_at_object.c_str(), TRUE)))
									{
										end_position = x->Position();
									}
								}
								return false;
							});
						}

						if (skillshot_data->multiple_number && (sender->ChampionId() != ChampionId::Khazix || sender->IsSpellEvolved(SpellSlot::W)))
						{
							const auto dir = (end_position - start_position).Normalized();
							for (int i = -(skillshot_data->multiple_number - 1) / 2; i <= (skillshot_data->multiple_number - 1) / 2; i++)
							{
								AddSkillshot(sender, start_position, start_position + dir.Rotated(skillshot_data->multiple_angle * i) * skillshot_data->Range(), skillshot_data, 0, nullptr);
							}
						}
						else
						{
							AddSkillshot(sender, start_position, end_position, skillshot_data, 0, nullptr);
						}
						break;
					}
				}
			}
		}
	}

	void OnNewPath(IGameObject* sender, OnNewPathEventArgs* args)
	{
		for (auto const& special_spell : SpellDb::special_spells)
		{
			special_spell->OnNewPath(sender, args);
		}
	}

	SkillshotInstance* AddSkillshot(IGameObject* caster, Vector const& start_pos, Vector const& end_pos, SkillshotData* skillshot_data, unsigned int start_time, IGameObject* missile_client)
	{
		if (skillshot_data)
		{
			const auto is_from_fog_of_war = caster == nullptr || !caster->IsVisible() && missile_client->IsValid();

			if (!Config::IsDebugEnabled && g_LocalPlayer->Distance(start_pos) > (skillshot_data->Range() + skillshot_data->Radius() + 1000) * 1.5f)
			{
				return nullptr;
			}

			auto start_position = start_pos;
			auto end_position = end_pos;
			auto direction = (end_position - start_position).Normalized();
			start_time = start_time > 0 ? start_time : g_Common->TickCount() - g_Common->Ping() / 2;
			auto end_time = skillshot_data->delay;

			if (!missile_client)
			{
				if (skillshot_data->behind_start > 0)
				{
					start_position = start_position.Extend(end_position, -skillshot_data->behind_start);
				}

				if (skillshot_data->infront_start > 0)
				{
					start_position = start_position.Extend(end_position, skillshot_data->infront_start);
				}
			}
			else
			{
				if (!skillshot_data->missile_delayed)
				{
					start_time -= skillshot_data->delay;
				}

				if (skillshot_data->missile_speed != 0)
				{
					start_time -= static_cast<int>(start_position.Distance(missile_client->Position()) / skillshot_data->missile_speed * 1000);
				}
			}

			if (skillshot_data->skillshot_type == kCone || skillshot_data->skillshot_type == kMissileCone ||
				skillshot_data->is_fixed_range || (skillshot_data->raw_range > 0 && end_position.Distance(start_position) > skillshot_data->raw_range))
			{
				end_position = start_position.Extend(end_position, skillshot_data->Range());
			}

			if (!missile_client)
			{
				if (skillshot_data->minimum_range)
				{
					if (start_position.Distance(end_position) < skillshot_data->minimum_range)
					{
						end_position = start_position.Extend(end_position, skillshot_data->minimum_range);
					}
				}

				if (skillshot_data->invert)
				{
					end_position = start_position.Extend(end_position, -start_position.Distance(end_position));
				}

				if (skillshot_data->perpendicular)
				{
					start_position = end_pos - direction.Perpendicular() * skillshot_data->radius_ex;
					end_position = end_pos + direction.Perpendicular() * skillshot_data->radius_ex;
				}
			}

			if (skillshot_data->extra_range > 0)
			{
				end_position = end_position.Extend(start_position, -fmin(skillshot_data->extra_range, skillshot_data->Range() - end_position.Distance(start_position)));
			}

			switch (skillshot_data->skillshot_type)
			{
			case kMissileLine:
			{
				end_time += skillshot_data->missile_accel != 0
					? 5000
					: static_cast<int>(start_position.Distance(end_position) / skillshot_data->missile_speed * 1000);
				break;
			}
			case kCircle:
			{
				if (skillshot_data->missile_speed != 0)
				{
					end_time += static_cast<int>(start_position.Distance(end_position) / skillshot_data->missile_speed * 1000);
				}
				else if (skillshot_data->raw_range == 0 && skillshot_data->raw_radius > 0)
				{
					end_position = start_position;
				}
				break;
			}
			case kArc:
			case kMissileCone:
			{
				end_time += static_cast<int>(start_position.Distance(end_position) / skillshot_data->missile_speed * 1000);
				break;
			}
			default: break;
			}

			direction = (end_position - start_position).Normalized();
			bool already_added = false;

			if (!missile_client)
			{
				if (!skillshot_data->dont_scan_for_duplicates)
				{
					for (auto const &skillshot : detected_skillshots)
					{
						if (skillshot->IsValid() && skillshot->skillshot_data->MenuName() == skillshot_data->MenuName() &&
							skillshot->caster == caster && skillshot->direction.AngleBetween(direction) < 3 /*&&
							skillshot->start_position.Distance(start_position) < 100*/)
						{
							already_added = true;
							break;
						}
					}
				}
			}
			else
			{
				for (auto const &skillshot : detected_skillshots)
				{
					if (skillshot->IsValid() && skillshot->missile_client == nullptr)
					{
						if (skillshot->skillshot_data->MenuName() == skillshot_data->MenuName() &&
							skillshot->caster == caster && skillshot->direction.AngleBetween(direction) < 3 /*&&
							skillshot->start_position.Distance(start_position) < 100*/)
						{
							skillshot->missile_client = missile_client;
							//skillshot->start_position = missile_client->MissileStartPosition();

							already_added = true;
							break;
						}
					}
				}
			}

			if (already_added)
			{
				return nullptr;
			}

			const auto skillshot_instance = new SkillshotInstance(skillshot_data, start_position, end_position, start_time, end_time + skillshot_data->extra_delay, caster);
			skillshot_instance->is_from_fow = is_from_fog_of_war;
			skillshot_instance->missile_client = missile_client;

			for (auto const& special_spell : SpellDb::special_spells)
			{
				if (special_spell->OnSkillshotAdd(skillshot_instance))
				{
					delete skillshot_instance;
					return nullptr;
				}
			}

			detected_skillshots.push_back(skillshot_instance);
			return skillshot_instance;
		}

		return nullptr;
	}
}