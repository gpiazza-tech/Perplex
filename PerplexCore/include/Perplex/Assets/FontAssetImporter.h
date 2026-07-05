#pragma once

#include "AssetType.h"
#include <Perplex/Core/Core.h>
#include <Perplex/Assets/AssetImporter.h>

#include <filesystem>

namespace Perplex
{
	class FontAssetImporter : public AssetImporter
	{
	public:
		FontAssetImporter() = default;
		~FontAssetImporter() {};

		bool CanImport(const std::filesystem::path& path) override { return path.extension().string() == ".bdf"; };
		AssetType Type() override { return AssetType::FontAsset; }

		Ref<void> Load(const std::filesystem::path& path) override;
	};
}