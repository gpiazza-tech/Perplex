#include <Perplex/pch.h>
#include <Perplex/ImGui/PrimitiveDrawers.h>

#include <glm/fwd.hpp>

#include <imgui.h>
#include <string>

namespace Perplex
{
	constexpr const char* format = "%.2f";
	constexpr float speed = 0.1f;
	constexpr float min = 0.0f;
	constexpr float max = 0.0f;

	void Draw(int& value, const char* label)
	{
		ImGui::DragInt(label, &value);
	}

	void Draw(float& value, const char* label)
	{
		ImGui::DragFloat(label, &value, speed, min, max, format);
	}

	void Draw(double& value, const char* label)
	{
		constexpr double step = 0.0;
		constexpr double stepFast = 0.0;

		ImGui::InputDouble(label, &value, step, stepFast, format);
	}

	void Draw(bool& value, const char* label)
	{
		ImGui::Checkbox(label, &value);
	}

	void Draw(std::string& value, const char* label)
	{
		constexpr size_t maxChars = 50;
		char buf[maxChars];

		ImGui::InputText(label, buf, maxChars);
	}

	void Draw(glm::vec2& value, const char* label)
	{
		ImGui::DragFloat2(label, &value.x, speed, min, max, format);
	}

	void Draw(glm::vec3& value, const char* label)
	{
		ImGui::DragFloat3(label, &value.x, speed, min, max, format);
	}
}