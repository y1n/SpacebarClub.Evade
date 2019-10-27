#include "Evade.h"

#include "SpellDetector.h"
#include "Collision.h"
#include "SpellDb.h"
#include "EvadeSpells.h"
#include "Evader.h"
#include "PathFinding.h"
#include "SpellBlocker.h"

namespace Evade
{
	IMenu* menu = nullptr;
	IMenu* evading_spells_menu = nullptr;
	IMenu* spells_menu = nullptr;
	IMenu* collision_menu = nullptr;
	IMenu* config_menu = nullptr;
	IMenu* drawing_menu = nullptr;
	IMenu* colors_menu = nullptr;

	std::map<std::string, SpellMenuConfig*> spell_configs;
	
	IMenuElement* IsEvadeEnabled;
	IMenuElement* IsDangerousOnly;
	IMenuElement* IsWalkingEnabled;

	Vector player_position;
	Vector previous_position;
	Vector EvadePoint;
	Vector EvadeToPoint;

	int LastSentMovePacketT = 0;
	int LastSentMovePacketT2 = 0;
	int LastEvadePointChangeT = 0;
	int LastWardJumpTick = 0;

	bool NoSolutionFound = false;
	bool is_evading = false;
	bool force_follow_path = false;

	class ITask
	{
	public:
		std::function<void()> m_callback;
		int m_time;
		int m_start_tick;

		ITask()
		{
			this->m_callback = nullptr;
			this->m_time = 0;
			this->m_start_tick = g_Common->TickCount();
		}
		ITask(int time, std::function<void()> callback) {
			this->m_callback = callback;
			this->m_time = time;
			this->m_start_tick = g_Common->TickCount();
		}
	};

	std::vector<ITask> debug_skillshots;

	void OnDraw();
	void OnCreate(IGameObject* sender);
	void OnDelete(IGameObject* sender);
	void OnProcessSpell(IGameObject* sender, OnProcessSpellEventArgs* args);
	void OnWndProc(OnWndProcEventArgs* args);
	SpellSlot GetWardSlot();

