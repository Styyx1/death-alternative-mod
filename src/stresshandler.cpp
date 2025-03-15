#include "stresshandler.h"
#include "settings.h"

StressHandler::StressApplication* StressHandler::StressApplication::GetSingleton()
{
	static StressApplication singleton{};
	return &singleton;
}

void StressHandler::StressApplication::IncreaseStress(float a_amount)
{
	Settings::stress_total_value->value += a_amount;
	CapStress(true);
}

void StressHandler::StressApplication::DecreaseStress(float a_amount)
{
	Settings::stress_total_value->value -= a_amount;
	CapStress(false);
}

void StressHandler::StressApplication::ApplyStressOnce()
{
	if (Settings::stress_enabled) {
		IncreaseStress(Settings::stress_increase_value);
		ShowStressMessage(Settings::stress_increase_text);
		stress_applied = true;
	}
	return;
}

void StressHandler::StressApplication::ReduceStress()
{
	if (Settings::stress_enabled) {
		DecreaseStress(Settings::stress_increase_value);
		ShowStressMessage(Settings::stress_decrease_text);
		stress_applied = false;
	}
}

void StressHandler::StressApplication::ShowStressMessage(std::string a_text)
{

	RE::DebugNotification(a_text.c_str(), nullptr, true);
}

void StressHandler::StressApplication::CapStress(bool a_upper)
{
	if (a_upper){
		if (Settings::stress_total_value->value > 100) {
			Settings::stress_total_value->value = 100.0f;
		}
	}
	else {
		if (Settings::stress_total_value->value < 1.0) {
			Settings::stress_total_value->value = 0.0f;
		}
	}
}

void StressHandler::StressApplication::IncreaseStressWithoutInjury(float a_amount)
{
	if (Settings::stress_enabled) {
		Settings::stress_total_value->value += a_amount;
		if (Settings::stress_total_value->value > 100) {
			Settings::stress_total_value->value = 100.0f;
		}

		if (Settings::stress_total_value->value < 1.0) {
			Settings::stress_total_value->value = 0.0f;
		}		
	}
	return;
}
