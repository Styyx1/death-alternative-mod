#pragma once

#include "settings.h"

namespace Utility
{
    struct Actors
    {
        // https://github.com/powerof3/PapyrusExtenderSSE/
        inline static std::vector<RE::Actor*> GetNearbyActors(RE::TESObjectREFR* a_ref, float a_radius, bool a_ignorePlayer)
        {
            {
                std::vector<RE::Actor*> result;
                if (const auto processLists = RE::ProcessLists::GetSingleton(); processLists) {
                    if (a_ignorePlayer && processLists->numberHighActors == 0) {
                        logs::debug("no process list");
                        return result;
                    }

                    const auto squaredRadius = a_radius * a_radius;
                    const auto originPos     = a_ref->GetPosition();

                    result.reserve(processLists->numberHighActors);

                    const auto get_actor_within_radius = [&](RE::Actor* a_actor) {
                        if (a_actor && a_actor != a_ref && originPos.GetSquaredDistance(a_actor->GetPosition()) <= squaredRadius) {
                            result.emplace_back(a_actor);
                        }
                        };
                    for (auto& actorHandle : processLists->highActorHandles) {
                        const auto actor = actorHandle.get();
                        get_actor_within_radius(actor.get());
                    }

                    if (!a_ignorePlayer) {
                        get_actor_within_radius(RE::PlayerCharacter::GetSingleton());
                    }

                    if (!result.empty()) {
                        logs::debug("vector is not empty");
                        return result;
                    }
                }
                return result;
            }
        } 
    };

    //https://github.com/colinswrath/BladeAndBlunt/blob/main/include/Conditions.h
	struct Spells
	{
        

        inline static bool ActorHasActiveMagicEffect(RE::Actor* a_actor, RE::EffectSetting* a_effect)
        {

            auto               activeEffects = a_actor->AsMagicTarget()->GetActiveEffectList();
            RE::EffectSetting* setting       = nullptr;
            for (auto& effect : *activeEffects) {
                setting = effect ? effect->GetBaseObject() : nullptr;
                if (setting && !effect->flags.any(RE::ActiveEffect::Flag::kInactive)) {
                    if (setting == a_effect) {
                        return true;
                    }
                }
            }
            return false;
        }

        static bool IsPermanent(RE::MagicItem* item)
        {
            switch (item->GetSpellType()) {
            case RE::MagicSystem::SpellType::kDisease:
            case RE::MagicSystem::SpellType::kAbility:
            case RE::MagicSystem::SpellType::kAddiction: {
                return true;
            }
            default: {
                return false;
            }
            }
        }

        inline static void ApplySpell(RE::Actor* caster, RE::Actor* target, RE::SpellItem* spell)
        {
            if (IsPermanent(spell)) {
                target->AddSpell(spell);
            }
            else {
                caster->GetMagicCaster(RE::MagicSystem::CastingSource::kInstant)->CastSpellImmediate(spell, false, target, 1.0F, false, 0.0F, nullptr);
            }
        }
	};

    struct Checks
    {
        static bool IsSurvivalEnabled() {
            if (!Settings::survival_mode_active) {
                return false;
            }
            if (Settings::survival_mode_active->value == 1.0) {
                return true;
            } else {
                return false;
            }
        }
    };

}