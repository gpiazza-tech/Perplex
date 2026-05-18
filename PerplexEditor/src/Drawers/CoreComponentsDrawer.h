#pragma once

#include <Perplex/Scene/Components.h>

namespace Perplex
{
	void DrawGui(IDComponent& component);
	void DrawGui(TagComponent& component);
	void DrawGui(TransformComponent& component);
	void DrawGui(SpriteRendererComponent& component);
	void DrawGui(CameraComponent& component);
	void DrawGui(ScriptComponent& component);
	void DrawGui(PerpixelRendererComponent& component);
}