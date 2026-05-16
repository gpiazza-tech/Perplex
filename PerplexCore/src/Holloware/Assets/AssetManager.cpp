#include "pch.h"
#include "AssetManager.h"

#include "Holloware/Core/Core.h"

#include "Asset.h"
#include "AssetType.h"
#include "AssetUpdateListener.h" 
#include "AssetImporter.h"
#include "SpriteAssetImporter.h"
#include "ScriptAssetImporter.h"
#include "SceneAssetImporter.h"

#include "Holloware/Core/Application.h"
#include "Holloware/Core/Project.h"
#include "Holloware/Core/UUID.h"

#include "Holloware/Serialization/JsonHelper.h"

#include <efsw/efsw.hpp>

#include <filesystem>
#include <unordered_map>
#include <functional>
#include <vector>

namespace Holloware
{
	namespace fs = std::filesystem;

	static fs::path s_AssetsPath = "NONE";

	static std::unordered_map<UUID, fs::path> s_PathMap;
	static std::unordered_map<UUID, Ref<void>> s_DataMap;
	static std::unordered_map<UUID, AssetType> s_TypeMap;

	static std::vector<std::unique_ptr<AssetImporter>> s_Importers;

	static std::function<void(Asset)> s_OnAssetImported;

	void AssetManager::Init()
	{
		s_AssetsPath = fs::path(Application::Get().GetCurrentProject().GetAssetsPath());

		// Register importers
		s_Importers.push_back(std::make_unique<SpriteAssetImporter>());
		s_Importers.push_back(std::make_unique<ScriptAssetImporter>());
		s_Importers.push_back(std::make_unique<SceneAssetImporter>());

		// Loop over all asset files in project
		for (const auto& entry : fs::recursive_directory_iterator(s_AssetsPath))
		{
			if (!entry.is_directory() && entry.path().extension() != ".meta")
			{
				Import(entry.path());
			}
		}

		// Start Watcher
		efsw::FileWatcher* fileWatcher = new efsw::FileWatcher();
		AssetUpdateListener* listener = new AssetUpdateListener();
		efsw::WatchID watchID = fileWatcher->addWatch(s_AssetsPath.string(), listener, true);
		fileWatcher->watch();
	}

	void AssetManager::Cleanup()
	{
		// Unload all assets
		for (auto& ref : s_DataMap)
			ref.second = nullptr;
	}

	void AssetManager::SetAssetImportedCallback(const std::function<void(Asset)>& func)
	{
		s_OnAssetImported = func;
	}

	void AssetManager::Import(const std::filesystem::path& path)
	{
		for (auto& importer : s_Importers)
		{
			if (!importer->CanImport(path))
				continue;

			fs::path meta = fs::path(path.string().append(".meta"));

			UUID uuid;
			nlohmann::json metaJson;

			if (fs::exists(meta))
			{
				metaJson = JsonHelper::LoadFromFile(meta.string());
				uint64_t intID = metaJson["UUID"];
				uuid = intID;
			}
			else
			{
				metaJson = nlohmann::json();
				metaJson["UUID"] = uuid;
			}

			metaJson["data"] = importer->Import(path);
			JsonHelper::WriteToFile(metaJson, meta.string());

			s_PathMap[uuid] = path;
			s_DataMap[uuid] = nullptr;
			s_TypeMap[uuid] = importer->Type();

			if (s_OnAssetImported)
				s_OnAssetImported(Asset(uuid));
		}
	}

	Ref<void> AssetManager::Load(Asset asset)
	{
		const fs::path& path = s_PathMap[asset];

		for (auto& importer : s_Importers)
		{
			if (importer->CanImport(path))
			{
				s_DataMap[asset] = importer->Load(path);
				return s_DataMap[asset];
			}
		}
		return nullptr;
	}

	void AssetManager::Unload(Asset asset)
	{
		s_DataMap[asset] = nullptr;
	}

	const std::filesystem::path& AssetManager::GetPath(Asset asset)
	{
		if (s_PathMap.find(asset) != s_PathMap.end())
		{
			return s_PathMap[asset];
		}
		return std::filesystem::path();
	}

	Ref<void> AssetManager::GetData(Asset asset)
	{
		try
		{
			if (s_DataMap[asset] == nullptr)
				return Load(asset);
			else
				return s_DataMap[asset];
		}
		catch (std::exception e)
		{
			HW_CORE_ERROR("AssetManager: {0}", e.what());
		}
		return nullptr;
	}

	AssetType AssetManager::GetType(Asset asset)
	{
		if (s_TypeMap.find(asset) != s_TypeMap.end())
		{
			return s_TypeMap[asset];
		}
		return AssetType::Asset;
	}

	Asset AssetManager::Get(const fs::path& path)
	{
		if (!fs::exists(path.string().append(".meta"))) return Asset();

		nlohmann::json json = JsonHelper::LoadFromFile(path.string().append(".meta"));
		return Asset(json["UUID"].get<UUID>());
	}

	void AssetManager::OnAssetReferenceDestroyed(const Asset& asset)
	{
		// If there is only 1 reference, it must be owned by the 
		// AssetManager, so we can safely unload the resource.
		if (s_DataMap[asset].use_count() == 1)
		{
			Unload(asset);
		}
	}
}