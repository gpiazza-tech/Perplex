#pragma once

#include "AssetType.h"
#include <Perplex/Core/Core.h>
#include <Perplex/Assets/AssetImporter.h>

#include <nlohmann/json_fwd.hpp>

#include <filesystem>

namespace Perplex
{
	class ScriptAssetImporter : public AssetImporter
	{
	public:
		bool CanImport(const std::filesystem::path& path) override { return path.extension() == ".c"; }
		AssetType Type() override { return AssetType::ScriptAsset; }

		nlohmann::json Import(const std::filesystem::path& path);

		Ref<void> Load(const std::filesystem::path& path) override;
	};
}