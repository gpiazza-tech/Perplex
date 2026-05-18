#include <Perplex/pch.h>
#include <Perplex/Assets/PrefabAssetImporter.h>

#include <Perplex/Core/Core.h>
#include <Perplex/Serialization/JsonHelper.h>

#include <nlohmann/json.hpp>

#include <filesystem>
#include <fstream>
#include <sstream>
#include <string>

namespace Perplex
{
	Ref<void> PrefabAssetImporter::Load(const std::filesystem::path& path)
	{
		nlohmann::json data = JsonHelper::LoadFromFile(path.string());
		return CreateRef<std::string>(data);
	}
}