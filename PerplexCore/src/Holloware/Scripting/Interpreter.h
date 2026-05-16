#pragma once

#include <string>
#include <vector>

namespace Holloware
{
	class Entity;
	class ScriptProperty;

	class Interpreter
	{
	public:
		static void Begin();
		static void End();

		static std::vector<ScriptProperty> FindProperties(const std::string& src);
		static std::string TrimProperties(const std::string& src);
	};
}