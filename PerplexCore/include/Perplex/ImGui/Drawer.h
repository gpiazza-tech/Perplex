#pragma once

#include <glm/fwd.hpp>
#include <Perplex/Core/PerplexTypes.h>

#include <cstdint>
#include <array>

namespace Perplex
{

	enum class AssetType : uint8_t;
	class Asset;

	struct Option { const char* Label; int Value; };

	class Drawer
	{
	public:
		static void DrawType(void* value, PerplexTypes type);
		static void Draw(float& value);
		static void Draw(int& value);
		static void Draw(bool& value);
		static void Draw(glm::vec3& value);

		static bool DrawAssetField(const char* label, Asset& asset, AssetType type);

		static bool DrawOptions(const char* label, int& current, Option options[], size_t optionCount);
	};
}