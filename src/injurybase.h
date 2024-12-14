#pragma once

#include "settings.h"
#include "utility.h"

// most of it is derived from: https://github.com/colinswrath/BladeAndBlunt/blob/main/include/InjuryPenaltyManager.h
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
		static DeathInjury* GetSingleton() {
			static DeathInjury singleton;
			return &singleton;
		}

		float currentInjuryPenalty;
		bool injury_active;
		bool can_apply_stress;

		float GetMaxActorValue(RE::Actor* a_actor, RE::ActorValue a_av);		

		void CheckInjuryAvPenalty(RE::Actor* a_actor);
		void ApplyAttributePenalty(RE::Actor* a_actor, float percentPen);

		void RemoveAttributePenalty(RE::Actor* a_actor);
		void RemoveAllExistingInjurySpells(RE::Actor* a);
		float GetMaxHealthAv(RE::Actor* a_actor);
		void ApplyStressToDeath();
		void HealStressFromDeath();
		
		//does not work for health, has its use for Stamina and Magicka
		inline static void RestoreAV(RE::Actor* a_actor, RE::ActorValue a_av, float a_value)
		{
			using func_t = decltype(RestoreAV);
			REL::Relocation<func_t> func{ REL::RelocationID(37513, 38455) };
			return func(a_actor, a_av, a_value);
		};

		//does not stick, no idea why. Removed for now
		static void SetAttributePenaltyUIGlobal(float penaltyPerc)
		{
			
			logs::info("Set Penalty UI");
			auto newVal = penaltyPerc * 100.0f;
			newVal = std::clamp(newVal, 0.0f, 100.0f);
			if (Settings::health_penalty_ui_global) {
				Settings::health_penalty_ui_global->value = newVal;
			}
		}
	};
}