	bool Load()
	{
		SpellDetector::Load();
		Collision::Initialize();

		if (SpellDb::spells.empty())
		{
			Unload();

			return false;
		}	

		/* Menu impl */
		std::map<std::string, SkillshotData*> champion_spells;
		auto evading_spells = EvadeSpellsDb::GetEvadingSpells();

		menu = g_Menu->CreateMenu("Evade", "evade");

		if (!evading_spells.empty())
		{
			evading_spells_menu = menu->AddSubMenu("Spell usage", "evading_spells");
			{
				for (auto const& spell : evading_spells)
				{
					IMenu* spell_menu = nullptr;

					if (evading_spells_menu->ElementExists(spell->MenuName()))
					{
						spell_menu = evading_spells_menu->GetSubMenu(spell->MenuName());
					}
					else
					{
						spell_menu = evading_spells_menu->AddSubMenu(spell->MenuDisplayName(), spell->MenuName());
					}

					if (spell->can_shield_allies)
					{
						auto shield_menu = spell_menu->AddSubMenu("Shielding", "shielding");

						for (auto const& ally : g_ObjectManager->GetChampions())
						{
							if (!ally->IsAlly())
								continue;

							if (shield_menu->ElementExists(ally->ChampionName()))
							{
								continue;
							}

							shield_menu->AddCheckBox(ally->ChampionName(), ally->ChampionName() + ".Shield", TRUE);
						}
					}

					if (!spell_menu->ElementExists(spell->MenuName() + ".DangerLevel"))
					{
						spell_menu->AddComboBox("Danger Level", spell->MenuName() + ".DangerLevel", { "Low", "Medium", "High", "Extreme" }, spell->danger_level);
					}

					if (spell->IsTargeted() && (spell->target_flags & kAllyWards))
					{
						if (!spell_menu->ElementExists(spell->MenuName() + ".WardJump"))
						{
							spell_menu->AddCheckBox("Ward jump", spell->MenuName() + ".WardJump", FALSE);
						}
					}

					if (!spell_menu->ElementExists(spell->MenuName() + ".IsEnabled"))
					{
						if (spell->spell_name == "Walking")
						{
							IsWalkingEnabled = spell_menu->AddCheckBox("Enabled", spell->MenuName() + ".IsEnabled", !spell->is_disabled_by_default);
						}
						else
						{
							spell_menu->AddCheckBox("Enabled", spell->MenuName() + ".IsEnabled", !spell->is_disabled_by_default);
						}
					}
				}
			}
		}

		spells_menu = menu->AddSubMenu("Skillshots", "spells");
		{
			for (auto const& spell : SpellDb::spells)
			{
				IMenu* champion_menu;
				IMenu* spell_menu;

				if (spells_menu->ElementExists(spell->champion_name))
				{
					champion_menu = spells_menu->GetSubMenu(spell->champion_name);
				}
				else
				{
					champion_menu = spells_menu->AddSubMenu(spell->champion_name, spell->champion_name);
				}

				if (champion_menu->ElementExists(spell->MenuName()))
				{
					spell_menu = champion_menu->GetSubMenu(spell->MenuName());
				}
				else
				{
					spell_menu = champion_menu->AddSubMenu(spell->MenuDisplayName(), spell->MenuName());
				}

				auto const is_enabled = spell_menu->AddCheckBox("Enabled", spell->MenuName() + ".IsEnabled", !spell->is_disabled_by_default);
				auto const is_dangerous = spell_menu->AddCheckBox("Is Dangerous", spell->MenuName() + ".IsDangerous", spell->is_dangerous);
				auto const draw = spell_menu->AddCheckBox("Draw", spell->MenuName() + ".Draw", TRUE);
				auto const danger_level = spell_menu->AddComboBox("Danger Level", spell->MenuName() + ".DangerLevel", { "Low", "Medium", "High", "Extreme" }, spell->danger_level);

				spell_configs[spell->MenuName()] = new SpellMenuConfig(spell_menu, is_enabled, is_dangerous, draw, danger_level);
			}
		}

		collision_menu = menu->AddSubMenu("Collision settings", "collision");
		{
			collision_menu->AddCheckBox("Enable Collision", "enabled", FALSE);
			collision_menu->AddCheckBox("Enable Collision on minions", "minions", TRUE);
			collision_menu->AddCheckBox("Enable Collision on champions", "champions", TRUE);
			collision_menu->AddCheckBox("Enable Collision on yasuo wall", "yasuo_wall", TRUE);
		}

		config_menu = menu->AddSubMenu("Config", "config_settings");
		{
			config_menu->AddCheckBox("Dodge skillshots from FoW", "fow_dodge", TRUE);
			config_menu->AddCheckBox("Dodge linear skillshots", "dodge_linear", TRUE);
			config_menu->AddCheckBox("Dodge circular skillshots", "dodge_circular", TRUE);
			config_menu->AddComboBox("Block spell casting while dodging", "block_spells", { "Always", "Only dangerous", "Never" }, 0);
		}

		drawing_menu = menu->AddSubMenu("Drawings", "drawing_settings");
		{
			drawing_menu->AddCheckBox("Draw circular skillshots", "draw_circular", TRUE);
			drawing_menu->AddCheckBox("Draw linear skillshots", "draw_linear", TRUE);
			drawing_menu->AddCheckBox("Draw status", "draw_status", TRUE);
			drawing_menu->AddCheckBox("Enable drawings", "enable_drawings", TRUE);
			
			colors_menu = drawing_menu->AddSubMenu("Colors", "color_settings");
			{
				colors_menu->AddColorPicker("Low danger spell color", "low_danger", 255, 255, 255, 255);
				colors_menu->AddColorPicker("Medium danger spell color", "medium_danger", 255, 255, 255, 255);
				colors_menu->AddColorPicker("High danger spell color", "high_danger", 255, 255, 255, 255);
				colors_menu->AddColorPicker("Extreme danger spell color", "extreme_danger", 255, 255, 255, 255);
			}
		}

		IsEvadeEnabled = menu->AddKeybind("Enable evade", "enabled", 'K', TRUE, KeybindType_Toggle);
		IsDangerousOnly = menu->AddKeybind("Dodge only dangerous spells", "dodge_only_dangerous", VK_SPACE, FALSE, KeybindType_Hold);

		/* Hooks */
		EventHandler<Events::GameUpdate>::AddEventHandler(OnUpdate);
		EventHandler<Events::OnIssueOrder>::AddEventHandler(OnIssueOrder);
		EventHandler<Events::OnCastSpell>::AddEventHandler(OnCastSpell);
		EventHandler<Events::OnBeforeAttackOrbwalker>::AddEventHandler(OnBeforeAttack);
		EventHandler<Events::OnHudDraw>::AddEventHandler(OnDraw);
		EventHandler<Events::OnCreateObject>::AddEventHandler(OnCreate);
		EventHandler<Events::OnDeleteObject>::AddEventHandler(OnDelete);
		EventHandler<Events::OnProcessSpellCast>::AddEventHandler(OnProcessSpell);
		EventHandler<Events::OnNewPath>::AddEventHandler(OnNewPath);

		if (Config::IsDebugEnabled)
			EventHandler<Events::OnWndProc>::AddEventHandler(OnWndProc);

		g_Common->Log("Evade-Core has been loaded.");
		return true;
	}

