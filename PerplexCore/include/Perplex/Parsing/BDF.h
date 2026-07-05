#pragma once

#include <pxr/pxr.h>

#include <filesystem>
#include <optional>
#include <string>
#include <cstddef>
#include <sstream>
#include <vector>

namespace Perplex
{
	struct BDFHeader
	{
		float Version{};
		std::string Font{};

		struct Size
		{
			int Point{};
			int X{};
			int Y{};
		};
		Size Size{};

		struct FontBoundingBox
		{
			int Width{};
			int Height{};
			int X{};
			int Y{};
		};
		FontBoundingBox FontBoundingBox{};

		// Properties are ignored
	};

	struct BDFGlyph
	{
		std::string Char{};
		int Encoding{};
		
		struct SWidth
		{
			int X{};
			int Y{};
		};
		SWidth SWidth{};

		struct DWidth
		{
			int X{};
			int Y{};
		};
		DWidth DWidth{};

		struct BBX
		{
			int Width{};
			int Height{};
			int OffsetX{};
			int OffsetY{};
		};
		BBX BBX{};

		pxr::ImageBuffer GlyphBuffer{};
	};

	struct BDFInfo
	{
		BDFHeader Header{};
		std::vector<BDFGlyph> Glyphs{};
	};

	// ParseBDFInfo does not parse the actual color data of the font, it only
	// returns metadata such as glyph sizes, offsets, codes, etc.
	// Loading the color data is handled solely by stb-image
	std::optional<BDFInfo> ParseBDFInfo(const std::filesystem::path& filepath) noexcept;

	template<typename... Args>
	concept AllStreamable = requires (Args... args, std::istringstream iss)
	{
		((iss >> args), ...);
	};

	template<AllStreamable... Args>
	void ParseLine(const std::string& line, Args&... args)
	{
		std::istringstream iss{ line };

		([&] {
			iss >> args;
			}(), ...);
	}

	std::vector<bool> HexToBitmap(std::string_view hex);
}