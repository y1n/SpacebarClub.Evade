#include "SpecialSpells.h"
#include "SpellDb.h"
#include "SpellDetector.h"
#include "SkillshotInstance.h"
#include <regex>

namespace SpecialSpells
{
	class Azir : public ISpecialSpell
	{
	public:
		Azir()
		{
			for (auto const& s : SpellDb::spells)
			{
				if (s->spell_name == "AzirQWrapper")
				{
					this->q = s;
				}
				else if (s->spell_name == "AzirR")
				{
					this->r = s;
				}
			}
		}

		ChampionId Champion() override
		{
			return ChampionId::Azir;
		}

		bool OnProcessSpellCast(IGameObject* sender, OnProcessSpellEventArgs* args) override
		{
			if (args->SpellData->SpellName == this->r->spell_name)
			{
				if (const auto skillshot_instance = SpellDetector::AddSkillshot(sender, args->StartPosition, args->EndPosition, this->r, 0, nullptr))
				{
					skillshot_instance->skillshot_data->raw_radius = r->raw_radius + 75 * (sender->GetSpellbook()->GetSpell(this->r->spell_slot)->Level() - 1);
				}
			}

			return args->SpellData->SpellName == this->q->spell_name;
		}

		bool OnNewPath(IGameObject* sender, OnNewPathEventArgs* args) override
		{
			if (args->IsDash)
			{
				if (sender->Type() == EntityType::AIMinionClient &&
					sender->BaseSkinName() == "AzirSoldier")
				{
					
					for (auto const& unit : g_ObjectManager->GetChampions())
					{
						if (!Config::IsDebugEnabled && unit->IsAlly())
						{
							continue;
						}

						if (unit == sender->GetOwner())
						{
							const auto start_position = sender->ServerPosition();
							const auto end_position = args->Path[args->Path.size() - 1];

							SpellDetector::AddSkillshot(unit, start_position, end_position, this->q, 0, nullptr);
							return true;
						}
					}
				}
			}

			return false;
		}

	private:
		SkillshotData * q{};
		SkillshotData * r{};
	};

	class BardR : public ISpecialSpell
	{
	public:
		BardR()
		{
			for (auto const& s : SpellDb::spells)
			{
				if (s->spell_name == "BardR")
				{
					this->r = s;
					break;
				}
			}
		}

		ChampionId Champion() override
		{
			return ChampionId::Bard;
		}

		bool OnSkillshotAdd(SkillshotInstance* skillshot_instance) override
		{
			if (skillshot_instance->start_position.Distance(skillshot_instance->end_position) <= 1050)
			{
				skillshot_instance->end_tick = skillshot_instance->start_tick + 1050;
			}

			return false;
		}

	private:
		SkillshotData * r{};
	};

	class DianaQ : public ISpecialSpell
	{
	public:
		DianaQ()
		{
			for (auto const& s : SpellDb::spells)
			{
				if (s->spell_name == "DianaQArc")
				{
					this->q = s;
					break;
				}
			}
		}

		ChampionId Champion() override
		{
			return ChampionId::Diana;
		}

		bool OnSkillshotAdd(SkillshotInstance* skillshot_instance) override
		{
			if (skillshot_instance->skillshot_data->spell_name == "DianaArc")
			{
				SpellDetector::AddSkillshot(skillshot_instance->caster, skillshot_instance->start_position, skillshot_instance->end_position, this->q, 0, skillshot_instance->missile_client);
			}

			return false;
		}

	private:
		SkillshotData * q{};
	};

	class DravenR : public ISpecialSpell
	{
	public:
		DravenR()
		{
			for (auto const& s : SpellDb::spells)
			{
				if (s->spell_name == "DravenRCast")
				{
					this->r = s;
					break;
				}
			}
		}

		ChampionId Champion() override
		{
			return ChampionId::Draven;
		}

