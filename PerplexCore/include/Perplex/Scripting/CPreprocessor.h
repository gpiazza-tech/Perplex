#pragma once

#include <string>
#include <vector>

namespace Perplex
{
	class Entity;
	class ScriptProperty;

	std::vector<ScriptProperty> FindProperties(const std::string& src);
	std::string TrimProperties(const std::string& src);
}