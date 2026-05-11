#pragma once

#include <string>

namespace Holloware
{
	struct IDComponent;
	std::string Label(IDComponent&);

	struct TagComponent;
	std::string Label(TagComponent&);

	struct TransformComponent;
	std::string Label(TransformComponent&);

	struct SpriteRendererComponent;
	std::string Label(SpriteRendererComponent&);

	struct CameraComponent;
	std::string Label(CameraComponent&);

	struct ScriptComponent;
	std::string Label(ScriptComponent&);

	struct PerpixelRendererComponent;
	std::string Label(PerpixelRendererComponent&);
}