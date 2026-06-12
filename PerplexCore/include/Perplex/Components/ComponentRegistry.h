#pragma once

#include <vector>

namespace Perplex
{
	class ComponentKind;

	class ComponentRegistry
	{
	public:
		static const std::vector<ComponentKind> GetBaseKinds();
		static const std::vector<ComponentKind> GetAdditiveKinds();

		static const std::vector<ComponentKind> GetAllKinds();
	};
}