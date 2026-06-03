#include <pch.h>
#include "PerpixelPanel.h"

#include <Perplex/Perpixel/PerpixelSystem.h>

#include <imgui/imgui.h>

namespace Perplex
{
	void RenderPerpixelInstanceGui(PerpixelInstance& instance)
	{
		if (ImGui::Button("Reset"))
			instance.ResetPixels();
	}

	void PerpixelPanel::OnImGuiRender(Ref<Scene> scene, UUID selection)
	{
		ImGui::Begin("Perpixel Controls");

		Entity selectedEntity = scene->GetEntity(selection);
		if (selectedEntity && selectedEntity.HasComponent<PerpixelRendererComponent>())
		{
			PerpixelSystem& perpixelSystem = scene->GetSystem<PerpixelSystem>();
			PerpixelInstance* perpixelInstance = perpixelSystem.GetInstance(selection);

			if (perpixelInstance != nullptr)
			{
				RenderPerpixelInstanceGui(*perpixelInstance);
			}
		}

		ImGui::End();
	}
}