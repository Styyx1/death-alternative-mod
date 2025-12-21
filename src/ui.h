#pragma once

#include <st-ui.h>

namespace Menu {

    using ux = StyyxUtil::UIUtil;

    

    template <class T>
    bool SettingSlider(const char* label, T& slider_var, const T min, const T max, const char* fmt,
        REX::TOML::Setting<T>& setting, const char* help)
    {
        ImGuiMCP::ImGui::SetNextItemWidth(200.f);
        bool changed = ImGuiMCP::ImGui::SliderScalar(label, ImGuiMCP::ImGuiDataType_::ImGuiDataType_Float, &slider_var,
            &min, &max, fmt);
        if (changed)
        {
            setting.SetValue(slider_var);
        }
        ImGuiMCP::ImGui::SameLine();
        ux::HelpMarker(help);
        return changed;
    }

    template <class T>
    bool SettingSliderINT(const char* label, T& slider_var, const T min, const T max, const char* fmt,
        REX::TOML::Setting<T>& setting, const char* help)
    {

        ImGuiMCP::ImGui::SetNextItemWidth(200.f);
        bool changed =
            ImGuiMCP::ImGui::SliderScalar(label, ImGuiMCP::ImGuiDataType_::ImGuiDataType_S32, &slider_var, &min, &max, fmt);
        if (changed)
        {
            setting.SetValue(slider_var);
        }
        ImGuiMCP::ImGui::SameLine();
        ux::HelpMarker(help);
        return changed;
    }

    inline bool SettingCheckbox(const char* label, bool& value_var, REX::TOML::Setting<bool>& setting, const char* help)
    {
        bool changed = ImGuiMCP::ImGui::Checkbox(label, &value_var);
        if (changed)
        {
            setting.SetValue(value_var);
        }
        ImGuiMCP::ImGui::SameLine();
        ux::HelpMarker(help);
        return changed;
    }




    namespace Titles {
        inline std::string MOD_TITLE = "Shades of Mortality";
        inline std::string SETTINGS_SEC = "Settings";
    }
    namespace Label {
        inline std::string system = FontAwesome::UnicodeToUtf8(0xf390) + " System";
        inline std::string save_settings = FontAwesome::UnicodeToUtf8(0xf0c7) + " Save Settings";
        inline std::string restore_defaults = FontAwesome::UnicodeToUtf8(0xf0e2) + " Reset Settings";
        inline std::string death_effects = FontAwesome::UnicodeToUtf8(0xf6b7) + " Death Effects";
        inline std::string injuries = FontAwesome::UnicodeToUtf8(0xf728) + " Death Injuries";
        inline std::string stress_integration = FontAwesome::UnicodeToUtf8(0xe040) + " Stress Integration";
    }
    bool SettingText(const char* label, std::string& text_var, REX::TOML::Setting<std::string>& setting, const char* help);
    void RegisterShadesMenu();
    void RestoreFromSettings();
    void RenderSystem();
    void ResetDefaults();


    namespace Settings {

        namespace Lab {
            inline std::string toggle_remove_gold = "Remove Gold on Death";
            inline std::string toggle_gold_remove_msg = "Gold Removal Notification";
            inline std::string toggle_death_without_gold = "Allow Death Without Gold";
            inline std::string toggle_health_injury = "Enable Health Injuries";
            inline std::string toggle_stamina_injury = "Enable Stamina Injuries";
            inline std::string toggle_magicka_injury = "Enable Magicka Injuries";
            inline std::string toggle_calm_spell = "Apply Calm on Death";
            inline std::string toggle_death_stagger = "Toggle Stagger";

            inline std::string min_sleep_duration = "Minimum Sleep Duration (Hours)";
            inline std::string stress_increase_value = "Stress Increase per Injury";
            inline std::string gold_remove_percentage = "Gold Removed on Death (%)";

            inline std::string reduce_percentage_health = "Health Injury Severity (%)";
            inline std::string reduce_percentage_stamina = "Stamina Injury Severity (%)";
            inline std::string reduce_percentage_magicka = "Magicka Injury Severity (%)";

