#include <Perplex/pch.h>
#include <Perplex/Angel/AngelAPIBinder.h>

#include <Perplex/Scene/Entity.h>

#include <angelscript.h>
#include <Perplex/Core/KeyCodes.h>

namespace Perplex
{
	static void Info(const std::string& msg) { HW_INFO(msg); }
	static void Warn(const std::string& msg) { HW_WARN(msg); }
	static void Error(const std::string& msg) { HW_ERROR(msg); }
	static void Trace(const std::string& msg) { HW_TRACE(msg); }

	static glm::vec3& Entity_Position(Entity* entity) { return entity->GetComponent<TransformComponent>().Position; }
	static bool Entity_Valid(Entity* entity) { return entity->operator bool(); }
	static std::string Entity_Tag(Entity* entity) { return entity->GetComponent<TagComponent>().Tag; }

	static glm::vec2 Vec2_opAdd(const glm::vec2& a, const glm::vec2& b) { return a + b; };
	static glm::vec2 Vec2_opSub(const glm::vec2& a, const glm::vec2& b) { return a - b; };
	static glm::vec2 Vec2_opMul(const glm::vec2& a, float b) { return a * b; };
	static glm::vec2 Vec2_opMul_r(float a, const glm::vec2& b) { return a * b; };
	static glm::vec2 Vec2_opDiv(const glm::vec2& a, float b) { return a / b; };
	static glm::vec2 Vec2_opDiv_r(float a, const glm::vec2& b) { return a / b; };

	static glm::vec3 Vec3_opAdd(const glm::vec3& a, const glm::vec3& b) { return a + b; };
	static glm::vec3 Vec3_opSub(const glm::vec3& a, const glm::vec3& b) { return a - b; };
	static glm::vec3 Vec3_opMul(const glm::vec3& a, float b) { return a * b; };
	static glm::vec3 Vec3_opMul_r(float a, const glm::vec3& b) { return a * b; };
	static glm::vec3 Vec3_opDiv(const glm::vec3& a, float b) { return a / b; };
	static glm::vec3 Vec3_opDiv_r(float a, const glm::vec3& b) { return a / b; };

	static glm::vec4 Vec4_opAdd(const glm::vec4& a, const glm::vec4& b) { return a + b; };
	static glm::vec4 Vec4_opSub(const glm::vec4& a, const glm::vec4& b) { return a - b; };
	static glm::vec4 Vec4_opMul(const glm::vec4& a, float b) { return a * b; };
	static glm::vec4 Vec4_opMul_r(float a, const glm::vec4& b) { return a * b; };
	static glm::vec4 Vec4_opDiv(const glm::vec4& a, float b) { return a / b; };
	static glm::vec4 Vec4_opDiv_r(float a, const glm::vec4& b) { return a / b; };



