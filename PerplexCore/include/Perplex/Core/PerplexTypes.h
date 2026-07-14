#pragma once

#include <cstdint>
#include <string>

namespace Perplex
{
	enum class PerplexTypes : uint8_t
	{
		None = 0,
		Int = 1,
		Float = 2,
		Double = 3,
		Bool = 4,
		Vec2 = 5,
		Vec3 = 6,
		Entity = 7,
		Asset = 8,
		PerplexObject = 9,
		Bounds = 10,
		Radius = 11
	};

	class PerplexTypesConversions
	{
	public:
		static PerplexTypes CToHwType(const std::string& cType)
		{
			if (cType == "int")
				return PerplexTypes::Int;
			else if (cType == "float")
				return PerplexTypes::Float;
			else if (cType == "double")
				return PerplexTypes::Double;
			else if (cType == "bool")
				return PerplexTypes::Bool;
			else if (cType == "Vec2")
				return PerplexTypes::Vec2;
			else if (cType == "Vec3")
				return PerplexTypes::Vec3;
			else if (cType == "Entity")
				return PerplexTypes::Entity;
			else if (cType == "PrefabAsset")
				return PerplexTypes::Asset;
			else if (cType == "SceneAsset")
				return PerplexTypes::Asset;
			else if (cType == "Bounds")
				return PerplexTypes::Bounds;
			else if (cType == "Radius")
				return PerplexTypes::Radius;
			else 
				return PerplexTypes::None;
		}
	};
}