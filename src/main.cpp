#include "sleepEvent.h"
#include "ResurrectionAPI.h"
#include "settings.h"
#include "deatheffects.h"
#include "injurybase.h"
#include "serialisation.h"

class ResurrectionManager : public ResurrectionAPI
{
    bool should_resurrect(RE::Actor* a) const override
    {
        RE::PlayerCharacter* player = RE::PlayerCharacter::GetSingleton();

        if (a == player) {            
            return true;
        }
        else {
            return false;
        }        
    }

    void resurrect(RE::Actor* a) override
    {
        auto* injury = Injuries::DeathInjury::GetSingleton();
        DeathEffects::Ethereal::SetEthereal(a);
        if (RE::PlayerCharacter* player = RE::PlayerCharacter::GetSingleton(); a == player) {
            Utility::Spells::ApplySpell(a, a, Settings::injury_spell_40);
            std::jthread([=] {
                std::this_thread::sleep_for(2s);
                SKSE::GetTaskInterface()->AddTask([=] {
                    injury->CheckInjuryAvPenalty(a);
                    });
                }).detach();
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

    if (auto serialization = SKSE::GetSerializationInterface()) {
        serialization->SetUniqueID(Serialisation::ID);
        serialization->SetSaveCallback(&Serialisation::SaveCallback);
        serialization->SetLoadCallback(&Serialisation::LoadCallback);
        serialization->SetRevertCallback(&Serialisation::RevertCallback);
    }

	return true;
}
