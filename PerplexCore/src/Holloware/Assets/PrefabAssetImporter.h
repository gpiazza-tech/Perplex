#pragma once

#include "Holloware/Core/Core.h"
#include "Holloware/Assets/AssetImporter.h"
#include "AssetType.h"

#include <nlohmann/json_fwd.hpp>

#include <filesystem>

namespace Holloware
{
	class PrefabAssetImporter : public AssetImporter
	{
	public:
		bool CanImport(const std::filesystem::path& path) override { return path.extension() == ".prefab"; }
		AssetType Type() override { return AssetType::PrefabAsset; }

		Ref<void> Load(const std::filesystem::path& path) override;
	};
}