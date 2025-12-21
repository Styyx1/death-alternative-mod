#pragma once

#include "settings.h"
#include "cache.h"

namespace Utility
{
    struct Locations
    {
        inline static const std::vector<std::string> safe_areas_difficulty_2
        {
            "LocTypeTemple",
            "LocTypeGuild",
            "LocTypeInn",
            "LocTypePlayerHouse"        
        };

        inline static const std::vector<std::string> unsafe_areas_difficulty_1
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

        enum class LocDifficulty : int {
            kOnlyInnAndPlayerHouse = 2,
            kNonDungeons = 1,
            kEverywhere = 0,
        };

        inline static LocDifficulty GetDifficultySetting() {
            const int32_t val = Config::Settings::sleep_location_difficulty.GetValue();
            switch (val) {
            case 2: 
                return LocDifficulty::kOnlyInnAndPlayerHouse;
            case 1: 
                return LocDifficulty::kNonDungeons;
            case 0: 
                return LocDifficulty::kEverywhere;
            default:
                REX::INFO("Invalid sleep_location_difficulty setting: {}, defaulting to Everywhere", val);
                return LocDifficulty::kEverywhere;
            }
        }

        inline static bool IsSafePlace(RE::TESObjectCELL* cell) {

            if (!cell)
                return false;

            const auto loc = cell->GetLocation();
            if (!loc)
                return true;
            //if cell and location fail, let the player cure injury regarless

            const auto diff = GetDifficultySetting();
            switch (diff) {
            case LocDifficulty::kEverywhere:
                return true;
            case LocDifficulty::kNonDungeons:
                return !loc->HasAnyKeywordByEditorID(unsafe_areas_difficulty_1);
            case LocDifficulty::kOnlyInnAndPlayerHouse:
                return loc->HasAnyKeywordByEditorID(safe_areas_difficulty_2);
            }                 
                                  
            return true;
        }
    };

}