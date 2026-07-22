#include <Perplex/pch.h>
#include <Perplex/Assets/ScriptAssetImporter.h>

#include <Perplex/Core/Core.h>
#include <Perplex/Scripting/CPreprocessor.h>
#include <Perplex/Scripting/ScriptData.h>
#include <Perplex/Scripting/ScriptProperty.h>
#include <Perplex/Serialization/JsonHelper.h>

#include <nlohmann/json_fwd.hpp>
#include <nlohmann/json.hpp>

#include <sstream>
#include <fstream>
#include <string>
#include <vector>
#include <filesystem>

namespace Perplex
{
	namespace fs = std::filesystem;

	nlohmann::json ScriptAssetImporter::Import(const std::filesystem::path& path)
	{
		nlohmann::json data = nlohmann::json();

		std::ifstream ifs(path.string());
		std::ostringstream oss;
		oss << ifs.rdbuf();

		fs::path filename = path.filename();

		data["sourceName"] = filename.string();
		data["source"] = TrimProperties(oss.str());
		data["properties"] = FindProperties(oss.str());

		return data;
	}

	Ref<void> ScriptAssetImporter::Load(const fs::path& path)
	{
		nlohmann::json data = JsonHelper::LoadFromFile(path.string().append(".meta"))["data"];

		ScriptData scriptData = ScriptData();
		scriptData.SourceName = data["sourceName"].get<std::string>();
		scriptData.Source = data["source"].get<std::string>();
		scriptData.Properties = data["properties"].get<std::vector<ScriptProperty>>();

		return CreateRef<ScriptData>(scriptData);
	}
}