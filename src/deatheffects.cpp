#include "mod-data.h"
#include "deatheffects.h"
#include "stresshandler.h"
#include "cache.h"

#undef GetObject
static std::string msg{};

namespace OnDeath {
	/// <summary>
	/// Player Exclusive Death handling. Player needs different stuff happen on death
	/// </summary>
	/// <param name="player"></param>
	void Effects::PlayerDeathEffects(RE::PlayerCharacter *player) 
	{
		StaggerOnDeath(player);
		ApplyEtherealEffect(player);
		HealOnDeath(player, Config::Settings::heal_self_on_death_percentage.GetValue());
		HealAndCalmEnemiesOnDeath(player, Config::Settings::heal_area.GetValue());
		RemovePlayerGold(player, Config::Settings::gold_remove_percentage.GetValue());
		auto inj_manager = Injuries::GetSingleton();
		inj_manager->ProcessInjury(player);
	}

	/// <summary>
	/// NPC Specific death handling. a lot more streamlined. does not heal enemies nearby, neither does it revive anyone
	/// </summary>
	/// <param name="a_actor"></param>
	void Effects::NPCDeathEffects(RE::Actor* a_actor)
	{
		StaggerOnDeath(a_actor);
		ApplyEtherealEffect(a_actor);
		HealOnDeath(a_actor, Config::Settings::heal_self_on_death_percentage.GetValue());
		RemoveDeathTokenNPC(a_actor);
		auto inj_manager = Injuries::GetSingleton();
		inj_manager->ProcessInjury(a_actor);

	}
	/// <summary>
	/// Removes Gold from the player inventory. 
	/// Has a minimimum that gets removed and is percentage based
	/// No need to actually check for the maximum cause the game clamps it anyway
	/// </summary>
	/// <param name="player"></param>
	/// <param name="a_percentage"></param>
	void Effects::RemovePlayerGold(RE::PlayerCharacter* player, float a_percentage)
	{	
		//not really needed but also doesn't hurt to be there. No nullcheck for the player cause if the player is nullptr, the game will probably crash regardless
		if (a_percentage <= 0)
			return;

		if (!Config::Settings::toggle_remove_gold.GetValue())
			return;

		RE::TESForm* const gold = RE::BGSDefaultObjectManager::GetSingleton()->GetObject(RE::DEFAULT_OBJECT::kGold);
		if (!gold)
			return;

		RE::TESBoundObject* goldBD = gold->As<RE::TESBoundObject>();
		auto before = player->GetItemCount(goldBD);
		const float modifier = a_percentage / 100.f;

		int32_t gold_to_remove_percent = static_cast<int32_t>(std::round(before * modifier));
		//Get the higher of the 2 values
		int32_t amount = std::max(MOD::MIN_GOLD_AMOUNT_REMOVED, gold_to_remove_percent);		
		player->RemoveItem(goldBD, amount, RE::ITEM_REMOVE_REASON::kRemove, nullptr, nullptr);

		if (Config::Settings::toggle_gold_remove_msg.GetValue())
		{			
			const int32_t after = player->GetItemCount(goldBD);
			const int32_t removed = before - after;

			if (removed > 0) {
				msg = std::format("{} {} removed", removed, gold->GetName());
				RE::SendHUDMessage::ShowHUDMessage(msg.c_str());
			}
		}		
	}
	/// <summary>
	/// Heals actor on death. reusable for player, actor and for enemy heal function
	/// Has a default percentage of 100 if called without the float param
	/// Always heals Stamina and Magicka to full
	/// </summary>
	/// <param name="a_actor"></param>
	/// <param name="percentage"></param>
	void Effects::HealOnDeath(RE::Actor* a_actor, float percentage)
	{
		if (!a_actor)
			return;
		//to prevent weird configs. i don't want to accidentally kill after death :)
		percentage = std::clamp(percentage, 0.f, 100.f);
		const float amount_restore_health = ActorUtil::GetMaxHealth(a_actor) * (percentage / 100.f);
		const float max_mag = ActorUtil::GetMaxMagicka(a_actor);
		const float max_stam = ActorUtil::GetMaxStamina(a_actor);
		float damage_health = a_actor->healthModifiers.modifiers[RE::ACTOR_VALUE_MODIFIER::kDamage] * -1;
		a_actor->RestoreActorValue(RE::ActorValue::kHealth, amount_restore_health + damage_health);
		float damage_mag = a_actor->magickaModifiers.modifiers[RE::ACTOR_VALUE_MODIFIER::kDamage] * -1;
		a_actor->RestoreActorValue(RE::ActorValue::kMagicka, max_mag + damage_mag);
		float damage_stam = a_actor->staminaModifiers.modifiers[RE::ACTOR_VALUE_MODIFIER::kDamage] * -1;
		a_actor->RestoreActorValue(RE::ActorValue::kStamina, max_stam + damage_stam);

	}

