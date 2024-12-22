#include "sleepEvent.h"
#include "injurybase.h"


#define contEve return RE::BSEventNotifyControl::kContinue

namespace SleepEvent
{

    RE::BSEventNotifyControl SleepEventHandler::ProcessEvent(const RE::TESSleepStopEvent* a_event, RE::BSTEventSource<RE::TESSleepStopEvent>* a_eventSource) noexcept
    {
        if (!a_event) {
            contEve;
        }
        if (a_event->interrupted) {
            contEve;
        }
        ProcessSleepStop();
        contEve;
    }

    void SleepEventHandler::RegisterSleepStop()
    {
        auto* eventSink = SleepEventHandler::GetSingleton();

        auto* eventSourceHolder = RE::ScriptEventSourceHolder::GetSingleton();
        eventSourceHolder->AddEventSink<RE::TESSleepStopEvent>(eventSink);

        logs::info("registered Sleep Stop Event");

    }

    RE::BSEventNotifyControl SleepStartHandler::ProcessEvent(const RE::TESSleepStartEvent* a_event, RE::BSTEventSource<RE::TESSleepStartEvent>* a_eventSource) noexcept
    {
        if (!a_event) {
            contEve;
        }
        ProcessSleepStart();
        contEve;
    }

    void SleepStartHandler::RegisterSleepStart()
    {
        auto* eventSink = SleepStartHandler::GetSingleton();

        auto* eventSourceHolder = RE::ScriptEventSourceHolder::GetSingleton();
        eventSourceHolder->AddEventSink<RE::TESSleepStartEvent>(eventSink);
        logs::info("registered Sleep Start Event");
    }

    void InstallEvents()

    {
        SleepEventHandler::RegisterSleepStop();
        SleepStartHandler::RegisterSleepStart();
    }

    void ProcessSleepStart()

    {
        hours = RE::Calendar::GetSingleton()->GetHoursPassed();
        auto sleepCheck = SleepEventHandler::GetSingleton();
        sleepCheck->is_sleeping = true;
    }
    void ProcessSleepStop()
    {
        hours = RE::Calendar::GetSingleton()->GetHoursPassed() - hours;
        if (hours >= Settings::min_sleep_duration) {
            auto* injuryBase = Injuries::DeathInjury::GetSingleton();
            RE::PlayerCharacter* player = RE::PlayerCharacter::GetSingleton();
            injuryBase->RemoveAllExistingInjurySpells(player);
            injuryBase->RemoveAttributePenalty(player);
            logs::debug("player slept for more than {} hours", Settings::min_sleep_duration);
        }
    }
}