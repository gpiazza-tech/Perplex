#pragma once

#include <Perplex/ImGui/PrimitiveDrawers.h>
#include <Perplex/Core/PerplexTypes.h>

#include <cstdint>

namespace Perplex
{
	enum class AssetType : uint8_t;
	class Asset;
	struct EntityData;

	struct Option { const char* Label; int Value; };

	void DrawVec3Control(const std::string& label, glm::vec3& values, float resetValue = 0.0f, float columnWidth = 100.0f);
	void DrawType(void* value, PerplexTypes type);
	bool DrawOptions(const char* label, int& current, Option options[], size_t optionCount);
	bool DrawAssetField(const char* label, Asset& asset, AssetType type);
	bool DrawEntityField(const char* label, EntityData& entity);

	template<typename T>
	void DrawAny(std::any& value, const char* label = "")
	{
		T cpy = std::any_cast<T>(value);
		Draw(cpy, label);
		value = cpy;
	}
}