	/// <summary>
	/// Applies a spell with an ethereal effect. Different effects for player and npcs
	/// </summary>
	/// <param name="a_actor"></param>
	void Effects::ApplyEtherealEffect(RE::Actor* a_actor)
	{
		if (!ActorUtil::IsEffectActive(a_actor, Config::Forms::ethereal_effect)) {
			if (a_actor->IsPlayerRef())
				MagicUtil::ApplySpell(a_actor, a_actor, Config::Forms::ethereal_spell_player);
			else
				MagicUtil::ApplySpell(a_actor, a_actor, Config::Forms::ethereal_spell_npcs);
		}
	}

	/// <summary>
	/// Heals and Calms enemies on death.
	/// optionally also resurrects enemies in the cell the player died in
	/// Healing is area based, resurrection is for the whole cell
	/// </summary>
	/// <param name="a_actor"></param>
	/// <param name="a_area"></param>
	void Effects::HealAndCalmEnemiesOnDeath(RE::Actor* a_actor, float a_area)
	{
		if (!a_actor)
			return;
		if (a_area <= 0.f)
			return;

		for (auto& actor : ActorUtil::GetNearbyActors(a_actor, a_area, false)) {
			if (actor->IsHostileToActor(a_actor) && !actor->IsDead()) {
				HealOnDeath(actor, Config::Settings::heal_enemies_on_death_percentage.GetValue());				
				if (actor != a_actor && Config::Settings::toggle_calm_spell.GetValue()) {
					MagicUtil::ApplySpell(actor, actor, Config::Forms::calm_spell);
				}
			}
		}
		ResurrectEnemiesOnDeath(a_actor->GetParentCell());
	}
	/// <summary>
	/// Helper function to remove the death token from NPCs. 
	/// A bit redundant but easier to maintain and potentially add to
	/// </summary>
	/// <param name="a_actor"></param>
	void Effects::RemoveDeathTokenNPC(RE::Actor* a_actor)
	{
		a_actor->RemoveItem(Config::Forms::cheat_death_token, 1, RE::ITEM_REMOVE_REASON::kRemove, nullptr, nullptr, nullptr);
	}

	/// <summary>
	/// resurrect all dead non-unique, non-quest, non-boss enemies in the cell and npcs without the no-resurrect keyword
	/// </summary>
	/// <param name="a_cell"></param>
	void Effects::ResurrectEnemiesOnDeath(RE::TESObjectCELL* a_cell) const
	{
		if (!a_cell)
			return;

		if (!Config::Settings::resurrect_enemies_in_cell.GetValue())
			return;

		for (auto& cell : a_cell->references) {
			auto actor = cell.get()->As<RE::Actor>();
			if (actor && !actor->IsPlayerRef() && actor->IsDead() && !actor->IsPlayerTeammate() && actor->GetActorBase() && !actor->GetActorBase()->IsUnique()) {
				if (actor->extraList.HasQuestObjectAlias())
					continue;
				if (ActorUtil::StartsDead(actor)) {
					continue;
				}
				if (actor->HasKeywordString("NotResurrect") || actor->GetActorBase()->HasKeywordString("NotResurrect")) {
					continue;
				}
				bool is_boss = false;
				auto loc = a_cell->GetLocation();
				if (loc && !loc->promotedRefs.empty()) {
					for (auto& ref : loc->promotedRefs) {
						if (ref.get().get() && ref.get().get() == actor) {
							is_boss = true;
							break;
						}
					}
				}
				if (is_boss)
					continue;

				actor->Resurrect(Config::Settings::reset_inventory_on_resurrection.GetValue(), true);
			}
		}
	}

