#include "sleepEvent.h"
#include "ResurrectionAPI.h"
#include "settings.h"
#include "deatheffects.h"
#include "injurybase.h"
#include "serialisation.h"
#include "hooks.h"
#include "effectEvent.h"
#include "stresshandler.h"

#undef GetObject

bool resurrect_only_with_gold(RE::PlayerCharacter* player) {
    RE::TESForm* const gold = RE::BGSDefaultObjectManager::GetSingleton()->GetObject(RE::DEFAULT_OBJECT::kGold);
    if (Settings::kill_without_gold) {
        if (player->GetItemCount(gold->As<RE::TESBoundObject>()) > 0) {
            return true;
        }			
        else {
            return false; 
        }			
    }
    return true;
}

bool kill_with_inj(RE::PlayerCharacter* player)
{
    if (Settings::kill_with_injury) {
        auto inj = Injuries::DeathInjury::GetSingleton();
        if (inj->injury_active) {
            return false;
        }
        else
            return true;
    }
    return true;
}

bool get_res_cond(RE::PlayerCharacter* player) {

    if (kill_with_inj(player)) {
        auto inj = Injuries::DeathInjury::GetSingleton();
        if (inj->injuryCount > std::clamp(Settings::number_of_injuries - 1, (std::uint32_t)1, (std::uint32_t)100)) {
            return false;
        }
    }
    return kill_with_inj(player) && resurrect_only_with_gold(player);

}

class ResurrectionManager : public ResurrectionAPI
{
    const bool should_resurrect(RE::Actor* a) override
    {
        RE::PlayerCharacter* player = RE::PlayerCharacter::GetSingleton();

        if (a == player) {
            return get_res_cond(player);
        }
        else {
            return DeathEffects::Ethereal::get_count(a, Settings::cheat_death_token) && Settings::enable_npcs;
        }
        return false;
    }

    bool busy = false;
    int counter = 0;

    void resurrect(RE::Actor* a) override
    {      
        RE::PlayerCharacter* player = RE::PlayerCharacter::GetSingleton(); 
        if (a == player) {
            if (!Injuries::DeathInjury::processing) {
                Injuries::DeathInjury* injury = Injuries::DeathInjury::GetSingleton();
                injury->HandlePlayerResurrection(player);
            }            
            return;
        }
        else {
            DeathEffects::Ethereal::ProcessNPCDeath(a);
            return;
        }
        return;
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
