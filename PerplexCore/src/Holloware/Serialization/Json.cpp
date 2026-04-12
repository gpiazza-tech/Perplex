#include <pch.h>
#include "Json.h"

#include <Holloware/Core/Log.h>

#include <nlohmann/json_fwd.hpp>

#include <filesystem>
#include <fstream>

namespace Holloware
{
	void JsonHelper::WriteToFile(const nlohmann::json& json, const std::filesystem::path& path)
	{
		std::ofstream output(path.string());
		output << json.dump(1);
		output.close();
	}

	nlohmann::json JsonHelper::LoadFromFile(const std::filesystem::path& path)
	{
		std::ifstream jsonFile(path.string());
		if (!jsonFile)
			HW_CORE_ERROR("file {0} does not exist!", path.string());

		nlohmann::json json = nlohmann::json::parse(jsonFile);
		jsonFile.close();

		return json;
	}
}