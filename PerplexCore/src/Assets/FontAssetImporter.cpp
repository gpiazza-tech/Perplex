#include <Perplex/pch.h>
#include <Perplex/Assets/FontAssetImporter.h>

#include <Perplex/Parsing/BDF.h>
#include <Perplex/Text/FontData.h>
#include <Perplex/Core/Core.h>
#include <pxr/pxr.h>

#include <filesystem>
#include <memory>
#include <optional>

namespace Perplex
{
	Ref<void> FontAssetImporter::Load(const std::filesystem::path& path)
	{
		std::optional<BDFInfo> bdfInfoOpt = ParseBDFInfo(path);
		PXR_ASSERT(bdfInfoOpt, "Failed to load BDF file from path {0}!", path.string().c_str());
		PXR_INFO("Loaded font from path {0}!", path.string().c_str());

		const BDFInfo& bdfInfo = bdfInfoOpt.value();
		Ref<FontData> fontData = CreateRef<FontData>();
		fontData->LineHeight = bdfInfo.Header.FontBoundingBox.Height;

		for (const auto& bdfGlyph : bdfInfo.Glyphs)
		{
			if (bdfGlyph.GlyphBuffer.GetWidth() > 0 && bdfGlyph.GlyphBuffer.GetHeight() > 0)
			{
				FontGlyph fontGlyph{};
				fontGlyph.Sprite = pxr::SpriteRegistry::AddSprite(bdfGlyph.GlyphBuffer);
				fontGlyph.OffsetX = bdfGlyph.BBX.OffsetX;
				fontGlyph.OffsetY = bdfGlyph.BBX.OffsetY;
				fontGlyph.Stride = bdfGlyph.DWidth.X;
				fontData->Glyphs.emplace(static_cast<char>(bdfGlyph.Encoding), fontGlyph);
			}
		}

		return std::static_pointer_cast<void>(fontData);
	}
}