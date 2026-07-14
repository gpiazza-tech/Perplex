#pragma once

#include <Perplex/ImGui/PrimitiveDrawers.h>
#include <Perplex/Core/PerplexTypes.h>

#include <c/perplex_math.h>

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

	inline void Draw(Bounds& bounds, const char* label)
	{
		Draw(bounds.CenterX, "Center X");
		Draw(bounds.CenterY, "Center Y");
		Draw(bounds.BoundsX, "Bounds X");
		Draw(bounds.BoundsY, "Bounds Y");
	}

	inline void Draw(Radius& radius, const char* label)
	{
		Draw(radius.CenterX, "Center X");
		Draw(radius.CenterY, "Center Y");
		Draw(radius.Radius, "Radius");
	}

	template<typename T>
	void DrawAny(std::any& value, const char* label = "")
	{
		T cpy = std::any_cast<T>(value);
		Draw(cpy, label);
		value = cpy;
	}

	bool DrawAssetVector(std::vector<Asset>& vec, const char* label, AssetType assetType);
}