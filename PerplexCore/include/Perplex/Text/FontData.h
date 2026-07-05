#pragma once

#include <pxr/pxr.h>

#include <unordered_map>

namespace Perplex
{
	struct FontGlyph
	{
		pxr::Sprite Sprite{};
		int OffsetX{};
		int OffsetY{};
		int Stride{};
	};

	struct FontData
	{
		std::unordered_map<char, FontGlyph> Glyphs{};
	};
}