#pragma once

#include "GuiSelection.h"

#include <glm/fwd.hpp>

#include <string>
#include <vector>

namespace Perplex
{
	bool Draw(bool& value, const char* label = "");
	bool Draw(int8_t& value, const char* label = "", int min = 0, int max = 0);
	bool Draw(int16_t& value, const char* label = "", int min = 0, int max = 0);
	bool Draw(int32_t& value, const char* label = "", int min = 0, int max = 0);
	bool Draw(int64_t& value, const char* label = "", int min = 0, int max = 0);
	bool Draw(uint8_t& value, const char* label = "", int min = 0, int max = 0);
	bool Draw(uint16_t& value, const char* label = "", int min = 0, int max = 0);
	bool Draw(uint32_t& value, const char* label = "", int min = 0, int max = 0);
	bool Draw(uint64_t& value, const char* label = "", int min = 0, int max = 0);
	bool Draw(float& value, const char* label = "");
	bool Draw(double& value, const char* label = "");
	bool Draw(std::string& value, const char* label = "", bool editable = true);
	bool Draw(glm::vec2& value, const char* label = "");
	bool Draw(glm::vec3& value, const char* label = "");
	bool Draw(glm::vec4& value, const char* label = "");
	bool DrawColor(glm::vec4& value, const char* label = "");
	bool DrawMultilineText(std::string& value, const char* label, float height);

	bool BeginDropdown(const char* label = "");
	void EndDropdown();
	void SameLine();
	void DrawSpace(float space = 20.0f);

	template<typename T, typename Getter, typename Setter>
	bool Draw(Getter getter, Setter setter, const char* label = "")
	{
		T value = getter();
		bool changed = Draw(value, label);

		if (changed)
			setter(value);

		return changed;
	}

	template<typename T>
	bool DrawSelection(GuiSelection<T> selection, const char* label = "")
	{
		bool changed = false;

		if (selection.Synced())
		{
			T temp = *selection.begin();
			changed = Draw(temp, label);
			selection.SetAll(temp);
		}

		else
		{
			// values are not equal; skip drawing
		}

		return changed;
	}

	template<typename T>
	bool DrawSelection(GuiSelection<T> selection, bool (*drawer)(T& value))
	{
		bool changed = false;

		if (selection.Synced())
		{
			T temp = *selection.begin();
			changed = drawer(temp);
			selection.SetAll(temp);
		}

		else
		{
			// values are not equal; skip drawing
		}

		return changed;
	}
}