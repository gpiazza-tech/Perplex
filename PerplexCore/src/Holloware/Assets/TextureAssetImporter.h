#pragma once

#include "Holloware/Core/Core.h"
#include "Holloware/Assets/AssetImporter.h"

#include <filesystem>

namespace Holloware
{
	class TextureAssetImporter : public AssetImporter
	{
	public:
		TextureAssetImporter() = default;
		~TextureAssetImporter() {};

		bool CanImport(const std::filesystem::path& path) override { return path.extension().string() == ".png"; };
		Ref<void> Load(const std::filesystem::path& path) override;
	};
}