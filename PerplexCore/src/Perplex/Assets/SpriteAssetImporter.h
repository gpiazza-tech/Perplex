#pragma once

#include "AssetType.h"
#include <Perplex/Core/Core.h>
#include <Perplex/Assets/AssetImporter.h>

#include <filesystem>

namespace Perplex
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