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
		RE::PlayerCharacter *player = RE::PlayerCharacter::GetSingleton();
		Injuries::DeathInjury::RemoveAttributePenalty(player);
		Injuries::DeathInjury::RemoveAllExistingInjurySpells(player);
		Injuries::DeathInjury::HealStressFromDeath();
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
	if (Injuries::DeathInjury::injury_active && Settings::use_health_injury)
	{
		Injuries::DeathInjury::SetAttributePenaltyUIGlobal(Settings::injury_health_decrease / 100);
	}
	_func(a_this, a_delta);
}
