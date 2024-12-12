#pragma once

#include "settings.h"
#include "utility.h"

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
		void HealthInjury(RE::Actor* a_actor, float a_percentage);		

		void CheckInjuryAvPenalty(RE::Actor* a_actor);
		void ApplyAttributePenalty(RE::Actor* a_actor, float percentPen);

		void RemoveAttributePenalty(RE::Actor* a_actor);
		void RemoveAllExistingInjurySpells(RE::Actor* a);
		float GetMaxHealthAv(RE::Actor* a_actor);
		void ApplyStressToDeath();
		void HealStressFromDeath();

		static void SetAttributePenaltyUIGlobal(float penaltyPerc)
		{
			auto newVal = penaltyPerc * 100.0f;
			newVal = std::clamp(newVal, 0.0f, 100.0f);
			if (Settings::health_penalty_ui_global) {
				Settings::health_penalty_ui_global->value = newVal;
			}
		}

		// ^ https://github.com/colinswrath/BladeAndBlunt/blob/main/include/InjuryPenaltyManager.h

		static RE::ActorValue LookupActorValueByName(const char* av_name)
		{
			// SE: 0x3E1450, AE: 0x3FC5A0, VR: ---
			using func_t = decltype(&LookupActorValueByName);
			REL::Relocation<func_t> func{ REL::RelocationID(26570, 27203) };
			return func(av_name);
		}

		inline static void RestoreAV(RE::Actor* a_actor, RE::ActorValue a_av, float a_value)
		{
			using func_t = decltype(RestoreAV);
			REL::Relocation<func_t> func{ REL::RelocationID(37513, 38455) };
			return func(a_actor, a_av, a_value);
		};

		inline static float GetActorValuePercentage(RE::Actor* a_actor, RE::ActorValue a_av)
		{
			using func_t = decltype(GetActorValuePercentage);
			REL::Relocation<func_t> func{ REL::RelocationID(36347, 37337) };
			return func(a_actor, a_av);
		}; //returns between 0 and 1


	};
}