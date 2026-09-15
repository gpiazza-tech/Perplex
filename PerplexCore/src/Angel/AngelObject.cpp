#include <Perplex/pch.h>
#include <Perplex/Angel/AngelObject.h>

#include <Perplex/Angel/AngelEngine.h>

#include <angelscript.h>

namespace Perplex
{
	AngelObject::AngelObject(asIScriptObject* obj, AngelModule* mod) : m_Object(obj), m_Module(mod) { }
}