#include "pch.h"
#include "Layer.h"

#include "Holloware/Core/Timestep.h"

#include <string>

namespace Holloware
{
	Layer::Layer(const std::string& debugName)
		: m_DebugName(debugName)
	{
	}

	Layer::~Layer()
	{
	}

	void Layer::OnUpdate(Timestep ts) {}
}