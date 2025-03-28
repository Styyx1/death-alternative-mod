#include "sleepEvent.h"
#include "ResurrectionAPI.h"
#include "settings.h"
#include "deatheffects.h"
#include "injurybase.h"
#include "serialisation.h"
#include "hooks.h"
#include "effectEvent.h"
#include "stresshandler.h"
#include "cache.h"

#undef GetObject

bool resurrect_only_with_gold(RE::PlayerCharacter *player)
{
    RE::TESForm *const gold = RE::BGSDefaultObjectManager::GetSingleton()->GetObject(RE::DEFAULT_OBJECT::kGold);
    if (Settings::kill_without_gold)
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

    bool should_res = true;
    auto injManager = Injuries::DeathInjury::GetSingleton();
    if (Settings::number_of_injuries > 0)
    {
        if (injManager->injuryCount > Settings::number_of_injuries)
        {
            should_res = false;
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
            return false;
        }
    }

    bool busy = false;
    int counter = 0;
    int debugCounter = 0;

    void resurrect(RE::Actor *a) override
    {
        RE::PlayerCharacter *player = RE::PlayerCharacter::GetSingleton();
        if (a == player)
        {
            DeathEffects::Ethereal::SetEthereal(player);
        }
        else
        {
            DeathEffects::Ethereal::ProcessNPCDeath(a);
            return;
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
        Settings::LoadSettings();
        Settings::LoadForms();
        Hooks::InstallHooks();
        Effect::InstallEvents();
        SleepEvent::InstallEvents();

        addSubscriber();
    }
    if (a_msg->type == SKSE::MessagingInterface::kPostLoadGame)
    {
        auto injManager = Injuries::DeathInjury::GetSingleton();
        injManager->CheckInjuryAvPenalty(RE::PlayerCharacter::GetSingleton());
    }
}

SKSEPluginLoad(const SKSE::LoadInterface *a_skse)
{
    SKSE::Init(a_skse);
    Cache::CacheAddLibAddresses();
    SKSE::GetMessagingInterface()->RegisterListener(InitListener);
    SKSE::AllocTrampoline(14 * 2);
    if (auto serialization = SKSE::GetSerializationInterface())
    {
        serialization->SetUniqueID(Serialisation::ID);
        serialization->SetSaveCallback(&Serialisation::SaveCallback);
        serialization->SetLoadCallback(&Serialisation::LoadCallback);
        serialization->SetRevertCallback(&Serialisation::RevertCallback);
    }

    return true;
}