	void Unload()
	{
		EventHandler<Events::GameUpdate>::RemoveEventHandler(OnUpdate);
		EventHandler<Events::OnIssueOrder>::RemoveEventHandler(OnIssueOrder);
		EventHandler<Events::OnCastSpell>::RemoveEventHandler(OnCastSpell);
		EventHandler<Events::OnBeforeAttackOrbwalker>::RemoveEventHandler(OnBeforeAttack);
		EventHandler<Events::OnHudDraw>::RemoveEventHandler(OnDraw);
		EventHandler<Events::OnCreateObject>::RemoveEventHandler(OnCreate);
		EventHandler<Events::OnDeleteObject>::RemoveEventHandler(OnDelete);
		EventHandler<Events::OnProcessSpellCast>::RemoveEventHandler(OnProcessSpell);
		EventHandler<Events::OnNewPath>::RemoveEventHandler(OnNewPath);

		if (Config::IsDebugEnabled)
			EventHandler<Events::OnWndProc>::RemoveEventHandler(OnWndProc);

		Collision::Unload();
		SpellDetector::Unload();

		if (!spell_configs.empty())
		{
			for (auto& spell_config : spell_configs)
				delete spell_config.second;

			spell_configs.clear();
		}

		if (!EvadeSpellsDb::GetEvadingSpells().empty())
		{
			for (auto& spell : EvadeSpellsDb::GetEvadingSpells())
				delete spell;

			EvadeSpellsDb::GetEvadingSpells().clear();
		}

		if (menu)
		{
			menu->Remove();
		}

		menu = nullptr;
		spells_menu = nullptr;
		collision_menu = nullptr;
		config_menu = nullptr;
		IsEvadeEnabled = nullptr;
		IsDangerousOnly = nullptr;

		g_Common->Log("Evade-Core was unloaded.");
	}

	bool IsPlayerSpellShielded()
	{
		if (g_LocalPlayer->HasBuffOfType( BuffType::SpellShield ) || g_LocalPlayer->HasBuffOfType( BuffType::SpellImmunity ))
		{
			return true;
		}

		switch (g_LocalPlayer->ChampionId())
		{
		case ChampionId::Olaf:
		{
			if (g_LocalPlayer->HasBuff(hash("OlafRagnarok")))
				return true;

			break;
		}
		case ChampionId::Sion:
		{
			if (g_LocalPlayer->HasBuff(hash("SionR")))
				return true;

			break;
		}
		}

		/*
		 * TODO LastCastedSpell
		 */

		return false;
	}

	bool IsPlayerImmobile()
	{
		float buff_time = 0;
		for (auto const& buff : g_LocalPlayer->GetBuffList())
		{
			if (buff.Valid)
			{
				if (buff.Type == BuffType::Charm || buff.Type == BuffType::Knockup ||
					buff.Type == BuffType::Stun || buff.Type == BuffType::Suppression ||
					buff.Type == BuffType::Snare)
				{
					if (buff_time < buff.EndTime)
						buff_time = buff.EndTime;
				}
			}
		}

		const auto time = buff_time == 0 ? -1 : static_cast<int>(g_Common->TickCount() + (buff_time - g_Common->Time()) * 1000);
		return time != -1 && time - g_Common->TickCount() > g_Common->Ping() / 2 + 70;
	}

	bool IsEnabled()
	{
		if (IsEvadeEnabled)
		{
			return IsEvadeEnabled->GetBool();
		}

		return false;
	}

	bool DodgeOnlyDangerous()
	{
		if (IsDangerousOnly)
		{
			return IsDangerousOnly->GetBool();
		}

		return false;
	}

	bool IsAboutToHit(int time, IGameObject* unit)
	{
		time += 150;

		for (auto const& spell_instance : SpellDetector::enabled_skillshots)
		{
			if (spell_instance->IsAboutToHit(time, unit == nullptr ? g_LocalPlayer : unit))
			{
				return true;
			}
		}

		return false;
	}

	bool IsEvading()
	{
		return is_evading;
	}

	void SetEvading(bool b)
	{
		if (b)
		{
			force_follow_path = true;
			LastSentMovePacketT = 0;
			g_LocalPlayer->IssueOrder(IssueOrderType::MoveTo, EvadePoint, false);
		}

		is_evading = b;
	}
	
