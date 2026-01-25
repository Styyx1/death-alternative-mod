#pragma once
#include "mod-data.h"

namespace Config {
	/// <summary>
	/// Various mod Settings from a toml file. Can be saved as well
	/// </summary>
	
	using namespace MOD;

	struct Settings : REX::Singleton<Settings>{

		inline static REX::TOML::Bool toggle_remove_gold{ SETTINGS, "bRemoveGold", true };
		inline static REX::TOML::Bool toggle_gold_remove_msg{ SETTINGS, "bToggleGoldRemoveNotification", true };
		inline static REX::TOML::Bool toggle_death_without_gold{ SETTINGS, "bDeathWithoutGold", false };
		inline static REX::TOML::Bool toggle_health_injury{ SETTINGS, "bToggleHealthInjury", true };
		inline static REX::TOML::Bool toggle_stamina_injury{ SETTINGS, "bToggleStaminaInjury", true };
		inline static REX::TOML::Bool toggle_magicka_injury{ SETTINGS, "bToggleMagickaInjury", true };
		inline static REX::TOML::Bool toggle_calm_spell{ SETTINGS, "bToggleCalmSpell", true };

		inline static REX::TOML::F32 min_sleep_duration{ SETTINGS, "fMinimumSleepDuration", 8.f };
		inline static REX::TOML::F32 stress_increase_value{ SETTINGS, "fStressIncreaseValue", 10.f };
		inline static REX::TOML::F32 gold_remove_percentage{ SETTINGS, "fGoldRemovePercentage", 10.f };
		inline static REX::TOML::F32 reduce_percentage_health{ SETTINGS, "fHealthInjuryPercentage", 40.f };
		inline static REX::TOML::F32 reduce_percentage_stamina{ SETTINGS, "fStaminaInjuryPercentage", 40.f };
		inline static REX::TOML::F32 reduce_percentage_magicka{ SETTINGS, "fMagickaInjuryPercentage", 40.f };
		inline static REX::TOML::F32 heal_self_on_death_percentage{ SETTINGS, "fHealSelfOnDeathPercentage", 100.f };
		inline static REX::TOML::F32 heal_enemies_on_death_percentage{ SETTINGS, "fHealEnemiesOnDeathPercentage", 100.f };
		inline static REX::TOML::F32 heal_area{ SETTINGS, "fHealEnemiesArea", 512.f };
		inline static REX::TOML::Bool resurrect_enemies_in_cell{ SETTINGS, "bResurrectEnemiesInCell", true };
		inline static REX::TOML::Bool reset_inventory_on_resurrection{ SETTINGS, "bResetInventoryOnResurrect", true };

		inline static REX::TOML::I32 max_injury_count{ SETTINGS, "iMaxInjuryCount", int32_t(0)};
		inline static REX::TOML::Bool reset_injury_count_on_cure{ SETTINGS, "bResetInjuryCounterOnCure", true };
		inline static REX::TOML::U32 sleep_location_difficulty{ SETTINGS, "uSleepLocationDifficulty", uint32_t(1) };

		inline static REX::TOML::Bool show_stress_notif{ SETTINGS, "bToggleStressMessage", true };
		inline static REX::TOML::Str stress_increase_message{ SETTINGS, "sStressIncreaseMessage", std::string("You hope this ends well...") };
		inline static REX::TOML::Str stress_decrease_message{ SETTINGS, "sStressDecreaseMessage", std::string("Curing your injury relaxed you!") };

		inline static REX::TOML::Bool toggle_death_stagger{ SETTINGS, "bToggleDeathStagger", false };

		

		void UpdateSettings(bool a_save) {
			const auto toml = REX::TOML::SettingStore::GetSingleton();
			toml->Init(TOML_DEF.data(), TOML_CUS.data());
			if (!a_save)
				toml->Load();
			else
				toml->Save();
		};
	};
	/// <summary>
	/// Form Loader. Loads records from the main mod esp, update esm and Stress and Fear if enabled. 
	/// These files need to be nullchecked everytime I use them
	/// </summary>
	struct Forms : REX::Singleton<Forms> {
		//forms
		inline static RE::SpellItem* ethereal_spell_player;
		inline static RE::SpellItem* ethereal_spell_npcs;
		inline static RE::SpellItem* calm_spell;
		inline static RE::TESObjectMISC* cheat_death_token;
		inline static RE::TESGlobal* stress_enabled;
		inline static RE::TESGlobal* stress_total_value;
		inline static RE::EffectSetting* ethereal_effect;
		inline static RE::TESGlobal* injury_counter_global;
		inline static RE::SpellItem* injury_health;
		inline static RE::SpellItem* injury_stamina;
		inline static RE::SpellItem* injury_magicka;

