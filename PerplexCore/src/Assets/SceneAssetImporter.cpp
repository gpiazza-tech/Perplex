#include <Perplex/pch.h>
#include <Perplex/Assets/SceneAssetImporter.h>

#include <Perplex/Core/Core.h>
#include <Perplex/Scene/SceneSerializer.h>

#include <filesystem>

namespace Perplex
{
	namespace fs = std::filesystem;

	Ref<void> SceneAssetImporter::Load(const fs::path& path)
	{
		return SceneSerializer::Deserialize(path);
	}
}