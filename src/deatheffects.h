#pragma once

namespace DeathEffects
{
	class Ethereal
	{
	public:
		static void SetEthereal(RE::Actor* a_actor);
		static void ProcessNPCDeath(RE::Actor* a_actor);
		static void RemoveGoldPlayer(RE::PlayerCharacter* player, float a_percentage);

		// Credits: https://github.com/fenix31415/UselessFenixUtils
		static uint32_t get_count_(RE::InventoryChanges* changes, RE::TESBoundObject* item)
		{
			using func_t = decltype(&get_count_);
			REL::Relocation<func_t> func{ REL::RelocationID(15868, 16108) };
			return func(changes, item);
		}

		static uint32_t get_count(RE::TESObjectREFR* refr, RE::TESBoundObject* item)
		{
			auto changes = refr->GetInventoryChanges();
			return get_count_(changes, item);
		}
	};
}
