#include "injurybase.h"
#include "stresshandler.h"

float Injuries::DeathInjury::GetMaxActorValue(RE::Actor *a_actor, RE::ActorValue a_av)
{
	return a_actor->GetActorValueModifier(RE::ACTOR_VALUE_MODIFIER::kTemporary, a_av) + a_actor->AsActorValueOwner()->GetPermanentActorValue(a_av);
}
// main function to call, leads to the other necessary functions
void Injuries::DeathInjury::CheckInjuryAvPenalty(RE::Actor *a_actor)
{
	if (a_actor->HasSpell(Settings::injury_spell))
	{
		if (!injury_active)
		{
			ApplyAttributePenalty(a_actor, Settings::injury_health_decrease, Settings::injury_stam_decrease, Settings::injury_mag_decrease);
			injuryCount++;
		}
		else if (!hasDiedThisCycle)
		{
			Utility::Spells::ApplySpell(a_actor, a_actor, Settings::temp_injury_spell);
			injuryCount++;
			hasDiedThisCycle = true;
		}
		return;
	}
	else
	{
		RemoveAttributePenalty(a_actor);
		hasDiedThisCycle = false;
	}
	return;
}

void Injuries::DeathInjury::ApplyAttributePenalty(RE::Actor *a_actor, float penalty_health, float penalty_stam, float penalty_mag)
{
	injury_active = true;
	RE::PlayerCharacter *player = RE::PlayerCharacter::GetSingleton();
	// Heal rate:
	if (Settings::use_health_injury)
	{
		float maxPenAv = GetMaxHealthAv(a_actor);
		float lastPenaltyMag = currentInjuryPenalty;
		float modifier = penalty_health / 100;
		float newPenaltyMag = std::roundf(maxPenAv * modifier);

		if (newPenaltyMag > maxPenAv)
		{
			newPenaltyMag = maxPenAv;
		}

		auto magDelta = lastPenaltyMag - newPenaltyMag;
		currentInjuryPenalty = newPenaltyMag; // Set tracker av not actual damage

		// wait 0.5 second to reduce health cause it does not regenerate fast enough otherwise and kills you again. RestoreActorValue does not work for me for some reason so i had to use a spell instead.
		std::jthread([=]
					 {
						 std::this_thread::sleep_for(0.5s);
						 player->AsActorValueOwner()->RestoreActorValue(RE::ACTOR_VALUE_MODIFIER::kPermanent, RE::ActorValue::kHealth, magDelta); // Damage or restore AV
					 })
			.detach();
	}

	// Stamina rate:
	if (Settings::use_stamina_injury)
	{
		float maxStamPenAv = GetMaxStaminaRate(a_actor);
		float lastPenaltyStamRate = currentStamRatePen;
		float stamRModi = penalty_stam / 100;
		float newStamRateMag = std::roundf(maxStamPenAv * stamRModi);
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
		float newmagickRateMag = std::roundf(maxmagickPenAv * magickRModi);
		if (newmagickRateMag > maxmagickPenAv)
		{
			newmagickRateMag = maxmagickPenAv;
		}
		auto magickRateMagDelta = lastPenaltymagickRate - newmagickRateMag;
		currentMagRatePen = newmagickRateMag;

		a_actor->AsActorValueOwner()->RestoreActorValue(RE::ACTOR_VALUE_MODIFIER::kPermanent, RE::ActorValue::kMagickaRate, magickRateMagDelta);
	}
	can_apply_stress = true;
	ApplyStressToDeath();
	return;
}

int counter = 0;

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
		SetAttributePenaltyUIGlobal(0.0f);
		a_actor->AsActorValueOwner()->RestoreActorValue(RE::ACTOR_VALUE_MODIFIER::kPermanent, RE::ActorValue::kHealth, currentPenaltyHealthMag);
	}
	if (currMagPenalty > 0)
	{
		currentMagRatePen = 0.0f;
		a_actor->AsActorValueOwner()->RestoreActorValue(RE::ACTOR_VALUE_MODIFIER::kPermanent, RE::ActorValue::kMagickaRate, currentPenaltyHealthMag);
	}
	if (currentStamPenalty > 0)
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
	return GetMaxActorValue(a_actor, RE::ActorValue::kHealth) + currentInjuryPenalty;
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

void Injuries::DeathInjury::HandlePlayerResurrection(RE::PlayerCharacter *player)
{
	counter++;
	logs::info("counter is {}", counter);
	if (counter == 1)
	{
		Utility::Spells::ApplySpell(player, player, Settings::injury_spell);

		player->AsActorValueOwner()->RestoreActorValue(RE::ACTOR_VALUE_MODIFIER::kDamage, RE::ActorValue::kStamina, 50.0f);

		DeathEffects::Ethereal::SetEthereal(player);
		StressHandler::StressApplication::IncreaseStressWithoutInjury(Settings::stress_increase_value);

		if (Settings::remove_gold)
		{
			DeathEffects::Ethereal::RemoveGoldPlayer(player, Settings::gold_remove_percentage);
		}
		if (player->HasPerk(Settings::lady_stone_perk))
		{
#undef GetObject
			RE::TESForm *const gold = RE::BGSDefaultObjectManager::GetSingleton()->GetObject(RE::DEFAULT_OBJECT::kGold);
			if (player->GetItemCount(gold->As<RE::TESBoundObject>()) >= Settings::gold_tax_global->value)
			{
				player->RemoveItem(gold->As<RE::TESBoundObject>(), Settings::gold_tax_global->value, RE::ITEM_REMOVE_REASON::kRemove, nullptr, nullptr, nullptr);
			}
			else
			{
				CheckInjuryAvPenalty(player);
			}
		}
		else
		{
			CheckInjuryAvPenalty(player);
		}
		logs::info("check death");
		hasDiedThisCycle = false;
		processing = false;
		std::jthread([=]
					 {
			std::this_thread::sleep_for(3s);
				counter = 0; })
			.detach();
		return;
	}
	else
	{
		std::jthread([=]
					 {
			std::this_thread::sleep_for(3s);
			counter = 0; })
			.detach();
		return;
	}
}
