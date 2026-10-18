#pragma once

class asIScriptEngine;

namespace Perplex
{
	static const char* PX_TYPESTR_VOID = "void";
	static const char* PX_TYPESTR_BOOL = "bool";
	static const char* PX_TYPESTR_INT8 = "int8";
	static const char* PX_TYPESTR_INT16 = "int16";
	static const char* PX_TYPESTR_INT32 = "int32";
	static const char* PX_TYPESTR_INT64 = "int64";
	static const char* PX_TYPESTR_UINT8 = "uint8";
	static const char* PX_TYPESTR_UINT16 = "uint16";
	static const char* PX_TYPESTR_UINT32 = "uint32";
	static const char* PX_TYPESTR_UINT64 = "uint64";
	static const char* PX_TYPESTR_FLOAT = "float";
	static const char* PX_TYPESTR_DOUBLE = "double";

	class AngelUtils
	{
	public:
		static const char* AngelTypeToString(asIScriptEngine* engine, int typeID);
		static int StringToAngelType(asIScriptEngine* engine, const char* typeStr);
		static size_t GetAngelTypeSize(asIScriptEngine* engine, int typeID);
	};
}