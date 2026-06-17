#pragma once

#include "GuiSelection.h"
#include <Perplex/Assets/Asset.h>
#include <Perplex/Assets/AssetType.h>

#include <glm/fwd.hpp>

#include <string>
#include <memory>
#include <vector>
#include <type_traits>
#include <format>

namespace Perplex
{
	bool Draw(int& value, const char* label = "", int min = 0, int max = 0);
	bool Draw(float& value, const char* label = "");
	bool Draw(double& value, const char* label = "");
	bool Draw(bool& value, const char* label = "");
	bool Draw(std::string& value, const char* label = "", bool editable = true);
	bool Draw(glm::vec2& value, const char* label = "");
	bool Draw(glm::vec3& value, const char* label = "");
	bool Draw(glm::vec4& value, const char* label = "");
	bool DrawColor(glm::vec4& value, const char* label = "");

	bool BeginDropdown(const char* label = "");
	void EndDropdown();
	void SameLine();
	void DrawSpace(float space);

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