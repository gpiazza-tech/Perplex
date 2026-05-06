#include "pch.h"
#include "ScriptAssetImporter.h"

#include "Holloware/Core/Core.h"
#include "Holloware/Scripting/Interpreter.h"
#include "Holloware/Scripting/ScriptData.h"
#include "Holloware/Scripting/ScriptProperty.h"
#include "Holloware/Serialization/Json.h"

#include <nlohmann/json_fwd.hpp>
#include <nlohmann/json.hpp>

#include <sstream>
#include <fstream>
#include <string>
#include <vector>
#include <filesystem>

namespace Holloware
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