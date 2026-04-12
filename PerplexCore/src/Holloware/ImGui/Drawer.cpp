#include "pch.h"
#include "Drawer.h"

#include "Holloware/Core/Core.h"
#include "Holloware/Core/HollowareTypes.h"
#include "Holloware/Core/HollowareObject.h"

#include <imgui.h>
#include <glm/fwd.hpp>
#include <glm/gtc/type_ptr.inl>

namespace Holloware
{
	void Drawer::DrawType(void* value, HollowareTypes type)
	{
		switch (type)
		{
		case Holloware::HollowareTypes::Int:
			Draw(*(int*)value);
			break;
		case Holloware::HollowareTypes::Float:
			Draw(*(float*)value);
			break;
		case Holloware::HollowareTypes::Bool:
			Draw(*(bool*)value);
			break;
		case Holloware::HollowareTypes::Vec3:
			Draw(*(glm::vec3*)value);
			break;
		case Holloware::HollowareTypes::HollowareObject:
			Draw((HollowareObject*)value);
			break;
		default:
			HW_CORE_ASSERT(false, "HollowareType {0} not supported!", type);
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

	void Drawer::Draw(HollowareObject* value)
	{
		value->DrawGui();
	}
}