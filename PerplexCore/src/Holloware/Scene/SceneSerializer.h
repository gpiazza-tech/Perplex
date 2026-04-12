#pragma once

#include <Holloware/Core/Core.h>

#include <filesystem>
#include <string>

namespace std { typedef basic_string<char> string; }

namespace Holloware
{
	class Scene;
	class Asset;

	class SceneSerializer
	{
	public:
		static bool Serialize(const Ref<Scene>& scene, const std::filesystem::path& path);
		static Ref<Scene> Deserialize(const std::filesystem::path& path);
	};
} 