	/// <summary>
	/// Stagger the actor on death. It is turned off by default
	/// </summary>
	/// <param name="a_actor"></param>
	void Effects::StaggerOnDeath(RE::Actor* a_actor)
	{
		if (Config::Settings::toggle_death_stagger.GetValue()) {
			a_actor->SetGraphVariableFloat("StaggerMagnitude", 1.0);
			a_actor->NotifyAnimationGraph("StaggerStart");
		
		}
			
	}
	/// <summary>
	/// Adds Injury Spell based on the InjuryType
	/// if the actor is the player, it increases the injury count
	/// caps to float maximum but that's definitely overkill
	/// </summary>
	/// <param name="a_actor"></param>
	/// <param name="type"></param>
	void Injuries::ApplyInjury(RE::Actor* a_actor, Config::Forms::InjuryType type)
	{
		switch (type) {
		case Config::Forms::InjuryType::kHealthInjury:
			ApplyHealthInjury(a_actor);
			break;
		case Config::Forms::InjuryType::kStaminaInjury:
			ApplyStaminaInjury(a_actor);
			break;
		case Config::Forms::InjuryType::kMagickaInjury:
			ApplyMagickaInjury(a_actor);
			break;
		}
		if (a_actor->IsPlayerRef()) {
			Config::Forms::injury_counter_global->value = MathUtil::AddWithCap<float>(Config::Forms::injury_counter_global->value, 1.0f, 100.0f);
		}

	}

	/// <summary>
	/// Process Injury which includes LadyStone Check, Stress application and selecting of the injury to use
	/// </summary>
	/// <param name="a_actor"></param>
	void Injuries::ProcessInjury(RE::Actor* a_actor)
	{
		if (!a_actor)
			return;			

		if (a_actor->IsPlayerRef()) {
			if (!CheckLadyStoneGoldAmount(Cache::GetPlayerSingleton()))
				return;
			ApplyStressOnDeath();
		}		

		auto injury = SelectInjury(a_actor);
		if (!injury)
			return;

		ApplyInjury(a_actor, *injury);		
	}

	/// <summary>
	/// Remove all injuries is used in sleep and magic effect event. 
	/// </summary>
	/// <param name="a_actor"></param>
	void Injuries::RemoveAllInjuries(RE::Actor* a_actor)
	{
		RemoveInjury(a_actor, RE::ActorValue::kHealth);
		RemoveInjury(a_actor, RE::ActorValue::kStamina);
		RemoveInjury(a_actor, RE::ActorValue::kMagicka);
		if (a_actor->IsPlayerRef()) {
			RemoveDeathStress();
		}
		if (Config::Settings::reset_injury_count_on_cure.GetValue()) {
			Config::Forms::injury_counter_global->value = 0;
		}

	}

	/// <summary>
	/// Remove Injury based on InjuryType
	/// </summary>
	/// <param name="a_actor"></param>
	/// <param name="a_av"></param>
	void Injuries::RemoveInjury(RE::Actor* a_actor, RE::ActorValue a_av)
	{
		switch (a_av) {
		case RE::ActorValue::kHealth:
			a_actor->RemoveSpell(Config::Forms::injury_health);
			return;
		case RE::ActorValue::kStamina:
			a_actor->RemoveSpell(Config::Forms::injury_stamina);
			return;
		case RE::ActorValue::kMagicka:
			a_actor->RemoveSpell(Config::Forms::injury_magicka);
			return;
		default:
			return;
		}
	}

