#pragma once

#include "../SDK/PluginSDK.h"
#include "../SDK/EventHandler.h"

class SkillshotInstance;
class ISpecialSpell
{
public:
	virtual ~ISpecialSpell() = default;

	virtual ChampionId Champion() = 0;

	virtual bool OnProcessSpellCast(IGameObject* sender, OnProcessSpellEventArgs* args) { return false; }
	virtual bool OnNewPath(IGameObject* sender, OnNewPathEventArgs* args) { return false; }
	virtual bool OnCreateMissile(IGameObject* missile, IGameObject* caster, ISpellData* spell_data) { return false; }
	virtual bool OnDeleteMissile(IGameObject* missile, IGameObject* caster, ISpellData* spell_data) { return false; }
	virtual bool OnCreateParticle(IGameObject* sender) { return false; }
	virtual bool OnDeleteParticle(IGameObject* sender) { return false; }
	virtual bool OnSkillshotAdd(SkillshotInstance* skillshot_instance) { return false; }
};