	void TryToEvade(Vector const& to)
	{
		int danger_level = kLow;

		for (auto const& skillshot_instance : SpellDetector::skillshots_in_danger)
		{
			const auto spell_danger_level = skillshot_instance->menu_config->danger_level->GetInt();

			if (spell_danger_level > danger_level)
				danger_level = spell_danger_level;
		}

		for (auto const& evade_spell : EvadeSpellsDb::GetEvadingSpells())
		{
			if (!evade_spell->IsEnabled() || evade_spell->DangerLevel() > danger_level)
				continue;

			if (evade_spell->spell_name == "Walking")
			{
				EvadingPosition evading_position;

				if (Evader::GetEvadingPosition(evading_position, EvadeToPoint))
				{
					EvadePoint = evading_position.end_position;

					auto nEvadePoint = EvadePoint.Extend(player_position, -100);
					if (Evader::IsPathSafe(g_LocalPlayer->CreatePath(nEvadePoint),
						Config::EvadingSecondTime, g_LocalPlayer->MoveSpeed(), 100).is_safe)
					{
						EvadePoint = nEvadePoint;
					}

					SetEvading(true);
					return;
				}
			}

			if (evade_spell->IsReady())
			{
				if (evade_spell->is_spellshield)
				{
					if (IsAboutToHit(evade_spell->delay))
					{
						g_LocalPlayer->CastSpell(evade_spell->SpellSlot(), g_LocalPlayer);
					}

					NoSolutionFound = true;
					return;
				}

				if (evade_spell->spell_type & kMovementSpeedBuff)
				{
					EvadingPosition result;

					if (Evader::GetEvadingPosition(result, to, evade_spell->MoveSpeedAmount(), evade_spell->delay))
					{
						EvadePoint = result.end_position;
						SetEvading(true);

						g_LocalPlayer->CastSpell(evade_spell->SpellSlot(), g_LocalPlayer);
						return;
					}
				}

				if (evade_spell->spell_type & kDash)
				{
					if (evade_spell->IsTargeted())
					{
						auto target = Evader::GetBestTarget(to, evade_spell);

						if (target != nullptr)
						{
							EvadePoint = target->ServerPosition();
							SetEvading(true);

							g_LocalPlayer->CastSpell(evade_spell->SpellSlot(), target);
							return;
						}

						if (g_Common->TickCount() - LastWardJumpTick < 250)
						{
							NoSolutionFound = true;
							return;
						}

						if (evade_spell->TargetFlags() & kAllyWards)
						{
							const auto ward = GetWardSlot();

							if (ward != SpellSlot::Invalid)
							{
								EvadingPosition result;

								if (Evader::GetDashPosition(result, to, evade_spell->speed, evade_spell->delay, 600, false))
								{
									LastWardJumpTick = g_Common->TickCount();
									NoSolutionFound = true;

									g_LocalPlayer->CastSpell(ward, result.end_position);
									return;
								}
							}
						}
					}
					else
					{
						EvadingPosition result;

						if (Evader::GetDashPosition(result, to, evade_spell->speed, evade_spell->delay, evade_spell->range, evade_spell->is_fixed_range))
						{
							EvadePoint = result.end_position;
							SetEvading(true);

							if (!evade_spell->invert)
							{
								if (evade_spell->requires_premove)
								{
									g_LocalPlayer->IssueOrder(IssueOrderType::MoveTo, EvadePoint, false);

									g_Common->DelayAction(g_Common->Ping() / 2 + 100, [evade_spell]()
									{
										g_LocalPlayer->CastSpell(evade_spell->SpellSlot(), EvadePoint);
									});
								}
								else
								{
									g_LocalPlayer->CastSpell(evade_spell->SpellSlot(), EvadePoint);
								}
							}
							else
							{
								g_LocalPlayer->CastSpell(evade_spell->SpellSlot(), player_position - (EvadePoint - player_position));
							}

							return;
						}
					}
				}

				if (evade_spell->spell_type & kBlink)
				{
					if (evade_spell->IsTargeted())
					{
						auto target = Evader::GetBestTarget(to, evade_spell);

						if (target != nullptr)
						{
							if (IsAboutToHit(evade_spell->delay))
							{
								EvadePoint = target->ServerPosition();
								SetEvading(true);
								g_LocalPlayer->CastSpell(evade_spell->SpellSlot(), target);
							}

							NoSolutionFound = true;
							return;
						}

						if (g_Common->TickCount() - LastWardJumpTick < 250)
						{
							NoSolutionFound = true;
							return;
						}

						if (evade_spell->TargetFlags() & kAllyWards)
						{
							const auto ward = GetWardSlot();

							if (ward != SpellSlot::Invalid)
							{
								EvadingPosition result;

								if (Evader::GetBlinkPosition(result, to, evade_spell->delay, 600))
								{
									LastWardJumpTick = g_Common->TickCount();
									NoSolutionFound = true;

									g_LocalPlayer->CastSpell(ward, result.end_position);
									return;
								}
							}
						}
					}
					else
					{
						EvadingPosition result;

						if (Evader::GetBlinkPosition(result, to, evade_spell->delay, evade_spell->range))
						{
							if (IsAboutToHit(evade_spell->delay))
							{
								EvadePoint = result.end_position;
								SetEvading(true);

								g_LocalPlayer->CastSpell(evade_spell->SpellSlot(), EvadePoint);
							}

							NoSolutionFound = true;
							return;
						}
					}
				}

				if (evade_spell->spell_type & kInvulnerability)
				{
					if (evade_spell->item_id == ItemId::Zhonyas_Hourglass)
					{
						if (IsAboutToHit(100))
						{
							g_LocalPlayer->CastSpell(evade_spell->SpellSlot());
						}

						NoSolutionFound = true;
						return;
					}

					if (evade_spell->IsTargeted())
					{
						auto target = Evader::GetBestTarget(to, evade_spell);

						if (target != nullptr)
						{
							if (IsAboutToHit(evade_spell->delay))
							{
								EvadePoint = target->ServerPosition();
								SetEvading(true);
								g_LocalPlayer->CastSpell(evade_spell->SpellSlot(), target);
							}

							NoSolutionFound = true;
							return;
						}
					}
					else
					{

						if (IsAboutToHit(evade_spell->delay))
						{
							if (evade_spell->is_selfcast)
							{
								g_LocalPlayer->CastSpell(evade_spell->SpellSlot());
							}
							else
							{
								g_LocalPlayer->CastSpell(evade_spell->SpellSlot(), player_position);
							}
						}

						NoSolutionFound = true;
						return;
					}
				}

				if (evade_spell->spell_type & kShield)
				{
					if (IsAboutToHit(evade_spell->delay))
					{
						g_LocalPlayer->CastSpell(evade_spell->SpellSlot(), g_LocalPlayer);
					}

					NoSolutionFound = true;
					return;
				}
			}
		}

		NoSolutionFound = true;
	}

