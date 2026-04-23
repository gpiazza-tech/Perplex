#pragma once

#include "Holloware/Core/Core.h"
#include "Holloware/Assets/AssetImporter.h"
#include "AssetType.h"

#include <filesystem>

namespace Holloware
{
	class SceneAssetImporter : public AssetImporter
	{
	public:
		bool CanImport(const std::filesystem::path& path) override { return path.extension() == ".hws"; }
		AssetType Type() override { return AssetType::SceneAsset; }

		Ref<void> Load(const std::filesystem::path& path) override;
	};
}