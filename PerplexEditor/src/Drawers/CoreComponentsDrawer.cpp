#include "Holloware/Scene/Components.h"

#include <Holloware/Core/Log.h>
#include "Holloware/Assets/Asset.h"
#include "Holloware/Assets/AssetManager.h"
#include "Holloware/ImGui/ImGuiUtilities.h"
#include "Holloware/ImGui/Drawer.h"
#include <Holloware/Assets/AssetType.h>
#include <Holloware/Scripting/ScriptData.h>
#include <rendering/Camera.h>

#include <imgui/imgui.h>
#include <glm/fwd.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <string.h>
#include <string>

namespace Holloware
{
	void DrawGui(IDComponent& component)
	{
		std::string tempString = std::to_string(component.ID);
		ImGui::Text(tempString.c_str());
	}

	void DrawGui(TagComponent& component)
	{
		char buffer[32];
		memset(buffer, 0, sizeof(buffer));
		strcpy_s(buffer, sizeof(buffer), component.Tag.c_str());
		if (ImGui::InputText("Tag", buffer, sizeof(buffer)))
		{
			component.Tag = std::string(buffer);
		}
	}

	void DrawGui(TransformComponent& component)
	{
		ImGuiUtilities::DrawVec3Control("Position", component.Position);

		glm::vec3 rotation = glm::degrees(component.Rotation);
		ImGuiUtilities::DrawVec3Control("Rotation", rotation);
		component.Rotation = glm::radians(rotation);

		ImGuiUtilities::DrawVec3Control("Scale", component.Scale);
	}

	void DrawGui(SpriteRendererComponent& component)
	{
		// Color
		Drawer::DrawAssetField("Sprite", component.SpriteAsset, AssetType::SpriteAsset);
		ImGui::ColorEdit4("Color", glm::value_ptr(component.Color));

		// Emission
		Drawer::DrawAssetField("Emission Mask", component.EmissionSpriteAsset, AssetType::SpriteAsset);
		ImGui::DragFloat("Emission", &component.Emission, 0.01f);
	}

	void DrawGui(CameraComponent& component)
	{
		ImGui::Checkbox("Primary", &component.Primary);
		ImGui::DragFloat("Zoom", &component.Zoom);
		ImGui::ColorEdit4("Background", &component.Background.r);
		ImGui::DragInt("Pixels Per Unit", &component.PixelsPerUnit);
		ImGui::Checkbox("PixelPerfect", &component.PixelPerfect);

		const char* scalingModeStrings[] = { "Width", "Height", "Larger Side", "Smaller Side" };
		const char* currentScalingModeString = scalingModeStrings[(int)component.ScalingMode - 1];

		if (ImGui::BeginCombo("Scaling Mode", currentScalingModeString))
		{
			for (int i = 0; i < 4; i++)
			{
				bool isSelected = currentScalingModeString == scalingModeStrings[i];
				if (ImGui::Selectable(scalingModeStrings[i], isSelected))
				{
					currentScalingModeString = scalingModeStrings[i];
					component.ScalingMode = ((pxr::ScalingMode)(i + 1)); // +1 to skip ScalingMode::None
				}

				if (isSelected)
					ImGui::SetItemDefaultFocus();
			}

			ImGui::EndCombo();
		}
	}

	void DrawGui(ScriptComponent& component)
	{
		ImGui::PushID(&component);

		if (Drawer::DrawAssetField("Source", component.ScriptAsset, AssetType::ScriptAsset))
		{
			if (component.ScriptAsset)
				component.Properties = component.ScriptAsset.GetData<ScriptData>()->Properties;
			else
				component.Properties.clear();
		}

		ImGui::Dummy(ImVec2(0.0f, 20.0f));

		for (auto& property : component.Properties)
		{
			property.DrawGui();
		}

		ImGui::PopID();
	}
}