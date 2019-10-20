#include "../SDK/PluginSDK.h"
#include "../SDK/EventHandler.h"
#include "../SDK/Vector.h"

#include "Evade.h"

PLUGIN_API const char PLUGIN_PRINT_NAME[32] = "Evade";
PLUGIN_API const char PLUGIN_PRINT_AUTHOR[32] = "SpacebarClub";
PLUGIN_API ChampionId PLUGIN_TARGET_CHAMP = ChampionId::Unknown;

PLUGIN_API bool OnLoadSDK(IPluginsSDK* plugin_sdk)
{
	DECLARE_GLOBALS(plugin_sdk);

	return Evade::Load();
}

PLUGIN_API void OnUnloadSDK()
{
	Evade::Unload();
}