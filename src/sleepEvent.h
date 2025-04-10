#pragma once
#include "settings.h"

// Credits: https://github.com/colinswrath/Survival-Mode-Improved-SKSE
namespace SleepEvent
{
    static inline float hours;
    void InstallEvents();
    void ProcessSleepStart();
    void ProcessSleepStop();

    class SleepStartHandler final : public RE::BSTEventSink<RE::TESSleepStartEvent>
    {
        SleepStartHandler() = default;
        SleepStartHandler(const SleepStartHandler &) = delete;
        SleepStartHandler(SleepStartHandler &&) = delete;
        SleepStartHandler &operator=(const SleepStartHandler &) = delete;
        SleepStartHandler &operator=(SleepStartHandler &&) = delete;

    public:
        static SleepStartHandler *GetSingleton();

        RE::BSEventNotifyControl ProcessEvent(const RE::TESSleepStartEvent *a_event, RE::BSTEventSource<RE::TESSleepStartEvent> *a_eventSource) noexcept override;
        static void RegisterSleepStart();
    };

    class SleepStopHandler final : public RE::BSTEventSink<RE::TESSleepStopEvent>
    {
        SleepStopHandler() = default;
        SleepStopHandler(const SleepStopHandler &) = delete;
        SleepStopHandler(SleepStopHandler &&) = delete;
        SleepStopHandler &operator=(const SleepStopHandler &) = delete;
        SleepStopHandler &operator=(SleepStopHandler &&) = delete;

    public:
        static SleepStopHandler *GetSingleton();

        inline static bool is_sleeping;

        RE::BSEventNotifyControl ProcessEvent(const RE::TESSleepStopEvent *a_event, RE::BSTEventSource<RE::TESSleepStopEvent> *a_eventSource) noexcept override;
        static void RegisterSleepStop();
    };
} // namespace Events