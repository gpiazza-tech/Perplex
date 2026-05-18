#include <Perplex/pch.h>
#include <Perplex/Serialization/Json.h>

#include <Perplex/Core/Log.h>

#include <nlohmann/json_fwd.hpp>
#include <nlohmann/json.hpp>

#include <array>
#include <cstddef>
#include <filesystem>
#include <fstream>

namespace Perplex
{
	namespace fs = std::filesystem;

	Json::Json(const fs::path& pathToLoad)
	{
		std::ifstream jsonFile(pathToLoad.string());
		if (!jsonFile)
		{
			HW_CORE_INFO("file {0} does not exist: returning emtpy Json", pathToLoad.string());
			return;
		}

		nlohmann::json json = nlohmann::json::parse(jsonFile);
		jsonFile.close();

		m_Json = json;
	}

	void Json::WriteToFile(const fs::path& path)
	{
		std::ofstream output(path.string());
		output << m_Json.dump(1);
		output.close();
	}
}