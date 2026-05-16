#include <Holloware.h>
#include <Holloware/Core/EntryPoint.h>

#include "RuntimeLayer.h"

namespace Holloware
{
	class HollowareEditor : public Application
	{
	public:
		HollowareEditor()
			: Application("Holloware Editor")
		{
			PushLayer(new RuntimeLayer());
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