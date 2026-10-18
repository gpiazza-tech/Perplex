#pragma once

#include <Perplex/ImGui/PrimitiveDrawers.h>
#include <Perplex/Core/PerplexTypes.h>
#include <Perplex/Angel/AngelProperty.h>

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
	bool DrawAngelProperty(AngelProperty& value);

	inline bool Draw(Bounds& bounds, const char* label)
	{
		bool changed{ false };

		changed |= Draw(bounds.Center, "Center");
		changed |= Draw(bounds.Extents, "Extents");

		return changed;
	}

	inline bool Draw(Radius& radius, const char* label)
	{
		bool changed{ false };

		changed |= Draw(radius.CenterX, "Center X");
		changed |= Draw(radius.CenterY, "Center Y");
		changed |= Draw(radius.Radius, "Radius");

		return changed;
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