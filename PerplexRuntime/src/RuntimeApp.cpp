#include <pch.h>

#include <Perplex/Perplex.h>
#include <Perplex/Core/EntryPoint.h>

#include "RuntimeLayer.h"

namespace Perplex
{
	class PerplexRuntime : public Application
	{
	public:
		PerplexRuntime()
			: Application("Perplex Runtime")
		{
			PushLayer(new RuntimeLayer());
		}
		~PerplexRuntime()
		{
		}
	};

	Application* CreateApplication()
	{
		return new PerplexRuntime();
	}
}