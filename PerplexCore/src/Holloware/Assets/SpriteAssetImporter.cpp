#include "pch.h"
#include "SpriteAssetImporter.h"

#include "Holloware/Core/Core.h"
#include <pxr/pxr.h>

#include <filesystem>
#include <memory>

namespace Holloware
{
	Ref<void> SpriteAssetImporter::Load(const std::filesystem::path& path)
	{
		Ref<pxr::Sprite> sprite = CreateRef<pxr::Sprite>(pxr::SpriteRegistry::GetSprite(path));
		return std::static_pointer_cast<void>(sprite);
	}
}