		bool OnProcessSpellCast(IGameObject* sender, OnProcessSpellEventArgs* args) override
		{
			if (args->SpellData->SpellName == "DravenRDoublecast")
			{
				SpellDetector::detected_skillshots.erase(std::remove_if(SpellDetector::detected_skillshots.begin(), SpellDetector::detected_skillshots.end(),
					[&](SkillshotInstance* x)
				{
					if (x->caster == sender && x->skillshot_data->spell_slot == this->r->spell_slot)
					{
						delete x;
						return true;
					}
					return false;
				}), SpellDetector::detected_skillshots.end());

				return true;
			}

			return false;
		}

	private:
		SkillshotData * r{};
	};

	class JannaQ : public ISpecialSpell
	{
	public:
		JannaQ()
		{
			for (auto const& s : SpellDb::spells)
			{
				if (s->spell_name == "JannaQ")
				{
					this->q = s;
					break;
				}
			}
		}

		ChampionId Champion() override
		{
			return ChampionId::Janna;
		}

		bool OnCreateMissile(IGameObject* missile, IGameObject* caster, ISpellData* spell_data) override
		{
			if (spell_data->SpellName == "HowlingGale")
			{
				if (const auto skillshot_instance = SpellDetector::AddSkillshot(caster, missile->MissileStartPosition(), missile->MissileEndPosition(), q, 0, missile))
				{
					skillshot_instance->skillshot_data->missile_speed = spell_data->MissileSpeed;
					skillshot_instance->end_tick = skillshot_instance->start_tick +
						skillshot_instance->skillshot_data->delay + skillshot_instance->start_position.Distance(skillshot_instance->end_position) / skillshot_instance->skillshot_data->missile_speed * 1000;
				}
				return true;
			}

			return false;
		}

	private:
		SkillshotData * q{};
	};

	class JarvanQ : public ISpecialSpell
	{
	public:
		JarvanQ()
		{
			for (auto const& s : SpellDb::spells)
			{
				if (s->spell_name == "JarvanIVQE")
				{
					this->q = s;
					break;
				}
			}
		}

		ChampionId Champion() override
		{
			return ChampionId::JarvanIV;
		}

		bool OnProcessSpellCast(IGameObject* sender, OnProcessSpellEventArgs* args) override
		{
			if (args->SpellData->SpellName == "JarvanIVDragonStrike")
			{
				auto start_position = args->StartPosition;
				auto qeEnd = start_position.Extend(args->EndPosition, this->q->raw_range);
				std::vector<Vector> endPos;

				for (auto const& spell : SpellDetector::detected_skillshots)
				{
					if (spell->skillshot_data->spell_slot == SpellSlot::E && spell->caster == sender)
					{
						if (spell->end_position.Distance(start_position, qeEnd, true) < this->q->radius_ex)
						{
							endPos.push_back(spell->end_position);
						}
					}
				}

				for (auto const& unit : g_ObjectManager->GetByType(EntityType::AIMinionClient, [&](IGameObject* x)
				{;
					return !x->IsDead() &&
						StringEquals(x->BaseSkinName().c_str(), "jarvanivstandard", TRUE) && 
						x->GetOwner() == sender &&
						x->Position().Distance(start_position, qeEnd, true) < this->q->radius_ex;
				}))
				{
					endPos.push_back(unit->Position());
				}

				if (!endPos.empty())
				{
					std::sort(endPos.begin(), endPos.end(), [&](Vector const& a, Vector const& b) { return a.DistanceSquared(qeEnd) < b.DistanceSquared(qeEnd); });
					SpellDetector::AddSkillshot(sender, start_position, endPos.front(), q, 0, nullptr);
					return true;
				}
			}
			return false;
		}

	private:
		SkillshotData * q{};
	};

	class Karma : public ISpecialSpell
	{
	public:
		Karma()
		{
			for (auto const& s : SpellDb::spells)
			{
				if (s->spell_name == "KarmaQMantra")
				{
					this->q = s;
					break;
				}
			}
		}

		ChampionId Champion() override
		{
			return ChampionId::Karma;
		}

