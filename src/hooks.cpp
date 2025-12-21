#include "hooks.h"
#include "deatheffects.h"

static float lastTime;

namespace Hooks {
	void PlayerPotion::PlayerUsePotion(RE::PlayerCharacter* a_this, RE::AlchemyItem* alch, RE::ExtraDataList* extra_list)
	{
		if (alch->HasKeywordString("CureInjury"))
		{
			const auto injury_manager = OnDeath::Injuries::GetSingleton();
			injury_manager->RemoveAllInjuries(a_this);
		}
		return _PlayerPotion(a_this, alch, extra_list);
	}
}

