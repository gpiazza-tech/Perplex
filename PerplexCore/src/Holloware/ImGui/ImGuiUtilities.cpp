#include "pch.h"
#include "ImGuiUtilities.h"

#include "Holloware/Core/UUID.h"
#include "Holloware/Scene/Components.h"
#include "Holloware/Scene/Entity.h"
#include "Holloware/Assets/Asset.h"

#include <glm/fwd.hpp>
#include <imgui.h>
#include <imgui_internal.h>

#include <any>
#include <string>

namespace Holloware
{
	void ImGuiUtilities::DrawVec3Control(const std::string& label, glm::vec3& values, float resetValue, float columnWidth)
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

	void ImGuiUtilities::DrawAnyIntControl(const char* label, std::any& val)
	{
		int buffer = std::any_cast<int>(val);
		ImGui::DragInt(label, &buffer);
		val = buffer;
	}

	void ImGuiUtilities::DrawAnyFloatControl(const char* label, std::any& val)
	{
		float buffer = std::any_cast<float>(val);
		ImGui::DragFloat(label, &buffer);
		val = buffer;
	}

	void ImGuiUtilities::DrawAnyDoubleControl(const char* label, std::any& val)
	{
		double buffer = std::any_cast<double>(val);
		ImGui::InputDouble(label, &buffer);
		val = buffer;
	}

	void ImGuiUtilities::DrawAnyBoolControl(const char* label, std::any& val)
	{
		bool buffer = std::any_cast<bool>(val);
		ImGui::Checkbox(label, &buffer);
		val = buffer;
	}

	void ImGuiUtilities::EntityInput(const char* label, EntityData& entityData)
	{
		// SCENE_HIERARCHY_ITEM

		ImGui::Text(label);

		ImGui::SameLine();

		const char* name = entityData.Tag != "" ? entityData.Tag.c_str() : "NULL ENTITY";
		ImGui::Button(name, {200, 20});

		if (ImGui::BeginDragDropTarget())
		{
			if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("SCENE_HIERARCHY_ITEM"))
			{
				Entity payloadEntity = *(Entity*)payload->Data;

				if (payloadEntity) 
				{
					entityData.ID = payloadEntity.GetUUID();
					entityData.Tag = payloadEntity.GetTag();
				}
			}
			ImGui::EndDragDropTarget();
		}
	}
}