	void OnUpdate()
	{
		if (g_LocalPlayer->IsCastingInterruptibleSpell() || g_LocalPlayer->IsRecalling())
		{
			SetEvading(false);
			EvadeToPoint.x = 0;
			EvadeToPoint.y = 0;
		}

		player_position = g_LocalPlayer->ServerPosition();
		SpellDetector::OnTick();

		if (Config::IsDebugEnabled)
		{
			for (auto const& task : debug_skillshots)
			{
				if (g_Common->TickCount() - task.m_start_tick > task.m_time)
				{
					if (task.m_callback)
						task.m_callback();
				}
			}
		}

		if (previous_position.IsValid() && player_position.Distance(previous_position) > 200)
		{
			SetEvading(false);
			EvadeToPoint.x = 0;
			EvadeToPoint.y = 0;
		}

		previous_position = player_position;

		if (!IsEnabled() || IsPlayerImmobile() || IsPlayerSpellShielded())
		{
			SetEvading(false);
			EvadeToPoint.x = 0;
			EvadeToPoint.y = 0;
			return;
		}

		if (g_LocalPlayer->IsDead() || g_LocalPlayer->IsInvulnerable() || !g_LocalPlayer->IsTargetable() ||
			g_LocalPlayer->IsCastingInterruptibleSpell() || g_LocalPlayer->IsDashing())
		{
			SetEvading(false);
			EvadeToPoint = Vector(0, 0, 0);
			return;
		}

		if (g_LocalPlayer->IsWindingUp() && !g_LocalPlayer->GetSpellbook()->IsAutoAttacking())
		{
			SetEvading(false);
			return;
		}

		for (auto const& ally : g_ObjectManager->GetChampions())
		{
			if (!ally->IsAlly() || ally->IsMe() || !ally->IsValidTarget(1000, false))
				continue;

			const auto safety_check = Evader::IsPointSafe(ally->ServerPosition());

			if (safety_check)
				continue;

			int danger_level = kLow;

			for (auto const& skillshot_instance : SpellDetector::detected_skillshots)
			{
				if (skillshot_instance->IsValid() && !skillshot_instance->IsSafePoint(ally->ServerPosition()))
				{
					const auto spell_danger_level = skillshot_instance->menu_config->danger_level->GetInt();

					if (spell_danger_level > danger_level)
						danger_level = spell_danger_level;
				}
			}

			for (auto const& evadeSpell : EvadeSpellsDb::GetEvadingSpells())
			{
				if (evadeSpell->IsEnabled() || evadeSpell->DangerLevel() < danger_level || !evadeSpell->IsReady() || !evadeSpell->can_shield_allies)
					continue;

				if (evading_spells_menu->ElementExists(evadeSpell->MenuName()))
				{
					auto menu = evading_spells_menu->GetSubMenu(evadeSpell->MenuName());

					if (menu->ElementExists("shielding"))
					{
						auto shielding_menu = menu->GetSubMenu("shielding");

						if (shielding_menu->ElementExists(ally->ChampionName() + ".Shield"))
						{
							if (shielding_menu->GetElement(ally->ChampionName() + ".Shield")->GetBool())
							{
								if (ally->Distance(player_position) < evadeSpell->range && IsAboutToHit(evadeSpell->delay, ally))
								{
									if (g_LocalPlayer->CastSpell(evadeSpell->SpellSlot(), ally))
									{
										return;
									}
								}
							}
						}
					}
				}
			}
		}

		auto current_path = g_LocalPlayer->RealPath();
		auto is_my_position_safe = Evader::IsPointSafe(player_position);
		auto is_my_path_safe = Evader::IsPathSafe(current_path, 100);

		NoSolutionFound = false;

		//Continue evading
		if (IsEvading() && Evader::IsPointSafe(EvadePoint))
		{
			if (is_my_position_safe)
			{
				//We are safe, stop evading.
				SetEvading(false);
			}
			else
			{
				if (g_Common->TickCount() - LastSentMovePacketT > 1000 / 15)
				{
					LastSentMovePacketT = g_Common->TickCount();
					g_LocalPlayer->IssueOrder(IssueOrderType::MoveTo, EvadePoint, false);
				}

				return;
			}
		}
		//Stop evading if the point is not safe.
		else if (IsEvading())
		{
			SetEvading(false);
		}

		//The path is not safe.
		if (!is_my_path_safe.is_safe)
		{
			//Inside the danger polygon.
			if (!is_my_position_safe)
			{
				//Search for an evade point:
				TryToEvade(EvadeToPoint.IsValid() ? EvadeToPoint : g_Common->CursorPosition());
			}
		}

		/* FOLLOWPATH */
		if (!NoSolutionFound && !IsEvading() && EvadeToPoint.IsValid() && is_my_position_safe)
		{
			if (IsWalkingEnabled->GetBool())
			{
				if (is_my_path_safe.is_safe && !force_follow_path)
				{
					return;
				}

				if (g_Common->TickCount() - LastSentMovePacketT2 > 1000 / 15 || !Pathfinder::PathFollow::IsFollowing())
				{
					LastSentMovePacketT2 = g_Common->TickCount();

					if (SpellDetector::detected_skillshots.empty())
					{
						if (g_LocalPlayer->Distance(EvadeToPoint) > 75)
						{
							g_LocalPlayer->IssueOrder(IssueOrderType::MoveTo, EvadeToPoint, false);
						}

						return;
					}

					auto distance_to_point = player_position.Distance(EvadeToPoint);
					auto end_point = SpellDetector::enabled_skillshots.size() > 1 ?
						player_position.Extend(EvadeToPoint, distance_to_point > 250 ? 250 : distance_to_point) :
						EvadeToPoint;
					auto path_to_destination = g_LocalPlayer->CreatePath(end_point);
					auto is_path_to_destination_safe = Evader::IsPathSafe(path_to_destination, 100);

					if (is_path_to_destination_safe.is_safe)
					{
						if (g_LocalPlayer->Distance(EvadeToPoint) > 75)
						{

							g_LocalPlayer->IssueOrder(IssueOrderType::MoveTo, EvadeToPoint, false);
						}

						return;
					}

					std::vector<Vector> pathfinder_path;

					if (Pathfinder::FindPath(player_position, end_point, pathfinder_path) &&
						!Evader::IsPathSafe(pathfinder_path, 100).intersection.is_valid &&
						pathfinder_path.size() >= 2 && pathfinder_path[0] == player_position)
					{
						Pathfinder::PathFollow::Start(pathfinder_path);
						Pathfinder::PathFollow::KeepFollowPath();
					}
					else
					{
						if (!is_my_path_safe.intersection.is_valid && current_path.size() <= 1)
						{
							is_my_path_safe = Evader::IsPathSafe(path_to_destination, 100);
						}

						if (is_my_path_safe.intersection.is_valid)
						{
							if (g_LocalPlayer->Distance(is_my_path_safe.intersection.is_valid) > 75)
							{
								g_LocalPlayer->IssueOrder(IssueOrderType::MoveTo, is_my_path_safe.intersection.is_valid, false);
								return;
							}
						}
					}
				}
			}
		}
	}