	void Injuries::ApplyHealthInjury(RE::Actor* a_actor)
	{
		if (Config::Settings::toggle_health_injury.GetValue()) {
			if (!a_actor->HasSpell(Config::Forms::injury_health)) {
				MagicUtil::ApplySpell(a_actor, a_actor, Config::Forms::injury_health);
			}
		}
	}

	void Injuries::ApplyStaminaInjury(RE::Actor* a_actor)
	{
		if (Config::Settings::toggle_stamina_injury.GetValue()) {
			if (!a_actor->HasSpell(Config::Forms::injury_stamina)) {
				MagicUtil::ApplySpell(a_actor, a_actor, Config::Forms::injury_stamina);
			}
		}
	}

	void Injuries::ApplyMagickaInjury(RE::Actor* a_actor)
	{
		if (Config::Settings::toggle_magicka_injury.GetValue()) {
			if (!a_actor->HasSpell(Config::Forms::injury_magicka)) {
				MagicUtil::ApplySpell(a_actor, a_actor, Config::Forms::injury_magicka);
			}
		}
	}
	/// <summary>
	/// Select Possible injury. Possible means "Is The setting active and does the actor have the inury already
	/// </summary>
	/// <param name="a_actor"></param>
	/// <returns></returns>
	std::vector<Config::Forms::InjuryType> Injuries::GetPossibleInjuries(RE::Actor* a_actor) const
	{
		std::vector<Config::Forms::InjuryType> result;

		if (Config::Settings::toggle_health_injury.GetValue() &&
			!a_actor->HasSpell(Config::Forms::injury_health)) {
			result.push_back(Config::Forms::InjuryType::kHealthInjury);
		}

		if (Config::Settings::toggle_stamina_injury.GetValue() &&
			!a_actor->HasSpell(Config::Forms::injury_stamina)) {
			result.push_back(Config::Forms::InjuryType::kStaminaInjury);
		}

		if (Config::Settings::toggle_magicka_injury.GetValue() &&
			!a_actor->HasSpell(Config::Forms::injury_magicka)) {
			result.push_back(Config::Forms::InjuryType::kMagickaInjury);
		}

		return result;
	}

	/// <summary>
	/// may safely return nothing if the actor already has all possible injuries
	/// </summary>
	/// <param name="a_actor"></param>
	/// <returns></returns>
	std::optional<Config::Forms::InjuryType> Injuries::SelectInjury(RE::Actor* a_actor) const
	{
		auto possible = GetPossibleInjuries(a_actor);
		if (possible.empty()) {
			return std::nullopt;
		}
		const auto idx = RandomiserUtil::GetRandomInt(0, possible.size() - 1);
		return possible[idx];
	}

	/// <summary>
	/// Legacy function, not really needed anymore but in case i use it at some point, i left it.
	/// </summary>
	/// <param name="player"></param>
	/// <returns></returns>
	bool Injuries::CheckLadyStoneGoldAmount(RE::PlayerCharacter* player)
	{
		if (player->HasPerk(Config::Forms::lady_stone_perk))
		{
			RE::TESForm* const gold = RE::BGSDefaultObjectManager::GetSingleton()->GetObject(RE::DEFAULT_OBJECT::kGold);
			RE::TESBoundObject* goldBD = gold->As<RE::TESBoundObject>();

			float glob_val = Config::Forms::gold_tax_global->value;

			if (player->GetItemCount(goldBD) >= glob_val)
			{
				player->RemoveItem(goldBD, glob_val, RE::ITEM_REMOVE_REASON::kRemove, nullptr, nullptr, nullptr);
				RemoveAllInjuries(player);
				return false;
			}
		}
		return true;
	}

	void Injuries::ApplyStressOnDeath()
	{
		Stress::StressOnDeath::GetSingleton()->IncreaseStressLevel(Config::Settings::stress_increase_value.GetValue());
	}

	void Injuries::RemoveDeathStress()
	{
		Stress::StressOnDeath::GetSingleton()->DecreaseStressLevel(Config::Settings::stress_increase_value.GetValue());
	}

}