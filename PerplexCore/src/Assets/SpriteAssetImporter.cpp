#include <Perplex/pch.h>
#include <Perplex/Assets/SpriteAssetImporter.h>

#include <Perplex/Core/Core.h>
#include <pxr/pxr.h>

#include <filesystem>
#include <memory>

namespace Perplex
{
	Ref<void> SpriteAssetImporter::Load(const std::filesystem::path& path)
	{
		Ref<pxr::Sprite> sprite = CreateRef<pxr::Sprite>(pxr::SpriteRegistry::AddSprite(pxr::LoadPNG(path)));
		return std::static_pointer_cast<void>(sprite);
	}
}