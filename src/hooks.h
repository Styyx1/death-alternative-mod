#pragma once
#include "injurybase.h"

namespace Hooks
{
	void InstallHooks();
	//Credits: https://github.com/muenchk/NPCsUsePotions/blob/3256f95831a84acccccf53802364e1edb9953e80/include/Hooks.h#L7
	struct PlayerPotionUsed {

		static void Install();

	private:
		static void PlayerUsePotion(uint64_t self, RE::AlchemyItem* alch, uint64_t extralist);
		static inline REL::Relocation<decltype(&PlayerUsePotion)> _PlayerUsePotion;

	};

	struct PlayerUpdate {
		static void Install();

	private:
		static void PlayerUpdateHook(RE::PlayerCharacter* a_this, float a_delta);
		static inline float frameCount = 0;
		static inline REL::Relocation<decltype(&PlayerUpdateHook)> _func;
	};
}
