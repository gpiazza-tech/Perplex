#include <Perplex/pch.h>
#include <Perplex/Assets/ScriptAssetImporter.h>

#include <Perplex/Core/Core.h>
#include <Perplex/Scripting/Interpreter.h>
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

		data["source"] = Interpreter::TrimProperties(oss.str());
		data["properties"] = Interpreter::FindProperties(oss.str());

		return data;
	}

	Ref<void> ScriptAssetImporter::Load(const fs::path& path)
	{
		nlohmann::json data = JsonHelper::LoadFromFile(path.string().append(".meta"))["data"];

		ScriptData scriptData = ScriptData();
		scriptData.Source = data["source"].get<std::string>();
		scriptData.Properties = data["properties"].get<std::vector<ScriptProperty>>();

		return CreateRef<ScriptData>(scriptData);
	}
}