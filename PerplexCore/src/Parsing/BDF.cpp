#include <Perplex/pch.h>
#include <Perplex/Parsing/BDF.h>

#include <glm/fwd.hpp>

#include <fstream>
#include <filesystem>
#include <optional>
#include <string>
#include <cstdint>
#include <sstream>
#include <string_view>
#include <vector>

namespace Perplex
{
	std::optional<BDFInfo> ParseBDFInfo(const std::filesystem::path& filepath) noexcept
	{
		BDFInfo bdf;
		std::ifstream filestream{ filepath };
		
		std::string line;
		std::string tag;

		BDFGlyph* currentGlyph{ nullptr }; // set on STARTCHAR
		size_t currentGlyphIndex{ 0 }; // incremented on ENDCHAR

		while (std::getline(filestream, line))
		{
			ParseLine(line, tag);

			if (tag == "STARTFONT")
				ParseLine(line, tag, bdf.Header.Version);
			else if (tag == "FONT")
				ParseLine(line, tag, bdf.Header.Font);
			else if (tag == "SIZE")
				ParseLine(line, tag, bdf.Header.Size.Point,
					bdf.Header.Size.X, bdf.Header.Size.Y);
			else if (tag == "FONTBOUNDINGBOX")
				ParseLine(line, tag,
					bdf.Header.FontBoundingBox.Width,
					bdf.Header.FontBoundingBox.Height,
					bdf.Header.FontBoundingBox.X,
					bdf.Header.FontBoundingBox.Y);
			else if (tag == "CHARS")
			{
				size_t chars{};
				ParseLine(line, tag, chars);
				bdf.Glyphs.reserve(chars);
			}
			else if (tag == "STARTCHAR")
			{
				bdf.Glyphs.push_back(BDFGlyph{});
				currentGlyph = &bdf.Glyphs.at(currentGlyphIndex);
				++currentGlyphIndex;
				ParseLine(line, tag, currentGlyph->Char);
			}
			else if (tag == "BITMAP")
			{
				currentGlyph->GlyphBuffer.Resize(0, currentGlyph->BBX.Width, 0, currentGlyph->BBX.Height);

				// Fill Buffer
				for (int y{ (int)currentGlyph->GlyphBuffer.GetHeight() - 1 }; y >= 0; --y)
				{
					if (!std::getline(filestream, line))
						return std::nullopt; // incorrect formatting
					ParseLine(line, tag);
					if (tag == "ENDCHAR")
						return std::nullopt; // incorrect formatting
					
					std::vector<bool> bitmapRow = HexToBitmap(tag);
					for (int x{}; x < currentGlyph->GlyphBuffer.GetWidth(); ++x)
					{
						if (bitmapRow.at((size_t)x))
							currentGlyph->GlyphBuffer.At((size_t)x, (size_t)y) = glm::u8vec4{ 255 };
					}
				}
			}
			else if (tag == "ENCODING")
				ParseLine(line, tag, currentGlyph->Encoding);
			else if (tag == "SWIDTH")
				ParseLine(line, tag,
					currentGlyph->SWidth.X,
					currentGlyph->SWidth.Y);
			else if (tag == "DWIDTH")
				ParseLine(line, tag,
					currentGlyph->DWidth.X,
					currentGlyph->DWidth.Y);
			else if (tag == "BBX")
			{
				ParseLine(line, tag,
					currentGlyph->BBX.Width,
					currentGlyph->BBX.Height,
					currentGlyph->BBX.OffsetX,
					currentGlyph->BBX.OffsetY);
			}
		}

		return bdf;
	}

	std::vector<bool> HexToBitmap(std::string_view hex)
	{
		std::vector<bool> bitmap{};
		size_t byteCount{ hex.length() / 2 };
		bitmap.reserve(byteCount * 8);

		for (size_t i{}; i < byteCount; ++i)
		{
			size_t strLocation = i * 2;
			std::string_view currentByteStr = hex.substr(strLocation, strLocation + 2);
			uint8_t byte = std::stoi(std::string{ currentByteStr }, nullptr, 16);

			// fill bytes
			for (uint8_t mask{ 0b10000000 }; mask != 0; mask >>= 1)
				bitmap.emplace_back(byte & mask);
		}

		return bitmap;
	}
}