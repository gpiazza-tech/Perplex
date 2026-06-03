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

		std::vector<pixel>& GetPixels() { return m_Pixels; }
		const std::vector<pixel>& GetPixels() const { return m_Pixels; }
	private:
		PerpixelShape m_SpawnShape{};

		std::vector<pixel> m_SpawnPixels{};
		std::vector<pixel> m_Pixels{};
	};
}