	void OnIssueOrder(IGameObject* sender, OnIssueOrderEventArgs* args)
	{
		if (args->IssueOrderType == IssueOrderType::MoveTo || args->IssueOrderType == IssueOrderType::AttackTo)
			if (is_evading)
			{
				EvadeToPoint.x = args->Position.x;
				EvadeToPoint.y = args->Position.y;
			}
		else
		{
			EvadeToPoint.x = 0;
			EvadeToPoint.y = 0;
		}

		if (SpellDetector::detected_skillshots.empty())
		{
			force_follow_path = false;
		}

		//Don't block the movement packets if cant find an evade point.
		if (NoSolutionFound)
		{
			return;
		}

		if (!IsEnabled() || !IsWalkingEnabled->GetBool() || IsPlayerSpellShielded())
		{
			return;
		}

		auto myPath = g_LocalPlayer->CreatePath(args->Position);
		auto safeResult = Evader::IsPointSafe(player_position);

		//If we are evading:
		if (IsEvading() || !safeResult)
		{
			auto rcSafePath = Evader::IsPathSafe(myPath, Config::EvadingRouteChangeTime);

			if (args->IssueOrderType == IssueOrderType::MoveTo)
			{
				auto willMove = false;

				if (IsEvading() && g_Common->TickCount() - LastEvadePointChangeT > Config::EvadePointChangeInterval)
				{
					EvadingPosition evading_position;

					//Update the evade point to the closest one:
					if (Evader::GetEvadingPosition(evading_position, args->Position))
					{
						if (evading_position.is_evadable)
						{
							//g_Common->Log("IssueOrder move 1 %d", g_Common->TickCount());
							EvadePoint = evading_position.end_position;
							SetEvading(true);
							LastEvadePointChangeT = g_Common->TickCount();
							willMove = true;
						}
					}
				}

				//If the path is safe let the user follow it.
				if (rcSafePath.is_safe && Evader::IsPointSafe(myPath[myPath.size() - 1]) && args->IssueOrderType == IssueOrderType::MoveTo)
				{
					//g_Common->Log("IssueOrder move 2 %d", g_Common->TickCount());
					EvadePoint = myPath[myPath.size() - 1];
					SetEvading(true);
					willMove = true;
				}

				if (!willMove)
				{
					force_follow_path = true;
				}
			}

			//Block the packets if we are evading or not safe.
			args->Process = false;
			return;
		}

		auto safePath = Evader::IsPathSafe(myPath, Config::CrossingTime);

		//Not evading, outside the skillshots.
		//The path is not safe, stop in the intersection point.
		if (!safePath.is_safe && args->IssueOrderType != IssueOrderType::AttackUnit)
		{
			if (safePath.intersection.is_valid)
			{
				if (g_LocalPlayer->Distance(safePath.intersection.point) > 75)
				{
					force_follow_path = true;
					//ObjectManager.Player.SendMovePacket(safePath.Intersection.Point);
				}
			}

			force_follow_path = true;
			args->Process = false;
		}

		//AutoAttacks.
		if (!safePath.is_safe && args->IssueOrderType == IssueOrderType::AttackUnit)
		{
			auto target = args->Target;
			if (target != nullptr && target->IsValid() && target->IsVisible())
			{
				//Out of attack range.
				if (!g_LocalPlayer->IsInAutoAttackRange(target))
				{
					if (safePath.intersection.is_valid)
					{
						//g_Common->Log("IssueOrder move 3 %d", g_Common->TickCount());
						g_LocalPlayer->IssueOrder(IssueOrderType::MoveTo, safePath.intersection.point, false);
					}

					args->Process = false;
				}
			}
		}
	}

