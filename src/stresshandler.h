#pragma once

namespace Stress {
	

	struct StressOnDeath final : REX::Singleton<StressOnDeath>{

		void IncreaseStressLevel(float amount);
		void DecreaseStressLevel(float amount);
	private:
		bool CanApplyStress() const;
		void CapStressLevel() const;		
		void ProcessStressMessage(const char* a_message);
		std::string stress_message_increase{};
		std::string stress_message_decrease{};
	};
}

