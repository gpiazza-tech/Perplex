#include <Perplex/pch.h>
#include <Perplex/Angel/AngelModule.h>

#include <Perplex/Angel/AngelObject.h>
#include <Perplex/Scene/Entity.h>

#include <angelscript.h>

namespace Perplex
{
	AngelModule::AngelModule(asIScriptModule* mod, AngelEngine* engine, const std::string& name) : m_Module(mod), m_Engine(engine), m_Name(name) { }

	AngelObject AngelModule::CreateObject(Entity entity)
	{
		return CreateObject(m_Name, entity);
	}

	AngelObject AngelModule::CreateObject(const std::string& className, Entity entity)
	{
		asITypeInfo* type = m_Module->GetTypeInfoByDecl(className.c_str());

		std::string constructorDecl = className + " @" + className + "()"; // e.g., "MyClass @MyClass()"
		asIScriptFunction* constructor = type->GetFactoryByDecl(constructorDecl.c_str());

		m_Engine->Prepare(constructor);
		m_Engine->Execute();
		asIScriptObject* object = m_Engine->GetReturn<asIScriptObject*>();

		// Set entity
		asUINT count = object->GetPropertyCount();
		for (asUINT i = 0; i < count; ++i) 
		{
			const char* propName = object->GetPropertyName(i);
			if (std::string_view{ "entity" } == propName)
			{
				*(Entity*)object->GetAddressOfProperty(i) = entity;
			}
		}

		object->AddRef();
		return AngelObject{ object, this };
	}

	asIScriptFunction* AngelModule::GetFunctionByName(const std::string& funcName)
	{
		return m_Module->GetFunctionByName(funcName.c_str());
	}

	asIScriptFunction* AngelModule::GetMethod(const std::string& className, const std::string& funcName)
	{
		asITypeInfo* type = m_Module->GetTypeInfoByName(className.c_str());
		return type->GetMethodByName(funcName.c_str());
	}
}