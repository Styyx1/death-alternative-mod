#pragma once
#include "settings.h"

namespace StressHandler
{
	class StressApplication
	{
		StressApplication() = default;
		StressApplication(const StressApplication&) = delete;
		StressApplication(StressApplication&&) = delete;
		StressApplication& operator=(const StressApplication&) = delete;
		StressApplication& operator=(StressApplication&&) = delete;

	public:
		static StressApplication* GetSingleton();

		bool stress_applied;

		void IncreaseStress(float a_amount);
		void DecreaseStress(float a_amount);
		void ApplyStressOnce();
		void ReduceStress();
		void ShowStressMessage(std::string a_text);
		void CapStress(bool a_upper);
		static void IncreaseStressWithoutInjury(float a_amount);
	};
}
