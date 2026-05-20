#include <Perplex/pch.h>
#include <Perplex/ImGui/PerplexDrawers.h>

#include <Perplex/ImGui/PrimitiveDrawers.h>
#include <Perplex/Core/Core.h>
#include <Perplex/Core/Log.h>
#include <Perplex/Core/PerplexTypes.h>
#include <Perplex/Assets/Asset.h>
#include <Perplex/Assets/AssetType.h>
#include <Perplex/Scene/Entity.h>

#include <imgui.h>
#include <imgui_internal.h>
#include <glm/fwd.hpp>

namespace Perplex
{
	void DrawType(void* value, PerplexTypes type)
	{
		switch (type)
		{
		case Perplex::PerplexTypes::Int:
			Draw(*(int*)value);
			break;
		case Perplex::PerplexTypes::Float:
			Draw(*(float*)value);
			break;
		case Perplex::PerplexTypes::Bool:
			Draw(*(bool*)value);
			break;
		case Perplex::PerplexTypes::Vec2:
			Draw(*(glm::vec2*)value);
			break;
		case Perplex::PerplexTypes::Vec3:
			Draw(*(glm::vec3*)value);
			break;
		default:
			HW_CORE_ASSERT(false, "Perplex Type {0} not supported!", type);
		}
	}

	bool DrawAssetField(const char* label, Asset& asset, AssetType type)
	{
		ImGui::PushID(&asset);

		std::string valueLabel = !asset ? "<none>" : asset.GetName().string();
		ImGui::Button(valueLabel.c_str(), { 200, 20 });

		bool reset = false;
		if (ImGui::BeginPopupContextItem())
		{
			if (ImGui::MenuItem("Reset"))
				reset = true;
			ImGui::EndPopup();
		}

		bool changed = false;
		if (ImGui::BeginDragDropTarget())
		{
			if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("CONTENT_BROWSER_ITEM"))
			{
				Asset* payloadAsset = (Asset*)payload->Data;
				if (!payloadAsset)
				{
					HW_CORE_WARN("File is not a valid Perplex Asset!");
				}
				else if (payloadAsset->GetType() != type)
				{
					HW_CORE_WARN("{0} does not match the field's asset type!", payloadAsset->GetName().string());
				}
				else
				{
					const std::string& pathString = payloadAsset->GetName().string();
					valueLabel = pathString;

					asset = *payloadAsset;
					changed = true;
				}
			}
			ImGui::EndDragDropTarget();
		}

		ImGui::SameLine();
		ImGui::Text(label);

		ImGui::PopID();

		if (reset)
		{
			asset = Asset();
			changed = true;
		}

		return changed;
	}

	bool DrawOptions(const char* label, int& current, Option options[], size_t optionCount)
	{
		Option* currentOption{};

		// find current
		for (size_t i{}; i < optionCount; i++)
		{
			if (current == options[i].Value)
				currentOption = options + i;
		}

		if (ImGui::BeginCombo(label, currentOption->Label))
		{
			for (size_t i{}; i < optionCount; i++)
			{
				bool isSelected = current == options[i].Value;
				if (ImGui::Selectable(options[i].Label, isSelected))
					current = options[i].Value;

				if (isSelected)
					ImGui::SetItemDefaultFocus();
			}

			ImGui::EndCombo();
		}

		return current != currentOption->Value;
	}

	void DrawVec3Control(const std::string& label, glm::vec3& values, float resetValue, float columnWidth)
	{
		ImGui::PushID(label.c_str());

		ImGui::Columns(2);
		ImGui::SetColumnWidth(0, columnWidth);
		ImGui::Text(label.c_str());
		ImGui::NextColumn();

		ImGui::PushMultiItemsWidths(3, ImGui::CalcItemWidth());
		ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2{ 0, 0 });

		float lineHeight = GImGui->Font->LegacySize + GImGui->Style.FramePadding.y * 2.0f;
		ImVec2 buttonSize = { lineHeight + 3.0f, lineHeight };

		ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.6f, 0.1f, 0.1f, 1.0f));
		ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0.7f, 0.2f, 0.2f, 1.0f));
		ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(0.6f, 0.1f, 0.1f, 1.0f));
		if (ImGui::Button("X", buttonSize))
			values.x = resetValue;
		ImGui::PopStyleColor(3);

		ImGui::SameLine();
		ImGui::DragFloat("##X", &values.x, 0.1f, 0.0f, 0.0f, "%.2f");
		ImGui::PopItemWidth();
		ImGui::SameLine();

		ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.2f, 0.5f, 0.3f, 1.0f));
		ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0.3f, 0.6f, 0.3f, 1.0f));
		ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(0.2f, 0.5f, 0.3f, 1.0f));
		if (ImGui::Button("Y", buttonSize))
			values.x = resetValue;
		ImGui::PopStyleColor(3);

		ImGui::SameLine();
		ImGui::DragFloat("##Y", &values.y, 0.1f, 0.0f, 0.0f, "%.2f");
		ImGui::PopItemWidth();
		ImGui::SameLine();

		ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.1f, 0.1f, 0.6f, 1.0f));
		ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0.2f, 0.2f, 0.7f, 1.0f));
		ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(0.1f, 0.1f, 0.6f, 1.0f));
		if (ImGui::Button("Z", buttonSize))
			values.x = resetValue;
		ImGui::PopStyleColor(3);

		ImGui::SameLine();
		ImGui::DragFloat("##Z", &values.z, 0.1f, 0.0f, 0.0f, "%.2f");
		ImGui::PopItemWidth();
		ImGui::SameLine();

		ImGui::PopStyleVar();

		ImGui::Columns(1);

		ImGui::PopID();
	}

	bool DrawEntityField(const char* label, EntityData& entityData)
	{
		bool changed = false;

		ImGui::Text(label);

		ImGui::SameLine();

		const char* name = entityData.Tag != "" ? entityData.Tag.c_str() : "NULL ENTITY";
		ImGui::Button(name, { 200, 20 });

		if (ImGui::BeginDragDropTarget())
		{
			if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("SCENE_HIERARCHY_ITEM"))
			{
				Entity payloadEntity = *(Entity*)payload->Data;

				if (payloadEntity)
				{
					entityData.ID = payloadEntity.GetUUID();
					entityData.Tag = payloadEntity.GetTag();

					changed = true;
				}
			}
			ImGui::EndDragDropTarget();
		}

		return changed;
	}
}