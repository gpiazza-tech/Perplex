#pragma once

#include <nlohmann/json_fwd.hpp>

#include <xhash>
#include <cstdint>
#include <functional>

namespace Holloware
{
	class UUID
	{
	public:
		UUID();
		UUID(uint64_t uuid);
		UUID(const UUID& other) = default;

		operator uint64_t() const { return m_UUID; };

		friend void to_json(nlohmann::json& json, const UUID& uuid);
		friend void from_json(const nlohmann::json& json, UUID& uuid);
	private:
		uint64_t m_UUID;
	};
}

namespace std
{
	template<>
	struct hash<Holloware::UUID>
	{
		std::size_t operator()(const Holloware::UUID& uuid) const
		{
			return hash<uint64_t>()((uint64_t)uuid);
		}
	};
}