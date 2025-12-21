#pragma once

// Credits: https://github.com/colinswrath/Survival-Mode-Improved-SKSE

/// <summary>
/// These Events are only used to cure death injuries after x hours of sleep
/// SleepStart event is basically undefined in Clib, but it exists and i only need to check when it happens and don't need anything from the event 
/// </summary>
namespace SleepEv {

    using RES = RE::BSEventNotifyControl;

    inline float sleep_hours;
    inline bool is_sleeping;

    void InstallSleep();
    
    struct SleepStartEvent final : REX::Singleton<SleepStartEvent>, RE::BSTEventSink<RE::TESSleepStartEvent> {
        void Install();
    private:
        void ProcessSleepStart();
        RES ProcessEvent(const RE::TESSleepStartEvent* a_event, RE::BSTEventSource<RE::TESSleepStartEvent>*) override;
    };
    struct SleepStopEvent final : REX::Singleton<SleepStopEvent>, RE::BSTEventSink<RE::TESSleepStopEvent> {
        void Install();
    private:
        void ProcessSleepStop();
        RES ProcessEvent(const RE::TESSleepStopEvent* a_event, RE::BSTEventSource<RE::TESSleepStopEvent>*) override;
    };

}