	void OnNewPath(IGameObject* sender, OnNewPathEventArgs* args)
	{
		if (sender != nullptr && sender->IsValid())
		{
			SpellDetector::OnNewPath(sender, args);

			if (sender->IsMe() && args->IsDash && !args->Path.empty())
			{
				const auto path = args->Path[args->Path.size() - 1];

				EvadeToPoint.x = path.x;
				EvadeToPoint.y = path.y;
			}
		}
	}

	void OnCastSpell(IGameObject* sender, OnCastSpellEventArgs* args)
	{
		if (!IsEvading())
		{
			return;
		}

		static auto block_level = config_menu->GetElement("block_spells");
		if (block_level->GetInt() == 2)
		{
			return;
		}

		auto is_dangerous = false;

		for (auto const& spell_instance : SpellDetector::detected_skillshots)
		{
			if (spell_instance->IsEnabled())
			{
				is_dangerous = spell_instance->menu_config->is_dangerous->GetBool();
			}

			if (is_dangerous) break;
		}

		if (block_level->GetInt() == 1 && !is_dangerous)
			return;

		args->Process = !SpellBlocker::ShouldBlockSpell(args->SpellSlot);
	}

	void OnBeforeAttack(BeforeAttackOrbwalkerArgs* args)
	{
		if (IsEvading())
			args->Process = false;
	}
	
