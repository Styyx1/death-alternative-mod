#include "settings.h"

void Settings::LoadSettings()
{
	logs::info("loading settings...");
	CSimpleIniA ini;
	ini.SetUnicode();
	ini.LoadFile(R"(.\Data\SKSE\Plugins\shades-of-mortality.ini)");
	//General
	enable_npcs = ini.GetBoolValue("General", "bEnableNPCS", true);
	injury_decrease_tier_1 = (float)ini.GetDoubleValue("General", "fTierOneInjury", 10.0f);
	injury_decrease_tier_2 = (float)ini.GetDoubleValue("General", "fTierTwoInjury", 20.0f);
	injury_decrease_tier_3 = (float)ini.GetDoubleValue("General", "fTierThreeInjury", 40.0f);
	min_sleep_duration = (float)ini.GetDoubleValue("General", "fSleepDuration", 8.0f);
	stress_increase_value = (float)ini.GetDoubleValue("General", "fStressIncreaseAmount", 10.0f);
	//Texts
	stress_increase_text = ini.GetValue("Texts", "sStressIncreaseText", "I hope this ends well...");
	stress_decrease_text = ini.GetValue("Texts", "sInjuryHealStressText", "I feel a bit more relaxed now!");

	/*
		inline static std::string stress_increase_text{ "I hope this ends well..." };
	inline static std::string stress_decrease_text{ "I feel a bit more relaxed now!" };
	*/

	logs::info("...loaded settings");

}

void Settings::LoadForms()
{
	const int ethereal_spell_form = 0x801;
	const int heal_spell_form = 0x803;
	const int inj_tier_1_form = 0x805;
	const int inj_tier_2_form = 0x806;
	const int inj_tier_3_form = 0x807;
	const int inj_dummy_effect_form = 0x804;
	const int cheat_death_token_form = 0x809;
	const char* mod_name = "shade-of-mortality.esp";

	logs::info("loading forms...");
	RE::TESDataHandler* dh = RE::TESDataHandler::GetSingleton();
	//mod forms:
	ethereal_spell = dh->LookupForm(ethereal_spell_form, mod_name)->As<RE::SpellItem>();
	death_heal = dh->LookupForm(heal_spell_form, mod_name)->As<RE::SpellItem>();
	injury_spell_10 = dh->LookupForm(inj_tier_1_form, mod_name)->As<RE::SpellItem>();
	injury_spell_20 = dh->LookupForm(inj_tier_2_form, mod_name)->As<RE::SpellItem>();
	injury_spell_40 = dh->LookupForm(inj_tier_3_form, mod_name)->As<RE::SpellItem>();
	cheat_death_token = dh->LookupForm(cheat_death_token_form, mod_name)->As<RE::TESObjectMISC>();

	const char* stress_mod = "Stress and Fear.esp";
	const int stress_total_form = 0x801;
	const int stress_enabled_form = 0x8a5;

	is_stress_mod_active = false;

	if (auto file = dh->LookupModByName(stress_mod); file && file->compileIndex != 0xFF) {
		logs::info("Stress and Fear is active");
		stress_enabled = dh->LookupForm(stress_total_form, stress_mod)->As<RE::TESGlobal>();
		stress_total_value = dh->LookupForm(stress_enabled_form, stress_mod)->As<RE::TESGlobal>();
		is_stress_mod_active = true;
	}


	// base game forms:
	health_penalty_ui_global = dh->LookupForm(0x2EDE, "Update.esm")->As<RE::TESGlobal>();
	survival_mode_active = dh->LookupForm(0x826, "ccqdrsse001-survivalmode.esl")->As<RE::TESGlobal>();

	logs::info("...loaded forms");
}
