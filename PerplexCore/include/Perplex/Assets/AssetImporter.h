#pragma once

#include "AssetType.h"
#include <Perplex/Core/Core.h>

#include <nlohmann/json_fwd.hpp>

#include <filesystem>

namespace Perplex
{
	class AssetImporter
	{
	public:
		virtual bool CanImport(const std::filesystem::path& path) = 0;
		virtual AssetType Type() = 0;

		virtual bool NeedsReimport(const std::filesystem::path& path);
		virtual nlohmann::json Import(const std::filesystem::path& path);

		virtual Ref<void> Load(const std::filesystem::path& path) = 0;
	};
}