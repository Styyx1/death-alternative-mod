#include "effectEvent.h"
#include "injurybase.h"

Effect::ApplyEffectEvent* Effect::ApplyEffectEvent::GetSingleton()
{
	static ApplyEffectEvent singleton{};
	return std::addressof(singleton);
}

RE::BSEventNotifyControl Effect::ApplyEffectEvent::ProcessEvent(const RE::TESMagicEffectApplyEvent* a_event, RE::BSTEventSource<RE::TESMagicEffectApplyEvent>* a_eventSource) noexcept
{
	if (!a_event) {
		return RE::BSEventNotifyControl::kContinue;
	}
	RE::PlayerCharacter* player = RE::PlayerCharacter::GetSingleton();

	auto effect = RE::TESForm::LookupByID<RE::EffectSetting>(a_event->magicEffect);
	if(!effect)	{
		return RE::BSEventNotifyControl::kContinue;
	}
	if (!effect->HasKeywordString("CureInjury")) {
		return RE::BSEventNotifyControl::kContinue;
	}
	auto injManager = Injuries::DeathInjury::GetSingleton();
	injManager->RemoveAttributePenalty(player);
	injManager->RemoveAllExistingInjurySpells(player);
	injManager->HealStressFromDeath();

	return RE::BSEventNotifyControl::kContinue;
}

void Effect::ApplyEffectEvent::RegisterApplyEffect()
{
	auto* eventSink = ApplyEffectEvent::GetSingleton();

	auto* eventSourceHolder = RE::ScriptEventSourceHolder::GetSingleton();
	eventSourceHolder->AddEventSink<RE::TESMagicEffectApplyEvent>(eventSink);

	logs::info("registered Apply Effect Event");
}

void Effect::InstallEvents()
{
	ApplyEffectEvent::RegisterApplyEffect();
}
