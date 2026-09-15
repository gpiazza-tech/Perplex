#include <Perplex/pch.h>
#include <Perplex/Assets/AssetManager.h>

#include <Perplex/Core/Core.h>
#include <Perplex/Assets/Asset.h>
#include <Perplex/Assets/AssetType.h>
#include <Perplex/Assets/AssetUpdateListener.h>
#include <Perplex/Assets/AssetImporter.h>
#include <Perplex/Assets/SpriteAssetImporter.h>
#include <Perplex/Assets/ScriptAssetImporter.h>
#include <Perplex/Assets/SceneAssetImporter.h>
#include <Perplex/Assets/PrefabAssetImporter.h>
#include <Perplex/Assets/FontAssetImporter.h>
#include <Perplex/Assets/AngelScriptAssetImporter.h>
#include <Perplex/Events/AssetEvent.h>

#include <Perplex/Core/UUID.h>
#include <Perplex/Serialization/JsonHelper.h>

#include <filesystem>
#include <unordered_map>
#include <functional>
#include <vector>

namespace Perplex
{
	namespace fs = std::filesystem;

	AssetManager::AssetManager(const fs::path& assetsPath)
		: m_AssetsPath(assetsPath)
	{
		// Register importers
		m_Importers.push_back(std::make_unique<SpriteAssetImporter>());
		m_Importers.push_back(std::make_unique<ScriptAssetImporter>());
		m_Importers.push_back(std::make_unique<SceneAssetImporter>());
		m_Importers.push_back(std::make_unique<PrefabAssetImporter>());
		m_Importers.push_back(std::make_unique<FontAssetImporter>());
		m_Importers.push_back(std::make_unique<AngelScriptAssetImporter>());

		// Loop over all asset files in project
		for (const auto& entry : fs::recursive_directory_iterator(m_AssetsPath))
		{
			if (!entry.is_directory() && entry.path().extension() != ".meta")
			{
				Import(entry.path());
			}
		}

		// Start Watcher
		efsw::FileWatcher* fileWatcher = new efsw::FileWatcher();
		AssetUpdateListener* listener = new AssetUpdateListener();
		efsw::WatchID watchID = fileWatcher->addWatch(m_AssetsPath.string(), listener, true);
		fileWatcher->watch();
	}

	AssetManager::~AssetManager() = default;

	void AssetManager::Import(const std::filesystem::path& path)
	{
		for (auto& importer : m_Importers)
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

			m_PathMap[uuid] = path;
			m_DataMap[uuid] = nullptr;
			m_TypeMap[uuid] = importer->Type();

			if (m_AssetImportedCallback)
			{
				auto event = AssetImportedEvent{ Asset(uuid) };
				m_AssetImportedCallback(event);
			}
		}
	}

	Ref<void> AssetManager::Load(Asset asset)
	{
		const fs::path& path = m_PathMap[asset];

		for (auto& importer : m_Importers)
		{
			if (importer->CanImport(path))
			{
				m_DataMap[asset] = importer->Load(path);
				return m_DataMap[asset];
			}
		}
		return nullptr;
	}

	void AssetManager::Unload(Asset asset)
	{
		m_DataMap.erase(asset);
	}

	const std::filesystem::path& AssetManager::GetPath(Asset asset)
	{ 
		static const std::filesystem::path empty{};

		if (m_PathMap.contains(asset))
			return m_PathMap[asset];
		
		return empty;
	}

	Ref<void> AssetManager::GetData(Asset asset)
	{
		try
		{
			if (m_DataMap[asset] == nullptr)
				return Load(asset);
			else
				return m_DataMap[asset];
		}
		catch (std::exception e)
		{
			HW_CORE_ERROR("AssetManager: {0}", e.what());
		}
		return nullptr;
	}

	Ref<void> AssetManager::LoadData(Asset asset)
	{
		try
		{
			return Load(asset);
		}
		catch (std::exception e)
		{
			HW_CORE_ERROR("AssetManager: {0}", e.what());
		}
		return nullptr;
	}

	AssetType AssetManager::GetType(Asset asset)
	{
		if (m_TypeMap.contains(asset))
			return m_TypeMap[asset];

		return AssetType::Asset;
	}

	Asset AssetManager::Get(const fs::path& path)
	{
		if (!fs::exists(path.string().append(".meta"))) return Asset();

		nlohmann::json json = JsonHelper::LoadFromFile(path.string().append(".meta"));
		return Asset(json["UUID"].get<UUID>());
	}
}