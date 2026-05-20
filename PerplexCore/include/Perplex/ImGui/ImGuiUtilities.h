#pragma once

#include <glm/fwd.hpp>

#include <any>
#include <string>

namespace std { typedef basic_string<char> string; }

namespace Perplex
{
	class Entity;
	class Asset;
	struct EntityData;

	class ImGuiUtilities 
	{
	public:
		static void SetGlobalStyles();
	};
}