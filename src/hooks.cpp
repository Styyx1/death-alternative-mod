#include "hooks.h"

void Hooks::InstallHooks()
{
	Hooks::PlayerPotionUsed::Install();
	Hooks::PlayerUpdate::Install();
	logs::info("installed hooks");
}

void Hooks::PlayerPotionUsed::Install()
{
	REL::Relocation<uintptr_t> target{REL::VariantID(39604, 40690, 0x6d7b00), REL::VariantOffset(0x15, 0x15, 0x15)};
	auto &trampoline = SKSE::GetTrampoline();
	_PlayerUsePotion = trampoline.write_call<5>(target.address(), PlayerUsePotion);
	logs::info("Installed <{}>", typeid(PlayerPotionUsed).name());
}

void Hooks::PlayerPotionUsed::PlayerUsePotion(uint64_t self, RE::AlchemyItem *alch, uint64_t extralist)
{
	if (alch->HasKeywordString("CureInjury"))
	{
		auto injManager = Injuries::DeathInjury::GetSingleton();
		RE::PlayerCharacter *player = RE::PlayerCharacter::GetSingleton();
		injManager->RemoveAttributePenalty(player);
		injManager->RemoveAllExistingInjurySpells(player);
		injManager->HealStressFromDeath();
	}
	return _PlayerUsePotion(self, alch, extralist);
}

void Hooks::PlayerUpdate::Install()
{
	REL::Relocation<std::uintptr_t> PlayerVTBL{RE::VTABLE_PlayerCharacter[0]};
	_func = PlayerVTBL.write_vfunc(0xAD, PlayerUpdateHook);
	logs::info("Installed <{}>", typeid(PlayerUpdate).name());
}

void Hooks::PlayerUpdate::PlayerUpdateHook(RE::PlayerCharacter *a_this, float a_delta)
{
	auto injManager = Injuries::DeathInjury::GetSingleton();
	if (injManager->injury_active && injManager->currentInjuryPenalty != 0 && Settings::use_health_injury)
	{
		injManager->SetAttributePenaltyUIGlobal(Settings::injury_health_decrease / 100);
	}
	_func(a_this, a_delta);
}
