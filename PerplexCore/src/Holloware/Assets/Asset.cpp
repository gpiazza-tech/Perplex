#include "pch.h"
#include "Asset.h"

#include "Holloware/Assets/AssetManager.h"
#include "Holloware/Serialization/Json.h"
#include "AssetType.h"

#include <nlohmann/json_fwd.hpp>
#include <nlohmann/json.hpp>

#include <filesystem>

namespace Holloware
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
	void from_json(const nlohmann::json& json, Asset& asset) { asset.m_Handler = json.get<uint64_t>(); }
}