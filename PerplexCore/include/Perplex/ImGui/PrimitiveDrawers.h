#pragma once

#include <glm/fwd.hpp>

#include <string>
#include <memory>
#include <vector>
#include <type_traits>
#include <algorithm>
#include <utility>

namespace Perplex
{
	void Draw(int& value, const char* label = "");
	void Draw(float& value, const char* label = "");
	void Draw(double& value, const char* label = "");
	void Draw(bool& value, const char* label = "");
	void Draw(std::string& value, const char* label = "");
	void Draw(glm::vec2& value, const char* label = "");
	void Draw(glm::vec3& value, const char* label = "");

	/*
	template<typename T>
	void DrawSelected(std::vector<std::reference_wrapper<T>>& selections, const char* label = "")
	{
		if (selections.size() == 1)
		{
			Draw(selections.at(0).get(), label);
			return;
		}

		bool allEqual = true;
		for (auto& selection : selections)
		{
			if (selection.get() != selections.at(0).get())
				allEqual = false;
		}

		if (allEqual)
		{
			T temp{};
			Draw(temp, label);
			for (auto& selection : selections)
				selection.get() = temp;
		}

		else
		{
			// values are not equal; skip drawing
		}
	}

	template<typename StructT, typename MemT>
	std::unique_ptr<std::vector<std::reference_wrapper<MemT>>> SelectionsFromStruct(std::unique_ptr<std::vector<std::reference_wrapper<StructT>>>, size_t memberOffset)
	{
		using VectorMemT = std::vector<std::reference_wrapper<MemT>>;
		std::unique_ptr<VectorMemT> selections = std::make_unique<VectorMemT>();
		
		for (size_t i = 0; i < objects.size(); ++i)
		{
			selections->emplace_back((*(void*)(size_t)&(objects->at(i).get()) + memborOffet));
		}

		return std::move(selections);
	}
	*/
}