#pragma once

namespace Effect
{

    void InstallEvents();


    class ApplyEffectEvent final : public RE::BSTEventSink<RE::TESMagicEffectApplyEvent>
    {
        ApplyEffectEvent() = default;
        ApplyEffectEvent(const ApplyEffectEvent&) = delete;
        ApplyEffectEvent(ApplyEffectEvent&&) = delete;
        ApplyEffectEvent& operator=(const ApplyEffectEvent&) = delete;
        ApplyEffectEvent& operator=(ApplyEffectEvent&&) = delete;

    public:

        static ApplyEffectEvent* GetSingleton() {
            static ApplyEffectEvent singleton;
            return &singleton;
        }

        RE::BSEventNotifyControl ProcessEvent(const RE::TESMagicEffectApplyEvent* a_event, RE::BSTEventSource<RE::TESMagicEffectApplyEvent>* a_eventSource) noexcept override;
        static void RegisterApplyEffect();
    };
}
