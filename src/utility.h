#pragma once

#include "settings.h"
#include "cache.h"

namespace Utility
{
    struct Actors
    {
        //Credits: https://github.com/powerof3/PapyrusExtenderSSE/
        inline static std::vector<RE::Actor*> GetNearbyActors(RE::TESObjectREFR* a_ref, float a_radius, bool a_ignorePlayer)
        {
            {
                std::vector<RE::Actor*> result;
                if (const auto processLists = RE::ProcessLists::GetSingleton(); processLists) {
                    if (a_ignorePlayer && processLists->numberHighActors == 0) {
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
                        return result;
                    }
                }
                return result;
            }
        } 
    };

    //Credits: https://github.com/colinswrath/BladeAndBlunt/blob/main/include/Conditions.h
	struct Spells
	{

        // https://github.com/colinswrath/BladeAndBlunt/blob/main/include/Conditions.h
        static bool HasSpell(RE::Actor *actor, RE::SpellItem *spell)
        {
            using func_t = decltype(&Utility::Spells::HasSpell);

            REL::Relocation<func_t> func{Cache::HasSpellAddress};

            return func(actor, spell);
        }
        inline static REL::Relocation<decltype(HasSpell)> _HasSpell;

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
    struct Locations
    {
        inline static const std::vector<std::string> safe_areas_diff2
        {
            "LocTypeGuild",
            "LocTypeInn",
            "LocTypePlayerHouse"        
        };

        inline static const std::vector<std::string> unsafe_areas_diff1
        {
            "LocTypeAnimalDen",
            "LocTypeBanditCamp",
            "LocTypeDraugrCrypt",
            "LocTypeDungeon",
            "LocTypeDwarvenAutomatons",
            "LocTypeFalmerHive",
            "LocTypeForswornCamp",
            "LocTypeGiantCamp",
            "LocTypeShipwreck",
            "LocTypeShip",
            "LocTypeSprigganGrove",
            "LocTypeVampireLair",
            "LocTypeWarlockLair",
            "LocTypeWerewolfLair"
        };

        inline static bool IsSafePlace(RE::TESObjectCELL* cell) {
            if (cell) {
                auto location = cell->GetLocation();
                if (location) {
                    switch (Settings::sleep_location_difficulty) {
                    case 0:
                        return true;
                    case 1:
                        return !location->HasAnyKeywordByEditorID(unsafe_areas_diff1);
                    case 2:
                        return location->HasAnyKeywordByEditorID(safe_areas_diff2);
                    default:
                        return false;
                    }
                }
                 
            }                       
            return true;
        }
    };

}