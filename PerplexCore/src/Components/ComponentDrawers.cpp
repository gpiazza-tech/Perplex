#include <Perplex/pch.h>
#include <Perplex/Components/ComponentDrawers.h>

#include <Perplex/Components/Components.h>
#include <Perplex/Core/UUID.h>
#include <Perplex/Assets/Asset.h>
#include <Perplex/ImGui/GuiSelection.h>
#include <Perplex/ImGui/PrimitiveDrawers.h>
#include <Perplex/ImGui/PerplexDrawers.h>
#include <Perplex/Assets/AssetType.h>
#include <Perplex/Scripting/ScriptData.h>
#include <pxr/pxr.h>

#include <imgui.h>
#include <glm/fwd.hpp>

#include <string>
#include <format>

namespace Perplex
{
	void Draw(GuiSelection<IDComponent> component)
	{
		GuiSelection<UUID> ids = PERPLEX_SUBSELECTION(component, ID);
		DrawSelection<UUID>(ids, [](UUID& value)
			{
				std::string strId = std::to_string(value);
				return Draw(strId, "ID", false);
			});
	}

	void Draw(GuiSelection<TagComponent> component)
	{
		GuiSelection<std::string> tags = PERPLEX_SUBSELECTION(component, Tag);
		DrawSelection(tags, "Tag");
	}

	void Draw(GuiSelection<TransformComponent> component)
	{
		ImVec2 padding = { 1.0f, 1.0f };

		GuiSelection<glm::vec3> position = PERPLEX_SUBSELECTION(component, Position);
		DrawSelection<glm::vec3>(position, [](glm::vec3& value) { DrawVec3Control("Position", value); return true; });
		ImGui::Dummy(padding);

		GuiSelection<glm::vec3> rotation = PERPLEX_SUBSELECTION(component, Rotation);
		DrawSelection<glm::vec3>(rotation, [](glm::vec3& value) { DrawVec3Control("Rotation", value); return true; });
		ImGui::Dummy(padding);

		GuiSelection<glm::vec3> scale = PERPLEX_SUBSELECTION(component, Scale);
		DrawSelection<glm::vec3>(scale, [](glm::vec3& value) { DrawVec3Control("Scale", value); return true; });
		ImGui::Dummy(padding);
	}

	void Draw(GuiSelection<SpriteRendererComponent> component)
	{
		// Color
		GuiSelection<Asset> spriteAsset = PERPLEX_SUBSELECTION(component, SpriteAsset);
		DrawSelection<Asset>(spriteAsset, [](Asset& value) { return DrawAssetField("Sprite", value, AssetType::SpriteAsset); });

		DrawSelection<glm::vec4>(PERPLEX_SUBSELECTION(component, Color), [](glm::vec4& val) { return DrawColor(val, "Color"); });

		// Emission
		GuiSelection<Asset> emissionAsset = PERPLEX_SUBSELECTION(component, EmissionSpriteAsset);
		DrawSelection<Asset>(emissionAsset, [](Asset& value) { return DrawAssetField("Emission Mask", value, AssetType::SpriteAsset); });

		DrawSelection<float>(PERPLEX_SUBSELECTION(component, Emission), "Emission");
	}

	void Draw(GuiSelection<CameraComponent> component)
	{
		DrawSelection(PERPLEX_SUBSELECTION(component, Primary), "Primary");
		DrawSelection(PERPLEX_SUBSELECTION(component, Zoom), "Zoom");
		DrawSelection<glm::vec4>(PERPLEX_SUBSELECTION(component, Background), [](glm::vec4& val) { return DrawColor(val, "Background"); });
		DrawSelection(PERPLEX_SUBSELECTION(component, PixelsPerUnit), "Pixels Per Unit");
		DrawSelection(PERPLEX_SUBSELECTION(component, PixelPerfect), "Pixel Perfect");

		GuiSelection<pxr::ScalingMode> scalingMode = PERPLEX_SUBSELECTION(component, ScalingMode);
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

		GuiSelection<Asset> scriptAsset = PERPLEX_SUBSELECTION(component, ScriptAsset);
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

		if (component.Size() == 1)
		{
			std::vector<ScriptProperty>& properties = component.At(0).Properties;

			for (auto& property : properties)
			{
				ImGui::PushID(property.GetPtr());
				property.DrawGui();
				ImGui::PopID();
			}
		}

		ImGui::PopID();
	}

