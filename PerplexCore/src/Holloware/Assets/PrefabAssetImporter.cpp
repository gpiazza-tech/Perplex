#include <pch.h>
#include "PrefabAssetImporter.h"

#include <Holloware/Core/Core.h>
#include <Holloware/Serialization/Json.h>

#include <nlohmann/json.hpp>

#include <filesystem>
#include <fstream>
#include <sstream>
#include <string>

namespace Holloware
{
	Ref<void> PrefabAssetImporter::Load(const std::filesystem::path& path)
	{
		nlohmann::json data = JsonHelper::LoadFromFile(path.string());
		return CreateRef<std::string>(data);
	}
}