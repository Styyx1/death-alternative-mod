#include "sleepEvent.h"
#include "ResurrectionAPI.h"
#include "settings.h"
#include "deatheffects.h"
#include "effectEvent.h"
#include "cache.h"
#include "ui.h"

#undef GetObject

bool resurrect_only_with_gold(RE::PlayerCharacter *player)
{
    RE::TESForm *const gold = RE::BGSDefaultObjectManager::GetSingleton()->GetObject(RE::DEFAULT_OBJECT::kGold);
    if (Config::Settings::toggle_death_without_gold.GetValue())
    {
        if (player->GetItemCount(gold->As<RE::TESBoundObject>()) > 0)
        {
            return true;
        }
        else
        {
            return false;
        }
    }
    return true;
}

bool get_res_cond(RE::PlayerCharacter *player)
{    
    bool has_injury_left = Config::Forms::injury_counter_global->value < Config::Settings::max_injury_count.GetValue();
    bool should_res = true;
   
    if (Config::Settings::max_injury_count > 0)
    {
        if (!has_injury_left)
        {
            return false;
        }
    }
    return should_res && resurrect_only_with_gold(player);
}

class ResurrectionManager : public ResurrectionAPI
{
    bool should_resurrect(RE::Actor *a) const override
    {
        RE::PlayerCharacter *player = Cache::GetPlayerSingleton();

        if (a == player)
        {
            return get_res_cond(player);
        }
        else
        {
            return ActorUtil::ActorHasItem(a, Config::Forms::cheat_death_token) || ActorUtil::HasEffectWithKeywordActive(a, "DoResurrect");
        }
    }

    void resurrect(RE::Actor *a) override
    {
        RE::PlayerCharacter *player = RE::PlayerCharacter::GetSingleton();
        const auto death = OnDeath::Effects::GetSingleton();

        if (a == player)
        {          
            death->PlayerDeathEffects(player);
        }
        else
        {
            death->NPCDeathEffects(a);
        }
        return;
    }
};

void addSubscriber()
{
    if (auto pluginHandle = REX::W32::GetModuleHandleA("ResurrectionAPI.dll"))
    {
        if (auto AddSubscriber = (AddSubscriber_t)GetProcAddress(pluginHandle, "ResurrectionAPI_AddSubscriber"))
        {
            AddSubscriber(std::make_unique<ResurrectionManager>());
        }
    }
}

void InitListener(SKSE::MessagingInterface::Message *a_msg)
{
    if (a_msg->type == SKSE::MessagingInterface::kDataLoaded)
    {
        Config::Forms::GetSingleton()->LoadForms();
        Effect::InstallEvents();
        SleepEv::InstallSleep();
        addSubscriber();

#ifdef _BUILD_DEFAULT_CONFIG
        Config::Settings::min_sleep_duration.SetValue(8.1);
        Config::Settings::GetSingleton()->UpdateSettings(true);
        REX::INFO("Saved Default Config");
#endif 
    }
}

SKSEPluginLoad(const SKSE::LoadInterface *a_skse)
{
    SKSE::Init(a_skse, {.trampoline = true});
    Cache::CacheAddLibAddresses();
    Config::Settings::GetSingleton()->UpdateSettings(false);
    SKSE::GetMessagingInterface()->RegisterListener(InitListener);    
    Menu::RegisterShadesMenu();

    return true;
}
