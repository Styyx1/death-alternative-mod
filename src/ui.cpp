#include "ui.h"
#include "settings.h"

namespace Menu {
    bool SettingText(const char* label, std::string& text_var, REX::TOML::Setting<std::string>& setting, const char* help) {
        char buffer[256];
        std::snprintf(buffer, sizeof(buffer), "%s", text_var.c_str());
        bool changed = ImGui::InputText(label, buffer, sizeof(buffer), ImGuiInputTextFlags_EnterReturnsTrue);
        if (changed)
        {
            text_var = std::string(buffer);
            setting.SetValue(text_var);
        }
        ImGui::SameLine();
        ux::HelpMarker(help);
        return changed;
    }
    void RegisterShadesMenu()
    {
        if (!SKSEMenuFramework::IsInstalled())
        {
            return;
        }
        SKSEMenuFramework::SetSection(Titles::MOD_TITLE);
        SKSEMenuFramework::AddSectionItem(Titles::SETTINGS_SEC, Settings::RenderSettings);
        RestoreFromSettings();
    }
    void RestoreFromSettings()
	{
        using namespace Settings::Var;
        using set = Config::Settings;

        toggle_remove_gold = set::toggle_remove_gold.GetValue();
        toggle_gold_remove_msg = set::toggle_gold_remove_msg.GetValue();
        toggle_death_without_gold = set::toggle_death_without_gold.GetValue();
        toggle_health_injury = set::toggle_health_injury.GetValue();
        toggle_stamina_injury = set::toggle_stamina_injury.GetValue();
        toggle_magicka_injury = set::toggle_magicka_injury.GetValue();
        toggle_calm_spell = set::toggle_calm_spell.GetValue();
        min_sleep_duration = set::min_sleep_duration.GetValue();
        stress_increase_value = set::stress_increase_value.GetValue();
        gold_remove_percentage = set::gold_remove_percentage.GetValue();
        reduce_percentage_health = set::reduce_percentage_health.GetValue();
        reduce_percentage_stamina = set::reduce_percentage_stamina.GetValue();
        reduce_percentage_magicka = set::reduce_percentage_magicka.GetValue();
        heal_self_on_death_percentage = set::heal_self_on_death_percentage.GetValue();
        heal_enemies_on_death_percentage = set::heal_enemies_on_death_percentage.GetValue();
        heal_area = set::heal_area.GetValue();
        resurrect_enemies_in_cell = set::resurrect_enemies_in_cell.GetValue();
        reset_inventory_on_resurrection = set::reset_inventory_on_resurrection.GetValue();
        max_injury_count = set::max_injury_count.GetValue();
        reset_injury_count_on_cure = set::reset_injury_count_on_cure.GetValue();
        sleep_location_difficulty = set::sleep_location_difficulty.GetValue();
        show_stress_notif = set::show_stress_notif.GetValue();
        stress_increase_message = set::stress_increase_message.GetValue();
        stress_decrease_message = set::stress_decrease_message.GetValue();
	}
    void RenderSystem()
    {
        ImGui::NewLine();
        ImGui::SeparatorText(Label::system.c_str());

        if (ImGui::Button(Label::save_settings.c_str()))
        {
            Config::Settings::GetSingleton()->UpdateSettings(true);
        }

        ImGui::SameLine();
        if (ImGui::Button(Label::restore_defaults.c_str()))
        {
            ResetDefaults();
        }
    }
    void ResetDefaults()
    {
        using namespace Settings::Var;
        using set = Config::Settings;

         toggle_remove_gold = true;
         toggle_gold_remove_msg = true;
         toggle_death_without_gold = false;
         toggle_health_injury = true;
         toggle_stamina_injury = true;
         toggle_magicka_injury = true;
         toggle_calm_spell = true;
         min_sleep_duration = 8.0;
         stress_increase_value = 10.0;
         gold_remove_percentage = 20.0;
         reduce_percentage_health = 40.0;
         reduce_percentage_stamina = 40.0;
         reduce_percentage_magicka = 40.0;
         heal_self_on_death_percentage = 100.0;
         heal_enemies_on_death_percentage = 100.0;
         heal_area = 512.0;
         resurrect_enemies_in_cell = true;
         reset_inventory_on_resurrection = true;
         max_injury_count = 0;
         reset_injury_count_on_cure = true;
         sleep_location_difficulty = 1;
         show_stress_notif = true;
         stress_increase_message = "You hope this ends well...";
         stress_decrease_message = "Curing your injury relaxed you!";

         set::toggle_remove_gold.SetValue(toggle_remove_gold);
         set::toggle_gold_remove_msg.SetValue(toggle_gold_remove_msg);
         set::toggle_death_without_gold.SetValue(toggle_death_without_gold);
         set::toggle_calm_spell.SetValue(toggle_calm_spell);
         set::resurrect_enemies_in_cell.SetValue(resurrect_enemies_in_cell);
         set::reset_inventory_on_resurrection.SetValue(reset_inventory_on_resurrection);
         set::gold_remove_percentage.SetValue(gold_remove_percentage);
         set::heal_self_on_death_percentage.SetValue(heal_self_on_death_percentage);
         set::heal_enemies_on_death_percentage.SetValue(heal_enemies_on_death_percentage);
         set::heal_area.SetValue(heal_area);         

         set::toggle_health_injury.SetValue(toggle_health_injury);
         set::toggle_stamina_injury.SetValue(toggle_stamina_injury);
         set::toggle_magicka_injury.SetValue(toggle_magicka_injury); 
         set::reset_injury_count_on_cure.SetValue(reset_injury_count_on_cure);
         set::min_sleep_duration.SetValue(min_sleep_duration);                 
         set::reduce_percentage_health.SetValue(reduce_percentage_health);
         set::reduce_percentage_stamina.SetValue(reduce_percentage_stamina);
         set::reduce_percentage_magicka.SetValue(reduce_percentage_magicka);         
         set::max_injury_count.SetValue(max_injury_count);         
         set::sleep_location_difficulty.SetValue(sleep_location_difficulty);

         set::show_stress_notif.SetValue(show_stress_notif);
         set::stress_increase_value.SetValue(stress_increase_value);
         set::stress_increase_message.SetValue(stress_increase_message);
         set::stress_decrease_message.SetValue(stress_decrease_message);

         set::GetSingleton()->UpdateSettings(true);
    }
}

