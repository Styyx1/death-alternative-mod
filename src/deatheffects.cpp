#include "deatheffects.h"
#include "utility.h"
#include "settings.h"
#include "injurybase.h"

#undef GetObject
// name is misleading. casts healing spell, casts ethereal spell, all enemies around the player stop combat.
void DeathEffects::Ethereal::SetEthereal(RE::Actor *a_actor)
{
	if (!Utility::Spells::ActorHasActiveMagicEffect(a_actor, Settings::ethereal_effect))
	{
		Utility::Spells::ApplySpell(a_actor, a_actor, Settings::death_heal);
		Utility::Spells::ApplySpell(a_actor, a_actor, Settings::ethereal_spell);
		logs::info("applied heal and ethereal spell");
		auto injManager = Injuries::DeathInjury::GetSingleton();
		for (auto actor : Utility::Actors::GetNearbyActors(a_actor, 500.0f, false))
		{
			if (Settings::heal_enemies_on_death && actor->IsHostileToActor(a_actor))
			{
				Utility::Spells::ApplySpell(actor, actor, Settings::death_heal);
				logs::info("applied {} to {}", Settings::death_heal->GetName(), actor->GetName());
				if (actor != a_actor)
				{
					Utility::Spells::ApplySpell(actor, actor, Settings::calm_spell_npcs);
					logs::info("applied {} to {}", Settings::calm_spell_npcs->GetName(), actor->GetName());
				}
			}
		}
		auto player = Cache::GetPlayerSingleton();
		if (!Utility::Spells::ActorHasActiveMagicEffect(a_actor, Settings::injury_display_effect))
		{
			logs::info("doesn't have injury effect");
			if (injManager->CheckLadyStoneGold(player))
			{
				logs::info("negative lady stone check. gonna apply spells");
				Utility::Spells::ApplySpell(player, player, Settings::injury_spell);
			}
		}
		if (Settings::remove_gold)
		{
			RemoveGoldPlayer(player, Settings::gold_remove_percentage);
		}

		injManager->can_apply_stress = true;
		injManager->ApplyStressToDeath();
	}
}

void DeathEffects::Ethereal::ProcessNPCDeath(RE::Actor *a_actor)
{
	Utility::Spells::ApplySpell(a_actor, a_actor, Settings::death_heal);
	Utility::Spells::ApplySpell(a_actor, a_actor, Settings::ethereal_spell_npcs);
	a_actor->RemoveItem(Settings::cheat_death_token, 1, RE::ITEM_REMOVE_REASON::kRemove, nullptr, nullptr, nullptr, nullptr);
	return;
}

// remove x% of gold from the player, at least 50
void DeathEffects::Ethereal::RemoveGoldPlayer(RE::PlayerCharacter *player, float a_percentage)
{
	RE::TESForm *const gold = RE::BGSDefaultObjectManager::GetSingleton()->GetObject(RE::DEFAULT_OBJECT::kGold);
	float modifier = a_percentage / 100;
	int32_t minimum_value = 50;
	int32_t gold_to_remove_percent = static_cast<int32_t>((player->GetItemCount(gold->As<RE::TESBoundObject>()) * modifier));
	int32_t maximum_gold_to_remove = player->GetItemCount(gold->As<RE::TESBoundObject>());
	if (minimum_value > maximum_gold_to_remove)
	{
		minimum_value = maximum_gold_to_remove;
	}
	if (maximum_gold_to_remove > 0)
	{
		uint32_t amount = std::clamp(gold_to_remove_percent, minimum_value, maximum_gold_to_remove);
		player->RemoveItem(gold->As<RE::TESBoundObject>(), amount, RE::ITEM_REMOVE_REASON::kRemove, nullptr, nullptr);
		if (Settings::show_gold_removal_message)
		{
			RE::DebugNotification(std::format("{} {} removed", amount, gold->GetName()).c_str());
		}
	}
	return;
}
