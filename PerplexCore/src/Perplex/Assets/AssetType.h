#pragma once

#include <cstdint>

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
}