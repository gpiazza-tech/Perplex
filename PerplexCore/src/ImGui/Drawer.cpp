#include <Perplex/pch.h>
#include <Perplex/ImGui/Drawer.h>

#include <Perplex/Core/Core.h>
#include <Perplex/Core/Log.h>
#include <Perplex/Core/PerplexTypes.h>
#include <Perplex/Assets/Asset.h>
#include <Perplex/Assets/AssetType.h>

#include <imgui.h>
#include <glm/fwd.hpp>
#include <glm/gtc/type_ptr.inl>

namespace Perplex
{
	void Drawer::DrawType(void* value, PerplexTypes type)
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
		case Perplex::PerplexTypes::Vec3:
			Draw(*(glm::vec3*)value);
			break;
		default:
			HW_CORE_ASSERT(false, "Perplex Type {0} not supported!", type);
		}
	}

	void Drawer::Draw(float& value)
	{
		ImGui::DragFloat("", &value, 0.1f, 0.0f, 0.0f, "%.2f");
	}

	void Drawer::Draw(int& value)
	{
		ImGui::DragInt("", &value);
	}

	void Drawer::Draw(bool& value)
	{
		ImGui::Checkbox("", &value);
	}

	void Drawer::Draw(glm::vec3& value)
	{
		float valArray[3] = { value.x, value.y, value.z };
		ImGui::DragFloat3("", valArray, 0.1f, 0.0f, 0.0f, "%.2f");
		value = glm::make_vec3(valArray);
	}

	bool Drawer::DrawAssetField(const char* label, Asset& asset, AssetType type)
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

	bool Drawer::DrawOptions(const char* label, int& current, Option options[], size_t optionCount)
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
}