#pragma once

#include "AssetType.h"
#include <Perplex/Core/Core.h>
#include <Perplex/Assets/AssetImporter.h>

#include <filesystem>

namespace Perplex
{
	class PrefabAssetImporter : public AssetImporter
	{
	public:
		bool CanImport(const std::filesystem::path& path) override { return path.extension() == ".pxp"; }
		AssetType Type() override { return AssetType::PrefabAsset; }

		Ref<void> Load(const std::filesystem::path& path) override;
	};
}