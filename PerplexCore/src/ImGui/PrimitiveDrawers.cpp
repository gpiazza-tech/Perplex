#include <Perplex/pch.h>
#include <Perplex/ImGui/PrimitiveDrawers.h>

#include <glm/fwd.hpp>
#include <imgui.h>
#include <misc/cpp/imgui_stdlib.h>

#include <string>

namespace Perplex
{
	constexpr const char* format = "%.2f";
	constexpr float speed = 0.1f;
	constexpr float min = 0.0f;
	constexpr float max = 0.0f;

	bool Draw(int& value, const char* label, int min, int max)
	{
		return ImGui::DragInt(label, &value, speed, min, max);
	}

	bool Draw(float& value, const char* label)
	{
		return ImGui::DragFloat(label, &value, speed, min, max, format);
	}

	bool Draw(double& value, const char* label)
	{
		constexpr double step = 0.0;
		constexpr double stepFast = 0.0;

		return ImGui::InputDouble(label, &value, step, stepFast, format);
	}

	bool Draw(bool& value, const char* label)
	{
		return ImGui::Checkbox(label, &value);
	}

	bool Draw(std::string& value, const char* label, bool editable)
	{
		if (editable)
			return ImGui::InputText(label, &value);

		else
		{
			ImGui::Text(value.c_str());
			return false;
		}
	}

	bool Draw(glm::vec2& value, const char* label)
	{
		return ImGui::DragFloat2(label, &value.x, speed, min, max, format);
	}

	bool Draw(glm::vec3& value, const char* label)
	{
		return ImGui::DragFloat3(label, &value.x, speed, min, max, format);
	}

	bool Draw(glm::vec4& value, const char* label)
	{
		return ImGui::DragFloat4(label, &value.x, speed, min, max, format);
	}

	bool DrawColor(glm::vec4& value, const char* label)
	{
		return ImGui::ColorEdit4(label, &value.r);
	}

	bool DrawMultilineText(std::string& value, const char* label, float height)
	{
		ImVec2 size{ ImGui::GetContentRegionAvail().x - 50.0f, height };
		return ImGui::InputTextMultiline(label, &value, size);
	}

	bool BeginDropdown(const char* label)
	{
		return ImGui::TreeNodeEx(label, ImGuiTreeNodeFlags_DefaultOpen | ImGuiTreeNodeFlags_AllowOverlap, label);
	}

	void EndDropdown()
	{
		ImGui::TreePop();
	}

	void SameLine()
	{
		ImGui::SameLine();
	}

	void DrawSpace(float space)
	{
		ImGui::Dummy(ImVec2{ 0.0f, space });
	}
}