	void Draw(GuiSelection<PerpixelRendererComponent> component)
	{
		GuiSelection<PerpixelShapeType> shapeType = PERPLEX_SUBSELECTION(component, Shape.Type);
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
			GuiSelection<Asset> colorAsset = PERPLEX_SUBSELECTION(component, Shape.Info.ColorAsset);
			GuiSelection<Asset> emissionAsset = PERPLEX_SUBSELECTION(component, Shape.Info.EmissionAsset);

			switch (shapeType.GetValue())
			{
			case PerpixelShapeType::Circle:
				DrawSelection(PERPLEX_SUBSELECTION(component, Shape.Info.CircleRadius), "Radius");
				break;
			case PerpixelShapeType::Rect:
				DrawSelection(PERPLEX_SUBSELECTION(component, Shape.Info.RectSize), "Size");
				break;
			case PerpixelShapeType::Sprite:
				DrawSelection<Asset>(colorAsset, [](Asset& value) { return DrawAssetField("Color Asset", value, AssetType::SpriteAsset); });
				DrawSelection<Asset>(emissionAsset, [](Asset& value) { return DrawAssetField("Emission Asset", value, AssetType::SpriteAsset); });
				break;
			default:
				break;
			}
		}

		DrawSelection<glm::vec4>(PERPLEX_SUBSELECTION(component, Shape.Info.Color), [](glm::vec4& val) { return DrawColor(val, "Color"); });
		DrawSelection(PERPLEX_SUBSELECTION(component, Shape.Info.Emission), "Emission");
	}

	void Draw(GuiSelection<BoxColliderComponent> component)
	{
		DrawSelection(PERPLEX_SUBSELECTION(component, Scale), "Scale");
	}

	void Draw(GuiSelection<PhysicsBodyComponent> component)
	{
		DrawSelection(PERPLEX_SUBSELECTION(component, GravityScale), "Gravity Scale");
		DrawSelection(PERPLEX_SUBSELECTION(component, Density), "Density");
		DrawSelection(PERPLEX_SUBSELECTION(component, Friction), "Friction");
	}

	void Draw(GuiSelection<SpriteAnimatorComponent> component)
	{
		DrawSelection(PERPLEX_SUBSELECTION(component, Speed), "Speed");
		DrawSelection(PERPLEX_SUBSELECTION(component, PlayOnStart), "Play On Start");

		DrawSelection<std::vector<Sprite>>(PERPLEX_SUBSELECTION(component, Sprites), [](std::vector<Sprite>& vec)
			{
				std::string label{ "Sprites" };
				Draw(label, "", false);

				bool changed = false;

				int size = static_cast<int>(vec.size());
				changed |= Draw(size, "Size", 0, INT_MAX);
				if (changed && size >= 0)
					vec.resize(static_cast<size_t>(size));

				if (BeginDropdown("Elements"))
				{
					for (size_t i{}; i < vec.size(); ++i)
					{
						auto& sprite = vec.at(i);

						std::string colorAssetLabel = std::format("Color {}", i + 1);
						changed |= DrawAssetField(colorAssetLabel.c_str(), sprite.ColorAsset, AssetType::SpriteAsset);

						std::string emissionAssetLabel = std::format("Emission {}", i + 1);
						changed |= DrawAssetField(emissionAssetLabel.c_str(), sprite.EmissionAsset, AssetType::SpriteAsset);

						DrawSpace(20.0f);
					}

					EndDropdown();
				}

				return changed;
			});

	}
}