void Menu::Settings::RenderEffects()
{
    using set = Config::Settings;

    ImGui::NewLine();
    ImGui::SeparatorText(Label::death_effects.c_str());

    SettingSlider(Lab::gold_remove_percentage.c_str(), Var::gold_remove_percentage, 1.0f, 100.0f, "%.2f%%", set::gold_remove_percentage, Tool::gold_remove_percentage.c_str());
    SettingSlider(Lab::heal_self_on_death_percentage.c_str(), Var::heal_self_on_death_percentage, 1.0f, 100.f, "%.2f%%", set::heal_self_on_death_percentage, Tool::heal_self_on_death_percentage.c_str());
    SettingSlider(Lab::heal_enemies_on_death_percentage.c_str(), Var::heal_enemies_on_death_percentage, 1.0f, 100.f, "%.2f%%", set::heal_enemies_on_death_percentage, Tool::heal_enemies_on_death_percentage.c_str());
    SettingSlider(Lab::heal_area.c_str(), Var::heal_area, 50.0f, 1024.f, "%.0f", set::heal_area, Tool::heal_area.c_str());

    SettingCheckbox(Lab::toggle_remove_gold.c_str(), Var::toggle_remove_gold, set::toggle_remove_gold, Tool::toggle_remove_gold.c_str());
    ImGui::SameLine();
    SettingCheckbox(Lab::toggle_gold_remove_msg.c_str(), Var::toggle_gold_remove_msg, set::toggle_gold_remove_msg, Tool::toggle_gold_remove_msg.c_str());

    SettingCheckbox(Lab::toggle_death_without_gold.c_str(), Var::toggle_death_without_gold, set::toggle_death_without_gold, Tool::toggle_death_without_gold.c_str());
    ImGui::SameLine();
    SettingCheckbox(Lab::toggle_calm_spell.c_str(), Var::toggle_calm_spell, set::toggle_calm_spell, Tool::toggle_calm_spell.c_str());

    SettingCheckbox(Lab::resurrect_enemies_in_cell.c_str(), Var::resurrect_enemies_in_cell, set::resurrect_enemies_in_cell, Tool::resurrect_enemies_in_cell.c_str());
    ImGui::SameLine();
    SettingCheckbox(Lab::reset_inventory_on_resurrection.c_str(), Var::reset_inventory_on_resurrection, set::reset_inventory_on_resurrection, Tool::reset_inventory_on_resurrection.c_str());

    SettingCheckbox(Lab::toggle_death_stagger.c_str(), Var::toggle_death_stagger, set::toggle_death_stagger, Tool::toggle_death_stagger.c_str());
}

