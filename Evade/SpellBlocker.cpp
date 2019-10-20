#include "SpellBlocker.h"

#define WHITELIST0() break
#define WHITELIST1(SLOT) if ((spell_slot == SpellSlot::SLOT) ) return false
#define WHITELIST2(SLOT1, SLOT2) if ((spell_slot == SpellSlot::SLOT1) || (spell_slot == SpellSlot::SLOT2) ) return false
#define WHITELIST3(SLOT1, SLOT2, SLOT3) if ((spell_slot == SpellSlot::SLOT1) || (spell_slot == SpellSlot::SLOT2) || (spell_slot == SpellSlot::SLOT3) ) return false
#define WHITELIST4() return false

namespace SpellBlocker
{
	ChampionId local_player_champion_id = ChampionId::Unknown;

	bool ShouldBlockSpell(SpellSlot spell_slot)
	{
		if (spell_slot == SpellSlot::Summoner1 || spell_slot == SpellSlot::Summoner2)
		{
			return false;
		}

		if (local_player_champion_id == ChampionId::Unknown)
		{
			local_player_champion_id = g_LocalPlayer->ChampionId();
		}

		switch (local_player_champion_id)
		{
		case ChampionId::Aatrox: 	    WHITELIST2(E, R);
		case ChampionId::Ahri: 	        WHITELIST2(W, R);
		case ChampionId::Akali: 	    WHITELIST3(W, E, R);
		case ChampionId::Alistar: 	    WHITELIST2(W, R);
		case ChampionId::Amumu: 	    WHITELIST2(W, R);
		case ChampionId::Anivia: 	    WHITELIST0();
		case ChampionId::Annie: 	    WHITELIST2(E, R);
		case ChampionId::Ashe: 	        WHITELIST2(Q, E);
		case ChampionId::AurelionSol: 	WHITELIST2(W, E);
		case ChampionId::Azir: 	        WHITELIST1(E);
		case ChampionId::Bard: 	        WHITELIST2(W, E);
		case ChampionId::Blitzcrank: 	WHITELIST1(W);
		case ChampionId::Brand: 	    WHITELIST0();
		case ChampionId::Braum: 	    WHITELIST1(E);
		case ChampionId::Caitlyn: 	    WHITELIST1(E);
		case ChampionId::Camille: 	    WHITELIST3(Q, E, R);
		case ChampionId::Cassiopeia: 	WHITELIST0();
		case ChampionId::Chogath: 	    WHITELIST0();
		case ChampionId::Corki: 	    WHITELIST2(W, E);
		case ChampionId::Darius: 	    WHITELIST2(Q, W);
		case ChampionId::Diana: 	    WHITELIST1(R);
		case ChampionId::Draven: 	    WHITELIST2(Q, W);
		case ChampionId::DrMundo: 	    WHITELIST3(W, E, R);
		case ChampionId::Ekko: 	        WHITELIST2(E, R);
		case ChampionId::Elise: 	    WHITELIST4();
		case ChampionId::Evelynn: 	    WHITELIST2(Q, W);
		case ChampionId::Ezreal: 	    WHITELIST1(E);
		case ChampionId::Fiddlesticks: 	WHITELIST1(Q);
		case ChampionId::Fiora: 	    WHITELIST4();
		case ChampionId::Fizz: 	        WHITELIST4();
		case ChampionId::Galio: 	    WHITELIST1(W);
		case ChampionId::Gangplank: 	WHITELIST1(W);
		case ChampionId::Garen: 	    WHITELIST2(Q, W);
		case ChampionId::Gnar: 	        WHITELIST1(E);
		case ChampionId::Gragas: 	    WHITELIST1(E);
		case ChampionId::Graves: 	    WHITELIST1(E);
		case ChampionId::Hecarim: 	    WHITELIST4();
		case ChampionId::Heimerdinger: 	WHITELIST1(R);
		case ChampionId::Illaoi: 	    WHITELIST1(W);
		case ChampionId::Irelia: 	    WHITELIST4();
		case ChampionId::Ivern: 	    WHITELIST2(W, E);
		case ChampionId::Janna: 	    WHITELIST2(Q, E);
		case ChampionId::JarvanIV: 	    WHITELIST1(R);
		case ChampionId::Jax: 	        WHITELIST4();
		case ChampionId::Jayce: 	    WHITELIST4();
		case ChampionId::Jhin: 	        WHITELIST0();
		case ChampionId::Jinx: 	        WHITELIST2(Q, E);
		case ChampionId::Kalista: 	    WHITELIST2(E, R);
		case ChampionId::Karma: 	    WHITELIST2(E, R);
		case ChampionId::Karthus: 	    WHITELIST2(Q, E);
		case ChampionId::Kassadin: 	    WHITELIST2(W, R);
		case ChampionId::Katarina: 	    WHITELIST2(W, E);
		case ChampionId::Kayle: 	    WHITELIST3(W, E, R);
		case ChampionId::Kayn: 	        WHITELIST2(E, R);
		case ChampionId::Kennen: 	    WHITELIST1(E);
		case ChampionId::Khazix: 	    WHITELIST2(E, R);
		case ChampionId::Kindred: 	    WHITELIST2(Q, R);
		case ChampionId::Kled: 	        WHITELIST1(E);
		case ChampionId::KogMaw: 	    WHITELIST1(W);
		case ChampionId::Leblanc: 	    WHITELIST2(W, R);
		case ChampionId::LeeSin: 	    WHITELIST2(Q, W);
		case ChampionId::Leona: 	    WHITELIST1(E);
		case ChampionId::Lissandra: 	WHITELIST2(E, R);
		case ChampionId::Lucian: 	    WHITELIST1(E);
		case ChampionId::Lulu: 	        WHITELIST3(W, E, R);
		case ChampionId::Lux: 	        WHITELIST0();
		case ChampionId::Malphite: 	    WHITELIST2(W, R);
		case ChampionId::Malzahar: 	    WHITELIST1(E);
		case ChampionId::Maokai: 	    WHITELIST1(W);
		case ChampionId::MasterYi: 	    WHITELIST3(Q, E, R);
		case ChampionId::MissFortune: 	WHITELIST1(W);
		case ChampionId::MonkeyKing: 	WHITELIST4();
		case ChampionId::Mordekaiser: 	WHITELIST2(Q, R);
		case ChampionId::Morgana: 	    WHITELIST2(E, R);
		case ChampionId::Nami: 	        WHITELIST1(E);
		case ChampionId::Nasus: 	    WHITELIST1(Q);
		case ChampionId::Nautilus: 	    WHITELIST0();
		case ChampionId::Nidalee: 	    WHITELIST2(W, R);
		case ChampionId::Nocturne: 	    WHITELIST1(W);
		case ChampionId::Nunu: 	        WHITELIST1(W);
		case ChampionId::Olaf: 	        WHITELIST2(W, R);
		case ChampionId::Orianna: 	    WHITELIST2(Q, E);
		case ChampionId::Ornn: 	        WHITELIST1(Q);
		case ChampionId::Pantheon: 	    WHITELIST1(W);
		case ChampionId::Poppy: 	    WHITELIST2(W, E);
		case ChampionId::Quinn: 	    WHITELIST3(W, E, R);
		case ChampionId::Rakan: 	    WHITELIST3(W, E, R);
		case ChampionId::Rammus: 	    WHITELIST4();
		case ChampionId::RekSai: 	    WHITELIST0();
		case ChampionId::Renekton: 	    WHITELIST1(Q);
		case ChampionId::Rengar: 	    WHITELIST1(W);
		case ChampionId::Riven: 	    WHITELIST2(Q, E);
		case ChampionId::Rumble: 	    WHITELIST1(W);
		case ChampionId::Ryze: 	        WHITELIST0();
		case ChampionId::Sejuani: 	    WHITELIST2(Q, W);
		case ChampionId::Shaco: 	    WHITELIST2(Q, R);
		case ChampionId::Shen: 	        WHITELIST1(E);
		case ChampionId::Shyvana: 	    WHITELIST2(W, R);
		case ChampionId::Singed: 	    WHITELIST2(Q, R);
		case ChampionId::Sion: 	        WHITELIST1(W);
		case ChampionId::Sivir: 	    WHITELIST2(E, R);
		case ChampionId::Skarner: 	    WHITELIST2(Q, W);
		case ChampionId::Sona: 	        WHITELIST3(Q, W, E);
		case ChampionId::Soraka: 	    WHITELIST2(W, R);
		case ChampionId::Swain: 	    WHITELIST2(Q, R);
		case ChampionId::Syndra: 	    WHITELIST3(Q, W, R);
		case ChampionId::TahmKench: 	WHITELIST1(E);
		case ChampionId::Taliyah: 	    WHITELIST0();
		case ChampionId::Talon: 	    WHITELIST2(E, R);
		case ChampionId::Taric: 	    WHITELIST3(Q, E, R);
		case ChampionId::Teemo: 	    WHITELIST1(W);
		case ChampionId::Thresh: 	    WHITELIST0();
		case ChampionId::Tristana: 	    WHITELIST2(Q, W);
		case ChampionId::Trundle: 	    WHITELIST2(W, R);
		case ChampionId::Tryndamere: 	WHITELIST3(Q, E, R);
		case ChampionId::TwistedFate: 	WHITELIST1(W);
		case ChampionId::Twitch: 	    WHITELIST1(Q);
		case ChampionId::Udyr: 	        WHITELIST2(W, E);
		case ChampionId::Urgot: 	    WHITELIST0();
		case ChampionId::Varus: 	    WHITELIST1(Q);
		case ChampionId::Vayne: 	    WHITELIST1(Q);
		case ChampionId::Veigar: 	    WHITELIST0();
		case ChampionId::Velkoz: 	    WHITELIST0();
		case ChampionId::Vi: 	        WHITELIST0();
		case ChampionId::Viktor: 	    WHITELIST1(E);
		case ChampionId::Vladimir: 	    WHITELIST3(Q, W, R);
		case ChampionId::Volibear: 	    WHITELIST3(Q, W, E);
		case ChampionId::Warwick: 	    WHITELIST3(W, E, R);
		case ChampionId::Xayah: 	    WHITELIST2(W, E);
		case ChampionId::Xerath: 	    WHITELIST0();
		case ChampionId::XinZhao: 	    WHITELIST4();
		case ChampionId::Yasuo: 	    WHITELIST3(W, E, R);
		case ChampionId::Yorick: 	    WHITELIST1(Q);
		case ChampionId::Zac: 	        WHITELIST1(W);
		case ChampionId::Zed: 	        WHITELIST3(W, E, R);
		case ChampionId::Ziggs: 	    WHITELIST0();
		case ChampionId::Zilean: 	    WHITELIST3(W, E, R);
		case ChampionId::Zyra: 	        WHITELIST1(W);
		case ChampionId::Zoe:           WHITELIST3(Q, W, R);
		case ChampionId::Kaisa:         WHITELIST3(Q, E, R);
		case ChampionId::Pyke:          WHITELIST3(W, E, R);
		default:                        WHITELIST4();
		}

		return spell_slot == SpellSlot::Q || spell_slot == SpellSlot::W || spell_slot == SpellSlot::E || spell_slot == SpellSlot::R;
	}
}