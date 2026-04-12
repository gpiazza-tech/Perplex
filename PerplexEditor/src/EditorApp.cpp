#include <Holloware.h>
#include <Holloware/Core/EntryPoint.h>

#include "EditorLayer.h"

namespace Holloware
{
	class HollowareEditor : public Application
	{
	public:
		HollowareEditor()
			: Application("Holloware Editor")
		{
			PushLayer(new EditorLayer());
		}
		~HollowareEditor()
		{
		}
	};

	Application* CreateApplication() 
	{
		return new HollowareEditor();
	}
}