		bool OnProcessSpellCast(IGameObject* sender, OnProcessSpellEventArgs* args) override
		{
			if (args->SpellData->SpellName == "KarmaQ")
			{
				if (sender->HasBuff(hash("KarmaMantra")))
				{
					SpellDetector::AddSkillshot(sender, args->StartPosition, args->EndPosition, this->q, 0, nullptr);
					return true;
				}
			}

			return false;
		}

	private:
		SkillshotData * q{};
	};

	class LuluQ : public ISpecialSpell
	{
	public:
		LuluQ()
		{
			for (auto const& s : SpellDb::spells)
			{
				if (s->spell_name == "LuluQ")
				{
					this->q = s;
					break;
				}
			}
		}

		ChampionId Champion() override
		{
			return ChampionId::Lulu;
		}

		bool OnProcessSpellCast(IGameObject* sender, OnProcessSpellEventArgs* args) override
		{
			if (args->SpellData->SpellName == "LuluQ")
			{
				for (auto const& minion : g_ObjectManager->GetByType(EntityType::AIMinionClient, [&](IGameObject* x)
				{
					return !x->IsDead() && StringEquals(x->BaseSkinName().c_str(), "lulufaerie") && x->IsVisible() && x->GetOwner() == sender;
				}))
				{
					SpellDetector::AddSkillshot(sender, minion->Position(), args->EndPosition, this->q, 0, nullptr);
					break;
				}
			}

			return false;
		}

	private:
		SkillshotData * q{};
	};

	class LuxR : public ISpecialSpell
	{
	public:
		LuxR()
		{
			for (auto const& s : SpellDb::spells)
			{
				if (s->spell_name == "LuxMaliceCannon")
				{
					this->r = s;
					break;
				}
			}
		}

		ChampionId Champion() override
		{
			return ChampionId::Lux;
		}

		bool OnCreateParticle(IGameObject* sender) override
		{
			std::regex r("Lux_.+_R_mis_beam");

			if (std::regex_match(sender->Name().c_str(), r, std::regex_constants::match_flag_type::match_default))
			{
				if (this->middle && this->middle->IsValid() && !this->middle->IsDead())
				{
					if (int(g_Common->TickCount() - middle_t) < 100 + g_Common->Ping() / 2)
					{
						const auto dir = (sender->Position() - this->middle->Position()).Normalized();
						const auto start_pos = this->middle->Position() - dir * this->r->Range() / 2;
						const auto end_pos = this->middle->Position() + dir * this->r->Range() / 2;

						if (std::find_if(SpellDetector::detected_skillshots.begin(), SpellDetector::detected_skillshots.end(), [&](SkillshotInstance* x)
						{
							return x->skillshot_data->champion_name == "Lux" && x->skillshot_data->spell_slot == SpellSlot::R && x->start_position.Distance(start_pos) < 200;
						}) != SpellDetector::detected_skillshots.end())
						{
							return true;
						}

						SpellDetector::AddSkillshot(nullptr, start_pos, end_pos, this->r, 0, nullptr);
						return true;
					}
				}
			}

			r = std::regex("Lux_.+_R_mis_beam_middle");

			if (std::regex_match(sender->Name().c_str(), r, std::regex_constants::match_flag_type::match_default))
			{
				this->middle = sender;
				this->middle_t = g_Common->TickCount();
				return true;
			}

			return false;
		}

	private:
		SkillshotData * r{};
		IGameObject* middle{};
		unsigned int middle_t{};
	};

	class MalzaharQ : public ISpecialSpell
	{
	public:
		MalzaharQ()
		{
			for (auto const& s : SpellDb::spells)
			{
				if (s->spell_name == "MalzaharQ")
				{
					this->q = s;
					break;
				}
			}
		}

		ChampionId Champion() override
		{
			return ChampionId::Malzahar;
		}

