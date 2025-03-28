#include "hooks.h"

static float lastTime;

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
		injManager->RemoveAllExistingInjurySpells(player);
		injManager->HealStressFromDeath();
	}
	return _PlayerUsePotion(self, alch, extralist);
}

void Hooks::PlayerUpdate::Install()
{
	auto &trampoline = SKSE::GetTrampoline();
	_func = trampoline.write_call<5>(OnFrame_Update_Hook.address(), PlayerUpdateHook);
	logs::info("Installed <{}>", typeid(PlayerUpdate).name());
}

void Hooks::PlayerUpdate::PlayerUpdateHook(std::int64_t a1)
{

	if (!Cache::GetUISingleton()->GameIsPaused())
	{

		if (Cache::g_deltaTime > 0)
		{
			lastTime += Cache::g_deltaTime;
			if (lastTime >= 0.5f)
			{
				RE::PlayerCharacter *player = Cache::GetPlayerSingleton();
				auto inj = Injuries::DeathInjury::GetSingleton();
				inj->CheckInjuryAvPenalty(player);
				lastTime = 0;
			}
		}
	}
	return _func(a1);
}