	void AngelAPIBinder::BindAPI(asIScriptEngine* engine)
	{
		int r;

		r = engine->RegisterGlobalFunction("void Info(const string& in)", asFUNCTION(Info), asCALL_CDECL); HW_CORE_ASSERT(r >= 0, "Failed to perform register!");
		r = engine->RegisterGlobalFunction("void Warn(const string& in)", asFUNCTION(Warn), asCALL_CDECL); HW_CORE_ASSERT(r >= 0, "Failed to perform register!");
		r = engine->RegisterGlobalFunction("void Error(const string& in)", asFUNCTION(Error), asCALL_CDECL); HW_CORE_ASSERT(r >= 0, "Failed to perform register!");
		r = engine->RegisterGlobalFunction("void Trace(const string& in)", asFUNCTION(Trace), asCALL_CDECL); HW_CORE_ASSERT(r >= 0, "Failed to perform register!");

		// Vec2
		r = engine->RegisterObjectType("Vec2", sizeof(glm::vec2), asOBJ_VALUE | asOBJ_POD | asOBJ_APP_CLASS_ALLFLOATS | asOBJ_APP_CLASS_C); HW_CORE_ASSERT(r >= 0, "Failed to perform register!");
		r = engine->RegisterObjectProperty("Vec2", "float x", asOFFSET(glm::vec2, x)); HW_CORE_ASSERT(r >= 0, "Failed to perform register!"); HW_CORE_ASSERT(r >= 0, "Failed to perform register!");
		r = engine->RegisterObjectProperty("Vec2", "float y", asOFFSET(glm::vec2, y)); HW_CORE_ASSERT(r >= 0, "Failed to perform register!"); HW_CORE_ASSERT(r >= 0, "Failed to perform register!");
		r = engine->RegisterObjectMethod("Vec2", "Vec2 opAdd(Vec2)", asFUNCTIONPR(Vec2_opAdd, (const glm::vec2&, const glm::vec2&), glm::vec2), asCALL_CDECL_OBJFIRST); HW_CORE_ASSERT(r >= 0, "Failed to perform register!");
		r = engine->RegisterObjectMethod("Vec2", "Vec2 opSub(Vec2)", asFUNCTIONPR(Vec2_opSub, (const glm::vec2&, const glm::vec2&), glm::vec2), asCALL_CDECL_OBJFIRST); HW_CORE_ASSERT(r >= 0, "Failed to perform register!");
		r = engine->RegisterObjectMethod("Vec2", "Vec2 opMul(float)", asFUNCTIONPR(Vec2_opMul, (const glm::vec2&, float), glm::vec2), asCALL_CDECL_OBJFIRST); HW_CORE_ASSERT(r >= 0, "Failed to perform register!");
		r = engine->RegisterObjectMethod("Vec2", "Vec2 opMul_r(float)", asFUNCTIONPR(Vec2_opMul_r, (float, const glm::vec2&), glm::vec2), asCALL_CDECL_OBJLAST); HW_CORE_ASSERT(r >= 0, "Failed to perform register!");
		r = engine->RegisterObjectMethod("Vec2", "Vec2 opDiv(float)", asFUNCTIONPR(Vec2_opDiv, (const glm::vec2&, float), glm::vec2), asCALL_CDECL_OBJFIRST); HW_CORE_ASSERT(r >= 0, "Failed to perform register!");
		r = engine->RegisterObjectMethod("Vec2", "Vec2 opDiv_r(float)", asFUNCTIONPR(Vec2_opDiv_r, (float, const glm::vec2&), glm::vec2), asCALL_CDECL_OBJLAST); HW_CORE_ASSERT(r >= 0, "Failed to perform register!");

		// Vec3
		r = engine->RegisterObjectType("Vec3", sizeof(glm::vec3), asOBJ_VALUE | asOBJ_POD | asOBJ_APP_CLASS_ALLFLOATS | asOBJ_APP_CLASS_C); HW_CORE_ASSERT(r >= 0, "Failed to perform register!");
		r = engine->RegisterObjectProperty("Vec3", "float x", asOFFSET(glm::vec3, x)); HW_CORE_ASSERT(r >= 0, "Failed to perform register!"); HW_CORE_ASSERT(r >= 0, "Failed to perform register!");
		r = engine->RegisterObjectProperty("Vec3", "float y", asOFFSET(glm::vec3, y)); HW_CORE_ASSERT(r >= 0, "Failed to perform register!"); HW_CORE_ASSERT(r >= 0, "Failed to perform register!");
		r = engine->RegisterObjectProperty("Vec3", "float z", asOFFSET(glm::vec3, z)); HW_CORE_ASSERT(r >= 0, "Failed to perform register!"); HW_CORE_ASSERT(r >= 0, "Failed to perform register!");
		r = engine->RegisterObjectMethod("Vec3", "Vec3 opAdd(Vec3)", asFUNCTIONPR(Vec3_opAdd, (const glm::vec3&, const glm::vec3&), glm::vec3), asCALL_CDECL_OBJFIRST); HW_CORE_ASSERT(r >= 0, "Failed to perform register!");
		r = engine->RegisterObjectMethod("Vec3", "Vec3 opSub(Vec3)", asFUNCTIONPR(Vec3_opSub, (const glm::vec3&, const glm::vec3&), glm::vec3), asCALL_CDECL_OBJFIRST); HW_CORE_ASSERT(r >= 0, "Failed to perform register!");
		r = engine->RegisterObjectMethod("Vec3", "Vec3 opMul(float)", asFUNCTIONPR(Vec3_opMul, (const glm::vec3&, float), glm::vec3), asCALL_CDECL_OBJFIRST); HW_CORE_ASSERT(r >= 0, "Failed to perform register!");
		r = engine->RegisterObjectMethod("Vec3", "Vec3 opMul_r(float)", asFUNCTIONPR(Vec3_opMul_r, (float, const glm::vec3&), glm::vec3), asCALL_CDECL_OBJLAST); HW_CORE_ASSERT(r >= 0, "Failed to perform register!");
		r = engine->RegisterObjectMethod("Vec3", "Vec3 opDiv(float)", asFUNCTIONPR(Vec3_opDiv, (const glm::vec3&, float), glm::vec3), asCALL_CDECL_OBJFIRST); HW_CORE_ASSERT(r >= 0, "Failed to perform register!");
		r = engine->RegisterObjectMethod("Vec3", "Vec3 opDiv_r(float)", asFUNCTIONPR(Vec3_opDiv_r, (float, const glm::vec3&), glm::vec3), asCALL_CDECL_OBJLAST); HW_CORE_ASSERT(r >= 0, "Failed to perform register!");

		// Vec4
		r = engine->RegisterObjectType("Vec4", sizeof(glm::vec4), asOBJ_VALUE | asOBJ_POD | asOBJ_APP_CLASS_ALLFLOATS | asOBJ_APP_CLASS_C); HW_CORE_ASSERT(r >= 0, "Failed to perform register!");
		r = engine->RegisterObjectProperty("Vec4", "float x", asOFFSET(glm::vec4, x)); HW_CORE_ASSERT(r >= 0, "Failed to perform register!"); HW_CORE_ASSERT(r >= 0, "Failed to perform register!");
		r = engine->RegisterObjectProperty("Vec4", "float y", asOFFSET(glm::vec4, y)); HW_CORE_ASSERT(r >= 0, "Failed to perform register!"); HW_CORE_ASSERT(r >= 0, "Failed to perform register!");
		r = engine->RegisterObjectProperty("Vec4", "float z", asOFFSET(glm::vec4, z)); HW_CORE_ASSERT(r >= 0, "Failed to perform register!"); HW_CORE_ASSERT(r >= 0, "Failed to perform register!");
		r = engine->RegisterObjectProperty("Vec4", "float w", asOFFSET(glm::vec4, w)); HW_CORE_ASSERT(r >= 0, "Failed to perform register!"); HW_CORE_ASSERT(r >= 0, "Failed to perform register!");
		r = engine->RegisterObjectMethod("Vec4", "Vec4 opAdd(Vec4)", asFUNCTIONPR(Vec4_opAdd, (const glm::vec4&, const glm::vec4&), glm::vec4), asCALL_CDECL_OBJFIRST); HW_CORE_ASSERT(r >= 0, "Failed to perform register!");
		r = engine->RegisterObjectMethod("Vec4", "Vec4 opSub(Vec4)", asFUNCTIONPR(Vec4_opSub, (const glm::vec4&, const glm::vec4&), glm::vec4), asCALL_CDECL_OBJFIRST); HW_CORE_ASSERT(r >= 0, "Failed to perform register!");
		r = engine->RegisterObjectMethod("Vec4", "Vec4 opMul(float)", asFUNCTIONPR(Vec4_opMul, (const glm::vec4&, float), glm::vec4), asCALL_CDECL_OBJFIRST); HW_CORE_ASSERT(r >= 0, "Failed to perform register!");
		r = engine->RegisterObjectMethod("Vec4", "Vec4 opMul_r(float)", asFUNCTIONPR(Vec4_opMul_r, (float, const glm::vec4&), glm::vec4), asCALL_CDECL_OBJLAST); HW_CORE_ASSERT(r >= 0, "Failed to perform register!");
		r = engine->RegisterObjectMethod("Vec4", "Vec4 opDiv(float)", asFUNCTIONPR(Vec4_opDiv, (const glm::vec4&, float), glm::vec4), asCALL_CDECL_OBJFIRST); HW_CORE_ASSERT(r >= 0, "Failed to perform register!");
		r = engine->RegisterObjectMethod("Vec4", "Vec4 opDiv_r(float)", asFUNCTIONPR(Vec4_opDiv_r, (float, const glm::vec4&), glm::vec4), asCALL_CDECL_OBJLAST); HW_CORE_ASSERT(r >= 0, "Failed to perform register!");

		// Input
		r = engine->RegisterEnum("KeyCode"); HW_CORE_ASSERT(r >= 0, "Failed to perform register!");
		r = engine->RegisterEnumValue("KeyCode", "KEY_0", HW_KEY_0); HW_CORE_ASSERT(r >= 0, "Failed to perform register!");
		r = engine->RegisterEnumValue("KeyCode", "KEY_1", HW_KEY_1); HW_CORE_ASSERT(r >= 0, "Failed to perform register!");
		r = engine->RegisterEnumValue("KeyCode", "KEY_2", HW_KEY_2); HW_CORE_ASSERT(r >= 0, "Failed to perform register!");
		r = engine->RegisterEnumValue("KeyCode", "KEY_3", HW_KEY_3); HW_CORE_ASSERT(r >= 0, "Failed to perform register!");
		r = engine->RegisterEnumValue("KeyCode", "KEY_4", HW_KEY_4); HW_CORE_ASSERT(r >= 0, "Failed to perform register!");
		r = engine->RegisterEnumValue("KeyCode", "KEY_5", HW_KEY_5); HW_CORE_ASSERT(r >= 0, "Failed to perform register!");
		r = engine->RegisterEnumValue("KeyCode", "KEY_6", HW_KEY_6); HW_CORE_ASSERT(r >= 0, "Failed to perform register!");
		r = engine->RegisterEnumValue("KeyCode", "KEY_7", HW_KEY_7); HW_CORE_ASSERT(r >= 0, "Failed to perform register!");
		r = engine->RegisterEnumValue("KeyCode", "KEY_8", HW_KEY_8); HW_CORE_ASSERT(r >= 0, "Failed to perform register!");
		r = engine->RegisterEnumValue("KeyCode", "KEY_9", HW_KEY_9); HW_CORE_ASSERT(r >= 0, "Failed to perform register!");
		r = engine->RegisterEnumValue("KeyCode", "KEY_A", HW_KEY_A); HW_CORE_ASSERT(r >= 0, "Failed to perform register!");
		r = engine->RegisterEnumValue("KeyCode", "KEY_B", HW_KEY_B); HW_CORE_ASSERT(r >= 0, "Failed to perform register!");
		r = engine->RegisterEnumValue("KeyCode", "KEY_C", HW_KEY_C); HW_CORE_ASSERT(r >= 0, "Failed to perform register!");
		r = engine->RegisterEnumValue("KeyCode", "KEY_D", HW_KEY_D); HW_CORE_ASSERT(r >= 0, "Failed to perform register!");
		r = engine->RegisterEnumValue("KeyCode", "KEY_E", HW_KEY_E); HW_CORE_ASSERT(r >= 0, "Failed to perform register!");
		r = engine->RegisterEnumValue("KeyCode", "KEY_F", HW_KEY_F); HW_CORE_ASSERT(r >= 0, "Failed to perform register!");
		r = engine->RegisterEnumValue("KeyCode", "KEY_G", HW_KEY_G); HW_CORE_ASSERT(r >= 0, "Failed to perform register!");
		r = engine->RegisterEnumValue("KeyCode", "KEY_H", HW_KEY_H); HW_CORE_ASSERT(r >= 0, "Failed to perform register!");
		r = engine->RegisterEnumValue("KeyCode", "KEY_I", HW_KEY_I); HW_CORE_ASSERT(r >= 0, "Failed to perform register!");
		r = engine->RegisterEnumValue("KeyCode", "KEY_J", HW_KEY_J); HW_CORE_ASSERT(r >= 0, "Failed to perform register!");
		r = engine->RegisterEnumValue("KeyCode", "KEY_K", HW_KEY_K); HW_CORE_ASSERT(r >= 0, "Failed to perform register!");
		r = engine->RegisterEnumValue("KeyCode", "KEY_L", HW_KEY_L); HW_CORE_ASSERT(r >= 0, "Failed to perform register!");
		r = engine->RegisterEnumValue("KeyCode", "KEY_M", HW_KEY_M); HW_CORE_ASSERT(r >= 0, "Failed to perform register!");
		r = engine->RegisterEnumValue("KeyCode", "KEY_N", HW_KEY_N); HW_CORE_ASSERT(r >= 0, "Failed to perform register!");
		r = engine->RegisterEnumValue("KeyCode", "KEY_O", HW_KEY_O); HW_CORE_ASSERT(r >= 0, "Failed to perform register!");
		r = engine->RegisterEnumValue("KeyCode", "KEY_P", HW_KEY_P); HW_CORE_ASSERT(r >= 0, "Failed to perform register!");
		r = engine->RegisterEnumValue("KeyCode", "KEY_Q", HW_KEY_Q); HW_CORE_ASSERT(r >= 0, "Failed to perform register!");
		r = engine->RegisterEnumValue("KeyCode", "KEY_R", HW_KEY_R); HW_CORE_ASSERT(r >= 0, "Failed to perform register!");
		r = engine->RegisterEnumValue("KeyCode", "KEY_S", HW_KEY_S); HW_CORE_ASSERT(r >= 0, "Failed to perform register!");
		r = engine->RegisterEnumValue("KeyCode", "KEY_T", HW_KEY_T); HW_CORE_ASSERT(r >= 0, "Failed to perform register!");
		r = engine->RegisterEnumValue("KeyCode", "KEY_U", HW_KEY_U); HW_CORE_ASSERT(r >= 0, "Failed to perform register!");
		r = engine->RegisterEnumValue("KeyCode", "KEY_V", HW_KEY_V); HW_CORE_ASSERT(r >= 0, "Failed to perform register!");
		r = engine->RegisterEnumValue("KeyCode", "KEY_W", HW_KEY_W); HW_CORE_ASSERT(r >= 0, "Failed to perform register!");
		r = engine->RegisterEnumValue("KeyCode", "KEY_X", HW_KEY_X); HW_CORE_ASSERT(r >= 0, "Failed to perform register!");
		r = engine->RegisterEnumValue("KeyCode", "KEY_Y", HW_KEY_Y); HW_CORE_ASSERT(r >= 0, "Failed to perform register!");
		r = engine->RegisterEnumValue("KeyCode", "KEY_Z", HW_KEY_Z); HW_CORE_ASSERT(r >= 0, "Failed to perform register!");

		r = engine->RegisterEnum("MouseButtonCode"); HW_CORE_ASSERT(r >= 0, "Failed to perform register!");
		r = engine->RegisterEnumValue("MouseButtonCode", "MOUSE_BUTTON_LEFT", HW_MOUSE_BUTTON_LEFT); HW_CORE_ASSERT(r >= 0, "Failed to perform register!");
		r = engine->RegisterEnumValue("MouseButtonCode", "MOUSE_BUTTON_MIDDLE", HW_MOUSE_BUTTON_MIDDLE); HW_CORE_ASSERT(r >= 0, "Failed to perform register!");
		r = engine->RegisterEnumValue("MouseButtonCode", "MOUSE_BUTTON_RIGHT", HW_MOUSE_BUTTON_RIGHT); HW_CORE_ASSERT(r >= 0, "Failed to perform register!");

		r = engine->RegisterGlobalFunction("bool KeyPressed(KeyCode)", asFUNCTION(Input::IsKeyPressed), asCALL_CDECL); HW_CORE_ASSERT(r >= 0, "Failed to perform register!");
		r = engine->RegisterGlobalFunction("bool MouseButtonPressed(MouseButtonCode)", asFUNCTION(Input::IsMouseButtonPressed), asCALL_CDECL); HW_CORE_ASSERT(r >= 0, "Failed to perform register!");

		r = engine->RegisterGlobalFunction("Vec2 MouseScreenPosition()", asFUNCTION(Input::GetMousePosition), asCALL_CDECL); HW_CORE_ASSERT(r >= 0, "Failed to perform register!");
		r = engine->RegisterGlobalFunction("Vec2 MouseWorldPosition()", asFUNCTION(Input::GetMouseWorldPosition), asCALL_CDECL); HW_CORE_ASSERT(r >= 0, "Failed to perform register!");
		r = engine->RegisterGlobalFunction("Vec2 MouseDelta()", asFUNCTION(Input::GetMouseDelta), asCALL_CDECL); HW_CORE_ASSERT(r >= 0, "Failed to perform register!"); HW_CORE_ASSERT(r >= 0, "Failed to perform register!");

		// Scene
		r = engine->RegisterObjectType("Scene", sizeof(Scene), asOBJ_VALUE | asOBJ_POD | asOBJ_APP_CLASS); HW_CORE_ASSERT(r >= 0, "Failed to perform register!");

		// Entity
		r = engine->RegisterObjectType("Entity", sizeof(Entity), asOBJ_VALUE | asOBJ_POD | asOBJ_APP_CLASS); HW_CORE_ASSERT(r >= 0, "Failed to perform register!");
		// r = engine->RegisterObjectMethod("Entity", "Scene@ Scene()", asMETHOD(Entity, GetScene), asCALL_THISCALL); HW_CORE_ASSERT(r >= 0, "Failed to perform register!"); HW_CORE_ASSERT(r >= 0, "Failed to perform register!");
		r = engine->RegisterObjectMethod("Entity", "string Tag()", asFUNCTION(Entity_Tag), asCALL_CDECL_OBJFIRST); HW_CORE_ASSERT(r >= 0, "Failed to perform register!");
		r = engine->RegisterObjectMethod("Entity", "Vec3& Position()", asFUNCTION(Entity_Position), asCALL_CDECL_OBJFIRST); HW_CORE_ASSERT(r >= 0, "Failed to perform register!");
		r = engine->RegisterObjectMethod("Entity", "bool Valid()", asFUNCTION(Entity_Valid), asCALL_CDECL_OBJFIRST); HW_CORE_ASSERT(r >= 0, "Failed to perform register!");
	}
}