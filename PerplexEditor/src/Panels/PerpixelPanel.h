#pragma once

#include <Perplex/Perplex.h>

namespace Perplex
{
	class PerpixelInstance;

	void RenderPerpixelInstanceGui(PerpixelInstance& instance);

	class PerpixelPanel
	{
	public:
		void OnImGuiRender(Ref<Scene> scene, UUID selection);
	};
}