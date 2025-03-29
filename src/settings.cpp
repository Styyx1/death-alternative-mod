#include "settings.h"

void Settings::LoadSettings()
{
	logs::info("loading settings...");
	CSimpleIniA ini;
	ini.SetUnicode();
	ini.LoadFile(R"(.\Data\SKSE\Plugins\shades-of-mortality.ini)");
	// bools
	enable_npcs = ini.GetBoolValue("General", "bEnableNPCS", true);
	remove_gold = ini.GetBoolValue("General", "bRemoveGold", true);
	show_gold_removal_message = ini.GetBoolValue("General", "bShowGoldRemoveMessage", false);
	heal_enemies_on_death = ini.GetBoolValue("General", "bHealEnemies", false);
	kill_without_gold = ini.GetBoolValue("General", "bKillWithoutGold", false);
	use_health_injury = ini.GetBoolValue("General", "bUseHealthInjury", true);
	use_stamina_injury = ini.GetBoolValue("General", "bUseStaminaRateInjury", true);
	use_magicka_injury = ini.GetBoolValue("General", "bUseMagickaRateInjury", true);
	use_calm_spell = ini.GetBoolValue("General", "bUseCalmSpell", false);
	// floats
	injury_health_decrease = (float)ini.GetDoubleValue("Values", "fInjuryHealthDecreaseModifier", 0.4f);
	injury_stam_decrease = (float)ini.GetDoubleValue("Values", "fInjuryStaminaDecreaseModifier", 0.4f);
	injury_mag_decrease = (float)ini.GetDoubleValue("Values", "fInjuryMagickaDecreaseModifier", 0.4f);
	min_sleep_duration = (float)ini.GetDoubleValue("Values", "fSleepDuration", 8.0f);
	stress_increase_value = (float)ini.GetDoubleValue("Values", "fStressIncreaseAmount", 10.0f);
	gold_remove_percentage = (float)ini.GetDoubleValue("Values", "fGoldRemovePercentage", 10.0f);
	// ints
	sleep_location_difficulty = (std::uint32_t)ini.GetDoubleValue("Values", "iSleepLocationDifficulty", 1);
	number_of_injuries = (std::uint32_t)ini.GetDoubleValue("Values", "iNumberOfInjuriesTillDeath", 3);
	// Texts
	stress_increase_text = ini.GetValue("Texts", "sStressIncreaseText", "I hope this ends well...");
	stress_decrease_text = ini.GetValue("Texts", "sInjuryHealStressText", "I feel a bit more relaxed now!");

	// calculations after getting the settings
	std::uint32_t inj_cap = 100;
	number_of_injuries = std::min(number_of_injuries, inj_cap);

	// LogAllSettings();

	logs::info("...loaded settings");
}

