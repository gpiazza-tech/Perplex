#pragma once

#include <glm/fwd.hpp>

#include <cstdint>

namespace Holloware
{
	enum class HollowareTypes : uint8_t;
	class HollowareObject;

	class Drawer
	{
	public:
		static void DrawType(void* value, HollowareTypes type);
		static void Draw(float& value);
		static void Draw(int& value);
		static void Draw(bool& value);
		static void Draw(glm::vec3& value);
		static void Draw(HollowareObject* value);
	};
}