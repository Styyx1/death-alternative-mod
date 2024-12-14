#include "injurybase.h"
#include "stresshandler.h"

float Injuries::DeathInjury::GetMaxActorValue(RE::Actor* a_actor, RE::ActorValue a_av)
{
    return a_actor->GetActorValueModifier(RE::ACTOR_VALUE_MODIFIER::kTemporary, a_av) + a_actor->AsActorValueOwner()->GetPermanentActorValue(a_av);
}
//main function to call, leads to the other necessary functions
void Injuries::DeathInjury::CheckInjuryAvPenalty(RE::Actor* a_actor)
{
	if (a_actor->HasSpell(Settings::injury_spell)) {
		logs::info("check if injury is active");
		ApplyAttributePenalty(a_actor, Settings::injury_decrease_modifier);
		return;
	}
	else {
		RemoveAttributePenalty(a_actor);
	}
	return;
}

void Injuries::DeathInjury::ApplyAttributePenalty(RE::Actor* a_actor, float percentPen)
{
	if (injury_active) {
		logs::info("injury is still active");
		return;
	}
	float maxPenAv = GetMaxHealthAv(a_actor);
	float lastPenaltyMag = currentInjuryPenalty;
	float modifier = percentPen / 100;
	float newPenaltyMag = std::roundf(maxPenAv * modifier);


	if (newPenaltyMag > maxPenAv) {
		newPenaltyMag = maxPenAv;
	}
	auto magDelta = lastPenaltyMag - newPenaltyMag;
	logs::info("stats check. maxPenAV = {}, newPenaltyMag = {}, magDelta = {}", maxPenAv, newPenaltyMag, magDelta);
	if (magDelta < 0) {
		logs::info("restores some health");
		a_actor->AsActorValueOwner()->RestoreActorValue(RE::ACTOR_VALUE_MODIFIER::kDamage, RE::ActorValue::kHealth, -1 * magDelta);  //Damage or restore AV
	}

	//Set tracker av not actual damage
	currentInjuryPenalty = newPenaltyMag;

	a_actor->AsActorValueOwner()->RestoreActorValue(RE::ACTOR_VALUE_MODIFIER::kPermanent, RE::ActorValue::kHealth, magDelta);	//Damage or restore AV
	logs::info("av check: current value is {} and current max is {}", a_actor->AsActorValueOwner()->GetActorValue(RE::ActorValue::kHealth), GetMaxActorValue(a_actor, RE::ActorValue::kHealth));
	injury_active = true;
	can_apply_stress = true;
	ApplyStressToDeath();
	/*if (Utility::Checks::IsSurvivalEnabled) {
		SetAttributePenaltyUIGlobal(percentPen);
	}*/
	return;
}

void Injuries::DeathInjury::RemoveAttributePenalty(RE::Actor* a_actor)
{
	float currentPenaltyMag = currentInjuryPenalty;
	injury_active = false;
	HealStressFromDeath();
	if (currentPenaltyMag > 0) {
		currentInjuryPenalty = 0.0f;
		/*if (Utility::Checks::IsSurvivalEnabled) {
			SetAttributePenaltyUIGlobal(0.0f);
		}*/		
		a_actor->AsActorValueOwner()->RestoreActorValue(RE::ACTOR_VALUE_MODIFIER::kPermanent, RE::ActorValue::kHealth, currentPenaltyMag);		
	}
	return;
}

//too complex for what it does, but it was something else at first, so i kept it. Can use this to expand with injury tiers if necessary again.
void Injuries::DeathInjury::RemoveAllExistingInjurySpells(RE::Actor* a)
{
	if (a->HasSpell(Settings::injury_spell)) {
		a->RemoveSpell(Settings::injury_spell);
	}
	return;
}

float Injuries::DeathInjury::GetMaxHealthAv(RE::Actor* a_actor)
{
	return GetMaxActorValue(a_actor, RE::ActorValue::kHealth) + currentInjuryPenalty;
}

void Injuries::DeathInjury::ApplyStressToDeath()
{
	if (can_apply_stress && Settings::is_stress_mod_active && Settings::stress_enabled->value != 0) {
		auto* stress = StressHandler::StressApplication::GetSingleton();
		logs::info("stress before death = {}", Settings::stress_total_value->value);
		stress->ApplyStressOnce();
		logs::info("stress after death = {}", Settings::stress_total_value->value);
		can_apply_stress = false;
	}
}

void Injuries::DeathInjury::HealStressFromDeath()
{
	if (!can_apply_stress && Settings::is_stress_mod_active && Settings::stress_enabled->value != 0) {
		auto* stress = StressHandler::StressApplication::GetSingleton();
		logs::info("stress before healing = {}", Settings::stress_total_value->value);
		stress->ReduceStress();
		logs::info("stress after healing = {}", Settings::stress_total_value->value);
		can_apply_stress = true;
	}
}
