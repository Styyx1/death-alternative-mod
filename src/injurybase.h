#pragma once

#include "settings.h"
#include "utility.h"
#include "deatheffects.h"

// Credits: https://github.com/colinswrath/BladeAndBlunt/blob/main/include/InjuryPenaltyManager.h
namespace Injuries
{
	class DeathInjury : public REX::Singleton<DeathInjury>
	{
	public:
		bool processing{false};
		float currentInjuryPenalty;
		float currentStamRatePen;
		float currentMagRatePen;
		static inline bool injury_active;
		bool can_apply_stress;
		std::int32_t injuryCount;
		bool hasDiedThisCycle{false};

		float GetMaxActorValue(RE::Actor *a_actor, RE::ActorValue a_av);
		float GetMaxHealthAv(RE::Actor *a_actor);
		float GetMaxStaminaRate(RE::Actor *a_actor);
		float GetMaxMagickaRate(RE::Actor *a_actor);
		int counter = 1;

		const bool IsInjuryActive() const;

		void CheckInjuryAvPenalty(RE::Actor *a_actor);

		void ApplyAttributePenalty(RE::Actor *a_actor, float penalty_health, float penalty_stam, float penalty_mag);
		void RemoveAttributePenalty(RE::Actor *a_actor);
		void RemoveAllExistingInjurySpells(RE::Actor *a);

		void ApplyStressToDeath();
		void HealStressFromDeath();

		bool CheckLadyStoneGold(RE::PlayerCharacter *player);
	};
}
