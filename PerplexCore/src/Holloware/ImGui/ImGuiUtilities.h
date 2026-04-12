#pragma once

#include <glm/fwd.hpp>

#include <any>
#include <string>

namespace std { typedef basic_string<char> string; }

namespace Holloware
{
	class Entity;
	class Asset;
	class EntityData;

	class ImGuiUtilities 
	{
	public:
		static void DrawVec3Control(const std::string& label, glm::vec3& values, float resetValue = 0.0f, float columnWidth = 100.0f);
		static void DrawAnyIntControl(const char* label, std::any& val);
		static void DrawAnyFloatControl(const char* label, std::any& val);
		static void DrawAnyDoubleControl(const char* label, std::any& val);
		static void DrawAnyBoolControl(const char* label, std::any& val);
		static void EntityInput(const char* label, EntityData& entity);
		static void AssetInput(const char* label, Asset& asset);
	};
}