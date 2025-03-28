#include "injurybase.h"
#include "stresshandler.h"

float Injuries::DeathInjury::GetMaxActorValue(RE::Actor *a_actor, RE::ActorValue a_av)
{
	return a_actor->GetActorValueModifier(RE::ACTOR_VALUE_MODIFIER::kTemporary, a_av) + a_actor->AsActorValueOwner()->GetPermanentActorValue(a_av);
}
const bool Injuries::DeathInjury::IsInjuryActive() const
{
	return injury_active;
}
// main function to call, leads to the other necessary functions
void Injuries::DeathInjury::CheckInjuryAvPenalty(RE::Actor *a_actor)
{
	if (Utility::Spells::HasSpell(a_actor, Settings::injury_spell))
	{
		ApplyAttributePenalty(a_actor, Settings::injury_health_decrease, Settings::injury_stam_decrease, Settings::injury_mag_decrease);
	}
	else
	{
		RemoveAttributePenalty(a_actor);
	}
}

void Injuries::DeathInjury::ApplyAttributePenalty(RE::Actor *a_actor, float penalty_health, float penalty_stam, float penalty_mag)
{
	RE::PlayerCharacter *player = RE::PlayerCharacter::GetSingleton();

	if (!IsInjuryActive())
	{
		injury_active = true;
		if (Settings::use_health_injury)
		{
			float maxPenAv = GetMaxHealthAv(a_actor);
			float lastPenaltyMag = currentInjuryPenalty;
			float modifier = penalty_health / 100;
			float newPenaltyMag = maxPenAv * modifier;

			if (newPenaltyMag > maxPenAv)
			{
				newPenaltyMag = maxPenAv;
			}

			auto magDelta = lastPenaltyMag - newPenaltyMag;

			// Set tracker av not actual damage
			currentInjuryPenalty = newPenaltyMag;

			player->AsActorValueOwner()->RestoreActorValue(RE::ACTOR_VALUE_MODIFIER::kPermanent, RE::ActorValue::kHealRate, magDelta); // Damage or restore AV
		}

		// Stamina rate:
		if (Settings::use_stamina_injury)
		{
			float maxStamPenAv = GetMaxStaminaRate(a_actor);
			float lastPenaltyStamRate = currentStamRatePen;
			float stamRModi = penalty_stam / 100;
			float newStamRateMag = maxStamPenAv * stamRModi;
			if (newStamRateMag > maxStamPenAv)
			{
				newStamRateMag = maxStamPenAv;
			}
			auto stamRateMagDelta = lastPenaltyStamRate - newStamRateMag;
			currentStamRatePen = newStamRateMag;

			a_actor->AsActorValueOwner()->RestoreActorValue(RE::ACTOR_VALUE_MODIFIER::kPermanent, RE::ActorValue::kStaminaRate, stamRateMagDelta);
		}

		// Magicka rate:
		if (Settings::use_magicka_injury)
		{
			float maxmagickPenAv = GetMaxMagickaRate(a_actor);
			float lastPenaltymagickRate = currentMagRatePen;
			float magickRModi = penalty_mag / 100;
			float newmagickRateMag = maxmagickPenAv * magickRModi;
			if (newmagickRateMag > maxmagickPenAv)
			{
				newmagickRateMag = maxmagickPenAv;
			}
			auto magickRateMagDelta = lastPenaltymagickRate - newmagickRateMag;
			currentMagRatePen = newmagickRateMag;

			a_actor->AsActorValueOwner()->RestoreActorValue(RE::ACTOR_VALUE_MODIFIER::kPermanent, RE::ActorValue::kMagickaRate, magickRateMagDelta);
		}
	}
}

void Injuries::DeathInjury::RemoveAttributePenalty(RE::Actor *a_actor)
{
	float currentPenaltyHealthMag = currentInjuryPenalty;
	float currMagPenalty = currentMagRatePen;

	float currentStamPenalty = currentStamRatePen;
	injury_active = false;
	HealStressFromDeath();
	if (currentPenaltyHealthMag > 0)
	{
		currentInjuryPenalty = 0.0f;
		a_actor->AsActorValueOwner()->RestoreActorValue(RE::ACTOR_VALUE_MODIFIER::kPermanent, RE::ActorValue::kHealRate, currentPenaltyHealthMag);
	}
	if (currMagPenalty > 0.0f)
	{
		currentMagRatePen = 0.0f;
		a_actor->AsActorValueOwner()->RestoreActorValue(RE::ACTOR_VALUE_MODIFIER::kPermanent, RE::ActorValue::kMagickaRate, currMagPenalty);
	}
	if (currentStamPenalty > 0.0f)
	{
		currentStamRatePen = 0.0f;
		a_actor->AsActorValueOwner()->RestoreActorValue(RE::ACTOR_VALUE_MODIFIER::kPermanent, RE::ActorValue::kStaminaRate, currentStamPenalty);
	}
	injuryCount = 0;
	return;
}

// too complex for what it does, but it was something else at first, so i kept it. Can use this to expand with injury tiers if necessary again.
void Injuries::DeathInjury::RemoveAllExistingInjurySpells(RE::Actor *a)
{
	if (a->HasSpell(Settings::injury_spell))
	{
		a->RemoveSpell(Settings::injury_spell);
	}
	return;
}

float Injuries::DeathInjury::GetMaxHealthAv(RE::Actor *a_actor)
{
	return GetMaxActorValue(a_actor, RE::ActorValue::kHealRate) + currentInjuryPenalty;
}

float Injuries::DeathInjury::GetMaxStaminaRate(RE::Actor *a_actor)
{
	return GetMaxActorValue(a_actor, RE::ActorValue::kStaminaRate) + currentStamRatePen;
}

float Injuries::DeathInjury::GetMaxMagickaRate(RE::Actor *a_actor)
{
	return GetMaxActorValue(a_actor, RE::ActorValue::kMagickaRate) + currentMagRatePen;
}

void Injuries::DeathInjury::ApplyStressToDeath()
{
	if (can_apply_stress && Settings::is_stress_mod_active && Settings::stress_enabled->value != 0)
	{
		auto *stress = StressHandler::StressApplication::GetSingleton();
		stress->ApplyStressOnce();
		can_apply_stress = false;
	}
}

void Injuries::DeathInjury::HealStressFromDeath()
{
	if (!can_apply_stress && Settings::is_stress_mod_active && Settings::stress_enabled->value != 0)
	{
		auto *stress = StressHandler::StressApplication::GetSingleton();
		stress->ReduceStress();
		can_apply_stress = true;
	}
}

#undef GetObject
bool Injuries::DeathInjury::CheckLadyStoneGold(RE::PlayerCharacter *player)
{
	if (player->HasPerk(Settings::lady_stone_perk))
	{
		RE::TESForm *const gold = RE::BGSDefaultObjectManager::GetSingleton()->GetObject(RE::DEFAULT_OBJECT::kGold);
		if (player->GetItemCount(gold->As<RE::TESBoundObject>()) >= Settings::gold_tax_global->value)
		{
			player->RemoveItem(gold->As<RE::TESBoundObject>(), Settings::gold_tax_global->value, RE::ITEM_REMOVE_REASON::kRemove, nullptr, nullptr, nullptr);
			player->RemoveSpell(Settings::injury_spell);
			return false;
		}
	}
	return true;
}
