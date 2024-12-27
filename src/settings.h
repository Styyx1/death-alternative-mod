#pragma once

class Settings
{
public:

	static void LoadSettings();
	static void LoadForms();
	static void LogBool(std::string bool_name, bool a_setting);

	inline static bool enable_npcs{ true };
	inline static bool remove_gold{ true };
	inline static bool show_gold_removal_message{ false };
	inline static bool heal_enemies_on_death{ false };
	inline static bool kill_without_gold{ false };
	inline static float injury_decrease_modifier{ 0.40f };
	inline static float min_sleep_duration{ 8.0f };
	inline static float stress_increase_value{ 10.0f };
	inline static float gold_remove_percentage{ 10.0f };
	inline static std::string stress_increase_text{ "I hope this ends well..." };
	inline static std::string stress_decrease_text{ "I feel a bit more relaxed now!" };

	inline static bool is_stress_mod_active { false };

	inline static RE::SpellItem* ethereal_spell;
	inline static RE::SpellItem* injury_spell;
	inline static RE::SpellItem* death_heal;
	inline static RE::SpellItem* ethereal_spell_npcs;
	inline static RE::TESObjectMISC* cheat_death_token;
	inline static RE::TESGlobal* survival_mode_active;
	inline static RE::TESGlobal* health_penalty_ui_global;

	inline static RE::TESGlobal* stress_enabled;
	inline static RE::TESGlobal* stress_total_value;

	inline static RE::EffectSetting* ethereal_effect;


};