#pragma once

#include <Perplex/ImGui/GuiSelection.h>

namespace Perplex
{
	struct IDComponent;
	void Draw(GuiSelection<IDComponent> component);

	struct TagComponent;
	void Draw(GuiSelection<TagComponent> component);

	struct EnableComponent;
	void Draw(GuiSelection<EnableComponent> component);

	struct TransformComponent;
	void Draw(GuiSelection<TransformComponent> component);

	struct SpriteRendererComponent;
	void Draw(GuiSelection<SpriteRendererComponent> component);

	struct BoxRendererComponent;
	void Draw(GuiSelection<BoxRendererComponent> component);

	struct CircleRendererComponent;
	void Draw(GuiSelection<CircleRendererComponent> component);

	struct LineRendererComponent;
	void Draw(GuiSelection<LineRendererComponent> component);

	struct CameraComponent;
	void Draw(GuiSelection<CameraComponent> component);

	struct ScriptComponent;
	void Draw(GuiSelection<ScriptComponent> component);

	struct PerpixelRendererComponent;
	void Draw(GuiSelection<PerpixelRendererComponent> component);

	struct BoxColliderComponent;
	void Draw(GuiSelection<BoxColliderComponent> component);

	struct PhysicsBodyComponent;
	void Draw(GuiSelection<PhysicsBodyComponent> component);

	struct SpriteAnimatorComponent;
	void Draw(GuiSelection<SpriteAnimatorComponent> component);

	struct TextComponent;
	void Draw(GuiSelection<TextComponent>);
}