		bool OnProcessSpellCast(IGameObject* sender, OnProcessSpellEventArgs* args) override
		{
			if (args->SpellData->SpellName == q->spell_name)
			{
				auto const end = args->EndPosition;
				auto const dir = (end - sender->Position()).Normalized().Perpendicular() * (q->Range() / 2.0f);
				auto const startPos = end - dir;
				auto const endPos = end + dir;

				SpellDetector::AddSkillshot(sender, startPos, endPos, q, 0);
				SpellDetector::AddSkillshot(sender, endPos, endPos, q, 0);

				return true;
			}

			return false;
		}

	private:
		SkillshotData * q{};
	};

	class SionE : public ISpecialSpell
	{
	public:
		SionE()
		{
			for (auto const& s : SpellDb::spells)
			{
				if (s->spell_name == "SionEMinion")
				{
					this->e = s;
					break;
				}
			}
		}

		ChampionId Champion() override
		{
			return ChampionId::Sion;
		}

		bool OnNewPath(IGameObject* sender, OnNewPathEventArgs* args) override
		{
			if (args->IsDash)
			{
				if (sender->Type() == EntityType::AIMinionClient)
				{
					IBuffInstance* buff = nullptr;

					for (auto & buffs : sender->GetBuffList())
					{
						if (buffs.Name == "SionE")
						{
							buff = &buffs;
							break;
						}
					}

					if (buff == nullptr || buff->Caster == nullptr)
						return false;

					auto caster = buff->Caster;

					if (!caster->IsAIHero() || caster->ChampionId() != this->Champion())
						return false;

					const auto start = args->Path.front();
					const auto end = args->Path.back();
					const auto direction = (end - start).Normalized();

					if (auto const skillshot_instance = SpellDetector::AddSkillshot(caster, start, end + direction * 110, this->e, 0, nullptr))
					{
						skillshot_instance->skillshot_data->delay = 0;
						skillshot_instance->end_tick = skillshot_instance->start_tick + skillshot_instance->start_position.Distance(skillshot_instance->end_position) /
							skillshot_instance->skillshot_data->missile_speed * 1000;
					}

					return true;
				}
			}

			return false;
		}


	private:
		SkillshotData * e{};
	};

	class SyndraE : public ISpecialSpell
	{
	public:
		SyndraE()
		{
			for (auto const& s : SpellDb::spells)
			{
				if (s->spell_name == "SyndraEQ")
				{
					this->e = s;
				}
				if (s->spell_name == "SyndraE")
				{
					this->e1 = s;
				}
			}
		}

		ChampionId Champion() override
		{
			return ChampionId::Syndra;
		}

		bool OnProcessSpellCast(IGameObject* sender, OnProcessSpellEventArgs* args) override
		{
			if (args->SpellData->SpellName == "SyndraE" || args->SpellData->SpellName == "SyndraE5")
			{
				if (auto const skillshot_instance = SpellDetector::AddSkillshot(sender, sender->Position(), args->EndPosition, e1, 0))
				{
					std::vector<Vector> orbs;

					for (auto const& spell : SpellDetector::detected_skillshots)
					{
						if (spell->skillshot_data->spell_slot == SpellSlot::Q && spell->caster == sender &&
							g_Common->TickCount() - spell->start_tick < 400)
						{
							orbs.push_back(spell->end_position);
						}
					}

					for (auto const& unit : g_ObjectManager->GetByType(EntityType::AIMinionClient, [&](IGameObject* x)
					{
						return !x->IsDead() && StringEquals(x->BaseSkinName().c_str(), "syndrasphere") && x->GetOwner() == sender;
					}))
					{
						orbs.push_back(unit->Position());
					}

					auto angle = 60.0f;
					auto edge1 = (skillshot_instance->end_position - sender->Position()).Rotated(-angle / 2.0f * M_PI / 180.0f);
					auto edge2 = edge1.Rotated(angle * M_PI / 180.0f);

					for (auto const& orb : orbs)
					{
						auto v = orb - sender->Position();
						if (edge1.CrossProduct(v) > 0 && v.CrossProduct(edge2) > 0 && orb.Distance(sender->Position()) < 800)
						{
							const auto startPos = orb.Extend(skillshot_instance->start_position, 100);
							const auto endPos = skillshot_instance->start_position.Extend(orb, skillshot_instance->start_position.Distance(orb) > 200 ? 1300 : 1100);
							const auto startT = skillshot_instance->start_tick + skillshot_instance->skillshot_data->delay + (int)(skillshot_instance->start_position.Distance(orb) / skillshot_instance->skillshot_data->missile_speed * 1000);

							SpellDetector::AddSkillshot(sender, startPos, endPos, e, startT);
						}
					}
				}

				return true;
			}

			return false;
		}

