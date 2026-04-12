#pragma once

#include <string>
#include <vector>

struct TCCState;

namespace Holloware
{
	class Entity;
	class ScriptProperty;

	class Interpreter
	{
	public:
		static void Begin();
		static void End();

		static TCCState* GenerateScriptState(const std::string& src);
		static std::vector<ScriptProperty> FindProperties(const std::string& src);
		static std::string TrimProperties(const std::string& src);
	};
}