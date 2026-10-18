#include <Perplex/pch.h>
#include <Perplex/Angel/AngelUtils.h>

#include <Perplex/Core/Core.h>

#include <angelscript.h>

#include <string_view>

namespace Perplex
{
	const char* AngelUtils::AngelTypeToString(asIScriptEngine* engine, int typeID)
	{
		if ((typeID & asTYPEID_MASK_OBJECT) == 0)
		{
			switch (typeID)
			{
			case asTYPEID_VOID:
				return PX_TYPESTR_VOID;
			case asTYPEID_BOOL:
				return PX_TYPESTR_BOOL;
			case asTYPEID_INT8:
				return PX_TYPESTR_INT8;
			case asTYPEID_INT16:
				return PX_TYPESTR_INT16;
			case asTYPEID_INT32:
				return PX_TYPESTR_INT32;
			case asTYPEID_INT64:
				return PX_TYPESTR_INT64;
			case asTYPEID_UINT8:
				return PX_TYPESTR_UINT8;
			case asTYPEID_UINT16:
				return PX_TYPESTR_UINT16;
			case asTYPEID_UINT32:
				return PX_TYPESTR_UINT32;
			case asTYPEID_UINT64:
				return PX_TYPESTR_UINT64;
			case asTYPEID_FLOAT:
				return PX_TYPESTR_FLOAT;
			case asTYPEID_DOUBLE:
				return PX_TYPESTR_DOUBLE;
			default:
				HW_CORE_ASSERT(false, "Could not resolve type ID!");
				return "";
			}
		}
		else
		{
			asITypeInfo* propertyTypeInfo = engine->GetTypeInfoById(typeID);
			HW_CORE_ASSERT(propertyTypeInfo, "Could not resolve type ID!");

			return  propertyTypeInfo->GetName();
		}
	}

	int AngelUtils::StringToAngelType(asIScriptEngine* engine, const char* typeStr)
	{
		std::string_view typeStrView{ typeStr };

		if (typeStrView == PX_TYPESTR_VOID)
			return asTYPEID_VOID;
		if (typeStrView == PX_TYPESTR_BOOL)
			return asTYPEID_BOOL;
		if (typeStrView == PX_TYPESTR_INT8)
			return asTYPEID_INT8;
		if (typeStrView == PX_TYPESTR_INT16)
			return asTYPEID_INT16;
		if (typeStrView == PX_TYPESTR_INT32)
			return asTYPEID_INT32;
		if (typeStrView == PX_TYPESTR_INT64)
			return asTYPEID_INT64;
		if (typeStrView == PX_TYPESTR_UINT8)
			return asTYPEID_UINT8;
		if (typeStrView == PX_TYPESTR_UINT16)
			return asTYPEID_UINT16;
		if (typeStrView == PX_TYPESTR_UINT32)
			return asTYPEID_UINT32;
		if (typeStrView == PX_TYPESTR_UINT64)
			return asTYPEID_UINT64;
		if (typeStrView == PX_TYPESTR_FLOAT)
			return asTYPEID_FLOAT;
		if (typeStrView == PX_TYPESTR_DOUBLE)
			return asTYPEID_DOUBLE;
		
		asITypeInfo* typeInfo = engine->GetTypeInfoByName(typeStr);
		HW_CORE_ASSERT(typeInfo, "Could not resolve type string!");

		return typeInfo->GetTypeId();
	}

	size_t AngelUtils::GetAngelTypeSize(asIScriptEngine* engine, int typeID)
	{
		if ((typeID & asTYPEID_MASK_OBJECT) == 0)
		{
			switch (typeID)
			{
			case asTYPEID_VOID:
				return 0;
			case asTYPEID_BOOL:
				return sizeof(bool);
			case asTYPEID_INT8:
				return sizeof(int8_t);
			case asTYPEID_INT16:
				return sizeof(int16_t);
			case asTYPEID_INT32:
				return sizeof(int32_t);
			case asTYPEID_INT64:
				return sizeof(int64_t);
			case asTYPEID_UINT8:
				return sizeof(uint8_t);
			case asTYPEID_UINT16:
				return sizeof(uint16_t);
			case asTYPEID_UINT32:
				return sizeof(uint32_t);
			case asTYPEID_UINT64:
				return sizeof(uint64_t);
			case asTYPEID_FLOAT:
				return sizeof(float);
			case asTYPEID_DOUBLE:
				return sizeof(double);
			default:
				HW_CORE_ASSERT(false, "Could not resolve type ID!");
				return 0;
			}
		}
		else
		{
			asITypeInfo* propertyTypeInfo = engine->GetTypeInfoById(typeID);
			HW_CORE_ASSERT(propertyTypeInfo, "Could not resolve type ID!");
			
			return propertyTypeInfo->GetSize();
		}
	}
}