void Settings::LoadForms()
{
	// for simplicity and ease of later changes/additions
	const int ethereal_spell_form = 0x801;
	const int heal_spell_form = 0x803;
	const int inj_tier_3_form = 0x807;
	const int cheat_death_token_form = 0x809;
	const int ethereal_npc_form = 0x80c;
	const int calm_spell_formID = 0x810;
	const int inj_display_formID = 0x804;
	const int ethereal_effect_form = 0x800;

	// apostasy related:
	const int dummy_perk_lady_ID = 0x812;
	const int dummy_temp_injury_ID = 0x814;
	const int gold_tax_global_ID = 0x815;

	const char *mod_name = "shade-of-mortality.esp";
	// Loading the forms needed for the mod from its esp
	logs::info("loading forms...");
	RE::TESDataHandler *dh = RE::TESDataHandler::GetSingleton();

	ethereal_spell = dh->LookupForm(ethereal_spell_form, mod_name)->As<RE::SpellItem>();
	death_heal = dh->LookupForm(heal_spell_form, mod_name)->As<RE::SpellItem>();
	injury_spell = dh->LookupForm(inj_tier_3_form, mod_name)->As<RE::SpellItem>();
	ethereal_spell_npcs = dh->LookupForm(ethereal_npc_form, mod_name)->As<RE::SpellItem>();
	calm_spell_npcs = dh->LookupForm(calm_spell_formID, mod_name)->As<RE::SpellItem>();
	injury_display_effect = dh->LookupForm<RE::EffectSetting>(inj_display_formID, mod_name);
	cheat_death_token = dh->LookupForm(cheat_death_token_form, mod_name)->As<RE::TESObjectMISC>();
	ethereal_effect = dh->LookupForm<RE::EffectSetting>(ethereal_effect_form, mod_name);

	// apostasy stuff:
	gold_tax_global = dh->LookupForm<RE::TESGlobal>(gold_tax_global_ID, mod_name);
	temp_injury_spell = dh->LookupForm<RE::SpellItem>(dummy_temp_injury_ID, mod_name);
	lady_stone_perk = dh->LookupForm<RE::BGSPerk>(dummy_perk_lady_ID, mod_name);

	// Stress and fear integration:
	const char *stress_mod = "Stress and Fear.esp";
	const int stress_total_form = 0x801;
	const int stress_enabled_form = 0x8a5;
	is_stress_mod_active = false;
	if (auto file = dh->LookupModByName(stress_mod); file && file->compileIndex != 0xFF)
	{
		logs::info("Stress and Fear is active");
		stress_enabled = dh->LookupForm(stress_enabled_form, stress_mod)->As<RE::TESGlobal>();
		stress_total_value = dh->LookupForm(stress_total_form, stress_mod)->As<RE::TESGlobal>();
		is_stress_mod_active = true;
	}
	// Base Game global for injury UI changes
	health_penalty_ui_global = dh->LookupForm(0x2EDE, "Update.esm")->As<RE::TESGlobal>();

	ChangeInjuryDescription();

	logs::info("...loaded forms");
}

void Settings::LogBool(std::string bool_name, bool a_setting)
{
	logs::info("{} is {}", bool_name, a_setting ? "true" : "false");
	return;
}

void Settings::LogAllSettings()
{
	LogBool("enable_npcs", enable_npcs);
	LogBool("remove_gold", remove_gold);
	LogBool("show_gold_removal_message", show_gold_removal_message);
	LogBool("heal_enemies_on_death", heal_enemies_on_death);
	LogBool("kill_without_gold", kill_without_gold);
	LogBool("use_health_injury", use_health_injury);
	LogBool("use_stamina_injury", use_stamina_injury);
	LogBool("use_magicka_injury", use_magicka_injury);
	LogBool("use_calm_spell", use_calm_spell);
	LogNumber("min_sleep_duration", min_sleep_duration);
	LogNumber("stress_increase_value", stress_increase_value);
	LogNumber("gold_remove_percentage", gold_remove_percentage);
	LogNumber("injury_health_decrease", injury_health_decrease);
	LogNumber("injury_stam_decrease", injury_stam_decrease);
	LogNumber("injury_mag_decrease", injury_mag_decrease);
	LogNumber("number_of_injuries", number_of_injuries);
	LogNumber("sleep_location_difficulty", sleep_location_difficulty);
}
// change the magic effect description for the injury effect so it accurately shows the modifier.
void Settings::ChangeInjuryDescription()
{
	std::string desc_text = "";
	if (injury_health_decrease == injury_mag_decrease && injury_mag_decrease == injury_stam_decrease)
	{
		desc_text = std::format("Reduces Health Regeneration, Stamina Regeneration and Magicka Regeneration by <{}%>", injury_health_decrease);
		Settings::injury_display_effect->magicItemDescription = desc_text.c_str();
		return;
	}
	else
	{
		desc_text = std::format("Reduces Health Regeneration by <{}%>, Stamina Regeneration by <{}%> and Magicka Regeneration by <{}%>", injury_health_decrease, injury_stam_decrease, injury_mag_decrease);
		Settings::injury_display_effect->magicItemDescription = desc_text.c_str();
		return;
	}
}
