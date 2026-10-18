#include <Perplex/pch.h>
#include <Perplex/Angel/AngelModule.h>

#include <Perplex/Angel/AngelEngine.h>
#include <Perplex/Angel/AngelObject.h>
#include <Perplex/Angel/AngelProperty.h>
#include <Perplex/Angel/AngelUtils.h>
#include <Perplex/Scene/Entity.h>

#include <angelscript.h>

#include <string>
#include <string_view>
#include <vector>
#include <string.h>

namespace Perplex
{
	AngelModule::AngelModule(asIScriptModule* mod, AngelEngine* engine, const std::string& name, const std::vector<AngelProperty>& properties) 
		: m_Module(mod), m_Engine(engine), m_Name(name), m_Properties(properties) { }

	AngelObject AngelModule::CreateObject(Entity entity, const std::vector<AngelProperty>& properties)
	{
		return CreateObject(m_Name, entity, properties);
	}

	AngelObject AngelModule::CreateObject(const std::string& className, Entity entity, const std::vector<AngelProperty>& properties)
	{
		if (!m_Module)
			return AngelObject(nullptr, nullptr);

		asITypeInfo* type = m_Module->GetTypeInfoByDecl(className.c_str());

		std::string constructorDecl = className + " @" + className + "()"; // e.g., "MyClass @MyClass()"
		asIScriptFunction* constructor = type->GetFactoryByDecl(constructorDecl.c_str());

		m_Engine->Prepare(constructor);
		m_Engine->Execute();
		asIScriptObject* object = m_Engine->GetReturn<asIScriptObject*>();

		// Set entity and properties
		asUINT count = object->GetPropertyCount();
		for (asUINT i = 0; i < count; ++i) 
		{
			std::string_view propName = object->GetPropertyName(i);
			std::string_view propType = AngelUtils::AngelTypeToString(m_Module->GetEngine(), object->GetPropertyTypeId(i));

			if (propName == "entity")
			{
				*(Entity*)object->GetAddressOfProperty(i) = entity;
			}
			else
			{
				for (const auto& property : properties)
				{
					if (propName == property.Name && propType == property.Type)
						std::memcpy(object->GetAddressOfProperty(i), property.Data.data(), property.Data.size());
				}
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