	private:
		SkillshotData * e1{};
		SkillshotData * e{};
	};

	class TaricE : public ISpecialSpell
	{
	public:
		TaricE()
		{
			for (auto const& s : SpellDb::spells)
			{
				if (s->spell_name == "TaricE")
				{
					this->e = s;
					break;
				}
			}
		}

		ChampionId Champion() override
		{
			return ChampionId::Taric;
		}

		bool OnProcessSpellCast(IGameObject* sender, OnProcessSpellEventArgs* args) override
		{
			if (args->SpellData->SpellName == "TaricE")
			{
				for (auto const& hero : g_ObjectManager->GetChampions())
				{
					if (!hero->IsDead() && hero->IsVisible() && hero->Team() == sender->Team() && hero->HasBuff(hash("TaricWAllyBuff")))
					{
						SpellDetector::AddSkillshot(hero, hero->Position(), args->EndPosition, e, 0);
					}
				}
			}

			return false;
		}

	private:
		SkillshotData * e{};
	};

	class ViktorE : public ISpecialSpell
	{
	public:
		ViktorE()
		{
			for (auto const& s : SpellDb::spells)
			{
				if (s->spell_name == "ViktorEExplosion")
				{
					this->e = s;
					break;
				}
			}
		}

		ChampionId Champion() override
		{
			return ChampionId::Viktor;
		}

		bool OnSkillshotAdd(SkillshotInstance* skillshot_instance) override
		{
			if (skillshot_instance->skillshot_data->spell_name == "ViktorE")
			{
				if (skillshot_instance->caster && skillshot_instance->caster->IsSpellEvolved(SpellSlot::E))
				{
					SpellDetector::AddSkillshot(skillshot_instance->caster, skillshot_instance->start_position, skillshot_instance->end_position, e, 0);
				}
			}

			return false;
		}

	private:
		SkillshotData * e{};
	};

	class VelkozQ : public ISpecialSpell
	{
	public:
		VelkozQ()
		{
			for (auto const& s : SpellDb::spells)
			{
				if (s->spell_name == "VelkozQSplit")
				{
					this->q = s;
					break;
				}
			}
		}

		ChampionId Champion() override
		{
			return ChampionId::Velkoz;
		}

		bool OnProcessSpellCast(IGameObject* sender, OnProcessSpellEventArgs* args) override
		{
			if (args->SpellData->SpellName == "VelkozQSplitActivate")
			{
				for (auto const& skillshot : SpellDetector::detected_skillshots)
				{
					if (skillshot->IsValid() && skillshot->caster == sender &&
						skillshot->skillshot_data->spell_name == "VelkozQ")
					{
						const auto dir = skillshot->direction.Perpendicular();
						const auto start_pos = skillshot->GetMissilePosition(0);

						SpellDetector::AddSkillshot(sender, start_pos, start_pos + dir * skillshot->skillshot_data->Range(), q, 0);
						SpellDetector::AddSkillshot(sender, start_pos, start_pos - dir * skillshot->skillshot_data->Range(), q, 0);

						return true;
					}
				}

				return true;
			}

			return false;
		}

	private:
		SkillshotData * q{};
	};

	class WarwickR : public ISpecialSpell
	{
	public:
		WarwickR()
		{
			for (auto const& s : SpellDb::spells)
			{
				if (s->spell_name == "WarwickR")
				{
					this->r = new SkillshotData(*s);
					break;
				}
			}
		}

