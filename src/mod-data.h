#pragma once

namespace MOD {
	inline constexpr std::string_view THIS_MOD_NAME = "Shades of Mortality";
	inline constexpr std::string_view RES_KEYWORD = "DoResurrect";

	inline constexpr std::string_view TOML_DEF = "Data/SKSE/Plugins/shades.toml";
	inline constexpr std::string_view TOML_CUS = "Data/SKSE/Plugins/shades_custom.toml";
	inline constexpr std::string_view SETTINGS = "Settings";

	//Formloading related
	inline constexpr std::string_view SHADES_MOD = "shade-of-mortality.esp";
	inline constexpr std::string_view SKYRIM_MOD = "Skyrim.esm";
	inline constexpr std::string_view STRESS_MOD = "Stress and Fear.esp";
	inline constexpr std::string_view UPDATE_MOD = "Update.esm";
	//shades:
	inline constexpr RE::FormID ETHEREAL_SPELL_ID = 0x801;

	inline constexpr RE::FormID CHEAT_DEATH_TOKEN_ID = 0x809;
	inline constexpr RE::FormID ETHEREAL_NPC_SPELL_ID = 0x80c;
	inline constexpr RE::FormID CALM_SPELL_ID = 0x810;
	
	inline constexpr RE::FormID ETHEREAL_EFFECT_ID = 0x800;
	inline constexpr RE::FormID DEATH_COUNTER_GLOBAL_ID = 0x81D;

	inline constexpr RE::FormID INJURY_HEALTH_ID = 0x81a;
	inline constexpr RE::FormID INJURY_STAMINA_ID = 0x81b;
	inline constexpr RE::FormID INJURY_MAGICKA_ID = 0x81c;

	//aposatasy related. legacy at this point especially since apostasy does not run on this mod's game version
	inline constexpr RE::FormID DUMMY_LADY_PERK_ID = 0x812;
	inline constexpr RE::FormID DUMMY_TEMP_INJURY_SPELL_ID = 0x814;
	inline constexpr RE::FormID GOLD_TAX_GLOBAL_ID = 0x815;
	//Stress and Fear integration
	inline constexpr RE::FormID STRESS_GLOBAL_FORM_ID = 0x801;
	inline constexpr RE::FormID STRESS_ENABLED_FORM_ID = 0x8a5;
	//Update esm
	inline constexpr RE::FormID INJURY_UI_GLOBAL_ID = 0x2EDE;

	inline constexpr float MAX_STRESS = 100.0f;
	inline constexpr float MIN_STRESS = 0.0f;
	inline constexpr int32_t MIN_GOLD_AMOUNT_REMOVED = 50;
};