void Menu::Settings::RenderInjury()
{
    using set = Config::Settings;

    static const char* sleepLocationLabels[] = {
    "Everywhere",
    "Only Non-Dungeons",
    "Player Homes and Temples"
    };

    ImGui::NewLine();
    ImGui::SeparatorText(Label::injuries.c_str());

    SettingCheckbox(Lab::toggle_health_injury.c_str(), Var::toggle_health_injury, set::toggle_health_injury, Tool::toggle_health_injury.c_str());
    ImGui::SameLine();
    SettingCheckbox(Lab::toggle_stamina_injury.c_str(), Var::toggle_stamina_injury, set::toggle_stamina_injury, Tool::toggle_stamina_injury.c_str());

    SettingCheckbox(Lab::toggle_magicka_injury.c_str(), Var::toggle_magicka_injury, set::toggle_magicka_injury, Tool::toggle_magicka_injury.c_str());
    ImGui::SameLine();
    SettingCheckbox(Lab::reset_injury_count_on_cure.c_str(), Var::reset_injury_count_on_cure, set::reset_injury_count_on_cure, Tool::reset_injury_count_on_cure.c_str());

    SettingSlider(Lab::min_sleep_duration.c_str(), Var::min_sleep_duration, 1.0f, 24.0f, "%.1fh", set::min_sleep_duration, Tool::min_sleep_duration.c_str());

    bool changed_h = SettingSlider(Lab::reduce_percentage_health.c_str(), Var::reduce_percentage_health, 1.0f, 99.f, "%.1f%%", set::reduce_percentage_health, Tool::reduce_percentage_health.c_str());
    bool changed_s = SettingSlider(Lab::reduce_percentage_stamina.c_str(), Var::reduce_percentage_stamina, 1.0f, 99.f, "%.1f%%", set::reduce_percentage_stamina, Tool::reduce_percentage_stamina.c_str());
    bool changed_m = SettingSlider(Lab::reduce_percentage_magicka.c_str(), Var::reduce_percentage_magicka, 1.0f, 99.f, "%.1f%%", set::reduce_percentage_magicka, Tool::reduce_percentage_magicka.c_str());

    if (changed_h || changed_s || changed_m) {
        Config::Forms::GetSingleton()->ChangeInjuryMagnitude();
    }

    SettingSliderINT(Lab::max_injury_count.c_str(), Var::max_injury_count, 0, 15, "%i", set::max_injury_count, Tool::max_injury_count.c_str());
    constexpr int sleepLocationCount = static_cast<int>(std::size(sleepLocationLabels));
    auto& value = Menu::Settings::Var::sleep_location_difficulty;
    if (ImGuiMCP::ImGui::Combo(
        Menu::Settings::Lab::sleep_location_difficulty.c_str(),
        reinterpret_cast<int*>(&value),
        sleepLocationLabels,
        sleepLocationCount))
    {
        Config::Settings::sleep_location_difficulty.SetValue(value);
    }
    ImGuiMCP::ImGui::SameLine();
    ux::HelpMarker(Menu::Settings::Tool::sleep_location_difficulty.c_str());
}

void Menu::Settings::RenderStress()
{
    ImGui::NewLine();
    ImGui::SeparatorText(Label::stress_integration.c_str());
    using set = Config::Settings;
    SettingCheckbox(Lab::show_stress_notif.c_str(), Var::show_stress_notif, set::show_stress_notif, Tool::show_stress_notif.c_str());
    SettingSlider(Lab::stress_increase_value.c_str(), Var::stress_increase_value, 1.0f, 30.0f, "%.0f", set::stress_increase_value, Tool::stress_increase_value.c_str());
    SettingText(Lab::stress_increase_message.c_str(), Var::stress_increase_message, set::stress_increase_message, Tool::stress_increase_message.c_str());
    SettingText(Lab::stress_decrease_message.c_str(), Var::stress_decrease_message, set::stress_decrease_message, Tool::stress_decrease_message.c_str());

    static const ImVec4 color = Config::Forms::IsStressModActive ? ImVec4(0.1529f, 0.5451f, 0.2118f, 1.0f) : ImVec4(0.5922f, 0.1098f, 0.1216f, 1.0f);
    static const char* isStressActive = Config::Forms::IsStressModActive ? "Stress and Fear integration is active" : "Stress and Fear integration is not active";
    ImGui::TextColored(color, "%s", isStressActive);

}

void __stdcall Menu::Settings::RenderSettings()
{
    FontAwesome::PushSolid();

    RenderEffects();
    RenderInjury();
    RenderStress();
    RenderSystem();

    FontAwesome::Pop();
}