		~WarwickR()
		{
			delete this->r;
		}

		ChampionId Champion() override
		{
			return ChampionId::Warwick;
		}

		bool OnProcessSpellCast(IGameObject* sender, OnProcessSpellEventArgs* args) override
		{
			if (sender->ChampionId() == this->Champion() && args->SpellData->SpellName == this->r->spell_name)
			{
				this->r->missile_speed = int(sender->MoveSpeed() * 4.5f);
				this->r->raw_range = int(sender->MoveSpeed() * 2.1f);

				SpellDetector::AddSkillshot(sender, args->StartPosition, args->EndPosition, this->r, 0);

				return true;
			}

			return false;
		}


	private:
		SkillshotData * r{};
	};

	class YorickE : public ISpecialSpell
	{
	public:
		YorickE()
		{
			for (auto const& s : SpellDb::spells)
			{
				if (s->spell_name == "YorickE")
				{
					this->e = s;
					break;
				}
			}
		}

		ChampionId Champion() override
		{
			return ChampionId::Yorick;
		}

		bool OnProcessSpellCast(IGameObject* sender, OnProcessSpellEventArgs* args) override
		{
			if (sender->ChampionId() == this->Champion() && args->SpellData->SpellName == this->e->spell_name)
			{
				const auto start_position = args->EndPosition.Extend(sender->ServerPosition(), 120);
				const auto end_position = start_position.Extend(sender->ServerPosition(), -1);
				const auto start_tick = g_Common->TickCount() - g_Common->Ping() / 2 + 350 + int(sender->ServerPosition().Distance(start_position) / 1800 * 1000);

				SpellDetector::AddSkillshot(sender, start_position, end_position, this->e, start_tick);

				return true;
			}

			return false;
		}


	private:
		SkillshotData * e{};
	};

	class ZacE : public ISpecialSpell
	{
	public:
		ZacE()
		{
			for (auto const& s : SpellDb::spells)
			{
				if (s->spell_name == "ZacE2")
				{
					this->e = s;
					break;
				}
			}
		}

		ChampionId Champion() override
		{
			return ChampionId::Zac;
		}

		bool OnProcessSpellCast(IGameObject* sender, OnProcessSpellEventArgs* args) override
		{
			if (sender->ChampionId() == this->Champion() && args->SpellData->SpellName == "ZacE")
			{
				this->last_cast_e[sender] = g_Common->TickCount();
			}

			return false;
		}

		bool OnNewPath(IGameObject* sender, OnNewPathEventArgs* args) override
		{
			if (args->IsDash && sender->IsAIHero() && sender->ChampionId() == this->Champion())
			{
				if (this->last_cast_e.find(sender) != this->last_cast_e.end())
				{
					if (g_Common->TickCount() - this->last_cast_e[sender] < 500)
					{
						const auto start_position = sender->ServerPosition();
						const auto end_position = args->Path[args->Path.size() - 1];

						SpellDetector::AddSkillshot(sender, start_position, end_position, this->e, 0, nullptr);
						return true;
					}
				}
			}

			return false;
		}

	private:
		SkillshotData * e{};
		std::map<IGameObject*, int> last_cast_e;
	};

	class ZedQ : public ISpecialSpell
	{
	public:
		ZedQ()
		{
			for (auto const& s : SpellDb::spells)
			{
				if (s->spell_name == "ZedQ")
				{
					this->q = s;
					break;
				}
			}
		}

		ChampionId Champion() override
		{
			return ChampionId::Zed;
		}

