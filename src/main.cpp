#include "sleepEvent.h"
#include "ResurrectionAPI.h"
#include "settings.h"
#include "deatheffects.h"
#include "injurybase.h"
#include "serialisation.h"
#include "hooks.h"
#include "effectEvent.h"

class ResurrectionManager : public ResurrectionAPI
{
    bool should_resurrect(RE::Actor* a) const override
    {
        RE::PlayerCharacter* player = RE::PlayerCharacter::GetSingleton();

        if (a == player) {            
            return true;
        }
        else {
            return DeathEffects::Ethereal::get_count(a, Settings::cheat_death_token) && Settings::enable_npcs;
        }        
    }

    void resurrect(RE::Actor* a) override
    {        
        if (RE::PlayerCharacter* player = RE::PlayerCharacter::GetSingleton(); a == player) {
            auto* injury = Injuries::DeathInjury::GetSingleton();
            injury->RestoreAV(a, RE::ActorValue::kStamina, 20.0f);
            DeathEffects::Ethereal::SetEthereal(a);
            Utility::Spells::ApplySpell(a, a, Settings::injury_spell);
            DeathEffects::Ethereal::RemoveGoldPlayer(player, Settings::gold_remove_percentage);
            std::jthread([=] {
                std::this_thread::sleep_for(2s);
                SKSE::GetTaskInterface()->AddTask([=] {
                    injury->CheckInjuryAvPenalty(a);
                    });
                }).detach();
        }
        else {
            DeathEffects::Ethereal::ProcessNPCDeath(a);
        }
    }
};

void addSubscriber()
{
	if (auto pluginHandle = REX::W32::GetModuleHandleA("ResurrectionAPI.dll")) {
		if (auto AddSubscriber = (AddSubscriber_t)GetProcAddress(pluginHandle, "ResurrectionAPI_AddSubscriber")) {
			AddSubscriber(std::make_unique<ResurrectionManager>());
		}
	}
}

void InitListener(SKSE::MessagingInterface::Message* a_msg) 
{
	if (a_msg->type == SKSE::MessagingInterface::kDataLoaded) {        
		Settings::LoadSettings();
		Settings::LoadForms();
        Hooks::InstallHooks();
        Effect::InstallEvents();
		SleepEvent::InstallEvents();
		addSubscriber();
	}
    if (a_msg->type == SKSE::MessagingInterface::kPostLoadGame) {
        auto* injury = Injuries::DeathInjury::GetSingleton();
        if (injury->injury_active) {
            injury->CheckInjuryAvPenalty(RE::PlayerCharacter::GetSingleton());
        }        
    }
} 

SKSEPluginLoad(const SKSE::LoadInterface* a_skse)
{
	SKSE::Init(a_skse);
	SKSE::GetMessagingInterface()->RegisterListener(InitListener);
    SKSE::AllocTrampoline(14);
    if (auto serialization = SKSE::GetSerializationInterface()) {
        serialization->SetUniqueID(Serialisation::ID);
        serialization->SetSaveCallback(&Serialisation::SaveCallback);
        serialization->SetLoadCallback(&Serialisation::LoadCallback);
        serialization->SetRevertCallback(&Serialisation::RevertCallback);
    }

	return true;
}
