#include "Holloware/Scene/Components.h"

#include "Holloware/Assets/Asset.h"
#include "Holloware/Assets/AssetManager.h"
#include "Holloware/ImGui/ImGuiUtilities.h"

#include <imgui/imgui.h>
#include <glm/gtc/type_ptr.hpp>

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
		ImGui::ColorEdit4("Color", glm::value_ptr(Color));

		static std::string label = "None";
		ImGui::Button(label.c_str(), { 200, 20 });
		if (ImGui::BeginDragDropTarget())
		{
			if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("CONTENT_BROWSER_ITEM"))
			{
				Asset* payloadAsset = (Asset*)payload->Data;

				const std::string& pathString = payloadAsset->GetPath().string();
				label = pathString;

				TextureAsset = *payloadAsset;
			}
			ImGui::EndDragDropTarget();
		}
	}

	void CameraComponent::DrawGui()
	{
		auto& camera = Camera;

		ImGui::Checkbox("Primary", &Primary);

		const char* projectionTypeStrings[] = { "Perspective", "Orthographic" };
		const char* currentProjectionTypeString = projectionTypeStrings[(int)camera.GetProjectionType()];

		if (ImGui::BeginCombo("Projection", currentProjectionTypeString))
		{
			for (int i = 0; i < 2; i++)
			{
				bool isSelected = currentProjectionTypeString = projectionTypeStrings[i];
				if (ImGui::Selectable(projectionTypeStrings[i], isSelected))
				{
					currentProjectionTypeString = projectionTypeStrings[i];
					camera.SetProjectionType((SceneCamera::ProjectionType)i);
				}

				if (isSelected)
					ImGui::SetItemDefaultFocus();
			}

			ImGui::EndCombo();
		}

		if (camera.GetProjectionType() == SceneCamera::ProjectionType::Orthographic)
		{
			float orthoSize = camera.GetOrthographicSize();
			if (ImGui::DragFloat("Size", &orthoSize))
				camera.SetOrthographicSize(orthoSize);

			float orthoNear = camera.GetOrthographicNearClip();
			if (ImGui::DragFloat("Near Clip", &orthoNear))
				camera.SetOrthographicNearClip(orthoNear);

			float orthoFar = camera.GetOrthographicFarClip();
			if (ImGui::DragFloat("Far Clip", &orthoFar))
				camera.SetOrthographicFarClip(orthoFar);

			ImGui::Checkbox("Fixed Aspect", &FixedAspectRatio);
		}


		if (camera.GetProjectionType() == SceneCamera::ProjectionType::Perspective)
		{
			float perspectiveFOV = glm::degrees(camera.GetPerspectiveVerticalFOV());
			if (ImGui::DragFloat("FOV", &perspectiveFOV))
				camera.SetPerspectiveVerticalFOV(glm::radians(perspectiveFOV));

			float perspectiveNear = camera.GetPerspectiveNearClip();
			if (ImGui::DragFloat("Near Clip", &perspectiveNear))
				camera.SetPerspectiveNearClip(perspectiveNear);

			float perspectiveFar = camera.GetPerspectiveFarClip();
			if (ImGui::DragFloat("Far Clip", &perspectiveFar))
				camera.SetPerspectiveFarClip(perspectiveFar);
		}
	}
}