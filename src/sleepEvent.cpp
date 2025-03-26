#include "sleepEvent.h"
#include "injurybase.h"

#define contEve return RE::BSEventNotifyControl::kContinue

namespace SleepEvent
{
    SleepStopHandler *SleepStopHandler::GetSingleton()
    {
        static SleepStopHandler singleton{};
        return std::addressof(singleton);
    }

    RE::BSEventNotifyControl SleepStopHandler::ProcessEvent(const RE::TESSleepStopEvent *a_event, RE::BSTEventSource<RE::TESSleepStopEvent> *a_eventSource) noexcept
    {
        if (!a_event)
        {
            contEve;
        }
        if (a_event->interrupted)
        {
            contEve;
        }
        ProcessSleepStop();
        contEve;
    }

    void SleepStopHandler::RegisterSleepStop()
    {
        auto *eventSink = SleepStopHandler::GetSingleton();

        auto *eventSourceHolder = RE::ScriptEventSourceHolder::GetSingleton();
        eventSourceHolder->AddEventSink<RE::TESSleepStopEvent>(eventSink);

        logs::info("Registered <{}>", typeid(SleepStopHandler).name());
    }

    SleepStartHandler *SleepStartHandler::GetSingleton()
    {
        static SleepStartHandler singleton{};
        return std::addressof(singleton);
    }

    RE::BSEventNotifyControl SleepStartHandler::ProcessEvent(const RE::TESSleepStartEvent *a_event, RE::BSTEventSource<RE::TESSleepStartEvent> *a_eventSource) noexcept
    {
        if (!a_event)
        {
            contEve;
        }
        ProcessSleepStart();
        contEve;
    }

    void SleepStartHandler::RegisterSleepStart()
    {
        auto *eventSink = SleepStartHandler::GetSingleton();

        auto *eventSourceHolder = RE::ScriptEventSourceHolder::GetSingleton();
        eventSourceHolder->AddEventSink<RE::TESSleepStartEvent>(eventSink);
        logs::info("Registered <{}>", typeid(SleepStartHandler).name());
    }

    void InstallEvents()

    {
        SleepStopHandler::RegisterSleepStop();
        SleepStartHandler::RegisterSleepStart();
    }

    void ProcessSleepStart()

    {
        hours = RE::Calendar::GetSingleton()->GetHoursPassed();
        auto sleepCheck = SleepStopHandler::GetSingleton();
        sleepCheck->is_sleeping = true;
    }
    void ProcessSleepStop()
    {
        hours = RE::Calendar::GetSingleton()->GetHoursPassed() - hours;
        if (hours >= Settings::min_sleep_duration - 0.2)
        {
            RE::PlayerCharacter *player = RE::PlayerCharacter::GetSingleton();
            if (Utility::Locations::IsSafePlace(player->GetParentCell()))
            {
                auto injManager = Injuries::DeathInjury::GetSingleton();
                injManager->DeathInjury::RemoveAllExistingInjurySpells(player);
                injManager->DeathInjury::RemoveAttributePenalty(player);
                logs::debug("player slept for more than {} hours", Settings::min_sleep_duration);
            }
            else
            {
                logs::debug("not a safe area, can't heal injury here");
            }
        }
    }
}