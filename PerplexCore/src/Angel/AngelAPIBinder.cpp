#include <Perplex/pch.h>
#include <Perplex/Angel/AngelAPIBinder.h>

#include <Perplex/Scene/Entity.h>
#include <Perplex/Perpixel/PerpixelInstance.h>
#include <Perplex/Perpixel/PerpixelSystem.h>
#include <Perplex/Core/KeyCodes.h>
#include <Perplex/Physics/Simulator.h>
#include <Perplex/Assets/Asset.h>
#include <Perplex/Components/Components.h>
#include <Perplex/Core/Core.h>
#include <Perplex/Core/Input.h>
#include <Perplex/Core/Log.h>
#include <Perplex/Core/MouseButtonCodes.h>
#include <Perplex/Perpixel/PerpixelSpawnShape.h>
#include <Perplex/Scene/Scene.h>
#include <Perplex/Scene/SceneManager.h>
#include <Perplex/Audio/AudioSystem.h>
#include <Perplex/Audio/AudioEngine.h>
#include <c/perplex_pixel.h>
#include <c/perplex_math.h>

#include <glm/fwd.hpp>

#include <angelscript.h>

#include <cmath>
#include <cstdint>
#include <cstdlib>
#include <string>
#include <Perplex/Physics/HitInfo.h>
#include <Perplex/Serialization/JsonStructDefs.h>
#include <Perplex/Serialization/JsonHelper.h>

namespace Perplex
{
	namespace Effects
	{
		static void CameraShake(float trauma)
		{
			SceneManager::Get().ActiveScene()->CameraShake(trauma);
		}
	}

	namespace Audio
	{
		static void Play(const std::string& path)
		{
			SceneManager::Get().ActiveScene()->GetSystem<AudioSystem>().PlaySound(path);
		}

		static Sound StartLoop(const std::string& path)
		{
			return SceneManager::Get().ActiveScene()->GetSystem<AudioSystem>().StartLoop(path);
		}

		static void EndLoop(Sound sound)
		{
			SceneManager::Get().ActiveScene()->GetSystem<AudioSystem>().EndLoop(sound);
		}
	}

	namespace Random
	{
		// Returns a random float
		static float Float()
		{
			return (float)std::rand() / (float)RAND_MAX;
		}

		// Returns a random integer
		static int Int()
		{
			return std::rand();
		}

		// Returns a random value from min inclusively to max exclusively
		static float FloatRange(float min, float max)
		{
			return RandomFloat() * (max - min) + min;
		}

		// Returns a random value from min inclusively to max exclusively
		static int IntRange(int min, int max)
		{
			return (int)(RandomFloat() * (max - min) + min);
		}

		static glm::vec2 PointInBounds(const Bounds& bounds)
		{
			glm::vec2 point;
			point.x = RandomFloatRange(bounds.Center.x - bounds.Extents.x, bounds.Center.x + bounds.Extents.x);
			point.y = RandomFloatRange(bounds.Center.y - bounds.Extents.y, bounds.Center.y + bounds.Extents.y);
			return point;
		}
	}

	namespace Math
	{
		static float Sin(float f) { return glm::sin(f); }
		static float Cos(float f) { return glm::cos(f); }
		static float Tan(float f) { return glm::tan(f); }
		static float Arcsin(float f) { return glm::asin(f); }
		static float Arccos(float f) { return glm::acos(f); }
		static float Arctan(float f) { return glm::atan(f); }

		static glm::vec2 AngleToDirection(float angle)
		{
			return glm::vec2{ glm::cos(angle), glm::sin(angle) };
		}

		static float LerpFloat(float a, float b, float t) { return a + t * (b - a); }
		static Vec2 LerpVec2(Vec2 a, Vec2 b, float t) { return { Lerp(a.x, b.x, t), Lerp(a.y, b.y, t) }; }
		static Vec3 LerpVec3(Vec3 a, Vec3 b, float t) { return { Lerp(a.x, b.x, t), Lerp(a.y, b.y, t), Lerp(a.z, b.z, t) }; }
		static Vec4 LerpVec4(Vec4 a, Vec4 b, float t) { return { Lerp(a.x, b.x, t), Lerp(a.y, b.y, t), Lerp(a.z, b.z, t), Lerp(a.w, b.w, t) }; }
	}

	namespace SaveManager
	{
		template<typename T>
		static void Save(const std::string& key, T value) { JsonHelper::SaveValue(key.c_str(), value); }
		template<typename T>
		static void Load(const std::string& key, T& value) { JsonHelper::LoadValue(key.c_str(), value); }
	}

	static void Info(const std::string& msg) { HW_INFO(msg); }
	static void Warn(const std::string& msg) { HW_WARN(msg); }
	static void Error(const std::string& msg) { HW_ERROR(msg); }
	static void Trace(const std::string& msg) { HW_TRACE(msg); }

	static glm::vec3& Entity_Position(Entity* entity) { return entity->GetComponent<TransformComponent>().Position; }
	static bool Entity_Valid(Entity* entity) { return entity->operator bool(); }
	static std::string Entity_Tag(Entity* entity) { return entity->GetComponent<TagComponent>().Tag; }
	static void Entity_Destroy(Entity* entity, float delay = 0.0f) { return entity->GetScene()->DestroyEntity(*entity, delay); }
	static void Entity_SetVelocity(Entity* entity, glm::vec2 velocity) { entity->GetScene()->GetSystem<Simulator>().SetVelocity(entity->GetUUID(), velocity); };
	static void Entity_ToPerpixel(Entity* entity)
	{
		if (!entity->HasComponent<SpriteRendererComponent>())
		{
			HW_CORE_WARN("Entity {0} does not have sprite renderer to convert to perpixel!", (uint64_t)entity->GetUUID());
			return;
		}

		SpriteRendererComponent& spriteRenderer = entity->GetComponent<SpriteRendererComponent>();

		PerpixelRendererComponent perpixelDef;
		perpixelDef.Shape.Type = PerpixelShapeType::Sprite;
		perpixelDef.Shape.Info.ColorAsset = spriteRenderer.SpriteAsset;
		perpixelDef.Shape.Info.EmissionAsset = spriteRenderer.EmissionSpriteAsset;
		perpixelDef.Shape.Info.Color = spriteRenderer.Color;
		perpixelDef.Shape.Info.Emission = spriteRenderer.Emission;
		PerpixelRendererComponent& perpixelRenderer = entity->AddComponent<PerpixelRendererComponent>(perpixelDef);

		entity->RemoveComponent<SpriteRendererComponent>();

		if (entity->HasComponent<BoxColliderComponent>())
			entity->RemoveComponent<BoxColliderComponent>();
		if (entity->HasComponent<PhysicsBodyComponent>())
			entity->RemoveComponent<PhysicsBodyComponent>();
	}