            inline std::string heal_self_on_death_percentage = "Heal Player on Death (%)";
            inline std::string heal_enemies_on_death_percentage = "Heal Enemies on Death (%)";
            inline std::string heal_area = "Enemy Heal Radius";

            inline std::string resurrect_enemies_in_cell = "Resurrect Enemies in Cell";
            inline std::string reset_inventory_on_resurrection = "Reset Inventory on Resurrection";

            inline std::string max_injury_count = "Maximum Injury Count";
            inline std::string reset_injury_count_on_cure = "Reset Injury Counter on Cure";
            inline std::string sleep_location_difficulty = "Sleep Location Difficulty";

            inline std::string show_stress_notif = "Show Stress Notifications";
            inline std::string stress_increase_message = "Stress Increase Message";
            inline std::string stress_decrease_message = "Stress Decrease Message";
        }

        namespace Tool {
            inline std::string toggle_remove_gold = "Removes a percentage of the player's gold upon death.";
            inline std::string toggle_gold_remove_msg = "Shows a notification when gold is removed.";
            inline std::string toggle_death_without_gold = "Allows death to occur when the player has no gold.";
            inline std::string toggle_health_injury = "Applies a health penalty when sustaining injuries.";
            inline std::string toggle_stamina_injury = "Applies a stamina penalty when sustaining injuries.";
            inline std::string toggle_magicka_injury = "Applies a magicka penalty when sustaining injuries.";
            inline std::string toggle_calm_spell = "Applies a calm effect after death to nearby enemies.";
            inline std::string toggle_death_stagger = "Applies stagger on death";

            inline std::string min_sleep_duration = "Minimum number of hours required to recover from injuries.";
            inline std::string stress_increase_value = "Amount of stress gained when getting injured.";
            inline std::string gold_remove_percentage = "Percentage of gold removed upon death.";

            inline std::string reduce_percentage_health = "Percentage of maximum health reduced by an injury.";
            inline std::string reduce_percentage_stamina = "Percentage of maximum stamina reduced by an injury.";
            inline std::string reduce_percentage_magicka = "Percentage of maximum magicka reduced by an injury.";

            inline std::string heal_self_on_death_percentage = "Percentage of health restored to the dying actor.";
            inline std::string heal_enemies_on_death_percentage = "Percentage of health restored to enemies on death.";
            inline std::string heal_area = "Radius in which enemies are affected by healing.";

            inline std::string resurrect_enemies_in_cell = "Resurrects enemies in the current cell upon death.";
            inline std::string reset_inventory_on_resurrection = "Resets enemy inventories after resurrection.";

            inline std::string max_injury_count = "Maximum number of times the player can die.";
            inline std::string reset_injury_count_on_cure = "Reset the Injury Counter on curing injuries";
            inline std::string sleep_location_difficulty = "Difficulty modifier based on sleep location quality.";

            inline std::string show_stress_notif = "Shows notifications when stress increases or decreases.";
            inline std::string stress_increase_message = "Message displayed when stress increases.";
            inline std::string stress_decrease_message = "Message displayed when stress decreases.";
        }

        namespace Var {
            inline bool toggle_remove_gold;
            inline bool toggle_gold_remove_msg;
            inline bool toggle_death_without_gold;
            inline bool toggle_health_injury;
            inline bool toggle_stamina_injury;
            inline bool toggle_magicka_injury;
            inline bool toggle_calm_spell;
            inline bool toggle_death_stagger;

            inline float min_sleep_duration;
            inline float stress_increase_value;
            inline float gold_remove_percentage;

            inline float reduce_percentage_health;
            inline float reduce_percentage_stamina;
            inline float reduce_percentage_magicka;

            inline float heal_self_on_death_percentage;
            inline float heal_enemies_on_death_percentage;
            inline float heal_area;

            inline bool resurrect_enemies_in_cell;
            inline bool reset_inventory_on_resurrection;

            inline int32_t max_injury_count;
            inline bool reset_injury_count_on_cure;
            inline uint32_t sleep_location_difficulty;

            inline bool show_stress_notif;
            inline std::string stress_increase_message;
            inline std::string stress_decrease_message;
        }
        void RenderEffects();
        void RenderInjury();
        void RenderStress();
        void __stdcall RenderSettings();
    }
}
