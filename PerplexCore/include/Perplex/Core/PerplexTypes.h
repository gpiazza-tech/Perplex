#pragma once

#include <cstdint>
#include <string>

namespace Perplex
{
	enum class PerplexTypes : uint8_t
	{
		None = 0,
		Int,
		Float,
		Double,
		Bool,
		Vec3,
		Entity,
		Asset,
		PerplexObject
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
			else if (cType == "Vec3")
				return PerplexTypes::Vec3;
			else if (cType == "Entity")
				return PerplexTypes::Entity;
			else if (cType == "Asset")
				return PerplexTypes::Asset;
			else 
				return PerplexTypes::None;
		}
	};
}