	static void Entity_SpawnPixel(Entity* entity, Pixel pixel)
	{
		PerpixelInstance* perpixelInstance = entity->GetScene()->GetSystem<PerpixelSystem>().GetInstance(entity->GetUUID());
		if (!perpixelInstance)
		{
			HW_CORE_WARN("Entity {0} does not have associated perpixel instance!", (uint64_t)entity->GetUUID());
			return;
		}
		perpixelInstance->SpawnPixel(pixel);
	}

	static void Entity_SetText(Entity& entity, const std::string& text)
	{
		if (!entity.HasComponent<TextComponent>())
		{
			HW_CORE_WARN("Entity {0} does not have text component!", (uint64_t)entity.GetUUID());
			return;
		}
		entity.GetComponent<TextComponent>().Text = text;
	}

	static void Vec2_DefaultConstructor(void* memory) { new(memory) glm::vec2(); };
	static void Vec2_2ArgConstructor(float x, float y, void* memory) { new(memory) glm::vec2(x, y); };
	static glm::vec2 Vec2_opAdd(const glm::vec2& a, const glm::vec2& b) { return a + b; };
	static glm::vec2 Vec2_opSub(const glm::vec2& a, const glm::vec2& b) { return a - b; };
	static glm::vec2 Vec2_opMul(const glm::vec2& a, float b) { return a * b; };
	static glm::vec2 Vec2_opMul_r(float a, const glm::vec2& b) { return a * b; };
	static glm::vec2 Vec2_opDiv(const glm::vec2& a, float b) { return a / b; };
	static glm::vec2 Vec2_opDiv_r(float a, const glm::vec2& b) { return a / b; };

	static void Vec3_DefaultConstructor(void* memory) { new(memory) glm::vec3(); };
	static void Vec3_2ArgConstructor(float x, float y, void* memory) { new(memory) glm::vec3(x, y, 0.0f); };
	static void Vec3_3ArgConstructor(float x, float y, float z, void* memory) { new(memory) glm::vec3(x, y, z); };
	static glm::vec3 Vec3_opAdd(const glm::vec3& a, const glm::vec3& b) { return a + b; };
	static glm::vec3 Vec3_opSub(const glm::vec3& a, const glm::vec3& b) { return a - b; };
	static glm::vec3 Vec3_opMul(const glm::vec3& a, float b) { return a * b; };
	static glm::vec3 Vec3_opMul_r(float a, const glm::vec3& b) { return a * b; };
	static glm::vec3 Vec3_opDiv(const glm::vec3& a, float b) { return a / b; };
	static glm::vec3 Vec3_opDiv_r(float a, const glm::vec3& b) { return a / b; };

	static void Vec4_DefaultConstructor(void* memory) { new(memory) glm::vec4(); };
	static void Vec4_1ArgConstructor(float x, void* memory) { new(memory) glm::vec4(x, x, x, x); };
	static void Vec4_4ArgConstructor(float x, float y, float z, float w, void* memory) { new(memory) glm::vec4(x, y, z, w); };
	static glm::vec4 Vec4_opAdd(const glm::vec4& a, const glm::vec4& b) { return a + b; };
	static glm::vec4 Vec4_opSub(const glm::vec4& a, const glm::vec4& b) { return a - b; };
	static glm::vec4 Vec4_opMul(const glm::vec4& a, float b) { return a * b; };
	static glm::vec4 Vec4_opMul_r(float a, const glm::vec4& b) { return a * b; };
	static glm::vec4 Vec4_opDiv(const glm::vec4& a, float b) { return a / b; };
	static glm::vec4 Vec4_opDiv_r(float a, const glm::vec4& b) { return a / b; };

	static Entity Scene_Spawn(Scene* scene, Asset prefabAsset)
	{
		if (!prefabAsset)
		{
			HW_ERROR("Trying to spawn empty prefab!");
			return Entity{};
		}

		Ref<Scene> prefabScene = prefabAsset.GetData<Scene>();
		Entity newEntity = scene->CopyEntity(prefabScene->GetParentEntities().at(0));
		return newEntity;
	}

