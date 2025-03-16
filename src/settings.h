#pragma once

class Settings
{
public:

	static void LoadSettings();
	static void LoadForms();
	static void LogBool(std::string bool_name, bool a_setting);
	static void LogAllSettings();
	static void ChangeInjuryDescription();

	//bool
	inline static bool enable_npcs{ true };
	inline static bool remove_gold{ true };
	inline static bool show_gold_removal_message{ false };
	inline static bool heal_enemies_on_death{ false };
	inline static bool kill_without_gold{ false };
	inline static bool use_health_injury{ true }; // compatibility with blade and blunt
	inline static bool use_stamina_injury{ true };
	inline static bool use_magicka_injury{ true };
	inline static bool use_calm_spell{ false };
	//float
	inline static float min_sleep_duration{ 8.0f };
	inline static float stress_increase_value{ 10.0f };
	inline static float gold_remove_percentage{ 10.0f };
	inline static float injury_health_decrease{ 0.40f };
	inline static float injury_stam_decrease{ 0.40f };
	inline static float injury_mag_decrease{ 0.40f };
	//int
	inline static std::uint32_t number_of_injuries{3};
	inline static std::uint32_t sleep_location_difficulty{ 1 };
	//string
	inline static std::string stress_increase_text{ "I hope this ends well..." };
	inline static std::string stress_decrease_text{ "I feel a bit more relaxed now!" };

	//not in ini:
	inline static bool is_stress_mod_active { false };
	//forms
	inline static RE::SpellItem* ethereal_spell;
	inline static RE::SpellItem* injury_spell;
	inline static RE::SpellItem* death_heal;
	inline static RE::SpellItem* ethereal_spell_npcs;
	inline static RE::SpellItem* calm_spell_npcs;
	inline static RE::TESObjectMISC* cheat_death_token;

	inline static RE::TESGlobal* health_penalty_ui_global;
	inline static RE::TESGlobal* stress_enabled;
	inline static RE::TESGlobal* stress_total_value;
	inline static RE::EffectSetting* ethereal_effect;
	inline static RE::EffectSetting* injury_display_effect;

	template<typename T>
	static void LogNumber(std::string setting_name, T value) {
		// Process value of type T
		logs::info("setting {} has a value of {}", setting_name, value);
	}
};