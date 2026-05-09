#include <pch.h>
#include "PerpixelInstance.h"

#include "PerpixelSpawnShape.h"
#include <perplex_pixel.h>

#include <vector>

namespace Holloware
{
	void PerpixelInstance::SetSpawnShape(const PerpixelShape& shape)
	{
		m_SpawnPixels = std::vector<pixel>{ 10 };

		for (size_t i{}; i < m_SpawnPixels.size(); i++)
		{
			m_SpawnPixels[i].Position = { 0.0f, 0.0f };
			m_SpawnPixels[i].Color = { 1.0f, 1.0f, 1.0f, 1.0f };
			m_SpawnPixels[i].Emission = 0.0f;
			m_SpawnPixels[i].Lifetime = 0.0f;
		}
	}
}