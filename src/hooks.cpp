#include "hooks.h"


void Hooks::InstallHooks()
{
	Hooks::PlayerPotionUsed::Install();
	logs::info("installed magic target hook");
}

void Hooks::PlayerPotionUsed::Install()
{
	REL::Relocation<uintptr_t> target{ REL::VariantID(39604, 40690, 0x6d7b00), REL::VariantOffset(0x15, 0x15, 0x15) };
	auto& trampoline = SKSE::GetTrampoline();
	_PlayerUsePotion = trampoline.write_call<5>(target.address(), PlayerUsePotion);
}

void Hooks::PlayerPotionUsed::PlayerUsePotion(uint64_t self, RE::AlchemyItem* alch, uint64_t extralist)
{
	if (alch->HasKeywordString("CureInjury")) {
		logs::info("Cure Injury effect detected");
		auto inj = Injuries::DeathInjury::GetSingleton();
		RE::PlayerCharacter* player = RE::PlayerCharacter::GetSingleton();
		inj->RemoveAttributePenalty(player);
		inj->RemoveAllExistingInjurySpells(player);
		inj->HealStressFromDeath();
	}
	return _PlayerUsePotion(self, alch, extralist);
}