		//apostasy specific stuff
		inline static RE::TESGlobal* gold_tax_global;
		inline static RE::SpellItem* temp_injury_spell;
		inline static RE::BGSPerk* lady_stone_perk;

		inline static bool IsStressModActive;

		/// <summary>
		/// To define the injury type used later in DeathEffects.cpp
		/// </summary>
		enum class InjuryType {
			kHealthInjury,
			kStaminaInjury,
			kMagickaInjury
		};


		void ChangeInjuryMagnitude() {
			injury_health->GetCostliestEffectItem()->effectItem.magnitude = Settings::reduce_percentage_health.GetValue();
			injury_stamina->GetCostliestEffectItem()->effectItem.magnitude = Settings::reduce_percentage_stamina.GetValue();
			injury_magicka->GetCostliestEffectItem()->effectItem.magnitude = Settings::reduce_percentage_magicka.GetValue();
		}

		void LoadForms() {
			const auto dh = RE::TESDataHandler::GetSingleton();

			if (!MiscUtil::IsModLoaded(SHADES_MOD)) {
				REX::FAIL("Can not find {} please make sure to enable it!", SHADES_MOD);
				return;
			}				

			ethereal_spell_player = dh->LookupForm<RE::SpellItem>(ETHEREAL_SPELL_ID, SHADES_MOD);
			ethereal_spell_npcs = dh->LookupForm<RE::SpellItem>(ETHEREAL_NPC_SPELL_ID, SHADES_MOD);
			calm_spell = dh->LookupForm<RE::SpellItem>(CALM_SPELL_ID, SHADES_MOD);
			cheat_death_token = dh->LookupForm<RE::TESObjectMISC>(CHEAT_DEATH_TOKEN_ID, SHADES_MOD);
			ethereal_effect = dh->LookupForm<RE::EffectSetting>(ETHEREAL_EFFECT_ID, SHADES_MOD);
			injury_counter_global = dh->LookupForm<RE::TESGlobal>(DEATH_COUNTER_GLOBAL_ID, SHADES_MOD);

			injury_health = dh->LookupForm<RE::SpellItem>(INJURY_HEALTH_ID, SHADES_MOD);
			injury_stamina = dh->LookupForm<RE::SpellItem>(INJURY_STAMINA_ID, SHADES_MOD);
			injury_magicka = dh->LookupForm<RE::SpellItem>(INJURY_MAGICKA_ID, SHADES_MOD);

			//Apostasy Specifics
			gold_tax_global = dh->LookupForm<RE::TESGlobal>(GOLD_TAX_GLOBAL_ID, SHADES_MOD);
			temp_injury_spell = dh->LookupForm<RE::SpellItem>(DUMMY_TEMP_INJURY_SPELL_ID, SHADES_MOD);
			lady_stone_perk = dh->LookupForm<RE::BGSPerk>(DUMMY_LADY_PERK_ID, SHADES_MOD);

			IsStressModActive = false;
			if (MiscUtil::IsModLoaded(STRESS_MOD)) {
				stress_total_value = dh->LookupForm<RE::TESGlobal>(STRESS_GLOBAL_FORM_ID, STRESS_MOD);
				stress_enabled = dh->LookupForm<RE::TESGlobal>(STRESS_ENABLED_FORM_ID, STRESS_MOD);
				IsStressModActive = true;

				//Nullcheck and type check to be safe
				//Previously i did not type check it in another mod and got a few issues with that
				if (stress_total_value) {
					if (!stress_total_value->Is(RE::FormType::Global))
						stress_total_value = nullptr;
				}				
				if (stress_enabled) {
					if (!stress_enabled->Is(RE::FormType::Global))
						stress_enabled = nullptr;
				}				
			}
			else {
				REX::INFO("{} is disabled. Stress integration not possible", STRESS_MOD);
			};
			REX::INFO("Forms for {} are loaded!", THIS_MOD_NAME);
			ChangeInjuryMagnitude();
		}
	};
}