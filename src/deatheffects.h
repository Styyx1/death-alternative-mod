#pragma once
#include "settings.h"

namespace OnDeath {
	struct Effects : REX::Singleton<Effects> {
		void PlayerDeathEffects(RE::PlayerCharacter* player);
		void NPCDeathEffects(RE::Actor* a_actor);
	private:
		void RemovePlayerGold(RE::PlayerCharacter* player, float a_percentage);
		void HealOnDeath(RE::Actor* a_actor, float percentage = 100.0f);
		void ApplyEtherealEffect(RE::Actor* a_actor);
		void HealAndCalmEnemiesOnDeath(RE::Actor* a_actor, float a_area);
		void RemoveDeathTokenNPC(RE::Actor* a_actor);
		void ResurrectEnemiesOnDeath(RE::TESObjectCELL* a_cell) const;
		void StaggerOnDeath(RE::Actor* a_actor);

	};
	struct Injuries : REX::Singleton<Injuries> {
		void ProcessInjury(RE::Actor* a_actor);		
		void RemoveAllInjuries(RE::Actor* a_actor);
		void RemoveInjury(RE::Actor* a_actor,  RE::ActorValue a_av);
		

	private:
		bool CheckLadyStoneGoldAmount(RE::PlayerCharacter* player);
		void ApplyInjury(RE::Actor* a_actor, Config::Forms::InjuryType type);
		void ApplyHealthInjury(RE::Actor* a_actor );
		void ApplyStaminaInjury(RE::Actor* a_actor);
		void ApplyMagickaInjury(RE::Actor* a_actor);
		std::vector<Config::Forms::InjuryType> GetPossibleInjuries(RE::Actor* a_actor) const;
		std::optional<Config::Forms::InjuryType> SelectInjury(RE::Actor* a_actor) const;
		void ApplyStressOnDeath();
		void RemoveDeathStress();

	};
}
