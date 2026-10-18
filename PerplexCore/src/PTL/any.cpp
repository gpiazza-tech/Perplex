#include <Perplex/pch.h>
#include <Perplex/PTL/any.h>

#include <nlohmann/json_fwd.hpp>

namespace Perplex::ptl
{
	void to_json(nlohmann::json& j, const any& val)
	{
		std::vector<std::byte> data{};
		std::byte* sourceBegin = (std::byte*)val.data();
		std::byte* sourceEnd = (std::byte*)val.data() + val.size();
		data.insert(data.end(), sourceBegin, sourceEnd);

		j["size"] = val.size();
		j["data"] = data;
	}

	void from_json(const nlohmann::json& j, any& val)
	{
		size_t size = j["size"];
		std::vector<std::byte> data = j["data"];

		val.set(data.data(), data.size());
	}
}