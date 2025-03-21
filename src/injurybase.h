#pragma once

#include "settings.h"
#include "utility.h"
#include "deatheffects.h"

// Credits: https://github.com/colinswrath/BladeAndBlunt/blob/main/include/InjuryPenaltyManager.h
namespace Injuries
{
	class DeathInjury
	{
		DeathInjury() = default;
		DeathInjury(const DeathInjury&) = delete;
		DeathInjury(DeathInjury&&) = delete;
		DeathInjury& operator=(const DeathInjury&) = delete;
		DeathInjury& operator=(DeathInjury&&) = delete;

	public:
		static DeathInjury* GetSingleton();
		inline static bool processing{false};
		float currentInjuryPenalty;
		float currentStamRatePen;
		float currentMagRatePen;
		bool injury_active;
		bool can_apply_stress;
		std::int32_t injuryCount;
		bool hasDiedThisCycle;

		float GetMaxActorValue(RE::Actor* a_actor, RE::ActorValue a_av);
		float GetMaxHealthAv(RE::Actor* a_actor);
		float GetMaxStaminaRate(RE::Actor* a_actor);
		float GetMaxMagickaRate(RE::Actor* a_actor);

		void CheckInjuryAvPenalty(RE::Actor* a_actor);
		void ApplyAttributePenalty(RE::Actor* a_actor, float penalty_health, float penalty_stam, float penalty_mag);
		void RemoveAttributePenalty(RE::Actor* a_actor);
		void RemoveAllExistingInjurySpells(RE::Actor* a);
		
		void ApplyStressToDeath();
		void HealStressFromDeath();

		void HandlePlayerResurrection(RE::PlayerCharacter* player);

		//does not work for health, has its use for Stamina and Magicka
		inline static void RestoreAV(RE::Actor* a_actor, RE::ActorValue a_av, float a_value)
		{
			{
				if (a_value == 0) {
					return;
				}
				if (a_actor) {
					a_actor->AsActorValueOwner()->RestoreActorValue(RE::ACTOR_VALUE_MODIFIER::kDamage, a_av, a_value);
				}
			}

			/*using func_t = decltype(RestoreAV);
			REL::Relocation<func_t> func{ REL::RelocationID(37513, 38455) };
			return func(a_actor, a_av, a_value);*/
		};

		//does not stick, no idea why. Removed for now
		static void SetAttributePenaltyUIGlobal(float penaltyPerc)
		{			
			auto newVal = penaltyPerc * 100.0f;
			newVal = std::clamp(newVal, 0.0f, 100.0f);
			if (Settings::health_penalty_ui_global) {
				Settings::health_penalty_ui_global->value = newVal;
			}
		}
	};
}