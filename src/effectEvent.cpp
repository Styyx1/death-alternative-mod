#include "effectEvent.h"
#include "deatheffects.h"

namespace Effect {
	void InstallEvents()
	{
		EffectApplyEvent::GetSingleton()->RegisterEffect();
	}

	void EffectApplyEvent::RegisterEffect()
	{
		if (const auto eventsrc = RE::ScriptEventSourceHolder::GetSingleton(); eventsrc) {
			eventsrc->AddEventSink<RE::TESMagicEffectApplyEvent>(this);
			REX::INFO("Registered for {}", typeid(RE::TESMagicEffectApplyEvent).name());
		}
	}

	RES EffectApplyEvent::ProcessEvent(const RE::TESMagicEffectApplyEvent* a_event, RE::BSTEventSource<RE::TESMagicEffectApplyEvent>*)
	{
		if (!a_event) {
			return RES::kContinue;
		}
		auto effect = RE::TESForm::LookupByID<RE::EffectSetting>(a_event->magicEffect);
		if (!effect) {
			return RES::kContinue;
		}
		if (!effect->HasKeywordString("CureInjury")) {
			return RES::kContinue;
		}
		auto targ = a_event->target.get();
		if (!targ) {
			return RES::kContinue;
		}
		auto actor = targ->As<RE::Actor>();
		if (!actor) {
			return RES::kContinue;
		}
		const auto injury_manager = OnDeath::Injuries::GetSingleton();
		injury_manager->RemoveAllInjuries(actor);

		return RES::kContinue;
	}
}

