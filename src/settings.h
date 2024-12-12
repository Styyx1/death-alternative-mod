#pragma once

class Settings
{
public:

	static void LoadSettings();
	static void LoadForms();

	inline static bool enable_npcs{ true };
	inline static float injury_decrease_tier_1{ 0.10f };
	inline static float injury_decrease_tier_2{ 0.20f };
	inline static float injury_decrease_tier_3{ 0.40f };
	inline static float min_sleep_duration{ 8.0f };
	inline static float stress_increase_value{ 10.0f };
	inline static std::string stress_increase_text{ "I hope this ends well..." };
	inline static std::string stress_decrease_text{ "I feel a bit more relaxed now!" };

	inline static bool is_stress_mod_active { false };

	inline static RE::SpellItem* ethereal_spell; //done
	inline static RE::SpellItem* injury_spell_10; //done
	inline static RE::SpellItem* injury_spell_20; //done
	inline static RE::SpellItem* injury_spell_40; //done
	inline static RE::SpellItem* death_heal; //done
	inline static RE::TESObjectMISC* cheat_death_token; //done
	inline static RE::TESGlobal* survival_mode_active; //done
	inline static RE::TESGlobal* health_penalty_ui_global; //done

	inline static RE::TESGlobal* stress_enabled;
	inline static RE::TESGlobal* stress_total_value;

	//inline static RE::EffectSetting* ethereal_effect;


};