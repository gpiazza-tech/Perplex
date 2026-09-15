#pragma once

class asIScriptEngine;

namespace Perplex
{
	class AngelAPIBinder
	{
	public:
		static void BindAPI(asIScriptEngine* engine);
	};
}