	void OnDraw()
	{
		if (Evade::drawing_menu->GetElement("enable_drawings")->GetBool())
		{
			if (Evade::drawing_menu->GetElement("draw_status")->GetBool())
			{
				g_Drawing->AddText
				(
					g_LocalPlayer->Position(), 
					Evade::IsEnabled() ? 0xFFFFFFFF : 0xFF0000FF, 
					14, 
					"Evading: %s", 
					Evade::IsEnabled() ? "Enabled" : "Disabled"
				);
			}

			if (Config::IsDebugEnabled)
			{
				Vector2 pos(50, 50);

				g_Drawing->AddTextOnScreen
				(
					pos,
					0xFFFFFFFF,
					14,
					"Active skillshot count %d",
					SpellDetector::detected_skillshots.size()
				);
				
				for (auto const& s : SpellDetector::detected_skillshots)
				{
					if (s->IsValid())
					{
						pos.y += 20;

						g_Drawing->AddTextOnScreen
						(
							pos,
							0xFFFFFFFF,
							14,
							"%s", s->skillshot_data->spell_name.c_str()
						);
					}
				}

				std::vector<Vector> path;
				auto start = g_LocalPlayer->Position();
				auto end = g_Common->CursorPosition();

				if (Pathfinder::FindPath(start, end, path))
				{
					for (size_t i = 0; i < path.size() - 1; i++)
					{
						g_Drawing->AddLine(path[i], path[i + 1], 0xFFFFFFFF);
					}
				}
			}

			SpellDetector::OnDraw();
		}
	}

	void OnCreate(IGameObject* sender)
	{
		if (sender != nullptr && sender->IsValid())
		{
			SpellDetector::OnCreate(sender);			
		}
	}

	void OnDelete(IGameObject* sender)
	{
		if (sender != nullptr && sender->IsValid())
		{
			SpellDetector::OnDelete(sender);
		}
	}

	void OnProcessSpell(IGameObject* sender, OnProcessSpellEventArgs* args)
	{
		if (sender != nullptr && sender->IsValid())
		{
			if (sender->IsMe() && args->SpellSlot == SpellSlot::Recall)
			{
				EvadeToPoint.x = 0;
				EvadeToPoint.y = 0;
			}

			SpellDetector::OnProcessSpell(sender, args);
		}
	}

	Vector start;
	Vector end;

	void OnWndProc(OnWndProcEventArgs* args)
	{
		if (args->uMsg == WM_LBUTTONDOWN)
		{
			start = g_Common->CursorPosition();
		}

		if (args->uMsg == WM_LBUTTONUP)
		{
			end = g_Common->CursorPosition();
		}

		if (args->uMsg == WM_KEYUP && args->wParam == 'L') //line missile skillshot
		{
			auto s = start;
			auto e = end;

			ITask task(2500, [s, e]()
			{
				static SkillshotData* spell = nullptr;

				if (spell == nullptr)
				{
					auto it = std::find_if(SpellDb::spells.begin(), SpellDb::spells.end(), [](SkillshotData* x) -> bool
					{
						return x->spell_name == "DebugLinearSpell";
					});

					if (it != SpellDb::spells.end())
					{
						spell = *it;
					}
				}

				if (spell != nullptr)
				{
					SpellDetector::AddSkillshot(g_LocalPlayer, s, e, spell, 0, nullptr);
				}
			});
			task.m_callback();
			debug_skillshots.push_back(task);
		}

		if (args->uMsg == WM_KEYUP && args->wParam == 'I') //circular skillshoot
		{
			auto s = start;
			auto e = end;

			ITask task(5000, [s, e]()
			{
				static SkillshotData* spell = nullptr;

				if (spell == nullptr)
				{
					auto it = std::find_if(SpellDb::spells.begin(), SpellDb::spells.end(), [](SkillshotData* x) -> bool
					{
						return x->spell_name == "DebugCircularSpell";
					});

					if (it != SpellDb::spells.end())
					{
						spell = *it;
					}
				}

				if (spell != nullptr)
				{
					SpellDetector::AddSkillshot(g_LocalPlayer, s, e, spell, 0, nullptr);
				}
			});
			task.m_callback();
			debug_skillshots.push_back(task);
		}
	}

	SpellSlot GetWardSlot()
	{
		int ward_ids[] =
		{
			2045, 2049, 2050, 2301, 2302, 2303, 3340, 3361, 3362, 3711, 1408, 1409, 1410, 1411, 2043, 2055, 3362, 2043
		};

		std::map<SpellSlot, int> item_map;

		int i = 0;
		const auto spellbook = g_LocalPlayer->GetSpellbook();

		for (auto const& item : g_LocalPlayer->GetItems())
		{
			auto const spell_slot = static_cast<SpellSlot>(i + 6);

			if (spellbook->GetSpellState(spell_slot) & kReady)
			{
				item_map[spell_slot] = int(item.ItemId);
			}

			i++;
		}

		for (auto const& ward_id : ward_ids)
		{
			for (auto const& item : item_map)
			{
				if (ward_id == item.second)
				{
					return item.first;
				}
			}
		}
		
		return SpellSlot::Invalid;
	}
}
