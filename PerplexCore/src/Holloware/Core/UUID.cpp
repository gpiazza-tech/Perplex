#include "pch.h"
#include "UUID.h"

#include "Holloware/Serialization/Json.h"

#include <nlohmann/json.hpp>
#include <random>

namespace Holloware
{
	static std::random_device s_RandomDevice;
	static std::mt19937_64 s_Engine(s_RandomDevice());
	static std::uniform_int_distribution<uint64_t> s_UniformDistribution;

	UUID::UUID()
		: m_UUID(s_UniformDistribution(s_Engine))
	{ }

	UUID::UUID(uint64_t uuid)
		: m_UUID(uuid)
	{ }

	void to_json(nlohmann::json& json, const UUID& uuid) { json = uuid.m_UUID; }
	void from_json(const nlohmann::json& json, UUID& uuid) { uuid.m_UUID = json.get<uint64_t>(); }
}