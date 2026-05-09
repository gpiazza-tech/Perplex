#pragma once

#include <perplex_pixel.h>

#include <vector>

namespace Holloware
{
	struct PerpixelShape;

	class PerpixelInstance
	{
	public:
		void ResetPixels() { m_Pixels = m_SpawnPixels; }
		void SetSpawnShape(const PerpixelShape& shape);

		std::vector<pixel>& GetPixels() { return m_Pixels; }
		const std::vector<pixel>& GetPixels() const { return m_Pixels; }
	private:
		std::vector<pixel> m_SpawnPixels{};
		std::vector<pixel> m_Pixels{};
	};
}