#pragma once

namespace Holloware
{
	struct IDComponent;
	void Draw(IDComponent& component);

	struct TagComponent;
	void Draw(TagComponent& component);

	struct TransformComponent;
	void Draw(TransformComponent& component);

	struct SpriteRendererComponent;
	void Draw(SpriteRendererComponent& component);

	struct CameraComponent;
	void Draw(CameraComponent& component);

	struct ScriptComponent;
	void Draw(ScriptComponent& component);

	struct PerpixelRendererComponent;
	void Draw(PerpixelRendererComponent& component);
}