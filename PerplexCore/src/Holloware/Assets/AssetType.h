#pragma once

#include <cstdint>

namespace Holloware
{
	enum class AssetType : uint8_t
	{
		Asset = 0,
		TextureAsset,
		PythonScriptAsset,
		SceneAsset
	};
}