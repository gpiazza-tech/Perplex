#pragma once

#include "Pixel.h"

#include <vector>

namespace Holloware
{
	struct PerpixelShape;

	class PerpixelInstance
	{
	public:
		void ResetPixels() { m_Pixels = m_SpawnPixels; }
		void SetSpawnShape(const PerpixelShape& shape);
	private:
		std::vector<Pixel> m_SpawnPixels{};
		std::vector<Pixel> m_Pixels{};
	};
}