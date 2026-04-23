#pragma once

#include "Holloware/Core/Core.h"
#include "Holloware/Assets/AssetImporter.h"
#include "AssetType.h"

#include <filesystem>

namespace Holloware
{
	class SpriteAssetImporter : public AssetImporter
	{
	public:
		SpriteAssetImporter() = default;
		~SpriteAssetImporter() {};

		bool CanImport(const std::filesystem::path& path) override { return path.extension().string() == ".png"; };
		AssetType Type() override { return AssetType::SpriteAsset; }

		Ref<void> Load(const std::filesystem::path& path) override;
	};
}