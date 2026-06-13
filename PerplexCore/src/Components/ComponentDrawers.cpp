#include <Perplex/pch.h>
#include <Perplex/Components/ComponentDrawers.h>

#include <Perplex/Scene/Components.h>
#include <Perplex/Assets/Asset.h>
#include <Perplex/ImGui/GuiSelection.h>
#include <Perplex/ImGui/PrimitiveDrawers.h>
#include <Perplex/ImGui/PerplexDrawers.h>
#include <Perplex/Assets/AssetType.h>
#include <Perplex/Scripting/ScriptData.h>
#include <pxr/pxr.h>

#include <imgui.h>
#include <glm/fwd.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <string.h>
#include <string>

namespace Perplex
{
	void Draw(GuiSelection<IDComponent> component)
	{
		GuiSelection<UUID> ids = PERPLEX_SUBSELECTION(component, UUID, ID);
		DrawSelection<UUID>(ids, [](UUID& value)
			{
				std::string strId = std::to_string(value);
				return Draw(strId, "ID", false);
			});
	}

	void Draw(GuiSelection<TagComponent> component)
	{
		GuiSelection<std::string> tags = PERPLEX_SUBSELECTION(component, std::string, Tag);
		DrawSelection(tags, "Tag");
	}

	void Draw(GuiSelection<TransformComponent> component)
	{
		ImVec2 padding = { 1.0f, 1.0f };

		GuiSelection<glm::vec3> position = PERPLEX_SUBSELECTION(component, glm::vec3, Position);
		DrawSelection<glm::vec3>(position, [](glm::vec3& value) { DrawVec3Control("Position", value); return true; });
		ImGui::Dummy(padding);

		GuiSelection<glm::vec3> rotation = PERPLEX_SUBSELECTION(component, glm::vec3, Rotation);
		DrawSelection<glm::vec3>(rotation, [](glm::vec3& value) { DrawVec3Control("Rotation", value); return true; });
		ImGui::Dummy(padding);

		GuiSelection<glm::vec3> scale = PERPLEX_SUBSELECTION(component, glm::vec3, Scale);
		DrawSelection<glm::vec3>(scale, [](glm::vec3& value) { DrawVec3Control("Scale", value); return true; });
		ImGui::Dummy(padding);
	}

	void Draw(GuiSelection<SpriteRendererComponent> component)
	{
		// Color
		GuiSelection<Asset> spriteAsset = PERPLEX_SUBSELECTION(component, Asset, SpriteAsset);
		DrawSelection<Asset>(spriteAsset, [](Asset& value) { return DrawAssetField("Sprite", value, AssetType::SpriteAsset); });

		DrawSelection<glm::vec4>(PERPLEX_SUBSELECTION(component, glm::vec4, Color), "Color");

		// Emission
		GuiSelection<Asset> emissionAsset = PERPLEX_SUBSELECTION(component, Asset, EmissionSpriteAsset);
		DrawSelection<Asset>(emissionAsset, [](Asset& value) { return DrawAssetField("Emission Mask", value, AssetType::SpriteAsset); });

		DrawSelection<float>(PERPLEX_SUBSELECTION(component, float, Emission), "Emission");
	}

	void Draw(GuiSelection<CameraComponent> component)
	{
		DrawSelection(PERPLEX_SUBSELECTION(component, bool, Primary), "Primary");
		DrawSelection(PERPLEX_SUBSELECTION(component, float, Zoom), "Zoom");
		DrawSelection(PERPLEX_SUBSELECTION(component, glm::vec4, Background), "Background");
		DrawSelection(PERPLEX_SUBSELECTION(component, int, PixelsPerUnit), "Pixels Per Unit");
		DrawSelection(PERPLEX_SUBSELECTION(component, bool, PixelPerfect), "Pixel Perfect");

		GuiSelection<pxr::ScalingMode> scalingMode = PERPLEX_SUBSELECTION(component, pxr::ScalingMode, ScalingMode);
		DrawSelection<pxr::ScalingMode>(scalingMode, [](pxr::ScalingMode& value)
			{
				Option scalingModeOptions[] =
				{
					{ "Width", (int)pxr::ScalingMode::Width },
					{ "Height", (int)pxr::ScalingMode::Height },
					{ "Larger Side", (int)pxr::ScalingMode::LargerSide },
					{ "Smaller Side", (int)pxr::ScalingMode::SmallerSide }
				};
				return DrawOptions("Scaling Mode", (int&)value, scalingModeOptions, 4);
			});
	}

