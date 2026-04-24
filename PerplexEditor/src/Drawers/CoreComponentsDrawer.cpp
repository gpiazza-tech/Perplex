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
	void IDComponent::DrawGui()
	{
		std::string tempString = std::to_string(ID);
		ImGui::Text(tempString.c_str());
	}

	void TagComponent::DrawGui()
	{
		char buffer[32]; 
		memset(buffer, 0, sizeof(buffer));
		strcpy_s(buffer, sizeof(buffer), Tag.c_str());
		if (ImGui::InputText("Tag", buffer, sizeof(buffer)))
		{
			Tag = std::string(buffer);
		}
	}

	void TransformComponent::DrawGui()
	{
		ImGuiUtilities::DrawVec3Control("Position", Position);

		glm::vec3 rotation = glm::degrees(Rotation);
		ImGuiUtilities::DrawVec3Control("Rotation", rotation);
		Rotation = glm::radians(rotation);

		ImGuiUtilities::DrawVec3Control("Scale", Scale);
	}

	void SpriteRendererComponent::DrawGui()
	{
		// Color
		Drawer::DrawAssetField("Sprite", SpriteAsset, AssetType::SpriteAsset);
		ImGui::ColorEdit4("Color", glm::value_ptr(Color));

		// Emission
		Drawer::DrawAssetField("Emission Mask", EmissionSpriteAsset, AssetType::SpriteAsset);
		ImGui::DragFloat("Emission", &Emission, 0.01f);
	}

	void CameraComponent::DrawGui()
	{
		ImGui::Checkbox("Primary", &Primary);
		ImGui::DragFloat("Zoom", &Zoom);
		ImGui::ColorEdit4("Background", &Background.r);
		ImGui::DragInt("Pixels Per Unit", &PixelsPerUnit);
		ImGui::Checkbox("PixelPerfect", &PixelPerfect);

		const char* scalingModeStrings[] = { "Width", "Height", "Larger Side", "Smaller Side" };
		const char* currentScalingModeString = scalingModeStrings[(int)ScalingMode - 1];

		if (ImGui::BeginCombo("Scaling Mode", currentScalingModeString))
		{
			for (int i = 0; i < 4; i++)
			{
				bool isSelected = currentScalingModeString == scalingModeStrings[i];
				if (ImGui::Selectable(scalingModeStrings[i], isSelected))
				{
					currentScalingModeString = scalingModeStrings[i];
					ScalingMode = ((pxr::ScalingMode)(i + 1)); // +1 to skip ScalingMode::None
				}

				if (isSelected)
					ImGui::SetItemDefaultFocus();
			}

			ImGui::EndCombo();
		}
	}

	void ScriptComponent::DrawGui()
	{
		ImGui::PushID(this);

		if (Drawer::DrawAssetField("Source", ScriptAsset, AssetType::ScriptAsset))
		{
			if (ScriptAsset)
				Properties = ScriptAsset.GetData<ScriptData>()->Properties;
			else
				Properties.clear();
		}

		ImGui::Dummy(ImVec2(0.0f, 20.0f));

		for (auto& property : Properties)
		{
			property.DrawGui();
		}

		ImGui::PopID();
	}
}