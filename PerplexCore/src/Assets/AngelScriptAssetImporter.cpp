#include <Perplex/pch.h>
#include <Perplex/Assets/AngelScriptAssetImporter.h>

#include <Perplex/Core/Core.h>
#include <Perplex/Core/Application.h>
#include <Perplex/Serialization/JsonHelper.h>
#include <Perplex/Angel/AngelModule.h>

#include <nlohmann/json_fwd.hpp>
#include <nlohmann/json.hpp>

#include <sstream>
#include <fstream>
#include <string>
#include <filesystem>

namespace Perplex
{
	namespace fs = std::filesystem;

	nlohmann::json AngelScriptAssetImporter::Import(const std::filesystem::path& path)
	{
		nlohmann::json data = nlohmann::json();

		std::ifstream ifs(path.string());
		std::ostringstream oss;
		oss << ifs.rdbuf(); 

		fs::path filename = path.filename();

		data["sourceName"] = filename.stem();
		data["source"] = oss.str();

		return data;
	}

	Ref<void> AngelScriptAssetImporter::Load(const fs::path& path)
	{
		nlohmann::json data = JsonHelper::LoadFromFile(path.string().append(".meta"))["data"];

		std::string sourceName = data["sourceName"].get<std::string>();
		std::string source = data["source"].get<std::string>();

		AngelModule mod = Application::Get().GetAngelEngine().CreateModule(sourceName, source);
		return CreateRef<AngelModule>(mod);
	}
}