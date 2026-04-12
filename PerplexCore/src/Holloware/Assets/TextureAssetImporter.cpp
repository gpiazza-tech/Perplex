#include "pch.h"
#include "TextureAssetImporter.h"

#include "Holloware/Core/Core.h"
#include "Holloware/Renderer/Texture.h"

#include <filesystem>
#include <memory>

namespace Holloware
{
	Ref<void> TextureAssetImporter::Load(const std::filesystem::path& path)
	{
		Ref<Texture2D> texture = Texture2D::Create(path);
		return std::static_pointer_cast<void>(texture);
	}
}