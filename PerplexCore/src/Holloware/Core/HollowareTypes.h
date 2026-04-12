#pragma once

#include <cstdint>
#include <string>

namespace Holloware
{
	enum class HollowareTypes : uint8_t
	{
		None = 0,
		Int,
		Float,
		Double,
		Bool,
		Vec3,
		Entity,
		Asset,
		HollowareObject
	};

	class HollowareTypesConversions
	{
	public:
		static HollowareTypes CToHwType(const std::string& cType)
		{
			if (cType == "int")
				return HollowareTypes::Int;
			else if (cType == "float")
				return HollowareTypes::Float;
			else if (cType == "double")
				return HollowareTypes::Double;
			else if (cType == "bool")
				return HollowareTypes::Bool;
			else if (cType == "Vec3")
				return HollowareTypes::Vec3;
			else if (cType == "Entity")
				return HollowareTypes::Entity;
			else if (cType == "Asset")
				return HollowareTypes::Asset;
			else 
				return HollowareTypes::None;
		}
	};
}