		bool OnProcessSpellCast(IGameObject* sender, OnProcessSpellEventArgs* args) override
		{
			if (args->SpellData->SpellName == "ZedQ")
			{
				std::vector<Vector> start_pos = { args->StartPosition };
				g_ObjectManager->GetByType(EntityType::AIMinionClient, [&](IGameObject* x)
				{
					if (!x->IsDead() && x->Team() == sender->Team())
					{
						if ((x->Name() == "Shadow" || x->BaseSkinName() == "Shadow") && x->GetOwner() == sender)
						{
							start_pos.push_back(x->Position());
						}
					}
					return false;
				});

				for (auto const& start_position : start_pos)
				{
					SpellDetector::AddSkillshot(sender, start_position, args->EndPosition, q, 0);
				}

				return true;
			}

			return false;
		}

	private:
		SkillshotData * q{};
	};

	class OriannaR : public ISpecialSpell
	{
	public:
		OriannaR()
		{
			for (auto const& s : SpellDb::spells)
			{
				if (s->spell_name == "OrianaDetonateCommand")
				{
					this->skillshot_data = s;
					break;
				}
			}
		}

		ChampionId Champion() override
		{
			return ChampionId::Orianna;
		}

		bool OnProcessSpellCast(IGameObject* sender, OnProcessSpellEventArgs* args) override
		{
			if (this->skillshot_data)
			{
				if (args->SpellData->SpellName == this->skillshot_data->spell_name)
				{
					auto balls = g_ObjectManager->GetByType(EntityType::AIMinionClient, [&](IGameObject* x)
					{
						return x->GetOwner() == sender && !x->IsDead() && x->Name() == "TheDoomBall";
					});

					if (!balls.empty())
					{
						std::sort(balls.begin(), balls.end(), [](IGameObject* a, IGameObject* b)
						{
							return a->Mana() > b->Mana();
						});

						auto ball = balls.front();

						SpellDetector::AddSkillshot(sender, ball->Position(), ball->Position(), skillshot_data, 0, nullptr);
						return true;
					}
				}
			}

			return false;
		}

	private:
		SkillshotData * skillshot_data{};
	};
	
	std::vector<ISpecialSpell*> special_spells;
	std::vector<ISpecialSpell*>& GetSpecialSpells(std::vector<ChampionId> const& champion_ids)
	{
		static BYTE load_spells = TRUE;

		if (load_spells)
		{
			for (auto const& champion : champion_ids)
			{
				switch (champion)
				{
				case ChampionId::Azir:
					special_spells.push_back(new Azir);
					break;
				case ChampionId::Bard:
					special_spells.push_back(new BardR);
					break;
				case ChampionId::Diana:
					special_spells.push_back(new DianaQ);
					break;
				case ChampionId::Draven:
					special_spells.push_back(new DravenR);
					break;
				case ChampionId::Janna:
					special_spells.push_back(new JannaQ);
					break;
				case ChampionId::JarvanIV:
					special_spells.push_back(new JarvanQ);
					break;
				case ChampionId::Karma:
					special_spells.push_back(new Karma);
					break;
				case ChampionId::Lulu:
					special_spells.push_back(new LuluQ);
					break;
				case ChampionId::Lux:
					special_spells.push_back(new LuxR);
					break;
				case ChampionId::Malzahar:
					special_spells.push_back(new MalzaharQ);
					break;
				case ChampionId::Sion:
					special_spells.push_back(new SionE);
					break;
				case ChampionId::Syndra:
					special_spells.push_back(new SyndraE);
					break;
				case ChampionId::Taric:
					special_spells.push_back(new TaricE);
					break;
				case ChampionId::Viktor:
					special_spells.push_back(new ViktorE);
					break;
				case ChampionId::Velkoz:
					special_spells.push_back(new VelkozQ);
					break;
				case ChampionId::Warwick:
					special_spells.push_back(new WarwickR);
					break;
				case ChampionId::Yorick:
					special_spells.push_back(new YorickE);
					break;
				case ChampionId::Zac:
					special_spells.push_back(new ZacE);
					break;
				case ChampionId::Zed:
					special_spells.push_back(new ZedQ);
					break;
				case ChampionId::Orianna:
					special_spells.push_back(new OriannaR);
					break;
				}
			}
			load_spells = FALSE;
		}

		return special_spells;
	}
}