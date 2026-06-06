#pragma once

namespace Perplex
{
	class Json;

	struct IDComponent;
	void ToJson(Json& json, const IDComponent& obj);
	void FromJson(const Json& json, IDComponent& obj);

	struct TagComponent;
	void ToJson(Json& json, const TagComponent& obj);
	void FromJson(const Json& json, TagComponent& obj);

	struct TransformComponent;
	void ToJson(Json& json, const TransformComponent& obj);
	void FromJson(const Json& json, TransformComponent& obj);

	struct SpriteRendererComponent;
	void ToJson(Json& json, const SpriteRendererComponent& obj);
	void FromJson(const Json& json, SpriteRendererComponent& obj);

	struct CameraComponent;
	void ToJson(Json& json, const CameraComponent& obj);
	void FromJson(const Json& json, CameraComponent& obj);

	struct ScriptComponent;
	void ToJson(Json& json, const ScriptComponent& obj);
	void FromJson(const Json& json, ScriptComponent& obj);

	struct PerpixelRendererComponent;
	void ToJson(Json& json, const PerpixelRendererComponent& obj);
	void FromJson(const Json& json, PerpixelRendererComponent& obj);

	struct BoxColliderComponent;
	void ToJson(Json& json, const BoxColliderComponent& obj);
	void FromJson(const Json& json, BoxColliderComponent& obj);

	struct PhysicsBodyComponent;
	void ToJson(Json& json, const PhysicsBodyComponent& obj);
	void FromJson(const Json& json, PhysicsBodyComponent& obj);
}