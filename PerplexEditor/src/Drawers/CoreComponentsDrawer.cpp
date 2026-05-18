#include <pch.h>
#include <Perplex/Scene/Components.h>

#include <Perplex/Core/Log.h>
#include <Perplex/Assets/Asset.h>
#include <Perplex/Assets/AssetManager.h>
#include <Perplex/ImGui/Drawer.h>
#include <Perplex/ImGui/ImGuiUtilities.h>
#include <Perplex/Assets/AssetType.h>
#include <Perplex/Scripting/ScriptData.h>
#include <rendering/Camera.h>

#include <imgui/imgui.h>
#include <glm/fwd.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <string.h>
#include <string>

namespace Perplex
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
		ImGui::Checkbox("Pixel Perfect", &component.PixelPerfect);

		Option scalingModeOptions[] = {
			{ "Width", (int)pxr::ScalingMode::Width },
			{ "Height", (int)pxr::ScalingMode::Height },
			{ "Larger Side", (int)pxr::ScalingMode::LargerSide },
			{ "Smaller Side", (int)pxr::ScalingMode::SmallerSide }
		};
		Drawer::DrawOptions("Scaling Mode", (int&)component.PixelPerfect, scalingModeOptions, 4);
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

	void DrawGui(PerpixelRendererComponent& component)
	{
		Option shapeTypeOptions[] = {
			{ "Sprite", (int)PerpixelShapeType::Sprite },
			{ "Rect", (int)PerpixelShapeType::Rect },
			{ "Circle", (int)PerpixelShapeType::Circle }
		};

		if (Drawer::DrawOptions("Shape Type", (int&)component.Shape, shapeTypeOptions, 3))
			component.Instance.SetSpawnShape(component.Shape);

		switch (component.Shape.Type)
		{
		case PerpixelShapeType::Circle:
			ImGui::DragFloat("Radius", &component.Shape.Info.CircleRadius, 0.1f);
			break;
		case PerpixelShapeType::Rect:
			ImGui::DragFloat2("Size", &component.Shape.Info.RectSize.x, 0.1f);
			break;
		case PerpixelShapeType::Sprite:
			Drawer::DrawAssetField("Sprite", component.Shape.Info.SpriteAsset, AssetType::SpriteAsset);
			break;
		default:
			break;
		}

		if (ImGui::Button("Reset Pixels"))
			component.Instance.ResetPixels();
	}
}