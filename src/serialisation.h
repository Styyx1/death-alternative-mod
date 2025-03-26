#pragma once
#include "injurybase.h"
// Credits: https://github.com/colinswrath/BladeAndBlunt/blob/main/include/Serialization.h
namespace Serialisation
{
	static constexpr std::uint32_t SerializationVersion = 1;
	static constexpr std::uint32_t ID = 'SSOM';
	static constexpr std::uint32_t SerializationType = 'SOMI';

	inline void SaveCallback(SKSE::SerializationInterface *a_skse)
	{

		if (!a_skse->OpenRecord(SerializationType, SerializationVersion))
		{
			logs::error("Failed to open shades of mortality record");
			return;
		}
		else
		{
			auto injManager = Injuries::DeathInjury::GetSingleton();
			auto penToSerialize = injManager->currentInjuryPenalty;
			auto stamToSerialize = injManager->currentStamRatePen;
			auto magToSerialize = injManager->currentMagRatePen;
			auto boolToSave = injManager->injury_active;
			auto numInjuries = injManager->injuryCount;

			if (!a_skse->WriteRecordData(penToSerialize))
			{
				logs::error("Failed to write size of record data");
				return;
			}

			if (!a_skse->WriteRecordData(stamToSerialize))
			{
				logs::error("Failed to write size of record data");
				return;
			}

			if (!a_skse->WriteRecordData(magToSerialize))
			{
				logs::error("Failed to write size of record data");
				return;
			}

			if (!a_skse->WriteRecordData(boolToSave))
			{
				logs::error("Failed to write size of record data");
				return;
			}
			if (!a_skse->WriteRecordData(numInjuries))
			{
				logs::error("Failed to write size of record data");
				return;
			}
		}
	}

	inline void LoadCallback(SKSE::SerializationInterface *a_skse)
	{
		std::uint32_t type;
		std::uint32_t version;
		std::uint32_t length;
		a_skse->GetNextRecordInfo(type, version, length);

		if (type != SerializationType)
		{
			return;
		}

		if (version != SerializationVersion)
		{
			logs::error("Unable to load data");
			return;
		}

		float deserializedVal;
		float deserializedStamR;
		float deserializedMagR;
		bool deserializedBool;
		bool deserializedStressBool;
		std::int32_t deserializedInjuryCount;

		if (!a_skse->ReadRecordData(deserializedVal))
		{
			logs::error("Failed to load size");
			return;
		}

		if (!a_skse->ReadRecordData(deserializedStamR))
		{
			logs::error("Failed to load size");
			return;
		}

		if (!a_skse->ReadRecordData(deserializedMagR))
		{
			logs::error("Failed to load size");
			return;
		}

		if (!a_skse->ReadRecordData(deserializedBool))
		{
			logs::error("Failed to load size");
			return;
		}
		if (!a_skse->ReadRecordData(deserializedInjuryCount))
		{
			logs::error("Failed to load size");
			return;
		}
		else
		{
			auto injManager = Injuries::DeathInjury::GetSingleton();
			injManager->currentInjuryPenalty = deserializedVal;
			injManager->currentStamRatePen = deserializedStamR;
			injManager->currentMagRatePen = deserializedMagR;
			injManager->injury_active = deserializedBool;
			injManager->injuryCount = deserializedInjuryCount;
		}
	}

	inline void RevertCallback([[maybe_unused]] SKSE::SerializationInterface *a_skse)
	{
		auto injManager = Injuries::DeathInjury::GetSingleton();
		injManager->currentInjuryPenalty = 0.0f;
		injManager->currentInjuryPenalty = 0.0f;
		injManager->currentStamRatePen = 0.0f;
		injManager->injury_active = false;
		injManager->injuryCount = 0;
	}
}
