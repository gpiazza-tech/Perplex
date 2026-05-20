#pragma once

#include <cstdint>
#include <string>

namespace Perplex
{
	enum class AssetType : uint8_t
	{
		Asset = 0,
		SpriteAsset,
		ScriptAsset,
		SceneAsset,
		PrefabAsset
	};

	inline AssetType StringToAssetType(const std::string& str)
	{
		if (str == "Asset")
			return AssetType::Asset;
		else if (str == "SpriteAsset")
			return AssetType::SpriteAsset;
		else if (str == "ScriptAsset")
			return AssetType::ScriptAsset;
		else if (str == "SceneAsset")
			return AssetType::SceneAsset;
		else if (str == "PrefabAsset")
			return AssetType::PrefabAsset;

		HW_CORE_ERROR("{0} is not a valid AssetType!", str);

		return AssetType::Asset;
	}
}