#pragma once

#include "Holloware/Core/Core.h"
#include "AssetType.h"

#include <filesystem>
#include <functional>

namespace Holloware
{
	class Asset;
	class AssetUpdateListener;

	class AssetManager
	{
	public:
		static void Init();
		static void Cleanup();

		static Asset Get(const std::filesystem::path& path);
		static const std::filesystem::path& GetPath(Asset asset);
		static Ref<void> GetData(Asset asset);
		static AssetType GetType(Asset asset);

		static void SetAssetImportedCallback(const std::function<void(Asset)>& func);
	private:
		static void Import(const std::filesystem::path& path);
		static Ref<void> Load(Asset asset);
		static void Unload(Asset asset);

		static void OnAssetReferenceDestroyed(const Asset& asset);

		friend Asset;
		friend AssetUpdateListener;
	};
}