#include <Perplex/pch.h>
#include <Perplex/Core/Layer.h>

#include <Perplex/Core/Timestep.h>

#include <string>

namespace Perplex
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