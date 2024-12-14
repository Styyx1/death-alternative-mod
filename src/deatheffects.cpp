#include "deatheffects.h"
#include "utility.h"
#include "settings.h"

#undef GetObject

void DeathEffects::Ethereal::SetEthereal(RE::Actor* a_actor)
{
	Utility::Spells::ApplySpell(a_actor, a_actor, Settings::death_heal);
	Utility::Spells::ApplySpell(a_actor, a_actor, Settings::ethereal_spell);
	
	for (auto& actor : Utility::Actors::GetNearbyActors(a_actor, 500.0f, false)) {
		actor->StopCombat();
		//actor->NotifyAnimationGraph("staggerStart");
	}
}

void DeathEffects::Ethereal::ProcessNPCDeath(RE::Actor* a_actor)
{
	Utility::Spells::ApplySpell(a_actor, a_actor, Settings::death_heal);
	Utility::Spells::ApplySpell(a_actor, a_actor, Settings::ethereal_spell_npcs);
	a_actor->RemoveItem(Settings::cheat_death_token, 1, RE::ITEM_REMOVE_REASON::kRemove, nullptr, nullptr, nullptr, nullptr);
	logs::info("Removed {} from {}", Settings::cheat_death_token->GetName(), a_actor->GetDisplayFullName());
	return;
}

void DeathEffects::Ethereal::RemoveGoldPlayer(RE::PlayerCharacter* player, float a_percentage)
{
	RE::TESForm* const gold = RE::BGSDefaultObjectManager::GetSingleton()->GetObject(RE::DEFAULT_OBJECT::kGold);
	float modifier = a_percentage / 100;
	uint32_t amount = std::clamp(player->GetItemCount(gold->As<RE::TESBoundObject>()), (int32_t)50, static_cast<int32_t>(player->GetItemCount(gold->As<RE::TESBoundObject>()) * modifier));
	player->RemoveItem(gold->As<RE::TESBoundObject>(), amount, RE::ITEM_REMOVE_REASON::kRemove, nullptr, nullptr);
	logs::info("removed {} pieces of {}", amount, gold->As<RE::TESBoundObject>()->GetName());
}
