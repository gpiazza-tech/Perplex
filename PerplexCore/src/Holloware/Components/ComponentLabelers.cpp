#include <pch.h>
#include "ComponentLabelers.h"

#include <Holloware/Scene/Components.h>

#include <string>

namespace Holloware
{
	std::string Label(IDComponent&) { return "ID"; }
	std::string Label(TagComponent&) { return "Tag"; }
	std::string Label(TransformComponent&) { return "Transform"; }
	std::string Label(SpriteRendererComponent&) { return "Sprite Renderer"; }
	std::string Label(CameraComponent&) { return "Camera"; }
	std::string Label(ScriptComponent&) { return "Script"; }
	std::string Label(PerpixelRendererComponent&) { return "Perpixel Renderer"; }
}