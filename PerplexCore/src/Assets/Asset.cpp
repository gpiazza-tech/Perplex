#include <Perplex/pch.h>
#include <Perplex/Assets/Asset.h>

#include <Perplex/Assets/AssetManager.h>
#include <Perplex/Serialization/Json.h>
#include <Perplex/Assets/AssetType.h>

#include <nlohmann/json_fwd.hpp>
#include <nlohmann/json.hpp>

#include <filesystem>

namespace Perplex
{
	Asset::Asset(std::filesystem::path path)
	{
		m_Handler = AssetManager::Get(path);
	}

	const std::filesystem::path& Asset::GetPath() const
	{
		return AssetManager::GetPath(*this);
	}

	std::filesystem::path Asset::GetName() const
	{
		return AssetManager::GetPath(*this).filename();
	}

	AssetType Asset::GetType() const
	{
		return AssetManager::GetType(*this);
	}

	void to_json(nlohmann::json& json, const Asset& asset) { json = (uint64_t)asset.m_Handler; }
	void from_json(const nlohmann::json& json, Asset& asset)
	{
		asset.m_Handler = json.get<uint64_t>();
		AssetManager::LoadData(asset);
	}
}