	void AngelAPIBinder::BindAPI(asIScriptEngine* engine)
	{
		int r;

		r = engine->RegisterGlobalFunction("void Info(const string& in)", asFUNCTION(Info), asCALL_CDECL); HW_CORE_ASSERT(r >= 0, "Failed to register!");
		r = engine->RegisterGlobalFunction("void Warn(const string& in)", asFUNCTION(Warn), asCALL_CDECL); HW_CORE_ASSERT(r >= 0, "Failed to register!");
		r = engine->RegisterGlobalFunction("void Error(const string& in)", asFUNCTION(Error), asCALL_CDECL); HW_CORE_ASSERT(r >= 0, "Failed to register!");
		r = engine->RegisterGlobalFunction("void Trace(const string& in)", asFUNCTION(Trace), asCALL_CDECL); HW_CORE_ASSERT(r >= 0, "Failed to register!");

		// Effects
		r = engine->SetDefaultNamespace("Effects");
		r = engine->RegisterGlobalFunction("void CameraShake(float)", asFUNCTION(Effects::CameraShake), asCALL_CDECL); HW_CORE_ASSERT(r >= 0, "Failed to register!");
		r = engine->SetDefaultNamespace("");
		
		// Random
		r = engine->SetDefaultNamespace("Random");
		r = engine->RegisterGlobalFunction("float Float()", asFUNCTION(Random::Float), asCALL_CDECL); HW_CORE_ASSERT(r >= 0, "Failed to register!");
		r = engine->RegisterGlobalFunction("int Int()", asFUNCTION(Random::Int), asCALL_CDECL); HW_CORE_ASSERT(r >= 0, "Failed to register!");
		r = engine->RegisterGlobalFunction("float Range(float, float)", asFUNCTION(Random::FloatRange), asCALL_CDECL); HW_CORE_ASSERT(r >= 0, "Failed to register!");
		r = engine->RegisterGlobalFunction("int Range(int, int)", asFUNCTION(Random::IntRange), asCALL_CDECL); HW_CORE_ASSERT(r >= 0, "Failed to register!");
		r = engine->SetDefaultNamespace("");

		// Audio
		r = engine->RegisterObjectType("Sound", sizeof(Sound), asOBJ_VALUE | asOBJ_POD | asOBJ_APP_CLASS_C); HW_CORE_ASSERT(r >= 0, "Failed to register!");

		r = engine->SetDefaultNamespace("Audio");
		r = engine->RegisterGlobalFunction("void Play(const string& in)", asFUNCTION(Audio::Play), asCALL_CDECL); HW_CORE_ASSERT(r >= 0, "Failed to register!");
		r = engine->RegisterGlobalFunction("void StartLoop(const string& in)", asFUNCTION(Audio::StartLoop), asCALL_CDECL); HW_CORE_ASSERT(r >= 0, "Failed to register!");
		r = engine->RegisterGlobalFunction("void EndLoop(Sound)", asFUNCTION(Audio::EndLoop), asCALL_CDECL); HW_CORE_ASSERT(r >= 0, "Failed to register!");
		r = engine->SetDefaultNamespace("");

		// Vec2
		r = engine->RegisterObjectType("Vec2", sizeof(glm::vec2), asOBJ_VALUE | asOBJ_POD | asOBJ_APP_CLASS_ALLFLOATS | asOBJ_APP_CLASS_C); HW_CORE_ASSERT(r >= 0, "Failed to register!");
		r = engine->RegisterObjectBehaviour("Vec2", asBEHAVE_CONSTRUCT, "void f()", asFUNCTION(Vec2_DefaultConstructor), asCALL_CDECL_OBJLAST); HW_CORE_ASSERT(r >= 0, "Failed to register!");
		r = engine->RegisterObjectBehaviour("Vec2", asBEHAVE_CONSTRUCT, "void f(float, float)", asFUNCTION(Vec2_2ArgConstructor), asCALL_CDECL_OBJLAST); HW_CORE_ASSERT(r >= 0, "Failed to register!");
		r = engine->RegisterObjectProperty("Vec2", "float x", asOFFSET(glm::vec2, x)); HW_CORE_ASSERT(r >= 0, "Failed to register!"); HW_CORE_ASSERT(r >= 0, "Failed to register!");
		r = engine->RegisterObjectProperty("Vec2", "float y", asOFFSET(glm::vec2, y)); HW_CORE_ASSERT(r >= 0, "Failed to register!"); HW_CORE_ASSERT(r >= 0, "Failed to register!");
		r = engine->RegisterObjectMethod("Vec2", "Vec2 opAdd(Vec2)", asFUNCTIONPR(Vec2_opAdd, (const glm::vec2&, const glm::vec2&), glm::vec2), asCALL_CDECL_OBJFIRST); HW_CORE_ASSERT(r >= 0, "Failed to register!");
		r = engine->RegisterObjectMethod("Vec2", "Vec2 opSub(Vec2)", asFUNCTIONPR(Vec2_opSub, (const glm::vec2&, const glm::vec2&), glm::vec2), asCALL_CDECL_OBJFIRST); HW_CORE_ASSERT(r >= 0, "Failed to register!");
		r = engine->RegisterObjectMethod("Vec2", "Vec2 opMul(float)", asFUNCTIONPR(Vec2_opMul, (const glm::vec2&, float), glm::vec2), asCALL_CDECL_OBJFIRST); HW_CORE_ASSERT(r >= 0, "Failed to register!");
		r = engine->RegisterObjectMethod("Vec2", "Vec2 opMul_r(float)", asFUNCTIONPR(Vec2_opMul_r, (float, const glm::vec2&), glm::vec2), asCALL_CDECL_OBJLAST); HW_CORE_ASSERT(r >= 0, "Failed to register!");
		r = engine->RegisterObjectMethod("Vec2", "Vec2 opDiv(float)", asFUNCTIONPR(Vec2_opDiv, (const glm::vec2&, float), glm::vec2), asCALL_CDECL_OBJFIRST); HW_CORE_ASSERT(r >= 0, "Failed to register!");
		r = engine->RegisterObjectMethod("Vec2", "Vec2 opDiv_r(float)", asFUNCTIONPR(Vec2_opDiv_r, (float, const glm::vec2&), glm::vec2), asCALL_CDECL_OBJLAST); HW_CORE_ASSERT(r >= 0, "Failed to register!");

		// Vec3
		r = engine->RegisterObjectType("Vec3", sizeof(glm::vec3), asOBJ_VALUE | asOBJ_POD | asOBJ_APP_CLASS_ALLFLOATS | asOBJ_APP_CLASS_C); HW_CORE_ASSERT(r >= 0, "Failed to register!");
		r = engine->RegisterObjectBehaviour("Vec3", asBEHAVE_CONSTRUCT, "void f()", asFUNCTION(Vec3_DefaultConstructor), asCALL_CDECL_OBJLAST); HW_CORE_ASSERT(r >= 0, "Failed to register!");
		r = engine->RegisterObjectBehaviour("Vec3", asBEHAVE_CONSTRUCT, "void f(float, float)", asFUNCTION(Vec3_2ArgConstructor), asCALL_CDECL_OBJLAST); HW_CORE_ASSERT(r >= 0, "Failed to register!");
		r = engine->RegisterObjectBehaviour("Vec3", asBEHAVE_CONSTRUCT, "void f(float, float, float)", asFUNCTION(Vec3_3ArgConstructor), asCALL_CDECL_OBJLAST); HW_CORE_ASSERT(r >= 0, "Failed to register!");
		r = engine->RegisterObjectProperty("Vec3", "float x", asOFFSET(glm::vec3, x)); HW_CORE_ASSERT(r >= 0, "Failed to register!");
		r = engine->RegisterObjectProperty("Vec3", "float y", asOFFSET(glm::vec3, y)); HW_CORE_ASSERT(r >= 0, "Failed to register!");
		r = engine->RegisterObjectProperty("Vec3", "float z", asOFFSET(glm::vec3, z)); HW_CORE_ASSERT(r >= 0, "Failed to register!");
		r = engine->RegisterObjectMethod("Vec3", "Vec3 opAdd(Vec3)", asFUNCTIONPR(Vec3_opAdd, (const glm::vec3&, const glm::vec3&), glm::vec3), asCALL_CDECL_OBJFIRST); HW_CORE_ASSERT(r >= 0, "Failed to register!");
		r = engine->RegisterObjectMethod("Vec3", "Vec3 opSub(Vec3)", asFUNCTIONPR(Vec3_opSub, (const glm::vec3&, const glm::vec3&), glm::vec3), asCALL_CDECL_OBJFIRST); HW_CORE_ASSERT(r >= 0, "Failed to register!");
		r = engine->RegisterObjectMethod("Vec3", "Vec3 opMul(float)", asFUNCTIONPR(Vec3_opMul, (const glm::vec3&, float), glm::vec3), asCALL_CDECL_OBJFIRST); HW_CORE_ASSERT(r >= 0, "Failed to register!");
		r = engine->RegisterObjectMethod("Vec3", "Vec3 opMul_r(float)", asFUNCTIONPR(Vec3_opMul_r, (float, const glm::vec3&), glm::vec3), asCALL_CDECL_OBJLAST); HW_CORE_ASSERT(r >= 0, "Failed to register!");
		r = engine->RegisterObjectMethod("Vec3", "Vec3 opDiv(float)", asFUNCTIONPR(Vec3_opDiv, (const glm::vec3&, float), glm::vec3), asCALL_CDECL_OBJFIRST); HW_CORE_ASSERT(r >= 0, "Failed to register!");
		r = engine->RegisterObjectMethod("Vec3", "Vec3 opDiv_r(float)", asFUNCTIONPR(Vec3_opDiv_r, (float, const glm::vec3&), glm::vec3), asCALL_CDECL_OBJLAST); HW_CORE_ASSERT(r >= 0, "Failed to register!");

		// Vec4
		r = engine->RegisterObjectType("Vec4", sizeof(glm::vec4), asOBJ_VALUE | asOBJ_POD | asOBJ_APP_CLASS_ALLFLOATS | asOBJ_APP_CLASS_C); HW_CORE_ASSERT(r >= 0, "Failed to register!");
		r = engine->RegisterObjectBehaviour("Vec4", asBEHAVE_CONSTRUCT, "void f()", asFUNCTION(Vec4_DefaultConstructor), asCALL_CDECL_OBJLAST); HW_CORE_ASSERT(r >= 0, "Failed to register!");
		r = engine->RegisterObjectBehaviour("Vec4", asBEHAVE_CONSTRUCT, "void f(float)", asFUNCTION(Vec4_1ArgConstructor), asCALL_CDECL_OBJLAST); HW_CORE_ASSERT(r >= 0, "Failed to register!");
		r = engine->RegisterObjectBehaviour("Vec4", asBEHAVE_CONSTRUCT, "void f(float, float, float, float)", asFUNCTION(Vec4_4ArgConstructor), asCALL_CDECL_OBJLAST); HW_CORE_ASSERT(r >= 0, "Failed to register!");
		r = engine->RegisterObjectProperty("Vec4", "float x", asOFFSET(glm::vec4, x)); HW_CORE_ASSERT(r >= 0, "Failed to register!");
		r = engine->RegisterObjectProperty("Vec4", "float y", asOFFSET(glm::vec4, y)); HW_CORE_ASSERT(r >= 0, "Failed to register!");
		r = engine->RegisterObjectProperty("Vec4", "float z", asOFFSET(glm::vec4, z)); HW_CORE_ASSERT(r >= 0, "Failed to register!");
		r = engine->RegisterObjectProperty("Vec4", "float w", asOFFSET(glm::vec4, w)); HW_CORE_ASSERT(r >= 0, "Failed to register!");
		r = engine->RegisterObjectMethod("Vec4", "Vec4 opAdd(Vec4)", asFUNCTIONPR(Vec4_opAdd, (const glm::vec4&, const glm::vec4&), glm::vec4), asCALL_CDECL_OBJFIRST); HW_CORE_ASSERT(r >= 0, "Failed to register!");
		r = engine->RegisterObjectMethod("Vec4", "Vec4 opSub(Vec4)", asFUNCTIONPR(Vec4_opSub, (const glm::vec4&, const glm::vec4&), glm::vec4), asCALL_CDECL_OBJFIRST); HW_CORE_ASSERT(r >= 0, "Failed to register!");
		r = engine->RegisterObjectMethod("Vec4", "Vec4 opMul(float)", asFUNCTIONPR(Vec4_opMul, (const glm::vec4&, float), glm::vec4), asCALL_CDECL_OBJFIRST); HW_CORE_ASSERT(r >= 0, "Failed to register!");
		r = engine->RegisterObjectMethod("Vec4", "Vec4 opMul_r(float)", asFUNCTIONPR(Vec4_opMul_r, (float, const glm::vec4&), glm::vec4), asCALL_CDECL_OBJLAST); HW_CORE_ASSERT(r >= 0, "Failed to register!");
		r = engine->RegisterObjectMethod("Vec4", "Vec4 opDiv(float)", asFUNCTIONPR(Vec4_opDiv, (const glm::vec4&, float), glm::vec4), asCALL_CDECL_OBJFIRST); HW_CORE_ASSERT(r >= 0, "Failed to register!");
		r = engine->RegisterObjectMethod("Vec4", "Vec4 opDiv_r(float)", asFUNCTIONPR(Vec4_opDiv_r, (float, const glm::vec4&), glm::vec4), asCALL_CDECL_OBJLAST); HW_CORE_ASSERT(r >= 0, "Failed to register!");

		// Math
		r = engine->SetDefaultNamespace("Math");
		r = engine->RegisterGlobalFunction("float Sin(float)", asFUNCTION(Math::Sin), asCALL_CDECL); HW_CORE_ASSERT(r >= 0, "Failed to register!");
		r = engine->RegisterGlobalFunction("float Cos(float)", asFUNCTION(Math::Cos), asCALL_CDECL); HW_CORE_ASSERT(r >= 0, "Failed to register!");
		r = engine->RegisterGlobalFunction("float Tan(float)", asFUNCTION(Math::Tan), asCALL_CDECL); HW_CORE_ASSERT(r >= 0, "Failed to register!");
		r = engine->RegisterGlobalFunction("float Arcsin(float)", asFUNCTION(Math::Arcsin), asCALL_CDECL); HW_CORE_ASSERT(r >= 0, "Failed to register!");
		r = engine->RegisterGlobalFunction("float Arccos(float)", asFUNCTION(Math::Arccos), asCALL_CDECL); HW_CORE_ASSERT(r >= 0, "Failed to register!");
		r = engine->RegisterGlobalFunction("float Arctan(float)", asFUNCTION(Math::Arctan), asCALL_CDECL); HW_CORE_ASSERT(r >= 0, "Failed to register!");
		r = engine->RegisterGlobalFunction("float Pi()", asFUNCTION(glm::pi<float>), asCALL_CDECL); HW_CORE_ASSERT(r >= 0, "Failed to register!");
		r = engine->RegisterGlobalFunction("Vec2 AngleToDirection(float)", asFUNCTION(Math::AngleToDirection), asCALL_CDECL); HW_CORE_ASSERT(r >= 0, "Failed to register!");
		r = engine->RegisterGlobalFunction("float Lerp(float, float, float)", asFUNCTION(Math::LerpFloat), asCALL_CDECL); HW_CORE_ASSERT(r >= 0, "Failed to register!");
		r = engine->RegisterGlobalFunction("Vec2 Lerp(Vec2, Vec2, float)", asFUNCTION(Math::LerpVec2), asCALL_CDECL); HW_CORE_ASSERT(r >= 0, "Failed to register!");
		r = engine->RegisterGlobalFunction("Vec3 Lerp(Vec3, Vec3, float)", asFUNCTION(Math::LerpVec3), asCALL_CDECL); HW_CORE_ASSERT(r >= 0, "Failed to register!");
		r = engine->RegisterGlobalFunction("Vec4 Lerp(Vec4, Vec4, float)", asFUNCTION(Math::LerpVec4), asCALL_CDECL); HW_CORE_ASSERT(r >= 0, "Failed to register!");
		r = engine->SetDefaultNamespace("");

		// Renderer
		r = engine->SetDefaultNamespace("Renderer");
		r = engine->RegisterGlobalFunction("void DrawCircle(Vec2, float, float, Vec4 = Vec4(1.0f), float = 0.0f, bool = true)", asFUNCTION(pxr::Renderer::DrawCircle), asCALL_CDECL); HW_CORE_ASSERT(r >= 0, "Failed to register!");
		r = engine->RegisterGlobalFunction("void DrawLine(Vec2, Vec2, Vec4 = Vec4(1.0f), float = 0.0f, bool = true)", asFUNCTION(pxr::Renderer::DrawLine), asCALL_CDECL); HW_CORE_ASSERT(r >= 0, "Failed to register!");
		r = engine->RegisterGlobalFunction("void DrawBox(Vec2, Vec2, Vec4 = Vec4(1.0f), float = 0.0f, bool = true)", asFUNCTION(pxr::Renderer::DrawBox), asCALL_CDECL); HW_CORE_ASSERT(r >= 0, "Failed to register!");
		r = engine->SetDefaultNamespace("");

		// Bounds
		r = engine->RegisterObjectType("Bounds", sizeof(Bounds), asOBJ_VALUE | asOBJ_POD | asOBJ_APP_CLASS_ALLFLOATS | asOBJ_APP_CLASS_C); HW_CORE_ASSERT(r >= 0, "Failed to register!");
		r = engine->RegisterObjectProperty("Bounds", "Vec2 center", asOFFSET(Bounds, Center)); HW_CORE_ASSERT(r >= 0, "Failed to register!");
		r = engine->RegisterObjectProperty("Bounds", "Vec2 extents", asOFFSET(Bounds, Extents)); HW_CORE_ASSERT(r >= 0, "Failed to register!");
		r = engine->RegisterObjectMethod("Bounds", "Vec2 RandomPoint()", asFUNCTIONPR(Random::PointInBounds, (const Bounds&), glm::vec2), asCALL_CDECL_OBJLAST); HW_CORE_ASSERT(r >= 0, "Failed to register!");

		// Input
		r = engine->RegisterEnum("KeyCode"); HW_CORE_ASSERT(r >= 0, "Failed to register!");
		r = engine->RegisterEnumValue("KeyCode", "KEY_0", HW_KEY_0); HW_CORE_ASSERT(r >= 0, "Failed to register!");
		r = engine->RegisterEnumValue("KeyCode", "KEY_1", HW_KEY_1); HW_CORE_ASSERT(r >= 0, "Failed to register!");
		r = engine->RegisterEnumValue("KeyCode", "KEY_2", HW_KEY_2); HW_CORE_ASSERT(r >= 0, "Failed to register!");
		r = engine->RegisterEnumValue("KeyCode", "KEY_3", HW_KEY_3); HW_CORE_ASSERT(r >= 0, "Failed to register!");
		r = engine->RegisterEnumValue("KeyCode", "KEY_4", HW_KEY_4); HW_CORE_ASSERT(r >= 0, "Failed to register!");
		r = engine->RegisterEnumValue("KeyCode", "KEY_5", HW_KEY_5); HW_CORE_ASSERT(r >= 0, "Failed to register!");
		r = engine->RegisterEnumValue("KeyCode", "KEY_6", HW_KEY_6); HW_CORE_ASSERT(r >= 0, "Failed to register!");
		r = engine->RegisterEnumValue("KeyCode", "KEY_7", HW_KEY_7); HW_CORE_ASSERT(r >= 0, "Failed to register!");
		r = engine->RegisterEnumValue("KeyCode", "KEY_8", HW_KEY_8); HW_CORE_ASSERT(r >= 0, "Failed to register!");
		r = engine->RegisterEnumValue("KeyCode", "KEY_9", HW_KEY_9); HW_CORE_ASSERT(r >= 0, "Failed to register!");
		r = engine->RegisterEnumValue("KeyCode", "KEY_A", HW_KEY_A); HW_CORE_ASSERT(r >= 0, "Failed to register!");
		r = engine->RegisterEnumValue("KeyCode", "KEY_B", HW_KEY_B); HW_CORE_ASSERT(r >= 0, "Failed to register!");
		r = engine->RegisterEnumValue("KeyCode", "KEY_C", HW_KEY_C); HW_CORE_ASSERT(r >= 0, "Failed to register!");
		r = engine->RegisterEnumValue("KeyCode", "KEY_D", HW_KEY_D); HW_CORE_ASSERT(r >= 0, "Failed to register!");
		r = engine->RegisterEnumValue("KeyCode", "KEY_E", HW_KEY_E); HW_CORE_ASSERT(r >= 0, "Failed to register!");
		r = engine->RegisterEnumValue("KeyCode", "KEY_F", HW_KEY_F); HW_CORE_ASSERT(r >= 0, "Failed to register!");
		r = engine->RegisterEnumValue("KeyCode", "KEY_G", HW_KEY_G); HW_CORE_ASSERT(r >= 0, "Failed to register!");
		r = engine->RegisterEnumValue("KeyCode", "KEY_H", HW_KEY_H); HW_CORE_ASSERT(r >= 0, "Failed to register!");
		r = engine->RegisterEnumValue("KeyCode", "KEY_I", HW_KEY_I); HW_CORE_ASSERT(r >= 0, "Failed to register!");
		r = engine->RegisterEnumValue("KeyCode", "KEY_J", HW_KEY_J); HW_CORE_ASSERT(r >= 0, "Failed to register!");
		r = engine->RegisterEnumValue("KeyCode", "KEY_K", HW_KEY_K); HW_CORE_ASSERT(r >= 0, "Failed to register!");
		r = engine->RegisterEnumValue("KeyCode", "KEY_L", HW_KEY_L); HW_CORE_ASSERT(r >= 0, "Failed to register!");
		r = engine->RegisterEnumValue("KeyCode", "KEY_M", HW_KEY_M); HW_CORE_ASSERT(r >= 0, "Failed to register!"); 
		r = engine->RegisterEnumValue("KeyCode", "KEY_N", HW_KEY_N); HW_CORE_ASSERT(r >= 0, "Failed to register!");
		r = engine->RegisterEnumValue("KeyCode", "KEY_O", HW_KEY_O); HW_CORE_ASSERT(r >= 0, "Failed to register!");
		r = engine->RegisterEnumValue("KeyCode", "KEY_P", HW_KEY_P); HW_CORE_ASSERT(r >= 0, "Failed to register!");
		r = engine->RegisterEnumValue("KeyCode", "KEY_Q", HW_KEY_Q); HW_CORE_ASSERT(r >= 0, "Failed to register!");
		r = engine->RegisterEnumValue("KeyCode", "KEY_R", HW_KEY_R); HW_CORE_ASSERT(r >= 0, "Failed to register!");
		r = engine->RegisterEnumValue("KeyCode", "KEY_S", HW_KEY_S); HW_CORE_ASSERT(r >= 0, "Failed to register!");
		r = engine->RegisterEnumValue("KeyCode", "KEY_T", HW_KEY_T); HW_CORE_ASSERT(r >= 0, "Failed to register!");
		r = engine->RegisterEnumValue("KeyCode", "KEY_U", HW_KEY_U); HW_CORE_ASSERT(r >= 0, "Failed to register!");
		r = engine->RegisterEnumValue("KeyCode", "KEY_V", HW_KEY_V); HW_CORE_ASSERT(r >= 0, "Failed to register!");
		r = engine->RegisterEnumValue("KeyCode", "KEY_W", HW_KEY_W); HW_CORE_ASSERT(r >= 0, "Failed to register!");
		r = engine->RegisterEnumValue("KeyCode", "KEY_X", HW_KEY_X); HW_CORE_ASSERT(r >= 0, "Failed to register!");
		r = engine->RegisterEnumValue("KeyCode", "KEY_Y", HW_KEY_Y); HW_CORE_ASSERT(r >= 0, "Failed to register!");
		r = engine->RegisterEnumValue("KeyCode", "KEY_Z", HW_KEY_Z); HW_CORE_ASSERT(r >= 0, "Failed to register!");
		r = engine->RegisterEnumValue("KeyCode", "KEY_SPACE" , HW_KEY_SPACE); HW_CORE_ASSERT(r >= 0, "Failed to register!");

		r = engine->RegisterEnum("MouseButtonCode"); HW_CORE_ASSERT(r >= 0, "Failed to register!");
		r = engine->RegisterEnumValue("MouseButtonCode", "MOUSE_BUTTON_LEFT", HW_MOUSE_BUTTON_LEFT); HW_CORE_ASSERT(r >= 0, "Failed to register!");
		r = engine->RegisterEnumValue("MouseButtonCode", "MOUSE_BUTTON_MIDDLE", HW_MOUSE_BUTTON_MIDDLE); HW_CORE_ASSERT(r >= 0, "Failed to register!");
		r = engine->RegisterEnumValue("MouseButtonCode", "MOUSE_BUTTON_RIGHT", HW_MOUSE_BUTTON_RIGHT); HW_CORE_ASSERT(r >= 0, "Failed to register!");

		r = engine->RegisterGlobalFunction("bool KeyPressed(KeyCode)", asFUNCTION(Input::IsKeyPressed), asCALL_CDECL); HW_CORE_ASSERT(r >= 0, "Failed to register!");
		r = engine->RegisterGlobalFunction("bool MouseButtonPressed(MouseButtonCode)", asFUNCTION(Input::IsMouseButtonPressed), asCALL_CDECL); HW_CORE_ASSERT(r >= 0, "Failed to register!");

		r = engine->RegisterGlobalFunction("Vec2 MouseScreenPosition()", asFUNCTION(Input::GetMousePosition), asCALL_CDECL); HW_CORE_ASSERT(r >= 0, "Failed to register!");
		r = engine->RegisterGlobalFunction("Vec2 MouseWorldPosition()", asFUNCTION(Input::GetMouseWorldPosition), asCALL_CDECL); HW_CORE_ASSERT(r >= 0, "Failed to register!");
		r = engine->RegisterGlobalFunction("Vec2 MouseDelta()", asFUNCTION(Input::GetMouseDelta), asCALL_CDECL); HW_CORE_ASSERT(r >= 0, "Failed to register!");
		
		// Asset
		r = engine->RegisterObjectType("Asset", sizeof(Asset), asOBJ_VALUE | asOBJ_POD | asOBJ_APP_CLASS); HW_CORE_ASSERT(r >= 0, "Failed to register!");
		r = engine->RegisterObjectType("PrefabAsset", sizeof(Asset), asOBJ_VALUE | asOBJ_POD | asOBJ_APP_CLASS); HW_CORE_ASSERT(r >= 0, "Failed to register!");

		// Scene
		r = engine->RegisterObjectType("Scene", sizeof(Scene), asOBJ_VALUE | asOBJ_POD | asOBJ_APP_CLASS); HW_CORE_ASSERT(r >= 0, "Failed to register!");
		
		// Pixel
		r = engine->RegisterObjectType("Pixel", sizeof(Pixel), asOBJ_VALUE | asOBJ_POD | asOBJ_APP_CLASS_C); HW_CORE_ASSERT(r >= 0, "Failed to register!");
		r = engine->RegisterObjectProperty("Pixel", "float lifetime", asOFFSET(Pixel, Lifetime)); HW_CORE_ASSERT(r >= 0, "Failed to register!");
		r = engine->RegisterObjectProperty("Pixel", "Vec4 color", asOFFSET(Pixel, Color)); HW_CORE_ASSERT(r >= 0, "Failed to register!");
		r = engine->RegisterObjectProperty("Pixel", "float emission", asOFFSET(Pixel, Emission)); HW_CORE_ASSERT(r >= 0, "Failed to register!");
		r = engine->RegisterObjectProperty("Pixel", "Vec2 position", asOFFSET(Pixel, Position)); HW_CORE_ASSERT(r >= 0, "Failed to register!");
		r = engine->RegisterObjectProperty("Pixel", "Vec2 velocity", asOFFSET(Pixel, Velocity)); HW_CORE_ASSERT(r >= 0, "Failed to register!");

		// Entity
		r = engine->RegisterObjectType("Entity", sizeof(Entity), asOBJ_VALUE | asOBJ_POD | asOBJ_APP_CLASS); HW_CORE_ASSERT(r >= 0, "Failed to register!");
		r = engine->RegisterObjectMethod("Entity", "Scene& Scene()", asMETHOD(Entity, Entity::GetScene), asCALL_THISCALL); HW_CORE_ASSERT(r >= 0, "Failed to register!");
		r = engine->RegisterObjectMethod("Entity", "string Tag()", asFUNCTION(Entity_Tag), asCALL_CDECL_OBJFIRST); HW_CORE_ASSERT(r >= 0, "Failed to register!");
		r = engine->RegisterObjectMethod("Entity", "Vec3& Position()", asFUNCTION(Entity_Position), asCALL_CDECL_OBJFIRST); HW_CORE_ASSERT(r >= 0, "Failed to register!");
		r = engine->RegisterObjectMethod("Entity", "bool Valid()", asFUNCTION(Entity_Valid), asCALL_CDECL_OBJFIRST); HW_CORE_ASSERT(r >= 0, "Failed to register!");
		r = engine->RegisterObjectMethod("Entity", "void Destroy(float = 0.0f)", asFUNCTION(Entity_Destroy), asCALL_CDECL_OBJFIRST); HW_CORE_ASSERT(r >= 0, "Failed to register!");
		r = engine->RegisterObjectMethod("Entity", "void SetVelocity(Vec2)", asFUNCTION(Entity_SetVelocity), asCALL_CDECL_OBJFIRST); HW_CORE_ASSERT(r >= 0, "Failed to register!");
		r = engine->RegisterObjectMethod("Entity", "void ToPerpixel()", asFUNCTION(Entity_ToPerpixel), asCALL_CDECL_OBJFIRST); HW_CORE_ASSERT(r >= 0, "Failed to register!");
		r = engine->RegisterObjectMethod("Entity", "void SpawnPixel(Pixel)", asFUNCTION(Entity_SpawnPixel), asCALL_CDECL_OBJFIRST); HW_CORE_ASSERT(r >= 0, "Failed to register!");
		r = engine->RegisterObjectMethod("Entity", "void SetText(const string& in)", asFUNCTION(Entity_SetText), asCALL_CDECL_OBJFIRST); HW_CORE_ASSERT(r >= 0, "Failed to register!");

		r = engine->RegisterObjectMethod("Scene", "Entity Spawn(PrefabAsset)", asFUNCTION(Scene_Spawn), asCALL_CDECL_OBJFIRST); HW_CORE_ASSERT(r >= 0, "Failed to register!");

		// Physics
		r = engine->RegisterObjectType("HitInfo", sizeof(HitInfo), asOBJ_VALUE | asOBJ_POD | asOBJ_APP_CLASS_C); HW_CORE_ASSERT(r >= 0, "Failed to register!");
		r = engine->RegisterObjectProperty("HitInfo", "Entity other", asOFFSET(HitInfo, other)); HW_CORE_ASSERT(r >= 0, "Failed to register!");
		r = engine->RegisterObjectProperty("HitInfo", "Vec2 point", asOFFSET(HitInfo, point)); HW_CORE_ASSERT(r >= 0, "Failed to register!");

		// Save Manager
		r = engine->SetDefaultNamespace("SaveManager");
		r = engine->RegisterGlobalFunction("void Save(const string& in, bool)", asFUNCTION(SaveManager::Save<bool>), asCALL_CDECL); HW_CORE_ASSERT(r >= 0, "Failed to register!");
		r = engine->RegisterGlobalFunction("void Save(const string& in, int)", asFUNCTION(SaveManager::Save<int>), asCALL_CDECL); HW_CORE_ASSERT(r >= 0, "Failed to register!");
		r = engine->RegisterGlobalFunction("void Save(const string& in, float)", asFUNCTION(SaveManager::Save<float>), asCALL_CDECL); HW_CORE_ASSERT(r >= 0, "Failed to register!");
		r = engine->RegisterGlobalFunction("void Save(const string& in, double)", asFUNCTION(SaveManager::Save<double>), asCALL_CDECL); HW_CORE_ASSERT(r >= 0, "Failed to register!");
		r = engine->RegisterGlobalFunction("void Save(const string& in, Vec2)", asFUNCTION(SaveManager::Save<glm::vec2>), asCALL_CDECL); HW_CORE_ASSERT(r >= 0, "Failed to register!");
		r = engine->RegisterGlobalFunction("void Save(const string& in, Vec3)", asFUNCTION(SaveManager::Save<glm::vec3>), asCALL_CDECL); HW_CORE_ASSERT(r >= 0, "Failed to register!");
		r = engine->RegisterGlobalFunction("void Save(const string& in, Vec4)", asFUNCTION(SaveManager::Save<glm::vec4>), asCALL_CDECL); HW_CORE_ASSERT(r >= 0, "Failed to register!");
		r = engine->RegisterGlobalFunction("void Save(const string& in, const string& in)", asFUNCTION(SaveManager::Save<const std::string&>), asCALL_CDECL); HW_CORE_ASSERT(r >= 0, "Failed to register!");

		r = engine->RegisterGlobalFunction("void Load(const string& in, bool&)", asFUNCTION(SaveManager::Load<bool>), asCALL_CDECL); HW_CORE_ASSERT(r >= 0, "Failed to register!");
		r = engine->RegisterGlobalFunction("void Load(const string& in, int&)", asFUNCTION(SaveManager::Load<int>), asCALL_CDECL); HW_CORE_ASSERT(r >= 0, "Failed to register!");
		r = engine->RegisterGlobalFunction("void Load(const string& in, float&)", asFUNCTION(SaveManager::Load<float>), asCALL_CDECL); HW_CORE_ASSERT(r >= 0, "Failed to register!");
		r = engine->RegisterGlobalFunction("void Load(const string& in, double&)", asFUNCTION(SaveManager::Load<double>), asCALL_CDECL); HW_CORE_ASSERT(r >= 0, "Failed to register!");
		r = engine->RegisterGlobalFunction("void Load(const string& in, Vec2&)", asFUNCTION(SaveManager::Load<glm::vec2>), asCALL_CDECL); HW_CORE_ASSERT(r >= 0, "Failed to register!");
		r = engine->RegisterGlobalFunction("void Load(const string& in, Vec3&)", asFUNCTION(SaveManager::Load<glm::vec3>), asCALL_CDECL); HW_CORE_ASSERT(r >= 0, "Failed to register!");
		r = engine->RegisterGlobalFunction("void Load(const string& in, Vec4&)", asFUNCTION(SaveManager::Load<glm::vec4>), asCALL_CDECL); HW_CORE_ASSERT(r >= 0, "Failed to register!");
		r = engine->RegisterGlobalFunction("void Load(const string& in, string&)", asFUNCTION(SaveManager::Load<std::string>), asCALL_CDECL); HW_CORE_ASSERT(r >= 0, "Failed to register!");
		r = engine->SetDefaultNamespace("");


	}
}