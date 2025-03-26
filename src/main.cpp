#include "sleepEvent.h"
#include "ResurrectionAPI.h"
#include "settings.h"
#include "deatheffects.h"
#include "injurybase.h"
#include "serialisation.h"
#include "hooks.h"
#include "effectEvent.h"
#include "stresshandler.h"

class ResurrectionManager : public ResurrectionAPI
{
    bool should_resurrect(RE::Actor *a) const override
    {
        RE::PlayerCharacter *player = RE::PlayerCharacter::GetSingleton();

        if (a == player)
        {
            return true;
        }
        else
        {
            return DeathEffects::Ethereal::get_count(a, Settings::cheat_death_token) && Settings::enable_npcs;
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
            auto injManager = Injuries::DeathInjury::GetSingleton();
            injManager->HandlePlayerResurrection(player);
            return;
        }
        else
        {
            //DeathEffects::Ethereal::ProcessNPCDeath(a);
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
