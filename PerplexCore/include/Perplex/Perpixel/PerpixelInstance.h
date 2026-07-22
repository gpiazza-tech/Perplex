#pragma once

#include <Perplex/Perpixel/PerpixelSpawnShape.h>
#include <c/perplex_pixel.h>

#include <vector>

namespace Perplex
{
	class PerpixelInstance
	{
	public:
		void ResetPixels() { m_Pixels = m_SpawnPixels; }
		void SetSpawnShape(const PerpixelShape& shape);
		const PerpixelShape& GetSpawnShape() const { return m_SpawnShape; }

		std::vector<Pixel>& GetPixels() { return m_Pixels; }
		const std::vector<Pixel>& GetPixels() const { return m_Pixels; }

		void SpawnPixel(const Pixel& pxl) { m_Pixels.emplace_back(pxl); };

		const size_t GetAlivePixelCount() const;
	private:
		PerpixelShape m_SpawnShape{};

		std::vector<Pixel> m_SpawnPixels{};
		std::vector<Pixel> m_Pixels{};
	};
}