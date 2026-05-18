#pragma once

#include "ScriptProperty.h"

#include <string>
#include <vector>

namespace Perplex
{
	class Entity;

	struct ScriptData
	{
		std::string Source;
		std::vector<ScriptProperty> Properties;
	};
}