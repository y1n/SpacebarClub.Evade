#include "SpellDb.h"
#include "SpecialSpells.h"

namespace SpellDb
{
	std::vector<SkillshotData*> spells;
	std::vector<ISpecialSpell*> special_spells;
	 
	void Initialize()
	{
		spells.push_back
		(
			InitializeSpell("AllChampions")->_spell_name("SummonerSnowball")->_spell_slot(SpellSlot::Summoner1)->_raw_range(1600)->_delay(0)->_raw_radius(50)->_missile_speed(1200)->_collision_flags(kHeroes | kMinions | kYasuoWall)->_is_fixed_range(TRUE)->_skillshot_type(kMissileLine)
		);

		spells.push_back
		(
			InitializeSpell("AllChampions")->_spell_name("SummonerPoroThrow")->_spell_slot(SpellSlot::Summoner1)->_raw_range(2500)->_delay(0)->_raw_radius(50)->_missile_speed(1200)->_collision_flags(kHeroes | kMinions | kYasuoWall)->_is_fixed_range(TRUE)->_skillshot_type(kMissileLine)
		);

		spells.push_back
		(
			InitializeSpell("Aatrox")->_spell_name("AatroxW")->_missile_names({ "AatroxW" })->_danger_level(kMedium)->_spell_slot(SpellSlot::W)->_raw_range(825)->_raw_radius(60)->_missile_speed(1600)->_collision_flags(kYasuoWall | kMinions | kEnemyChampions)->_is_fixed_range(TRUE)->_skillshot_type(kMissileLine)
		);

		spells.push_back
		(
			InitializeSpell("Ahri")->_spell_name("AhriOrbofDeception")->_missile_names({ "AhriOrbMissile" })->_danger_level(kMedium)->_raw_range(1000)->_raw_radius(100)->_missile_speed(2500)->_collision_flags(kYasuoWall)->_is_fixed_range(TRUE)->_missile_min_speed(400)->_missile_max_speed(2500)->_skillshot_type(kMissileLine)
		);

		spells.push_back
		(
			InitializeSpell("Ahri")->_spell_name("AhriOrbReturn")->_missile_names({ "AhriOrbReturn" })->_danger_level(kHigh)->_raw_range(20000)->_delay(0)->_raw_radius(100)->_missile_speed(60)->_collision_flags(kYasuoWall)->_missile_follows_unit(TRUE)->_missile_accel(1900)->_missile_min_speed(60)->_missile_max_speed(2600)->_skillshot_type(kMissileLine)
		);

		spells.push_back
		(
			InitializeSpell("Ahri")->_spell_name("AhriSeduce")->_missile_names({ "AhriSeduceMissile" })->_danger_level(kHigh)->_spell_slot(SpellSlot::E)->_raw_range(1000)->_raw_radius(60)->_missile_speed(1550)->_collision_flags(kHeroes | kMinions | kYasuoWall)->_is_fixed_range(TRUE)->_is_dangerous(TRUE)->_skillshot_type(kMissileLine)
		);

		spells.push_back
		(
			InitializeSpell("Akali")->_spell_name("AkaliE")->_missile_names({ "AkaliEMis" })->_danger_level(kMedium)->_spell_slot(SpellSlot::E)->_raw_radius(70)->_raw_range(900)->_missile_speed(1800)->_skillshot_type(kMissileLine)->_collision_flags(kHeroes | kMinions | kYasuoWall)->_is_fixed_range(TRUE)->_is_dangerous(TRUE)
		);

		spells.push_back
		(
			InitializeSpell("Alistar")->_spell_name("Pulverize")->_danger_level(kHigh)->_raw_radius(365)->_skillshot_type(kCircle)->_is_dangerous(TRUE)
		);

		spells.push_back
		(
			InitializeSpell("Amumu")->_spell_name("BandageToss")->_missile_names({ "SadMummyBandageToss" })->_danger_level(kHigh)->_raw_range(1100)->_raw_radius(80)->_missile_speed(2000)->_collision_flags(kHeroes | kMinions | kYasuoWall)->_is_fixed_range(TRUE)->_is_dangerous(TRUE)->_skillshot_type(kMissileLine)
		);

		spells.push_back
		(
			InitializeSpell("Amumu")->_spell_name("Tantrum")->_danger_level(kMedium)->_spell_slot(SpellSlot::E)->_raw_radius(350)->_skillshot_type(kCircle)
		);

		spells.push_back
		(
			InitializeSpell("Amumu")->_spell_name("CurseoftheSadMummy")->_spell_slot(SpellSlot::R)->_raw_radius(550)->_skillshot_type(kCircle)->_is_dangerous(TRUE)->_danger_level(kExtreme)
		);

		spells.push_back
		(
			InitializeSpell("Anivia")->_spell_name("FlashFrostSpell")->_missile_names({ "FlashFrostSpell" })->_danger_level(kHigh)->_raw_range(1100)->_raw_radius(110)->_missile_speed(850)->_collision_flags(kYasuoWall)->_is_fixed_range(TRUE)->_is_dangerous(TRUE)->_skillshot_type(kMissileLine)
		);

		spells.push_back
		(
			InitializeSpell("Annie")->_spell_name("AnnieR")->_spell_slot(SpellSlot::R)->_raw_range(600)->_raw_radius(250)->_skillshot_type(kCircle)->_is_dangerous(TRUE)->_danger_level(kExtreme)
		);

		spells.push_back
		(
			InitializeSpell("Ashe")->_spell_name("Volley")->_missile_names({ "VolleyAttack", "VolleyAttackWithSound" })->_danger_level(kMedium)->_spell_slot(SpellSlot::W)->_raw_range(1150)->_raw_radius(20)->_missile_speed(1500)->_collision_flags(kHeroes | kMinions | kYasuoWall)->_is_fixed_range(TRUE)->_infront_start(TRUE)->_skillshot_type(kMissileLine)->_multiple_angle(4.62f * M_PI / 180.0f)->_multiple_number(9)->_infront_start(85)
		);

		spells.push_back
		(
			InitializeSpell("Ashe")->_spell_name("EnchantedCrystalArrow")->_missile_names({ "EnchantedCrystalArrow" })->_spell_slot(SpellSlot::R)->_raw_range(25000)->_raw_radius(130)->_missile_speed(1600)->_collision_flags(kHeroes | kYasuoWall)->_is_fixed_range(TRUE)->_is_dangerous(TRUE)->_skillshot_type(kMissileLine)->_danger_level(kExtreme)
		);

		spells.push_back
		(
			InitializeSpell("AurelionSol")->_spell_name("AurelionSolQ")->_missile_names({ "AurelionSolQMissile" })->_danger_level(kMedium)->_raw_range(550)->_raw_radius(210)->_missile_speed(850)->_collision_flags(kYasuoWall)->_is_fixed_range(TRUE)->_missile_min_speed(600)->_missile_max_speed(1000)->_skillshot_type(kMissileLine)
		);

		spells.push_back
		(
			InitializeSpell("AurelionSol")->_spell_name("AurelionSolR")->_missile_names({ "AurelionSolRBeamMissile" })->_danger_level(kHigh)->_spell_slot(SpellSlot::R)->_raw_range(1500)->_delay(350)->_raw_radius(120)->_missile_speed(4500)->_is_fixed_range(TRUE)->_is_dangerous(TRUE)->_skillshot_type(kMissileLine)
		);

		spells.push_back
		(
			InitializeSpell("Azir")->_spell_name("AzirQWrapper")->_danger_level(kMedium)->_raw_range(20000)->_delay(0)->_raw_radius(70)->_missile_speed(1600)->_collision_flags(kYasuoWall)->_skillshot_type(kMissileLine)
		);

		spells.push_back
		(
			InitializeSpell("Azir")->_spell_name("AzirR")->_danger_level(kExtreme)->_spell_slot(SpellSlot::R)->_raw_range(750)->_raw_radius(285)->_missile_speed(1400)->_collision_flags(kYasuoWall)->_is_fixed_range(TRUE)->_behind_start(300)->_skillshot_type(kMissileLine)->_is_dangerous(TRUE)
		);

		spells.push_back
		(
			InitializeSpell("Bard")->_spell_name("BardQ")->_missile_names({ "BardQMissile" })->_danger_level(kHigh)->_raw_range(950)->_raw_radius(60)->_missile_speed(1500)->_collision_flags(kYasuoWall)->_is_fixed_range(TRUE)->_is_dangerous(TRUE)->_skillshot_type(kMissileLine)
		);

		spells.push_back
		(
			InitializeSpell("Bard")->_spell_name("BardQ2")->_missile_names({ "BardQMissile2" })->_danger_level(kHigh)->_raw_range(525)->_raw_radius(80)->_missile_speed(1500)->_collision_flags(kYasuoWall)->_is_fixed_range(TRUE)->_is_dangerous(TRUE)->_skillshot_type(kMissileLine)
		);

		spells.push_back
		(
			InitializeSpell("Bard")->_spell_name("BardR")->_missile_names({ "BardRMissileFixedTravelTime", "BardRMissileRange1", "BardRMissileRange2", "BardRMissileRange3", "BardRMissileRange4", "BardRMissileRange5" })->_danger_level(kHigh)->_spell_slot(SpellSlot::R)->_raw_range(3400)->_delay(500)->_raw_radius(350)->_missile_speed(2100)->_skillshot_type(kCircle)->_missile_min_speed(500)
		);

		spells.push_back
		(
			InitializeSpell("Blitzcrank")->_spell_name("RocketGrab")->_missile_names({ "RocketGrabMissile" })->_danger_level(kHigh)->_raw_range(1050)->_raw_radius(70)->_missile_speed(1800)->_collision_flags(kHeroes | kMinions | kYasuoWall)->_is_fixed_range(TRUE)->_is_dangerous(TRUE)->_skillshot_type(kMissileLine)
		);

		spells.push_back
		(
			InitializeSpell("Brand")->_spell_name("BrandQ")->_missile_names({ "BrandQMissile" })->_danger_level(kHigh)->_raw_range(1100)->_raw_radius(60)->_missile_speed(1600)->_collision_flags(kHeroes | kMinions | kYasuoWall)->_is_fixed_range(TRUE)->_is_dangerous(TRUE)->_skillshot_type(kMissileLine)
		);

		spells.push_back
		(
			InitializeSpell("Brand")->_spell_name("BrandW")->_danger_level(kMedium)->_spell_slot(SpellSlot::W)->_raw_range(900)->_delay(850)->_raw_radius(240)->_skillshot_type(kCircle)
		);

		spells.push_back
		(
			InitializeSpell("Braum")->_spell_name("BraumQ")->_missile_names({ "BraumQMissile" })->_danger_level(kHigh)->_raw_range(1050)->_raw_radius(60)->_missile_speed(1700)->_collision_flags(kHeroes | kMinions | kYasuoWall)->_is_fixed_range(TRUE)->_is_dangerous(TRUE)->_skillshot_type(kMissileLine)
		);

		spells.push_back
		(
			InitializeSpell("Braum")->_spell_name("BraumRWrapper")->_missile_names({ "BraumRMissile" })->_danger_level(kExtreme)->_spell_slot(SpellSlot::R)->_raw_range(1200)->_delay(550)->_raw_radius(115)->_missile_speed(1400)->_collision_flags(kYasuoWall)->_is_fixed_range(TRUE)->_is_dangerous(TRUE)->_radius_ex(330)->_skillshot_type(kMissileLine)
		);

		spells.push_back
		(
			InitializeSpell("Caitlyn")->_spell_name("CaitlynPiltoverPeacemaker")->_missile_names({ "CaitlynPiltoverPeacemaker" })->_danger_level(kMedium)->_raw_range(1300)->_delay(625)->_raw_radius(60)->_missile_speed(2200)->_collision_flags(kYasuoWall)->_is_fixed_range(TRUE)->_skillshot_type(kMissileLine)
		);

		spells.push_back
		(
			InitializeSpell("Caitlyn")->_spell_name("CaitlynQBehind")->_missile_names({ "CaitlynPiltoverPeacemaker2" })->_danger_level(kMedium)->_raw_range(1300)->_delay(0)->_raw_radius(90)->_missile_speed(2200)->_collision_flags(kYasuoWall)->_skillshot_type(kMissileLine)
		);

		spells.push_back
		(
			InitializeSpell("Caitlyn")->_spell_name("CaitlynEntrapment")->_missile_names({ "CaitlynEntrapmentMissile" })->_spell_slot(SpellSlot::E)->_raw_range(800)->_delay(160)->_raw_radius(70)->_missile_speed(1600)->_collision_flags(kHeroes | kMinions | kYasuoWall)->_is_fixed_range(TRUE)->_skillshot_type(kMissileLine)
		);

		spells.push_back
		(
			InitializeSpell("Cassiopeia")->_spell_name("CassiopeiaQ")->_danger_level(kMedium)->_raw_range(850)->_delay(750)->_raw_radius(160)->_skillshot_type(kCircle)
		);

		spells.push_back
		(
			InitializeSpell("Cassiopeia")->_spell_name("CassiopeiaR")->_spell_slot(SpellSlot::R)->_raw_range(790)->_delay(500)->_raw_radius(80)->_skillshot_type(kCone)->_is_dangerous(TRUE)->_danger_level(kExtreme)
		);

		spells.push_back
		(
			InitializeSpell("Chogath")->_spell_name("Rupture")->_danger_level(kHigh)->_raw_range(950)->_delay(1200)->_raw_radius(250)->_skillshot_type(kCircle)
		);

		spells.push_back
		(
			InitializeSpell("Chogath")->_spell_name("FeralScream")->_danger_level(kMedium)->_spell_slot(SpellSlot::W)->_raw_range(650)->_delay(500)->_raw_radius(55)->_skillshot_type(kCone)
		);

		spells.push_back
		(
			InitializeSpell("Corki")->_spell_name("PhosphorusBomb")->_missile_names({ "PhosphorusBombMissile", "PhosphorusBombMissileMin" })->_danger_level(kMedium)->_raw_range(825)->_delay(300)->_raw_radius(250)->_missile_speed(1000)->_skillshot_type(kCircle)->_collision_flags(kYasuoWall)
		);

		spells.push_back
		(
			InitializeSpell("Corki")->_spell_name("MissileBarrageMissile")->_missile_names({ "MissileBarrageMissile" })->_danger_level(kMedium)->_spell_slot(SpellSlot::R)->_raw_range(1300)->_delay(180)->_raw_radius(40)->_missile_speed(2000)->_collision_flags(kHeroes | kMinions | kYasuoWall)->_is_fixed_range(TRUE)->_skillshot_type(kMissileLine)
		);

		spells.push_back
		(
			InitializeSpell("Corki")->_spell_name("MissileBarrageMissile2")->_missile_names({ "MissileBarrageMissile2" })->_danger_level(kMedium)->_spell_slot(SpellSlot::R)->_raw_range(1500)->_delay(180)->_raw_radius(40)->_missile_speed(2000)->_collision_flags(kHeroes | kMinions | kYasuoWall)->_is_fixed_range(TRUE)->_skillshot_type(kMissileLine)
		);

		spells.push_back
		(
			InitializeSpell("Darius")->_spell_name("DariusCleave")->_danger_level(kHigh)->_delay(750)->_raw_radius(325)->_skillshot_type(kRing)->_missile_follows_unit(TRUE)->_radius_ex(75)->_is_disabled_by_default(TRUE)->_dont_cross(TRUE)
		);

		spells.push_back
		(
			InitializeSpell("Darius")->_spell_name("DariusAxeGrabCone")->_danger_level(kHigh)->_spell_slot(SpellSlot::E)->_raw_range(510)->_raw_radius(50)->_skillshot_type(kCone)->_is_dangerous(TRUE)
		);

		spells.push_back
		(
			InitializeSpell("Diana")->_spell_name("DianaArc")->_danger_level(kHigh)->_raw_range(850)->_raw_radius(195)->_missile_speed(1400)->_skillshot_type(kCircle)->_collision_flags(kYasuoWall)->_is_dangerous(TRUE)
		);

		spells.push_back
		(
			InitializeSpell("Diana")->_spell_name("DianaQArc")->_danger_level(kHigh)->_raw_range(850)->_raw_radius(70)->_missile_speed(1400)->_skillshot_type(kArc)->_collision_flags(kYasuoWall)->_is_dangerous(TRUE)->_dont_cross(TRUE)
		);

		spells.push_back
		(
			InitializeSpell("DrMundo")->_spell_name("InfectedCleaverMissile")->_missile_names({ "InfectedCleaverMissile" })->_danger_level(kMedium)->_raw_range(1050)->_raw_radius(60)->_missile_speed(2000)->_collision_flags(kHeroes | kMinions | kYasuoWall)->_is_fixed_range(TRUE)->_skillshot_type(kMissileLine)
		);

		spells.push_back
		(
			InitializeSpell("Draven")->_spell_name("DravenDoubleShot")->_missile_names({ "DravenDoubleShotMissile" })->_danger_level(kHigh)->_spell_slot(SpellSlot::E)->_raw_range(1100)->_raw_radius(130)->_missile_speed(1400)->_collision_flags(kYasuoWall)->_is_fixed_range(TRUE)->_is_dangerous(TRUE)->_skillshot_type(kMissileLine)
		);

		spells.push_back
		(
			InitializeSpell("Draven")->_spell_name("DravenRCast")->_extra_spell_names({ "DravenRDoublecast" })->_missile_names({ "DravenR" })->_spell_slot(SpellSlot::R)->_raw_range(25000)->_delay(425)->_raw_radius(160)->_missile_speed(2000)->_collision_flags(kYasuoWall)->_is_fixed_range(TRUE)->_is_dangerous(TRUE)->_infront_start(TRUE)->_skillshot_type(kMissileLine)
		);

		spells.push_back
		(
			InitializeSpell("Ekko")->_spell_name("EkkoQ")->_missile_names({ "EkkoQMis" })->_danger_level(kMedium)->_raw_range(950)->_raw_radius(60)->_missile_speed(1650)->_collision_flags(kHeroes | kYasuoWall)->_is_fixed_range(TRUE)->_is_dangerous(TRUE)->_skillshot_type(kMissileLine)
		);

		spells.push_back
		(
			InitializeSpell("Ekko")->_spell_name("EkkoQReturn")->_missile_names({ "EkkoQReturn" })->_danger_level(kLow)->_raw_range(20000)->_delay(0)->_raw_radius(100)->_missile_speed(2300)->_collision_flags(kYasuoWall)->_missile_follows_unit(TRUE)->_skillshot_type(kMissileLine)
		);

		spells.push_back
		(
			InitializeSpell("Ekko")->_spell_name("EkkoW")->_missile_names({ "EkkoWMis" })->_danger_level(kHigh)->_spell_slot(SpellSlot::W)->_raw_range(20000)->_delay(2250)->_raw_radius(375)->_skillshot_type(kCircle)->_is_disabled_by_default(TRUE)->_extra_duration(1500)->_extra_delay({ 1050 })
		);

		spells.push_back
		(
			InitializeSpell("Ekko")->_spell_name("EkkoR")->_danger_level(kHigh)->_spell_slot(SpellSlot::R)->_raw_range(20000)->_raw_radius(375)->_missile_speed(1650)->_skillshot_type(kCircle)->_start_from_particle("Ekko_Base_R_TrailEnd")->_end_at_particle("Ekko_Base_R_TrailEnd")
		);

		spells.push_back
		(
			InitializeSpell("Elise")->_spell_name("EliseHumanE")->_missile_names({ "EliseHumanE" })->_danger_level(kHigh)->_spell_slot(SpellSlot::E)->_raw_range(1100)->_raw_radius(55)->_missile_speed(1600)->_collision_flags(kHeroes | kMinions | kYasuoWall)->_is_fixed_range(TRUE)->_is_dangerous(TRUE)->_skillshot_type(kMissileLine)
		);

		spells.push_back
		(
			InitializeSpell("Evelynn")->_spell_name("EvelynnQ")->_missile_names({ "EvelynnQ" })->_spell_slot(SpellSlot::Q)->_raw_range(800)->_raw_radius(70)->_missile_speed(2400)->_skillshot_type(kMissileLine)->_danger_level(kMedium)->_is_fixed_range(TRUE)->_is_dangerous(TRUE)
		);

		spells.push_back
		(
			InitializeSpell("Evelynn")->_spell_name("EvelynnR")->_spell_slot(SpellSlot::R)->_raw_range(0)->_raw_radius(450)->_skillshot_type(kCircle)->_danger_level(kHigh)->_is_dangerous(TRUE)
		);

		spells.push_back
		(
			InitializeSpell("Ezreal")->_spell_name("EzrealQ")->_missile_names({ "EzrealQ" })->_danger_level(kMedium)->_raw_range(1200)->_raw_radius(60)->_missile_speed(2000)->_collision_flags(kHeroes | kMinions | kYasuoWall)->_is_fixed_range(TRUE)->_skillshot_type(kMissileLine)
		);

		spells.push_back
		(
			InitializeSpell("Ezreal")->_spell_name("EzrealW")->_missile_names({ "EzrealW" })->_danger_level(kMedium)->_raw_range(1150)->_raw_radius(60)->_missile_speed(1650)->_collision_flags(kHeroes | kMinions | kYasuoWall)->_is_fixed_range(TRUE)->_skillshot_type(kMissileLine)
		);

		spells.push_back
		(
			InitializeSpell("Ezreal")->_spell_name("EzrealR")->_missile_names({ "EzrealR" })->_danger_level(kHigh)->_spell_slot(SpellSlot::R)->_raw_range(25000)->_delay(1000)->_raw_radius(160)->_missile_speed(2000)->_collision_flags(kYasuoWall)->_is_fixed_range(TRUE)->_is_dangerous(TRUE)->_skillshot_type(kMissileLine)
		);

		spells.push_back
		(
			InitializeSpell("Fiora")->_spell_name("FioraW")->_missile_names({ "FioraWMissile" })->_danger_level(kMedium)->_spell_slot(SpellSlot::W)->_raw_range(770)->_delay(500)->_raw_radius(70)->_missile_speed(3200)->_collision_flags(kYasuoWall)->_is_fixed_range(TRUE)->_behind_start(50)->_skillshot_type(kMissileLine)
		);

		spells.push_back
		(
			InitializeSpell("Fizz")->_spell_name("FizzR")->_missile_names({ "FizzRMissile" })->_spell_slot(SpellSlot::R)->_raw_range(1300)->_raw_radius(120)->_missile_speed(1350)->_extra_range(125)->_collision_flags(kHeroes | kYasuoWall)->_danger_level(kExtreme)->_is_dangerous(TRUE)->_skillshot_type(kMissileLine)
		);

		spells.push_back
		(
			InitializeSpell("Galio")->_spell_name("GalioQ")->_danger_level(kMedium)->_raw_range(825)->_raw_radius(200)->_missile_speed(1300)->_skillshot_type(kCircle)->_collision_flags(kYasuoWall)->_spell_slot(SpellSlot::Q)
		);

		spells.push_back
		(
			InitializeSpell("Galio")->_spell_name("GalioE")->_danger_level(kMedium)->_spell_slot(SpellSlot::E)->_raw_range(950)->_raw_radius(120)->_delay(250)->_missile_speed(1300)->_behind_start(300)->_collision_flags(kYasuoWall)->_is_dangerous(TRUE)->_skillshot_type(kMissileLine)
		);

		spells.push_back
		(
			InitializeSpell("Galio")->_spell_name("GalioR")->_spell_slot(SpellSlot::R)->_raw_radius(650)->_raw_range(6000)->_delay(2250)->_skillshot_type(kCircle)->_is_dangerous(TRUE)->_is_disabled_by_default(TRUE)->_dont_cross(TRUE)->_danger_level(kExtreme)
		);

		spells.push_back
		(
			InitializeSpell("Gnar")->_spell_name("GnarQMissile")->_missile_names({ "GnarQMissile" })->_danger_level(kMedium)->_raw_range(1125)->_raw_radius(55)->_missile_speed(2500)->_collision_flags(kYasuoWall)->_is_fixed_range(TRUE)->_missile_min_speed(1400)->_missile_max_speed(2500)->_skillshot_type(kMissileLine)
		);

		spells.push_back
		(
			InitializeSpell("Gnar")->_spell_name("GnarQMissileReturn")->_missile_names({ "GnarQMissileReturn" })->_danger_level(kMedium)->_raw_range(3000)->_delay(0)->_raw_radius(75)->_missile_speed(60)->_collision_flags(kYasuoWall)->_is_fixed_range(TRUE)->_missile_accel(800)->_missile_min_speed(60)->_missile_max_speed(2600)->_invert(TRUE)->_is_disabled_by_default(TRUE)->_skillshot_type(kMissileLine)
		);

		spells.push_back
		(
			InitializeSpell("Gnar")->_spell_name("GnarBigQMissile")->_missile_names({ "GnarBigQMissile" })->_danger_level(kMedium)->_raw_range(1150)->_delay(500)->_raw_radius(90)->_missile_speed(2100)->_collision_flags(kHeroes | kMinions | kYasuoWall)->_is_fixed_range(TRUE)->_radius_ex(210)->_skillshot_type(kMissileLine)
		);

		spells.push_back
		(
			InitializeSpell("Gnar")->_spell_name("GnarBigW")->_danger_level(kMedium)->_spell_slot(SpellSlot::W)->_raw_range(600)->_delay(600)->_raw_radius(110)->_skillshot_type(kLine)->_is_fixed_range(TRUE)
		);

		spells.push_back
		(
			InitializeSpell("Gnar")->_spell_name("GnarE")->_danger_level(kMedium)->_spell_slot(SpellSlot::E)->_raw_range(475)->_delay(0)->_raw_radius(150)->_missile_speed(900)->_skillshot_type(kCircle)
		);

		spells.push_back
		(
			InitializeSpell("Gnar")->_spell_name("GnarBigE")->_danger_level(kMedium)->_spell_slot(SpellSlot::E)->_raw_range(475)->_delay(0)->_raw_radius(350)->_missile_speed(800)->_skillshot_type(kCircle)
		);

		spells.push_back
		(
			InitializeSpell("Gnar")->_spell_name("GnarR")->_spell_slot(SpellSlot::R)->_delay(275)->_raw_radius(500)->_skillshot_type(kCircle)->_is_dangerous(TRUE)->_danger_level(kExtreme)
		);

		spells.push_back
		(
			InitializeSpell("Gragas")->_spell_name("GragasQ")->_missile_names({ "GragasQMissile" })->_extra_duration(0 /*4300*/)->_toggle_name("Gragas_.+_Q_(Enemy|Ally)")->_spellbook_spell_name("GragasQToggle")->_danger_level(kMedium)->_raw_range(850)->_raw_radius(280)->_missile_speed(1000)->_collision_flags(kYasuoWall)->_skillshot_type(kCircle)->_dont_cross(TRUE)
		);

		spells.push_back
		(
			InitializeSpell("Gragas")->_spell_name("GragasE")->_danger_level(kMedium)->_spell_slot(SpellSlot::E)->_raw_range(600)->_delay(0)->_raw_radius(200)->_missile_speed(900)->_collision_flags(kHeroes | kMinions)->_is_fixed_range(TRUE)->_skillshot_type(kMissileLine)
		);

		spells.push_back
		(
			InitializeSpell("Gragas")->_spell_name("GragasR")->_missile_names({ "GragasRBoom" })->_spell_slot(SpellSlot::R)->_raw_range(1050)->_raw_radius(350)->_missile_speed(1800)->_skillshot_type(kCircle)->_collision_flags(kYasuoWall)->_is_dangerous(TRUE)->_danger_level(kExtreme)
		);

		spells.push_back
		(
			InitializeSpell("Graves")->_spell_name("GravesQLineSpell")->_missile_names({ "GravesQLineMis" })->_danger_level(kMedium)->_raw_range(808)->_raw_radius(40)->_missile_speed(3000)->_collision_flags(kYasuoWall)->_is_fixed_range(TRUE)->_skillshot_type(kMissileLine)
		);

		spells.push_back
		(
			InitializeSpell("Graves")->_spell_name("GravesQReturn")->_missile_names({ "GravesQReturn" })->_danger_level(kMedium)->_raw_range(808)->_delay(0)->_raw_radius(100)->_missile_speed(1600)->_collision_flags(kYasuoWall)->_is_fixed_range(TRUE)->_skillshot_type(kMissileLine)
		);

		spells.push_back
		(
			InitializeSpell("Graves")->_spell_name("GravesSmokeGrenade")->_missile_names({ "GravesSmokeGrenadeBoom" })->_spell_slot(SpellSlot::W)->_raw_range(950)->_raw_radius(225)->_missile_speed(1500)->_collision_flags(kYasuoWall)->_skillshot_type(kCircle)->_extra_duration(4250)
		);

		spells.push_back
		(
			InitializeSpell("Graves")->_spell_name("GravesChargeShot")->_missile_names({ "GravesChargeShotShot" })->_spell_slot(SpellSlot::R)->_raw_range(1000)->_raw_radius(100)->_missile_speed(2100)->_collision_flags(kYasuoWall)->_is_fixed_range(TRUE)->_is_dangerous(TRUE)->_skillshot_type(kMissileLine)->_danger_level(kExtreme)
		);

		spells.push_back
		(
			InitializeSpell("Hecarim")->_spell_name("HecarimUlt")->_spell_slot(SpellSlot::R)->_raw_range(1000)->_delay(0)->_raw_radius(240)->_missile_speed(1100)->_skillshot_type(kCircle)->_is_dangerous(TRUE)->_danger_level(kExtreme)
		);

		spells.push_back
		(
			InitializeSpell("Heimerdinger")->_spell_name("HeimerdingerQTurretBlast")->_missile_names({ "HeimerdingerTurretEnergyBlast" })->_danger_level(kMedium)->_raw_range(1000)->_delay(0)->_raw_radius(50)->_missile_speed(1650)->_collision_flags(kYasuoWall)->_is_fixed_range(TRUE)->_missile_min_speed(1200)->_missile_max_speed(1650)->_skillshot_type(kMissileLine)
		);

		spells.push_back
		(
			InitializeSpell("Heimerdinger")->_spell_name("HeimerdingerQTurretBigBlast")->_missile_names({ "HeimerdingerTurretBigEnergyBlast" })->_danger_level(kMedium)->_raw_range(1000)->_delay(0)->_raw_radius(75)->_missile_speed(1650)->_collision_flags(kMinions | kYasuoWall)->_is_fixed_range(TRUE)->_missile_min_speed(1200)->_missile_max_speed(1650)->_skillshot_type(kMissileLine)
		);

		spells.push_back
		(
			InitializeSpell("Heimerdinger")->_spell_name("HeimerdingerW")->_missile_names({ "HeimerdingerWAttack2", "HeimerdingerWAttack2Ult" })->_danger_level(kMedium)->_spell_slot(SpellSlot::W)->_raw_range(1350)->_delay(0)->_raw_radius(40)->_missile_speed(750)->_collision_flags(kHeroes | kMinions | kYasuoWall)->_is_fixed_range(TRUE)->_missile_accel(4000)->_missile_min_speed(750)->_missile_max_speed(3000)->_skillshot_type(kMissileLine)
		);

		spells.push_back
		(
			InitializeSpell("Heimerdinger")->_spell_name("HeimerdingerE")->_missile_names({ "HeimerdingerESpell" })->_danger_level(kMedium)->_spell_slot(SpellSlot::E)->_raw_range(950)->_raw_radius(100)->_missile_speed(1200)->_skillshot_type(kCircle)->_collision_flags(kYasuoWall)
		);

		spells.push_back
		(
			InitializeSpell("Heimerdinger")->_spell_name("HeimerdingerEUlt")->_missile_names({ "HeimerdingerESpell_ult" })->_danger_level(kMedium)->_spell_slot(SpellSlot::E)->_raw_range(1000)->_raw_radius(150)->_missile_speed(1200)->_skillshot_type(kCircle)->_collision_flags(kYasuoWall)
		);

		spells.push_back
		(
			InitializeSpell("Heimerdinger")->_spell_name("HeimerdingerEUltBounce")->_missile_names({ "HeimerdingerESpell_ult2", "HeimerdingerESpell_ult3" })->_danger_level(kMedium)->_spell_slot(SpellSlot::E)->_raw_range(1000)->_delay(175)->_raw_radius(150)->_missile_speed(1400)->_skillshot_type(kCircle)->_collision_flags(kYasuoWall)->_is_disabled_by_default(TRUE)->_missile_delayed(TRUE)
		);

		spells.push_back
		(
			InitializeSpell("Illaoi")->_spell_name("IllaoiQ")->_danger_level(kMedium)->_raw_range(850)->_delay(750)->_raw_radius(100)->_skillshot_type(kLine)->_is_fixed_range(TRUE)->_is_dangerous(TRUE)->_behind_start(50)
		);

		spells.push_back
		(
			InitializeSpell("Illaoi")->_spell_name("IllaoiE")->_missile_names({ "IllaoiEMis" })->_danger_level(kHigh)->_spell_slot(SpellSlot::E)->_raw_range(950)->_delay(265)->_raw_radius(50)->_missile_speed(1900)->_collision_flags(kHeroes | kMinions | kYasuoWall)->_is_fixed_range(TRUE)->_is_dangerous(TRUE)->_skillshot_type(kMissileLine)
		);

		spells.push_back
		(
			InitializeSpell("Illaoi")->_spell_name("IllaoiR")->_danger_level(kHigh)->_spell_slot(SpellSlot::R)->_delay(500)->_raw_radius(450)->_skillshot_type(kCircle)->_is_dangerous(TRUE)->_danger_level(kExtreme)
		);

		spells.push_back
		(
			InitializeSpell("Irelia")->_spell_name("IreliaR")->_missile_names({ "IreliaR" })->_danger_level(kMedium)->_spell_slot(SpellSlot::R)->_raw_range(950)->_delay(400)->_raw_radius(130)->_missile_speed(2000)->_collision_flags(kYasuoWall | kHeroes)->_is_fixed_range(TRUE)->_skillshot_type(kMissileLine)
		);

		spells.push_back
		(
			InitializeSpell("Ivern")->_spell_name("IvernQ")->_missile_names({ "IvernQ" })->_danger_level(kHigh)->_raw_range(1100)->_raw_radius(65)->_missile_speed(1350)->_collision_flags(kHeroes | kMinions | kYasuoWall)->_is_fixed_range(TRUE)->_is_dangerous(TRUE)->_skillshot_type(kMissileLine)
		);

		spells.push_back
		(
			InitializeSpell("Janna")->_spell_name("JannaQ")->_missile_names({ "HowlingGaleSpell" })->_danger_level(kMedium)->_raw_range(20000)->_delay(0)->_raw_radius(120)->_missile_speed(900)->_collision_flags(kYasuoWall)->_skillshot_type(kMissileLine)
		);

		spells.push_back
		(
			InitializeSpell("JarvanIV")->_spell_name("JarvanIVDragonStrike")->_danger_level(kHigh)->_raw_range(770)->_delay(425)->_raw_radius(70)->_skillshot_type(kLine)->_is_fixed_range(TRUE)
		);

		spells.push_back
		(
			InitializeSpell("JarvanIV")->_spell_name("JarvanIVQE")->_danger_level(kHigh)->_raw_range(910)->_delay(450)->_raw_radius(120)->_missile_speed(2600)->_is_dangerous(TRUE)->_radius_ex(210)->_skillshot_type(kMissileLine)
		);

		spells.push_back
		(
			InitializeSpell("JarvanIV")->_spell_name("JarvanIVDemacianStandard")->_danger_level(kMedium)->_spell_slot(SpellSlot::E)->_raw_range(850)->_delay(450)->_raw_radius(150)->_skillshot_type(kCircle)
		);

		spells.push_back
		(
			InitializeSpell("Jayce")->_spell_name("JayceShockBlast")->_missile_names({ "JayceShockBlastMis" })->_danger_level(kMedium)->_delay(230)->_raw_range(1050)->_raw_radius(70)->_missile_speed(1450)->_collision_flags(kHeroes | kMinions | kYasuoWall)->_is_fixed_range(TRUE)->_radius_ex(275)->_skillshot_type(kMissileLine)
		);

		spells.push_back
		(
			InitializeSpell("Jayce")->_spell_name("JayceQAccel")->_missile_names({ "JayceShockBlastWallMis" })->_danger_level(kMedium)->_raw_range(2000)->_delay(0)->_raw_radius(70)->_missile_speed(2350)->_collision_flags(kHeroes | kMinions | kYasuoWall)->_radius_ex(275)->_skillshot_type(kMissileLine)
		);

		spells.push_back
		(
			InitializeSpell("Jhin")->_spell_name("JhinW")->_danger_level(kHigh)->_spell_slot(SpellSlot::W)->_raw_range(2550)->_delay(750)->_raw_radius(40)->_skillshot_type(kLine)->_collision_flags(kHeroes | kYasuoWall)->_is_fixed_range(TRUE)
		);

		spells.push_back
		(
			InitializeSpell("Jhin")->_spell_name("JhinRShot")->_missile_names({ "JhinRShotMis", "JhinRShotMis4" })->_danger_level(kHigh)->_spell_slot(SpellSlot::R)->_raw_range(3500)->_delay(190)->_raw_radius(80)->_missile_speed(5000)->_collision_flags(kHeroes | kYasuoWall)->_is_fixed_range(TRUE)->_is_dangerous(TRUE)->_skillshot_type(kMissileLine)
		);

		spells.push_back
		(
			InitializeSpell("Jinx")->_spell_name("JinxWMissile")->_missile_names({ "JinxWMissile" })->_danger_level(kHigh)->_spell_slot(SpellSlot::W)->_raw_range(1500)->_delay(600)->_raw_radius(60)->_missile_speed(3300)->_collision_flags(kHeroes | kMinions | kYasuoWall)->_is_fixed_range(TRUE)->_is_dangerous(TRUE)->_skillshot_type(kMissileLine)
		);

		spells.push_back
		(
			InitializeSpell("Jinx")->_spell_name("JinxR")->_missile_names({ "JinxR" })->_spell_slot(SpellSlot::R)->_raw_range(25000)->_delay(600)->_raw_radius(140)->_missile_speed(1700)->_collision_flags(kHeroes | kYasuoWall)->_is_fixed_range(TRUE)->_is_dangerous(TRUE)->_skillshot_type(kMissileLine)->_danger_level(kExtreme)
		);

		spells.push_back
		(
			InitializeSpell("Kaisa")->_spell_name("KaisaW")->_missile_names({ "KaisaW" })->_spell_slot(SpellSlot::W)->_raw_range(3000)->_delay(400)->_raw_radius(60)->_missile_speed(1750)->_collision_flags(kHeroes | kMinions | kYasuoWall)->_is_fixed_range(TRUE)->_is_dangerous(TRUE)->_skillshot_type(kMissileLine)
		);

		spells.push_back
		(
			InitializeSpell("Kalista")->_spell_name("KalistaMysticShot")->_missile_names({ "KalistaMysticShotMisTrue" })->_danger_level(kMedium)->_raw_range(1200)->_raw_radius(40)->_missile_speed(2400)->_collision_flags(kHeroes | kMinions | kYasuoWall)->_is_fixed_range(TRUE)->_skillshot_type(kMissileLine)
		);

		spells.push_back
		(
			InitializeSpell("Karma")->_spell_name("KarmaQ")->_missile_names({ "KarmaQMissile" })->_danger_level(kMedium)->_raw_range(1050)->_raw_radius(60)->_missile_speed(1700)->_collision_flags(kHeroes | kMinions | kYasuoWall)->_is_fixed_range(TRUE)->_skillshot_type(kMissileLine)
		);

		spells.push_back
		(
			InitializeSpell("Karma")->_spell_name("KarmaQMantra")->_missile_names({ "KarmaQMissileMantra" })->_danger_level(kMedium)->_raw_range(950)->_raw_radius(80)->_missile_speed(1700)->_collision_flags(kHeroes | kMinions | kYasuoWall)->_is_fixed_range(TRUE)->_extra_duration(1650)->_radius_ex(280)->_skillshot_type(kMissileLine)
		);

		spells.push_back
		(
			InitializeSpell("Karthus")->_spell_name("KarthusLayWasteA1")->_extra_spell_names({ "KarthusLayWasteA2", "KarthusLayWasteA3", "KarthusLayWasteDeadA1", "KarthusLayWasteDeadA2", "KarthusLayWasteDeadA3" })->_danger_level(kMedium)->_raw_range(875)->_delay(950)->_raw_radius(160)->_skillshot_type(kCircle)
		);

		spells.push_back
		(
			InitializeSpell("Kennen")->_spell_name("KennenShurikenHurlMissile1")->_missile_names({ "KennenShurikenHurlMissile1" })->_danger_level(kMedium)->_raw_range(1050)->_delay(175)->_raw_radius(50)->_missile_speed(1700)->_collision_flags(kHeroes | kMinions | kYasuoWall)->_is_fixed_range(TRUE)->_skillshot_type(kMissileLine)
		);

		spells.push_back
		(
			InitializeSpell("Khazix")->_spell_name("KhazixW")->_extra_spell_names({ "KhazixWLong" })->_missile_names({ "KhazixWMissile" })->_danger_level(kMedium)->_spell_slot(SpellSlot::W)->_raw_range(1025)->_raw_radius(70)->_missile_speed(1700)->_collision_flags(kHeroes | kMinions | kYasuoWall)->_is_fixed_range(TRUE)->_skillshot_type(kMissileLine)->_multiple_number(3)->_multiple_angle(22.0f * M_PI / 180.0f)
		);

		spells.push_back
		(
			InitializeSpell("Khazix")->_spell_name("KhazixE")->_danger_level(kMedium)->_spell_slot(SpellSlot::E)->_raw_range(700)->_delay(0)->_raw_radius(300)->_missile_speed(1250)->_skillshot_type(kCircle)
		);

		spells.push_back
		(
			InitializeSpell("Khazix")->_spell_name("KhazixELong")->_danger_level(kMedium)->_spell_slot(SpellSlot::E)->_raw_range(900)->_delay(0)->_raw_radius(300)->_missile_speed(1200)->_skillshot_type(kCircle)
		);

		spells.push_back
		(
			InitializeSpell("Kled")->_spell_name("KledQ")->_missile_names({ "KledQMissile" })->_danger_level(kMedium)->_raw_range(800)->_raw_radius(45)->_missile_speed(1600)->_collision_flags(kHeroes | kYasuoWall)->_is_fixed_range(TRUE)->_is_dangerous(TRUE)->_skillshot_type(kMissileLine)
		);

		spells.push_back
		(
			InitializeSpell("Kled")->_spell_name("KledRiderQ")->_missile_names({ "KledRiderQMissile" })->_danger_level(kMedium)->_raw_range(700)->_raw_radius(40)->_missile_speed(3000)->_collision_flags(kYasuoWall)->_is_fixed_range(TRUE)->_skillshot_type(kMissileLine)->_multiple_number(5)->_multiple_angle(5.0f * M_PI / 180.0f)
		);

		spells.push_back
		(
			InitializeSpell("Kled")->_spell_name("KledEDash")->_danger_level(kMedium)->_spell_slot(SpellSlot::E)->_raw_range(550)->_delay(0)->_raw_radius(100)->_missile_speed(970)->_is_fixed_range(TRUE)->_is_dangerous(TRUE)->_skillshot_type(kMissileLine)
		);

		spells.push_back
		(
			InitializeSpell("KogMaw")->_spell_name("KogMawQ")->_missile_names({ "KogMawQ" })->_danger_level(kMedium)->_raw_range(1200)->_raw_radius(70)->_missile_speed(1650)->_collision_flags(kHeroes | kMinions | kYasuoWall)->_is_fixed_range(TRUE)->_skillshot_type(kMissileLine)
		);

		spells.push_back
		(
			InitializeSpell("KogMaw")->_spell_name("KogMawVoidOozeMissile")->_missile_names({ "KogMawVoidOozeMissile" })->_danger_level(kMedium)->_spell_slot(SpellSlot::E)->_raw_range(1300)->_raw_radius(120)->_missile_speed(1400)->_collision_flags(kYasuoWall)->_is_fixed_range(TRUE)->_skillshot_type(kMissileLine)
		);

		spells.push_back
		(
			InitializeSpell("KogMaw")->_spell_name("KogMawLivingArtillery")->_danger_level(kMedium)->_spell_slot(SpellSlot::R)->_raw_range(1200)->_delay(1150)->_raw_radius(240)->_skillshot_type(kCircle)
		);

		spells.push_back
		(
			InitializeSpell("Kayn")->_spell_name("KaynW")->_danger_level(kMedium)->_spell_slot(SpellSlot::W)->_raw_range(700)->_delay(500)->_raw_radius(80)->_missile_speed(INT_MAX)->_skillshot_type(kLine)->_is_fixed_range(TRUE)->_is_dangerous(TRUE)
		);

		spells.push_back
		(
			InitializeSpell("Kayn")->_spell_name("KaynAssW")->_danger_level(kMedium)->_spell_slot(SpellSlot::W)->_raw_range(875)->_delay(600)->_raw_radius(80)->_missile_speed(INT_MAX)->_skillshot_type(kLine)->_is_fixed_range(TRUE)
		);

		spells.push_back
		(
			InitializeSpell("Leblanc")->_spell_name("LeblancW")->_danger_level(kMedium)->_spell_slot(SpellSlot::W)->_raw_range(600)->_delay(0)->_raw_radius(220)->_missile_speed(1600)->_skillshot_type(kCircle)
		);

		spells.push_back
		(
			InitializeSpell("Leblanc")->_spell_name("LeblancRW")->_danger_level(kMedium)->_spell_slot(SpellSlot::W)->_raw_range(600)->_delay(0)->_raw_radius(220)->_missile_speed(1600)->_skillshot_type(kCircle)
		);

		spells.push_back
		(
			InitializeSpell("Leblanc")->_spell_name("LeblancE")->_missile_names({ "LeblancEMissile" })->_danger_level(kHigh)->_spell_slot(SpellSlot::E)->_raw_range(950)->_raw_radius(55)->_missile_speed(1750)->_collision_flags(kHeroes | kMinions | kYasuoWall)->_is_fixed_range(TRUE)->_is_dangerous(TRUE)->_skillshot_type(kMissileLine)
		);

		spells.push_back
		(
			InitializeSpell("Leblanc")->_spell_name("LeblancRE")->_missile_names({ "LeblancREMissile" })->_danger_level(kHigh)->_spell_slot(SpellSlot::E)->_raw_range(950)->_raw_radius(55)->_missile_speed(1750)->_collision_flags(kHeroes | kMinions | kYasuoWall)->_is_fixed_range(TRUE)->_is_dangerous(TRUE)->_skillshot_type(kMissileLine)
		);

		spells.push_back
		(
			InitializeSpell("LeeSin")->_spell_name("BlindMonkQOne")->_missile_names({ "BlindMonkQOne" })->_danger_level(kHigh)->_raw_range(1100)->_raw_radius(60)->_missile_speed(1800)->_collision_flags(kHeroes | kMinions | kYasuoWall)->_is_fixed_range(TRUE)->_is_dangerous(TRUE)->_skillshot_type(kMissileLine)
		);

		spells.push_back
		(
			InitializeSpell("Leona")->_spell_name("LeonaZenithBlade")->_missile_names({ "LeonaZenithBladeMissile" })->_danger_level(kHigh)->_spell_slot(SpellSlot::E)->_raw_range(900)->_raw_radius(70)->_missile_speed(2000)->_collision_flags(kYasuoWall)->_is_fixed_range(TRUE)->_is_dangerous(TRUE)->_skillshot_type(kMissileLine)
		);

		spells.push_back
		(
			InitializeSpell("Leona")->_spell_name("LeonaSolarFlare")->_spell_slot(SpellSlot::R)->_raw_range(1200)->_delay(930)->_raw_radius(300)->_skillshot_type(kCircle)->_is_dangerous(TRUE)->_danger_level(kExtreme)
		);

		spells.push_back
		(
			InitializeSpell("Lissandra")->_spell_name("LissandraQMissile")->_missile_names({ "LissandraQMissile" })->_danger_level(kMedium)->_raw_range(700)->_raw_radius(75)->_missile_speed(2200)->_collision_flags(kYasuoWall)->_is_fixed_range(TRUE)->_skillshot_type(kMissileLine)
		);

		spells.push_back
		(
			InitializeSpell("Lissandra")->_spell_name("LissandraQShards")->_missile_names({ "LissandraQShards" })->_danger_level(kMedium)->_raw_range(1650)->_delay(0)->_raw_radius(90)->_missile_speed(2200)->_collision_flags(kYasuoWall)->_skillshot_type(kMissileLine)
		);

		spells.push_back
		(
			InitializeSpell("Lissandra")->_spell_name("LissandraEMissile")->_missile_names({ "LissandraEMissile" })->_danger_level(kMedium)->_spell_slot(SpellSlot::E)->_raw_range(1025)->_raw_radius(125)->_missile_speed(850)->_collision_flags(kYasuoWall)->_is_fixed_range(TRUE)->_skillshot_type(kMissileLine)
		);

		spells.push_back
		(
			InitializeSpell("Lucian")->_spell_name("LucianQ")->_danger_level(kMedium)->_raw_range(900)->_delay(350)->_raw_radius(65)->_skillshot_type(kLine)->_is_fixed_range(TRUE)
		);

		spells.push_back
		(
			InitializeSpell("Lucian")->_spell_name("LucianW")->_missile_names({ "LucianWMissile" })->_danger_level(kMedium)->_spell_slot(SpellSlot::W)->_raw_range(900)->_delay(325)->_raw_radius(55)->_missile_speed(1600)->_collision_flags(kHeroes | kMinions | kYasuoWall)->_is_fixed_range(TRUE)->_skillshot_type(kMissileLine)
		);

		spells.push_back
		(
			InitializeSpell("Lucian")->_spell_name("LucianR")->_missile_names({ "LucianRMissileOffhand", "LucianRMissile" })->_danger_level(kMedium)->_spell_slot(SpellSlot::R)->_raw_range(1200)->_delay(0)->_raw_radius(110)->_missile_speed(2800)->_collision_flags(kHeroes | kMinions | kYasuoWall)->_is_fixed_range(TRUE)->_is_disabled_by_default(TRUE)->_skillshot_type(kMissileLine)
		);

		spells.push_back
		(
			InitializeSpell("Lulu")->_spell_name("LuluQ")->_missile_names({ "LuluQMissile", "LuluQMissileTwo" })->_danger_level(kMedium)->_raw_range(950)->_raw_radius(60)->_missile_speed(1450)->_collision_flags(kYasuoWall)->_is_fixed_range(TRUE)->_skillshot_type(kMissileLine)
		);

		spells.push_back
		(
			InitializeSpell("Lux")->_spell_name("LuxLightBinding")->_missile_names({ "LuxLightBindingMis" })->_danger_level(kHigh)->_raw_range(1300)->_raw_radius(70)->_missile_speed(1200)->_collision_flags(kYasuoWall)->_is_fixed_range(TRUE)->_is_dangerous(TRUE)->_skillshot_type(kMissileLine)
		);

		spells.push_back
		(
			InitializeSpell("Lux")->_spell_name("LuxLightStrikeKugel")->_missile_names({ "LuxLightStrikeKugel" })->_danger_level(kMedium)->_spell_slot(SpellSlot::E)->_raw_range(1100)->_raw_radius(330)->_missile_speed(1300)->_skillshot_type(kCircle)->_collision_flags(kYasuoWall)->_dont_cross(TRUE)->_is_disabled_by_default(TRUE)
		);

		spells.push_back
		(
			InitializeSpell("Lux")->_spell_name("LuxMaliceCannon")->_spell_slot(SpellSlot::R)->_raw_range(3300)->_delay(1000)->_raw_radius(150)->_skillshot_type(kLine)->_is_fixed_range(TRUE)->_is_dangerous(TRUE)->_danger_level(kExtreme)
		);

		spells.push_back
		(
			InitializeSpell("Malphite")->_spell_name("UFSlash")->_spell_slot(SpellSlot::R)->_raw_range(1000)->_delay(0)->_raw_radius(270)->_missile_speed(1600)->_skillshot_type(kCircle)->_is_dangerous(TRUE)->_danger_level(kExtreme)
		);

		spells.push_back
		(
			InitializeSpell("Malzahar")->_spell_name("MalzaharQ")->_missile_names({ "MalzaharQMissile" })->_danger_level(kMedium)->_raw_range(750)->_delay(520)->_raw_radius(85)->_missile_speed(1600)->_is_fixed_range(TRUE)->_skillshot_type(kMissileLine)
		);

		spells.push_back
		(
			InitializeSpell("Maokai")->_spell_name("MaokaiQ")->_missile_names({ "MaokaiQMissile" })->_danger_level(kHigh)->_raw_range(650)->_delay(350)->_raw_radius(110)->_missile_speed(1600)->_is_fixed_range(TRUE)->_is_dangerous(TRUE)->_skillshot_type(kMissileLine)
		);

		spells.push_back
		(
			InitializeSpell("Morgana")->_spell_name("DarkBindingMissile")->_missile_names({ "DarkBindingMissile" })->_danger_level(kHigh)->_raw_range(1300)->_raw_radius(70)->_missile_speed(1200)->_collision_flags(kHeroes | kMinions | kYasuoWall)->_is_fixed_range(TRUE)->_is_dangerous(TRUE)->_skillshot_type(kMissileLine)
		);

		spells.push_back
		(
			InitializeSpell("Nami")->_spell_name("NamiQ")->_missile_names({ "NamiQMissile" })->_danger_level(kHigh)->_raw_range(850)->_raw_radius(200)->_missile_speed(2500)->_skillshot_type(kCircle)->_collision_flags(kYasuoWall)->_is_dangerous(TRUE)->_extra_delay({ 700 })
		);

		spells.push_back
		(
			InitializeSpell("Nami")->_spell_name("NamiRMissile")->_missile_names({ "NamiRMissile" })->_danger_level(kExtreme)->_spell_slot(SpellSlot::R)->_raw_range(2750)->_delay(500)->_raw_radius(250)->_missile_speed(850)->_collision_flags(kYasuoWall)->_is_fixed_range(TRUE)->_skillshot_type(kMissileLine)
		);

		spells.push_back
		(
			InitializeSpell("Nautilus")->_spell_name("NautilusAnchorDragMissile")->_missile_names({ "NautilusAnchorDragMissile" })->_danger_level(kHigh)->_raw_range(1150)->_raw_radius(90)->_missile_speed(2000)->_collision_flags(kHeroes | kMinions | kYasuoWall)->_is_fixed_range(TRUE)->_is_dangerous(TRUE)->_skillshot_type(kMissileLine)
		);

		spells.push_back
		(
			InitializeSpell("Nidalee")->_spell_name("JavelinToss")->_missile_names({ "JavelinToss" })->_danger_level(kHigh)->_raw_range(1500)->_raw_radius(40)->_missile_speed(1300)->_collision_flags(kHeroes | kMinions | kYasuoWall)->_is_fixed_range(TRUE)->_is_dangerous(TRUE)->_skillshot_type(kMissileLine)
		);

		spells.push_back
		(
			InitializeSpell("Nocturne")->_spell_name("NocturneDuskbringer")->_missile_names({ "NocturneDuskbringer" })->_danger_level(kMedium)->_raw_range(1200)->_raw_radius(60)->_missile_speed(1400)->_collision_flags(kYasuoWall)->_is_fixed_range(TRUE)->_skillshot_type(kMissileLine)
		);

		spells.push_back
		(
			InitializeSpell("Olaf")->_spell_name("OlafAxeThrowCast")->_missile_names({ "OlafAxeThrow" })->_danger_level(kMedium)->_raw_range(1000)->_minimum_range(400)->_raw_radius(90)->_missile_speed(1600)->_collision_flags(kYasuoWall)->_skillshot_type(kMissileLine)->_is_dangerous(TRUE)
		);

		spells.push_back
		(
			InitializeSpell("Ornn")->_spell_name("OrnnQ")->_missile_names({ "OrnnQMissile" })->_danger_level(kMedium)->_raw_range(1000)->_raw_radius(65)->_missile_speed(1800)->_collision_flags(kYasuoWall | kHeroes | kMinions)->_skillshot_type(kMissileLine)->_is_dangerous(TRUE)->_is_fixed_range(TRUE)
		);

		spells.push_back
		(
			InitializeSpell("Ornn")->_spell_name("OrnnRWave")->_missile_names({ "OrnnRWave2" })->_spell_slot(SpellSlot::R)->_danger_level(kExtreme)->_raw_range(3500)->_raw_radius(200)->_missile_speed(1650)->_skillshot_type(kMissileLine)->_is_dangerous(TRUE)->_is_fixed_range(TRUE)
		);

		spells.push_back
		(
			InitializeSpell("Orianna")->_spell_name("OrianaIzunaCommands")->_missile_names({ "OrianaIzuna" })->_danger_level(kMedium)->_raw_range(2000)->_delay(0)->_raw_radius(80)->_missile_speed(1400)->_collision_flags(kYasuoWall)->_radius_ex(230)->_skillshot_type(kMissileLine)
		);

		spells.push_back
		(
			InitializeSpell("Orianna")->_spell_name("OrianaRedactCommand")->_spell_slot(SpellSlot::E)->_missile_names({ "OrianaRedact" })->_danger_level(kMedium)->_raw_range(2000)->_delay(0)->_raw_radius(90)->_missile_speed(1850)->_collision_flags(kYasuoWall)->_missile_follows_unit(TRUE)->_radius_ex(230)->_skillshot_type(kMissileLine)
		);

		spells.push_back
		(
			InitializeSpell("Orianna")->_spell_name("OrianaDetonateCommand")->_spell_slot(SpellSlot::R)->_delay(500)->_raw_radius(410)->_skillshot_type(kCircle)->_is_dangerous(TRUE)->_danger_level(kExtreme)
		);

		spells.push_back
		(
			InitializeSpell("Pantheon")->_spell_name("PantheonE")->_danger_level(kMedium)->_spell_slot(SpellSlot::E)->_raw_range(640)->_delay(400)->_raw_radius(70)->_skillshot_type(kCone)->_extra_duration(750)
		);

		spells.push_back
		(
			InitializeSpell("Poppy")->_spell_name("PoppyQ")->_danger_level(kMedium)->_raw_range(430)->_delay(500)->_raw_radius(100)->_skillshot_type(kLine)->_is_fixed_range(TRUE)->_extra_duration(900)
		);

		spells.push_back
		(
			InitializeSpell("Poppy")->_spell_name("PoppyRSpellInstant")->_danger_level(kExtreme)->_spell_slot(SpellSlot::R)->_raw_range(450)->_delay(350)->_raw_radius(100)->_skillshot_type(kLine)->_is_fixed_range(TRUE)->_is_dangerous(TRUE)
		);

		spells.push_back
		(
			InitializeSpell("Poppy")->_spell_name("PoppyRSpell")->_missile_names({ "PoppyRMissile" })->_danger_level(kHigh)->_spell_slot(SpellSlot::R)->_raw_range(1200)->_delay(350)->_raw_radius(100)->_missile_speed(1600)->_collision_flags(kHeroes | kYasuoWall)->_is_dangerous(TRUE)->_skillshot_type(kMissileLine)
		);

		spells.push_back
		(
			InitializeSpell("Pyke")->_spell_name("PykeQRange")->_missile_names({ "PykeQRange" })->_danger_level(kHigh)->_spell_slot(SpellSlot::Q)->_raw_range(1100)->_delay(250)->_raw_radius(70)->_missile_speed(2000)->_collision_flags(kHeroes | kYasuoWall)->_is_dangerous(TRUE)->_skillshot_type(kMissileLine)
		);

		spells.push_back
		(
			InitializeSpell("Pyke")->_spell_name("PykeE")->_missile_names({ "PykeEMissile" })->_danger_level(kHigh)->_spell_slot(SpellSlot::E)->_raw_range(1100)->_delay(1000)->_raw_radius(110)->_missile_speed(2500)->_collision_flags(kYasuoWall)->_is_dangerous(TRUE)->_skillshot_type(kMissileLine)->_missile_follows_unit(TRUE)
		);

		spells.push_back
		(
			InitializeSpell("Quinn")->_spell_name("QuinnQ")->_missile_names({ "QuinnQ" })->_danger_level(kMedium)->_raw_range(1050)->_raw_radius(60)->_missile_speed(1550)->_collision_flags(kHeroes | kMinions | kYasuoWall)->_is_fixed_range(TRUE)->_skillshot_type(kMissileLine)
		);

		spells.push_back
		(
			InitializeSpell("Rakan")->_spell_name("RakanQ")->_missile_names({ "RakanQMis" })->_danger_level(kMedium)->_delay(250)->_raw_range(850)->_raw_radius(65)->_missile_speed(1850)->_collision_flags(kHeroes | kMinions | kYasuoWall)->_is_fixed_range(TRUE)->_skillshot_type(kMissileLine)
		);

		spells.push_back
		(
			InitializeSpell("Rakan")->_spell_name("RakanW")->_danger_level(kHigh)->_raw_range(650)->_raw_radius(275)->_missile_speed(2000)->_delay(250)->_extra_duration(280)->_is_dangerous(TRUE)->_skillshot_type(kCircle)
		);

		spells.push_back
		(
			InitializeSpell("RekSai")->_spell_name("RekSaiQBurrowed")->_missile_names({ "RekSaiQBurrowedMis" })->_danger_level(kHigh)->_raw_range(1500)->_delay(125)->_raw_radius(65)->_missile_speed(1950)->_collision_flags(kHeroes | kMinions | kYasuoWall)->_is_fixed_range(TRUE)->_skillshot_type(kMissileLine)
		);

		spells.push_back
		(
			InitializeSpell("Rengar")->_spell_name("RengarE")->_missile_names({ "RengarEMis" })->_danger_level(kHigh)->_spell_slot(SpellSlot::E)->_raw_range(1000)->_raw_radius(70)->_missile_speed(1500)->_collision_flags(kHeroes | kMinions | kYasuoWall)->_is_fixed_range(TRUE)->_skillshot_type(kMissileLine)
		);

		spells.push_back
		(
			InitializeSpell("Rengar")->_spell_name("RengarEEmp")->_missile_names({ "RengarEEmpMis" })->_danger_level(kHigh)->_spell_slot(SpellSlot::E)->_raw_range(1000)->_raw_radius(70)->_missile_speed(1500)->_collision_flags(kHeroes | kMinions | kYasuoWall)->_is_fixed_range(TRUE)->_is_dangerous(TRUE)->_skillshot_type(kMissileLine)
		);

		spells.push_back
		(
			InitializeSpell("Riven")->_spell_name("RivenIzunaBlade")->_missile_names({ "RivenWindslashMissileCenter", "RivenWindslashMissileLeft", "RivenWindslashMissileRight" })->_spell_slot(SpellSlot::R)->_danger_level(kExtreme)->_raw_range(1075)->_raw_radius(100)->_missile_speed(1600)->_collision_flags(kYasuoWall)->_is_fixed_range(TRUE)->_is_dangerous(TRUE)->_skillshot_type(kMissileLine)->_multiple_number(5)->_multiple_angle(6.69f * M_PI / 180.0f)
		);

		spells.push_back
		(
			InitializeSpell("Rumble")->_spell_name("RumbleGrenade")->_missile_names({ "RumbleGrenadeMissile", "RumbleGrenadeMissileDangerZone" })->_danger_level(kMedium)->_spell_slot(SpellSlot::E)->_raw_range(950)->_raw_radius(60)->_missile_speed(2000)->_collision_flags(kHeroes | kMinions | kYasuoWall)->_is_fixed_range(TRUE)->_skillshot_type(kMissileLine)
		);

		spells.push_back
		(
			InitializeSpell("Rumble")->_spell_name("RumbleR")->_missile_names({ "RumbleCarpetBombMissile" })->_danger_level(kHigh)->_spell_slot(SpellSlot::R)->_raw_range(20000)->_delay(400)->_raw_radius(150)->_missile_speed(1600)->_can_be_deleted(FALSE)->_missile_delayed(TRUE)->_skillshot_type(kMissileLine)
		);

		spells.push_back
		(
			InitializeSpell("Ryze")->_spell_name("RyzeQ")->_missile_names({ "RyzeQ" })->_danger_level(kMedium)->_raw_range(1000)->_raw_radius(55)->_missile_speed(1700)->_collision_flags(kHeroes | kMinions | kYasuoWall)->_is_fixed_range(TRUE)->_skillshot_type(kMissileLine)
		);

		spells.push_back
		(
			InitializeSpell("Sejuani")->_spell_name("SejuaniQ")->_spell_slot(SpellSlot::Q)->_danger_level(kHigh)->_raw_range(650)->_extra_range(150)->_delay(0)->_raw_radius(75)->_missile_speed(1000)->_collision_flags(kHeroes)->_is_dangerous(TRUE)->_skillshot_type(kMissileLine)
		);

		spells.push_back
		(
			InitializeSpell("Sejuani")->_spell_name("SejuaniR")->_missile_names({ "SejuaniRMissile" })->_danger_level(kExtreme)->_spell_slot(SpellSlot::R)->_raw_range(1200)->_raw_radius(120)->_missile_speed(1600)->_collision_flags(kHeroes | kYasuoWall)->_is_fixed_range(TRUE)->_is_dangerous(TRUE)->_skillshot_type(kMissileLine)
		);

		spells.push_back
		(
			InitializeSpell("Shen")->_spell_name("ShenE")->_danger_level(kHigh)->_spell_slot(SpellSlot::E)->_raw_range(600)->_delay(0)->_raw_radius(50)->_missile_speed(1300)->_is_dangerous(TRUE)->_skillshot_type(kMissileLine)
		);

		spells.push_back
		(
			InitializeSpell("Shyvana")->_spell_name("ShyvanaFireball")->_missile_names({ "ShyvanaFireballMissile" })->_danger_level(kMedium)->_spell_slot(SpellSlot::E)->_raw_range(950)->_raw_radius(60)->_missile_speed(1575)->_collision_flags(kYasuoWall)->_is_fixed_range(TRUE)->_skillshot_type(kMissileLine)
		);

		spells.push_back
		(
			InitializeSpell("Shyvana")->_spell_name("ShyvanaFireballDragon2")->_missile_names({ "ShyvanaFireballDragonMissile", "ShyvanaFireballDragonMissileBig", "ShyvanaFireballDragonMissileMax" })->_danger_level(kHigh)->_spell_slot(SpellSlot::E)->_raw_range(780)->_delay(350)->_raw_radius(60)->_missile_speed(1575)->_collision_flags(kHeroes | kYasuoWall)->_infront_start(TRUE)->_radius_ex(350)->_skillshot_type(kMissileLine)
		);

		spells.push_back
		(
			InitializeSpell("Shyvana")->_spell_name("ShyvanaTransformCast")->_danger_level(kHigh)->_spell_slot(SpellSlot::R)->_raw_range(950)->_delay(300)->_raw_radius(150)->_missile_speed(1100)->_is_dangerous(TRUE)->_skillshot_type(kMissileLine)
		);

		spells.push_back
		(
			InitializeSpell("Sion")->_spell_name("SionE")->_missile_names({ "SionEMissile" })->_danger_level(kHigh)->_spell_slot(SpellSlot::E)->_raw_range(800)->_raw_radius(80)->_missile_speed(1800)->_collision_flags(kHeroes | kYasuoWall)->_is_fixed_range(TRUE)->_is_dangerous(TRUE)->_skillshot_type(kMissileLine)
		);

		spells.push_back
		(
			InitializeSpell("Sion")->_spell_name("SionEMinion")->_danger_level(kHigh)->_spell_slot(SpellSlot::E)->_raw_range(1400)->_delay(0)->_raw_radius(60)->_missile_speed(2100)->_is_dangerous(TRUE)->_skillshot_type(kMissileLine)
		);

		spells.push_back
		(
			InitializeSpell("Sivir")->_spell_name("SivirQ")->_missile_names({ "SivirQMissile" })->_danger_level(kMedium)->_raw_range(1250)->_raw_radius(90)->_missile_speed(1350)->_collision_flags(kYasuoWall)->_is_fixed_range(TRUE)->_skillshot_type(kMissileLine)
		);

		spells.push_back
		(
			InitializeSpell("Sivir")->_spell_name("SivirQReturn")->_missile_names({ "SivirQMissileReturn" })->_danger_level(kMedium)->_raw_range(20000)->_delay(0)->_raw_radius(100)->_missile_speed(1350)->_collision_flags(kYasuoWall)->_missile_follows_unit(TRUE)->_skillshot_type(kMissileLine)
		);

		spells.push_back
		(
			InitializeSpell("Skarner")->_spell_name("SkarnerFracture")->_missile_names({ "SkarnerFractureMissile" })->_danger_level(kMedium)->_spell_slot(SpellSlot::E)->_raw_range(1000)->_raw_radius(70)->_missile_speed(1500)->_collision_flags(kYasuoWall)->_is_fixed_range(TRUE)->_skillshot_type(kMissileLine)
		);

		spells.push_back
		(
			InitializeSpell("Sona")->_spell_name("SonaR")->_missile_names({ "SonaR" })->_spell_slot(SpellSlot::R)->_raw_range(1000)->_raw_radius(140)->_missile_speed(2400)->_collision_flags(kYasuoWall)->_is_fixed_range(TRUE)->_is_dangerous(TRUE)->_skillshot_type(kMissileLine)->_danger_level(kExtreme)
		);

		spells.push_back
		(
			InitializeSpell("Soraka")->_spell_name("SorakaQ")->_missile_names({ "SorakaQMissile" })->_danger_level(kMedium)->_raw_range(800)->_raw_radius(230)->_missile_speed(1100)->_skillshot_type(kCircle)->_collision_flags(kYasuoWall)
		);

		spells.push_back
		(
			InitializeSpell("Soraka")->_spell_name("SorakaE")->_danger_level(kHigh)->_spell_slot(SpellSlot::E)->_raw_range(920)->_delay(1770)->_raw_radius(250)->_skillshot_type(kCircle)
		);

		spells.push_back
		(
			InitializeSpell("Swain")->_spell_name("SwainW")->_danger_level(kHigh)->_spell_slot(SpellSlot::W)->_raw_range(3500)->_delay(1500)->_raw_radius(240)->_missile_speed(INT_MAX)->_skillshot_type(kCircle)->_is_dangerous(TRUE)
		);

		spells.push_back
		(
			InitializeSpell("Swain")->_spell_name("SwainE")->_danger_level(kHigh)->_missile_names({ "SwainE", "SwainEReturnMissile" })->_spell_slot(SpellSlot::E)->_raw_range(850)->_delay(250)->_raw_radius(85)->_missile_speed(1500)->_collision_flags(kYasuoWall)->_is_fixed_range(TRUE)->_skillshot_type(kMissileLine)->_is_dangerous(TRUE)
		);

		spells.push_back
		(
			InitializeSpell("Syndra")->_spell_name("SyndraQ")->_missile_names({ "SyndraQSpell" })->_danger_level(kMedium)->_raw_range(825)->_delay(650)->_raw_radius(180)->_skillshot_type(kCircle)->_missile_delayed(TRUE)
		);

		spells.push_back
		(
			InitializeSpell("Syndra")->_spell_name("SyndraWCast")->_danger_level(kMedium)->_spell_slot(SpellSlot::W)->_raw_range(950)->_raw_radius(210)->_missile_speed(1500)->_skillshot_type(kCircle)
		);

		spells.push_back
		(
			InitializeSpell("Syndra")->_spell_name("SyndraE")->_danger_level(kMedium)->_is_dangerous(TRUE)->_spell_slot(SpellSlot::E)->_raw_range(650)->_raw_radius(40)->_missile_speed(2500)->_collision_flags(kYasuoWall)->_skillshot_type(kMissileCone)->_dont_cross(TRUE)
		);

		spells.push_back
		(
			InitializeSpell("Syndra")->_spell_name("SyndraE5")->_danger_level(kMedium)->_is_dangerous(TRUE)->_spell_slot(SpellSlot::E)->_raw_range(650)->_raw_radius(60)->_missile_speed(2500)->_collision_flags(kYasuoWall)->_skillshot_type(kMissileCone)->_dont_cross(TRUE)
		);

		spells.push_back
		(
			InitializeSpell("Syndra")->_spell_name("SyndraEQ")->_danger_level(kMedium)->_is_dangerous(TRUE)->_spell_slot(SpellSlot::E)->_raw_range(1300)->_delay(0)->_raw_radius(55)->_missile_speed(2000)->_collision_flags(kYasuoWall)->_skillshot_type(kMissileLine)
		);

		spells.push_back
		(
			InitializeSpell("TahmKench")->_spell_name("TahmKenchQ")->_missile_names({ "TahmKenchQMissile" })->_danger_level(kHigh)->_raw_range(850)->_raw_radius(70)->_missile_speed(2800)->_collision_flags(kHeroes | kMinions | kYasuoWall)->_is_fixed_range(TRUE)->_is_dangerous(TRUE)->_skillshot_type(kMissileLine)
		);

		spells.push_back
		(
			InitializeSpell("Taliyah")->_spell_name("TaliyahQ")->_missile_names({ "TaliyahQMis" })->_danger_level(kMedium)->_raw_range(1000)->_raw_radius(100)->_missile_speed(3600)->_collision_flags(kHeroes | kMinions | kYasuoWall)->_is_fixed_range(TRUE)->_missile_min_speed(1500)->_missile_max_speed(3600)->_is_disabled_by_default(TRUE)->_skillshot_type(kMissileLine)
		);

		spells.push_back
		(
			InitializeSpell("Taliyah")->_spell_name("TaliyahWVC")->_danger_level(kMedium)->_spell_slot(SpellSlot::W)->_raw_range(900)->_delay(800)->_raw_radius(200)->_skillshot_type(kCircle)->_is_dangerous(TRUE)
		);

		spells.push_back
		(
			InitializeSpell("Taric")->_spell_name("TaricE")->_danger_level(kHigh)->_spell_slot(SpellSlot::E)->_raw_range(650)->_delay(1000)->_raw_radius(100)->_skillshot_type(kLine)->_is_fixed_range(TRUE)->_is_dangerous(TRUE)->_missile_connected_to_unit(TRUE)
		);

		spells.push_back
		(
			InitializeSpell("Thresh")->_spell_name("ThreshQ")->_missile_names({ "ThreshQMissile" })->_danger_level(kHigh)->_raw_range(1100)->_delay(500)->_raw_radius(70)->_missile_speed(1900)->_collision_flags(kHeroes | kMinions | kYasuoWall)->_is_fixed_range(TRUE)->_is_dangerous(TRUE)->_skillshot_type(kMissileLine)
		);

		spells.push_back
		(
			InitializeSpell("Thresh")->_spell_name("ThreshE")->_missile_names({ "ThreshEMissile1" })->_danger_level(kHigh)->_spell_slot(SpellSlot::E)->_raw_range(1075)->_delay(0)->_raw_radius(110)->_behind_start(530)->_missile_speed(2000)->_collision_flags(kYasuoWall)->_is_fixed_range(TRUE)->_is_dangerous(TRUE)->_skillshot_type(kMissileLine)
		);

		spells.push_back
		(
			InitializeSpell("Tristana")->_spell_name("TristanaW")->_danger_level(kMedium)->_spell_slot(SpellSlot::W)->_raw_range(900)->_delay(300)->_raw_radius(270)->_missile_speed(1100)->_skillshot_type(kCircle)
		);

		spells.push_back
		(
			InitializeSpell("TwistedFate")->_spell_name("WildCards")->_missile_names({ "SealFateMissile" })->_danger_level(kMedium)->_raw_range(1450)->_raw_radius(40)->_missile_speed(1000)->_collision_flags(kYasuoWall)->_is_fixed_range(TRUE)->_skillshot_type(kMissileLine)->_multiple_number(3)->_multiple_angle(28.0f * M_PI / 180.0f)
		);

		spells.push_back
		(
			InitializeSpell("Twitch")->_spell_name("TwitchVenomCask")->_missile_names({ "TwitchVenomCaskMissile" })->_danger_level(kMedium)->_spell_slot(SpellSlot::W)->_raw_range(950)->_raw_radius(285)->_missile_speed(1400)->_skillshot_type(kCircle)->_collision_flags(kYasuoWall)->_extra_duration(2850)->_is_disabled_by_default(TRUE)
		);

		spells.push_back
		(
			InitializeSpell("Urgot")->_spell_name("UrgotQ")->_missile_names({ "UrgotQMissile" })->_danger_level(kMedium)->_raw_range(800)->_delay(525)->_raw_radius(150)->_missile_speed(5000)->_collision_flags(kYasuoWall)->_can_be_deleted(FALSE)->_missile_delayed(TRUE)->_is_fixed_range(FALSE)->_skillshot_type(kCircle)
		);

		spells.push_back
		(
			InitializeSpell("Urgot")->_spell_name("UrgotE")->_danger_level(kHigh)->_spell_slot(SpellSlot::E)->_delay(500)->_raw_range(500)->_raw_radius(100)->_missile_speed(1500)->_skillshot_type(kMissileLine)->_is_fixed_range(TRUE)->_is_dangerous(TRUE)
		);

		spells.push_back
		(
			InitializeSpell("Urgot")->_spell_name("UrgotR")->_missile_names({ "UrgotR" })->_danger_level(kExtreme)->_spell_slot(SpellSlot::R)->_delay(400)->_raw_range(1650)->_raw_radius(80)->_missile_speed(3200)->_skillshot_type(kMissileLine)->_collision_flags(kYasuoWall)->_is_fixed_range(TRUE)->_is_dangerous(TRUE)
		);

		spells.push_back
		(
			InitializeSpell("Varus")->_spell_name("VarusQCast")->_missile_names({ "VarusQMissile" })->_danger_level(kMedium)->_raw_range(20000)->_delay(0)->_raw_radius(70)->_missile_speed(1900)->_collision_flags(kYasuoWall)->_skillshot_type(kMissileLine)
		);

		spells.push_back
		(
			InitializeSpell("Varus")->_spell_name("VarusE")->_missile_names({ "VarusEMissile" })->_danger_level(kMedium)->_spell_slot(SpellSlot::E)->_raw_range(925)->_raw_radius(260)->_missile_speed(1500)->_skillshot_type(kCircle)->_collision_flags(kYasuoWall)->_extra_delay({ 550 })
		);

		spells.push_back
		(
			InitializeSpell("Varus")->_spell_name("VarusR")->_missile_names({ "VarusRMissile" })->_danger_level(kHigh)->_spell_slot(SpellSlot::R)->_raw_range(1250)->_raw_radius(120)->_missile_speed(1950)->_collision_flags(kHeroes | kYasuoWall)->_is_fixed_range(TRUE)->_is_dangerous(TRUE)->_skillshot_type(kMissileLine)
		);

		spells.push_back
		(
			InitializeSpell("Veigar")->_spell_name("VeigarBalefulStrike")->_missile_names({ "VeigarBalefulStrikeMis" })->_danger_level(kMedium)->_raw_range(950)->_raw_radius(70)->_missile_speed(2200)->_collision_flags(kYasuoWall)->_is_fixed_range(TRUE)->_skillshot_type(kMissileLine)
		);

		spells.push_back
		(
			InitializeSpell("Veigar")->_spell_name("VeigarDarkMatter")->_danger_level(kMedium)->_spell_slot(SpellSlot::W)->_raw_range(900)->_delay(1250)->_raw_radius(225)->_skillshot_type(kCircle)
		);

		spells.push_back
		(
			InitializeSpell("Veigar")->_spell_name("VeigarEventHorizon")->_danger_level(kHigh)->_spell_slot(SpellSlot::E)->_raw_range(700)->_delay(800)->_raw_radius(370)->_skillshot_type(kRing)->_radius_ex(70)->_extra_duration(3000)->_is_dangerous(TRUE)->_dont_cross(TRUE)
		);

		spells.push_back
		(
			InitializeSpell("Velkoz")->_spell_name("VelkozQ")->_missile_names({ "VelkozQMissile" })->_danger_level(kMedium)->_raw_range(1100)->_raw_radius(50)->_missile_speed(1300)->_collision_flags(kHeroes | kMinions | kYasuoWall)->_is_fixed_range(TRUE)->_skillshot_type(kMissileLine)
		);

		spells.push_back
		(
			InitializeSpell("Velkoz")->_spell_name("VelkozQSplit")->_missile_names({ "VelkozQMissileSplit" })->_danger_level(kMedium)->_raw_range(1100)->_delay(0)->_raw_radius(45)->_missile_speed(2100)->_collision_flags(kHeroes | kMinions | kYasuoWall)->_is_fixed_range(TRUE)->_skillshot_type(kMissileLine)
		);

		spells.push_back
		(
			InitializeSpell("Velkoz")->_spell_name("VelkozW")->_missile_names({ "VelkozWMissile" })->_danger_level(kMedium)->_spell_slot(SpellSlot::W)->_raw_range(1200)->_raw_radius(88)->_missile_speed(1700)->_collision_flags(kYasuoWall)->_behind_start(100)->_is_fixed_range(TRUE)->_skillshot_type(kMissileLine)
		);

		spells.push_back
		(
			InitializeSpell("Velkoz")->_spell_name("VelkozE")->_missile_names({ "VelkozEMissile" })->_danger_level(kMedium)->_spell_slot(SpellSlot::E)->_raw_range(800)->_raw_radius(225)->_missile_speed(1500)->_skillshot_type(kCircle)->_extra_delay({ 550 })
		);

		spells.push_back
		(
			InitializeSpell("Vi")->_missile_names({ "ViQMissile" })->_danger_level(kHigh)->_raw_range(800)->_extra_range(100)->_delay(0)->_raw_radius(90)->_missile_speed(1600)->_collision_flags(kHeroes)->_is_dangerous(TRUE)->_skillshot_type(kMissileLine)
		);

		spells.push_back
		(
			InitializeSpell("Viktor")->_spell_name("ViktorGravitonField")->_danger_level(kHigh)->_spell_slot(SpellSlot::W)->_raw_range(700)->_delay(1500)->_raw_radius(300)->_skillshot_type(kCircle)
		);

		spells.push_back
		(
			InitializeSpell("Viktor")->_spell_name("ViktorE")->_missile_names({ "ViktorDeathRayMissile", "ViktorEAugMissile" })->_danger_level(kMedium)->_spell_slot(SpellSlot::E)->_raw_range(710)->_delay(0)->_raw_radius(80)->_missile_speed(1050)->_collision_flags(kYasuoWall)->_is_fixed_range(TRUE)->_skillshot_type(kMissileLine)
		);

		spells.push_back
		(
			InitializeSpell("Viktor")->_spell_name("ViktorEExplosion")->_danger_level(kMedium)->_spell_slot(SpellSlot::E)->_raw_range(710)->_delay(1000)->_raw_radius(80)->_missile_speed(1500)->_is_fixed_range(TRUE)->_skillshot_type(kMissileLine)
		);

		spells.push_back
		(
			InitializeSpell("Warwick")->_spell_name("WarwickR")->_danger_level(kExtreme)->_is_dangerous(TRUE)->_spell_slot(SpellSlot::R)->_raw_range(2000)->_extra_range(100)->_delay(0)->_raw_radius(150)->_missile_speed(1500)->_is_fixed_range(TRUE)->_skillshot_type(kMissileLine)
		);

		spells.push_back
		(
			InitializeSpell("Xayah")->_spell_name("XayahQ")->_missile_names({ "XayahQMissile1", "XayahQMissile2" })->_spell_slot(SpellSlot::Q)->_danger_level(kMedium)->_raw_range(1100)->_delay(600)->_raw_radius(50)->_missile_speed(2400)->_missile_delayed(TRUE)->_skillshot_type(kMissileLine)->_is_fixed_range(TRUE)
		);

		spells.push_back
		(
			InitializeSpell("Xerath")->_spell_name("XerathArcanopulse2")->_danger_level(kMedium)->_raw_range(20000)->_delay(530)->_raw_radius(100)->_skillshot_type(kLine)
		);

		spells.push_back
		(
			InitializeSpell("Xerath")->_spell_name("XerathArcaneBarrage2")->_danger_level(kMedium)->_spell_slot(SpellSlot::W)->_raw_range(1100)->_delay(780)->_raw_radius(260)->_skillshot_type(kCircle)
		);

		spells.push_back
		(
			InitializeSpell("Xerath")->_spell_name("XerathMageSpear")->_missile_names({ "XerathMageSpearMissile" })->_danger_level(kMedium)->_spell_slot(SpellSlot::E)->_raw_range(1100)->_raw_radius(60)->_missile_speed(1400)->_collision_flags(kHeroes | kMinions | kYasuoWall)->_is_fixed_range(TRUE)->_is_dangerous(TRUE)->_skillshot_type(kMissileLine)
		);

		spells.push_back
		(
			InitializeSpell("Xerath")->_spell_name("XerathRMissileWrapper")->_missile_names({ "XerathLocusPulse" })->_danger_level(kHigh)->_spell_slot(SpellSlot::R)->_raw_range(20000)->_delay(650)->_raw_radius(200)->_skillshot_type(kCircle)->_missile_delayed(TRUE)->_is_dangerous(TRUE)
		);

		spells.push_back
		(
			InitializeSpell("Yasuo")->_spell_name("YasuoQ")->_extra_spell_names({ "YasuoQ2" })->_danger_level(kMedium)->_raw_range(520)->_delay(400)->_raw_radius(55)->_skillshot_type(kLine)->_is_fixed_range(TRUE)->_invert(TRUE)->_is_dangerous(TRUE)
		);

		spells.push_back
		(
			InitializeSpell("Yasuo")->_spell_name("YasuoQ3")->_missile_names({ "YasuoQ3Mis" })->_danger_level(kHigh)->_raw_range(1100)->_delay(300)->_raw_radius(90)->_missile_speed(1200)->_collision_flags(kYasuoWall)->_is_fixed_range(TRUE)->_invert(TRUE)->_is_dangerous(TRUE)->_skillshot_type(kMissileLine)
		);

		spells.push_back
		(
			InitializeSpell("Yorick")->_spell_name("YorickW")->_danger_level(kMedium)->_spell_slot(SpellSlot::W)->_raw_range(600)->_delay(750)->_raw_radius(250)->_skillshot_type(kCircle)->_is_disabled_by_default(TRUE)
		);

		spells.push_back
		(
			InitializeSpell("Yorick")->_spell_name("YorickE")->_danger_level(kMedium)->_spell_slot(SpellSlot::E)->_raw_range(550)->_delay(50)->_raw_radius(120)->_skillshot_type(kLine)->_is_fixed_range(TRUE)
		);

		spells.push_back
		(
			InitializeSpell("Zac")->_spell_name("ZacQ")->_danger_level(kMedium)->_raw_range(800)->_missile_speed(2800)->_delay(250)->_raw_radius(120)->_skillshot_type(kMissileLine)->_is_fixed_range(TRUE)
		);

		spells.push_back
		(
			InitializeSpell("Zac")->_spell_name("ZacE2")->_danger_level(kHigh)->_spell_slot(SpellSlot::E)->_raw_range(1800)->_delay(0)->_raw_radius(250)->_missile_speed(1350)->_skillshot_type(kCircle)->_is_dangerous(TRUE)
		);

		spells.push_back
		(
			InitializeSpell("Zed")->_spell_name("ZedQ")->_missile_names({ "ZedQMissile" })->_danger_level(kMedium)->_raw_range(925)->_raw_radius(50)->_missile_speed(1700)->_collision_flags(kYasuoWall)->_is_fixed_range(TRUE)->_skillshot_type(kMissileLine)
		);

		spells.push_back
		(
			InitializeSpell("Ziggs")->_spell_name("ZiggsQ")->_missile_names({ "ZiggsQSpell" })->_danger_level(kMedium)->_raw_range(850)->_raw_radius(125)->_missile_speed(1700)->_collision_flags(kYasuoWall)->_skillshot_type(kCircle)
		);

		spells.push_back
		(
			InitializeSpell("Ziggs")->_spell_name("ZiggsQBounce")->_missile_names({ "ZiggsQSpell2", "ZiggsQSpell3" })->_danger_level(kMedium)->_raw_range(1100)->_delay(400)->_raw_radius(125)->_missile_speed(1600)->_collision_flags(kYasuoWall)->_skillshot_type(kCircle)->_missile_delayed(TRUE)
		);

		spells.push_back
		(
			InitializeSpell("Ziggs")->_spell_name("ZiggsW")->_missile_names({ "ZiggsW" })->_danger_level(kMedium)->_spell_slot(SpellSlot::W)->_raw_range(1000)->_raw_radius(275)->_missile_speed(1750)->_collision_flags(kYasuoWall)->_skillshot_type(kCircle)
		);

		spells.push_back
		(
			InitializeSpell("Ziggs")->_spell_name("ZiggsE")->_missile_names({ "ZiggsE2" })->_danger_level(kMedium)->_spell_slot(SpellSlot::E)->_raw_range(900)->_raw_radius(235)->_missile_speed(1550)->_collision_flags(kYasuoWall)->_skillshot_type(kCircle)
		);

		spells.push_back
		(
			InitializeSpell("Ziggs")->_spell_name("ZiggsR")->_missile_names({ "ZiggsRBoom", "ZiggsRBoomMedium", "ZiggsRBoomLong" })->_danger_level(kMedium)->_spell_slot(SpellSlot::R)->_raw_range(5000)->_delay(400)->_raw_radius(500)->_missile_speed(1500)->_skillshot_type(kCircle)->_extra_delay({ 1100 })->_danger_level(kExtreme)
		);

		spells.push_back
		(
			InitializeSpell("Zilean")->_spell_name("ZileanQ")->_missile_names({ "ZileanQMissile" })->_toggle_name("Zilean_.+_Q_Indicator_(Green|Red)")->_danger_level(kMedium)->_raw_range(900)->_raw_radius(150)->_missile_speed(2000)->_skillshot_type(kCircle)->_collision_flags(kYasuoWall)->_can_be_deleted(FALSE)->_dont_cross(TRUE)
		);

		spells.push_back
		(
			InitializeSpell("Zoe")->_spell_name("ZoeQ")->_missile_names({ "ZoeQMissile" })->_spell_slot(SpellSlot::Q)->_danger_level(kMedium)->_is_fixed_range(TRUE)->_raw_range(875)->_raw_radius(50)->_missile_speed(1200)->_skillshot_type(kMissileLine)->_collision_flags(kYasuoWall | kHeroes | kMinions)->_is_dangerous(FALSE)
		);

		spells.push_back
		(
			InitializeSpell("Zoe")->_spell_name("ZoeQ2")->_missile_names({ "ZoeQMis2" })->_spell_slot(SpellSlot::Q)->_danger_level(kMedium)->_raw_range(2500)->_raw_radius(70)->_missile_speed(2500)->_skillshot_type(kMissileLine)->_collision_flags(kYasuoWall | kHeroes | kMinions)->_is_dangerous(FALSE)
		);

		spells.push_back
		(
			InitializeSpell("Zoe")->_spell_name("ZoeE")->_missile_names({ "ZoeEMis" })->_spell_slot(SpellSlot::E)->_danger_level(kMedium)->_is_fixed_range(TRUE)->_raw_range(800)->_raw_radius(50)->_missile_speed(1700)->_skillshot_type(kMissileLine)->_collision_flags(kYasuoWall | kHeroes | kMinions)->_is_dangerous(FALSE)
		);

		spells.push_back
		(
			InitializeSpell("Zyra")->_spell_name("ZyraQ")->_danger_level(kMedium)->_raw_range(800)->_delay(850)->_raw_radius(140)->_skillshot_type(kLine)->_perpendicular(TRUE)->_radius_ex(450)
		);

		spells.push_back
		(
			InitializeSpell("Zyra")->_spell_name("ZyraE")->_missile_names({ "ZyraE" })->_danger_level(kHigh)->_spell_slot(SpellSlot::E)->_raw_range(1150)->_raw_radius(70)->_missile_speed(1150)->_collision_flags(kYasuoWall)->_is_fixed_range(TRUE)->_is_dangerous(TRUE)->_skillshot_type(kMissileLine)
		);

		spells.push_back
		(
			InitializeSpell("Zyra")->_spell_name("ZyraR")->_danger_level(kHigh)->_spell_slot(SpellSlot::R)->_raw_range(700)->_delay(2200)->_raw_radius(520)->_skillshot_type(kCircle)
		);

		std::vector<std::string> available_names;
		std::vector<ChampionId> champion_ids;
				
		for (auto const& unit : g_ObjectManager->GetChampions())
		{
			if (Config::IsDebugEnabled || unit->IsEnemy())
			{
				if (std::find(available_names.begin(), available_names.end(), unit->ChampionName()) == available_names.end())
				{
					champion_ids.push_back(unit->ChampionId());
					available_names.push_back(unit->ChampionName());
				}
			}
		}

		auto const vec_special_spells = SpecialSpells::GetSpecialSpells(champion_ids);
		special_spells.insert(special_spells.end(), vec_special_spells.begin(), vec_special_spells.end());

		spells.erase(std::remove_if(spells.begin(), spells.end(), [&available_names](SkillshotData* x)
		{
			if (std::find(available_names.begin(), available_names.end(), x->champion_name) == available_names.end())
			{
				delete x;
				return true;
			}

			return false;
		}), spells.end());

		if (Config::IsDebugEnabled)
		{
			spells.push_back
			(
				InitializeSpell(g_LocalPlayer->ChampionName())->_spell_name("DebugLinearSpell")->_danger_level(kHigh)->_spell_slot(SpellSlot::R)->_raw_range(3000)->_delay(1000)->_raw_radius(160)->_missile_speed(2000)->_collision_flags(kYasuoWall)->_is_fixed_range(TRUE)->_is_dangerous(TRUE)->_skillshot_type(kMissileLine)
			);

			spells.push_back
			(
				InitializeSpell(g_LocalPlayer->ChampionName())->_spell_name("DebugCircularSpell")->_danger_level(kMedium)->_spell_slot(SpellSlot::E)->_raw_range(1100)->_raw_radius(330)->_missile_speed(1300)->_can_be_deleted(false)->_extra_duration(5000)->_skillshot_type(kCircle)->_collision_flags(kYasuoWall)->_dont_cross(TRUE)
			);
		}
	}
}