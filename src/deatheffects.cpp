#include "deatheffects.h"
#include "utility.h"
#include "settings.h"

void DeathEffects::Ethereal::SetEthereal(RE::Actor* a_actor)
{
	Utility::Spells::ApplySpell(a_actor, a_actor, Settings::death_heal);
	Utility::Spells::ApplySpell(a_actor, a_actor, Settings::ethereal_spell);
	
	for (auto& actor : Utility::Actors::GetNearbyActors(a_actor, 500.0f, false)) {
		actor->StopCombat();
		//actor->NotifyAnimationGraph("staggerStart");
	}
}
