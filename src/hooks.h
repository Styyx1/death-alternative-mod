#pragma once

namespace Hooks
{
	struct PlayerPotion {
	private:
		static void PlayerUsePotion(RE::PlayerCharacter* a_this, RE::AlchemyItem* alch, RE::ExtraDataList* extra_list);
		static inline REL::Hook _PlayerPotion{REL::ID(40690), 0x15, PlayerUsePotion};
	};
}
