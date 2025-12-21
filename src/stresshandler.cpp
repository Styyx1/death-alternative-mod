#include "stresshandler.h"
#include "settings.h"
#include "mod-data.h"

namespace Stress {

using namespace MOD;

	void StressOnDeath::IncreaseStressLevel(float amount)
	{
		using namespace Config;

		if (!CanApplyStress())
			return;

		const float before = Forms::stress_total_value->value;
		Forms::stress_total_value->value += amount;
		CapStressLevel();
		if (Forms::stress_total_value->value != before) {
			stress_message_increase = Config::Settings::stress_increase_message.GetValue();
			ProcessStressMessage(stress_message_increase.c_str());
		}
	} 
	void StressOnDeath::DecreaseStressLevel(float amount)
	{
		using namespace Config;

		if (!CanApplyStress())
			return;
		const float before = Forms::stress_total_value->value;
		Forms::stress_total_value->value -= amount;
		CapStressLevel();
		if (Forms::stress_total_value->value != before) {
			stress_message_decrease = Config::Settings::stress_decrease_message.GetValue();
			ProcessStressMessage(stress_message_decrease.c_str());
		}
	}
	bool StressOnDeath::CanApplyStress() const
	{
		using namespace Config;

		if (!Forms::stress_enabled || !Forms::stress_total_value) {
			return false;
		}
		if (Forms::stress_enabled->value <= 0.0f) {
			return false;
		}		
		return true;
	}
	void StressOnDeath::CapStressLevel() const
	{
		using namespace Config;

		if (Forms::stress_total_value->value >= MAX_STRESS) {
			Forms::stress_total_value->value = MAX_STRESS;
			return;
		}
		if (Forms::stress_total_value->value < MIN_STRESS) {
			Forms::stress_total_value->value = MIN_STRESS;
			return;
		}
	}
	void StressOnDeath::ProcessStressMessage(const char* a_message)
	{
		if(Config::Settings::show_stress_notif.GetValue())
			RE::SendHUDMessage::ShowHUDMessage(a_message);
	}
}