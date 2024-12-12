#pragma once
#include "injurybase.h"

namespace Serialisation
{
	static constexpr std::uint32_t SerializationVersion = 1;
	static constexpr std::uint32_t ID = 'SSOM';
	static constexpr std::uint32_t SerializationType = 'SOMI';


	inline void SaveCallback(SKSE::SerializationInterface* a_skse)
	{

		if (!a_skse->OpenRecord(SerializationType, SerializationVersion)) {
			logs::error("Failed to open shades of mortality record");
			return;
		} else {
			auto injManager = Injuries::DeathInjury::GetSingleton();
			auto penToSerialize = injManager->currentInjuryPenalty;
			auto boolToSave = injManager->injury_active;
			auto stressBoolToSave = injManager->can_apply_stress;

			if (!a_skse->WriteRecordData(penToSerialize)) {
				logs::error("Failed to write size of record data");
				return;
			}
			if (!a_skse->WriteRecordData(boolToSave)) {
				logs::error("Failed to write size of record data");
				return;
			}
			if (!a_skse->WriteRecordData(stressBoolToSave)) {
				logs::error("Failed to write size of record data");
				return;
			}
			else { 
				logs::info("Serialized: {}", std::to_string(penToSerialize));
				logs::info("Serialized: {}", boolToSave ? "true" : "false");
				logs::info("Serialized: {}", stressBoolToSave ? "true" : "false");
			}
		}
	}

	inline void LoadCallback(SKSE::SerializationInterface* a_skse)
	{
		std::uint32_t type;
		std::uint32_t version;
		std::uint32_t length;
		a_skse->GetNextRecordInfo(type, version, length);

		auto injManager = Injuries::DeathInjury::GetSingleton();

		if (type != SerializationType) {
			return;
		}

		if (version != SerializationVersion) {
			logs::error("Unable to load data");
			return;
		}

		float deserializedVal;
		bool deserializedBool;
		bool deserializedStressBool;
		if (!a_skse->ReadRecordData(deserializedVal)) {
			logs::error("Failed to load size");
			return;
		} 
		if (!a_skse->ReadRecordData(deserializedBool)) {
			logs::error("Failed to load size");
			return;
		}
		if (!a_skse->ReadRecordData(deserializedStressBool)) {
			logs::error("Failed to load size");
			return;
		}
		else {
			injManager->currentInjuryPenalty = deserializedVal;
			injManager->injury_active = deserializedBool;
			injManager->can_apply_stress = deserializedStressBool;
			logs::info("Deserialized: {}", std::to_string(deserializedVal));
			logs::info("Deserialized: {}", deserializedBool ? "true" : "false");
			logs::info("Deserialized: {}", deserializedStressBool ? "true" : "false");
		}
	}

	inline void RevertCallback([[maybe_unused]] SKSE::SerializationInterface* a_skse)
	{
		auto injManager = Injuries::DeathInjury::GetSingleton();
		injManager->currentInjuryPenalty = 0.0f;
		injManager->injury_active = false;
		injManager->injury_active = false;
	}
}