	void Draw(GuiSelection<ScriptComponent> component)
	{
		ImGui::PushID(&component);

		GuiSelection<Asset> scriptAsset = PERPLEX_SUBSELECTION(component, Asset, ScriptAsset);
		bool scriptAssetChanged = DrawSelection<Asset>(scriptAsset, [](Asset& value) { return DrawAssetField("Source", value, AssetType::ScriptAsset); });

		if (scriptAssetChanged)
		{
			for (auto& componentRef : component)
			{
				if (componentRef.get().ScriptAsset)
					componentRef.get().Properties = componentRef.get().ScriptAsset.GetData<ScriptData>()->Properties;
				else
					componentRef.get().Properties.clear();
			}
		}

		ImGui::Dummy(ImVec2(0.0f, 20.0f));

		if (scriptAsset.Size() == 1)
		{
			Ref<ScriptData> scriptData = scriptAsset.At(0).GetData<ScriptData>();

			if (scriptData)
			{
				std::vector<ScriptProperty>& properties = scriptData->Properties;

				for (auto& property : properties)
				{
					ImGui::PushID(property.GetPtr());
					property.DrawGui();
					ImGui::PopID();
				}
			}
		}

		ImGui::PopID();
	}

	void Draw(GuiSelection<PerpixelRendererComponent> component)
	{
		GuiSelection<PerpixelShapeType> shapeType = PERPLEX_SUBSELECTION(component, PerpixelShapeType, Shape.Type);
		DrawSelection<PerpixelShapeType>(shapeType, [](PerpixelShapeType& value)
			{
				Option shapeTypeOptions[] =
				{
					{ "Sprite", (int)PerpixelShapeType::Sprite },
					{ "Rect", (int)PerpixelShapeType::Rect },
					{ "Circle", (int)PerpixelShapeType::Circle }
				};
				return DrawOptions("Shape Type", (int&)value, shapeTypeOptions, 3);
			});

		if (shapeType.Synced())
		{
			GuiSelection<Asset> colorAsset = PERPLEX_SUBSELECTION(component, Asset, Shape.Info.ColorAsset);
			GuiSelection<Asset> emissionAsset = PERPLEX_SUBSELECTION(component, Asset, Shape.Info.EmissionAsset);

			switch (shapeType.GetValue())
			{
			case PerpixelShapeType::Circle:
				DrawSelection(PERPLEX_SUBSELECTION(component, float, Shape.Info.CircleRadius), "Radius");
				break;
			case PerpixelShapeType::Rect:
				DrawSelection(PERPLEX_SUBSELECTION(component, glm::vec2, Shape.Info.RectSize), "Size");
				break;
			case PerpixelShapeType::Sprite:
				DrawSelection<Asset>(colorAsset, [](Asset& value) { return DrawAssetField("Color Asset", value, AssetType::SpriteAsset); });
				DrawSelection<Asset>(emissionAsset, [](Asset& value) { return DrawAssetField("Emission Asset", value, AssetType::SpriteAsset); });
				break;
			default:
				break;
			}
		}

		DrawSelection(PERPLEX_SUBSELECTION(component, glm::vec4, Shape.Info.Color), "Color");
		DrawSelection(PERPLEX_SUBSELECTION(component, float, Shape.Info.Emission), "Emission");
	}

	void Draw(GuiSelection<BoxColliderComponent> component)
	{
		DrawSelection(PERPLEX_SUBSELECTION(component, glm::vec2, Scale), "Scale");
	}

	void Draw(GuiSelection<PhysicsBodyComponent> component)
	{
		DrawSelection(PERPLEX_SUBSELECTION(component, float, GravityScale), "Gravity Scale");
		DrawSelection(PERPLEX_SUBSELECTION(component, float, Density), "Density");
		DrawSelection(PERPLEX_SUBSELECTION(component, float, Friction), "Friction");
	}

	void Draw(GuiSelection<SpriteAnimatorComponent> component)
	{
		GuiSelection<Asset> startSprite = PERPLEX_SUBSELECTION(component, Asset, AnimationAsset);
		DrawSelection<Asset>(startSprite, [](Asset& value) { return DrawAssetField("Animation Asset", value, AssetType::AnimationAsset); });
	}
}