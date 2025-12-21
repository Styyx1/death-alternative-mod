#include "sleepEvent.h"
#include "deatheffects.h"
#include "settings.h"
#include "cache.h"
#include "utility.h"


namespace SleepEv {
    void InstallSleep()
    {
        SleepStartEvent::GetSingleton()->Install();
        SleepStopEvent::GetSingleton()->Install();
    }
    void SleepStartEvent::Install()
    {
        if (RE::ScriptEventSourceHolder* const eventsrc = RE::ScriptEventSourceHolder::GetSingleton(); eventsrc) {
            eventsrc->AddEventSink<RE::TESSleepStartEvent>(this);
            REX::INFO("Registered <{}>", typeid(SleepStartEvent).name());
        } 
    }

    void SleepStartEvent::ProcessSleepStart() {
        sleep_hours = RE::Calendar::GetSingleton()->GetHoursPassed();
        is_sleeping = true;
    }

    RES SleepStartEvent::ProcessEvent(const RE::TESSleepStartEvent* a_event, RE::BSTEventSource<RE::TESSleepStartEvent>*)
    {
        if (!a_event) {
            return RES::kContinue;
        }
        ProcessSleepStart();
        return RES::kContinue;
    }

    void SleepStopEvent::Install() {

        if (const auto eventsrc = RE::ScriptEventSourceHolder::GetSingleton(); eventsrc) {
            eventsrc->AddEventSink<RE::TESSleepStopEvent>(this);
            REX::INFO("Registered <{}>", typeid(RE::TESSleepStopEvent).name());
        } 
    }

    void SleepStopEvent::ProcessSleepStop() {
        sleep_hours = RE::Calendar::GetSingleton()->GetHoursPassed() - sleep_hours;
        if (sleep_hours >= Config::Settings::min_sleep_duration - 0.2)
        {
            RE::PlayerCharacter* player = Cache::GetPlayerSingleton();
            if (Utility::Locations::IsSafePlace(player->GetParentCell()))
            {

                const auto injury_manager = OnDeath::Injuries::GetSingleton();
                injury_manager->RemoveAllInjuries(player);
                REX::DEBUG("player slept for more than {} hours", Config::Settings::min_sleep_duration);
            }
            else
            {
                REX::DEBUG("not a safe area, can't heal injury here");
            }
        }
    }

    RES SleepStopEvent::ProcessEvent(const RE::TESSleepStopEvent* a_event, RE::BSTEventSource<RE::TESSleepStopEvent>*)
    {
        if(!a_event)
            return RES::kContinue;
        if(a_event->interrupted)
            return RES::kContinue;

        ProcessSleepStop();
        return RES::kContinue;
    }
}