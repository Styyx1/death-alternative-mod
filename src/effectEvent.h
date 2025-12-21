#pragma once

namespace Effect
{
    using RES = RE::BSEventNotifyControl;

    void InstallEvents();

    struct EffectApplyEvent : REX::Singleton<EffectApplyEvent>, RE::BSTEventSink<RE::TESMagicEffectApplyEvent> {
        void RegisterEffect();
    private:
        RES ProcessEvent(const RE::TESMagicEffectApplyEvent* a_event, RE::BSTEventSource<RE::TESMagicEffectApplyEvent>*) override;
    };
}
