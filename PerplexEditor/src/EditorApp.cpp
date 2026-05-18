#include <pch.h>

#include <Perplex/Perplex.h>
#include <Perplex/Core/EntryPoint.h>

#include "EditorLayer.h"

namespace Perplex
{
	class PerplexEditor : public Application
	{
	public:
		PerplexEditor()
			: Application("Perplex Editor")
		{
			PushLayer(new EditorLayer());
		}
		~PerplexEditor()
		{
		}
	};

	Application* CreateApplication() 
	{
		return new PerplexEditor();
	}
}