#pragma once

#include <cstdint>

namespace Holloware
{
	enum class AssetType : uint8_t
	{
		Asset = 0,
		SpriteAsset,
		ScriptAsset,
		SceneAsset,
		PrefabAsset
	};
}