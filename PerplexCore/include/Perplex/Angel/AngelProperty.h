#pragma once

#include <string>
#include <vector>
#include <cstddef>

namespace Perplex
{
	struct AngelProperty
	{
		std::string Name{};
		std::string Type{};
		std::vector<std::byte> Data{};
	};
}