#pragma once

#include <vector>

namespace Perplex
{
	class Component;

	class ComponentRegistry
	{
		static const std::vector<Component>& GetAll();
	};
}