#include <Perplex/pch.h>
#include <Perplex/Angel/AngelEngine.h>

#include <Perplex/Angel/AngelAPIBinder.h>
#include <Perplex/Angel/AngelModule.h>
#include <Perplex/Angel/AngelProperty.h>
#include <Perplex/Angel/AngelUtils.h>
#include <Perplex/Core/Core.h>
#include <Perplex/Core/Log.h>

#include <angelscript.h>
#include <scriptstdstring/scriptstdstring.h>
#include <scriptarray/scriptarray.h>
#include <scriptbuilder/scriptbuilder.h>
#include <scripthandle/scripthandle.h>

#include <string>
#include <utility>
#include <cstddef>
#include <algorithm>
#include <cstdint>
#include <vector>

namespace Perplex
{
	void MessageCallback(const asSMessageInfo* msg, void* param)
	{
		if (msg->type == asMSGTYPE_ERROR)
			HW_ERROR("ANGELSCRIPT {0} ({1}, {2}): {3}", msg->section, msg->row, msg->col, msg->message);
		else if (msg->type == asMSGTYPE_WARNING)
			HW_WARN("ANGELSCRIPT {0} ({1}, {2}): {3}", msg->section, msg->row, msg->col, msg->message);
		else if (msg->type == asMSGTYPE_INFORMATION)
			HW_INFO("ANGELSCRIPT {0} ({1}, {2}): {3}", msg->section, msg->row, msg->col, msg->message);
	}

	void PrintExceptionInfo(asIScriptContext* ctx)
	{
		asIScriptEngine* engine = ctx->GetEngine();

		HW_CORE_ERROR("Script Exception: {0}", ctx->GetExceptionString());

		const asIScriptFunction* function = ctx->GetExceptionFunction();
		HW_CORE_ERROR("Module: {0}, Function: {1}, Line: {2}", function->GetModuleName(), function->GetDeclaration(), ctx->GetExceptionLineNumber());
	}

	AngelEngine::AngelEngine()
	{
		m_Engine = asCreateScriptEngine();
		 
		int r = m_Engine->SetMessageCallback(asFUNCTION(MessageCallback), 0, asCALL_CDECL);
		HW_CORE_ASSERT(r >= 0, "Failed to set message callback!");

		RegisterStdString(m_Engine);
		RegisterScriptArray(m_Engine, true);
		RegisterScriptHandle(m_Engine);
		m_Engine->SetEngineProperty(asEP_ALLOW_UNSAFE_REFERENCES, true);
		
		AngelAPIBinder::BindAPI(m_Engine);

		m_Context = m_Engine->CreateContext();
	}

	AngelEngine::~AngelEngine()
	{
		m_Context->Release();
		m_Engine->ShutDownAndRelease(); 
	}

	AngelModule AngelEngine::CreateModule(const std::string& name, const std::string& source)
	{
		CScriptBuilder builder;

		int r = builder.StartNewModule(m_Engine, name.c_str());
		HW_CORE_ASSERT(r >= 0, "Unrecoverable error while starting a new module.");

		r = builder.AddSectionFromMemory("EntityScript", "shared class EntityScript { Entity entity; }");
		HW_CORE_ASSERT(r >= 0, "Failed to add EntityScript section!");

		r = builder.AddSectionFromMemory(name.c_str(), source.c_str());
		if (r < 0)
			HW_CORE_ERROR("Failed to add section from file!");

		r = builder.BuildModule();
		if (r < 0)
		{
			HW_CORE_ERROR("Compilation error!"); 
			return AngelModule{ nullptr, this, name, std::vector<AngelProperty>() };
		}

		std::vector<AngelProperty> properties{};

		asITypeInfo* typeInfo = m_Engine->GetModule(name.c_str())->GetTypeInfoByName(name.c_str());
		int typeID = typeInfo->GetTypeId();
		uint32_t propertyCount = typeInfo->GetPropertyCount();

		// Used to find default values of properties
		asIScriptObject* defaultObject = static_cast<asIScriptObject*>(m_Engine->CreateScriptObject(typeInfo));

		for (uint32_t i{ 0 }; i < propertyCount; ++i)
		{
			int propertyIndex = static_cast<int>(i);

			std::vector<std::string> metadata = builder.GetMetadataForTypeProperty(typeID, propertyIndex);
			if (std::find(metadata.begin(), metadata.end(), "gui") == metadata.end())
				continue;

			const char* propertyName{ nullptr };
			int propertyTypeID{ 0 };
			typeInfo->GetProperty(propertyIndex, &propertyName, &propertyTypeID);
			const char* propertyTypeName = AngelUtils::AngelTypeToString(m_Engine, propertyTypeID);
			size_t propertyTypeSize = AngelUtils::GetAngelTypeSize(m_Engine, propertyTypeID);

			void* propertyData = defaultObject->GetAddressOfProperty(i);

			AngelProperty property{};
			property.Name = propertyName;
			property.Type = propertyTypeName;
			property.Data.reserve(propertyTypeSize);
			property.Data.insert(property.Data.end(), (std::byte*)propertyData, (std::byte*)propertyData + propertyTypeSize);

			properties.push_back(std::move(property));
		}

		defaultObject->Release();

		asIScriptModule* mod = m_Engine->GetModule(name.c_str());
		return AngelModule{ mod, this, name, properties };
	}

	void AngelEngine::Prepare(asIScriptFunction* func)
	{
		int r = m_Context->Prepare(func);
		HW_CORE_ASSERT(r >= 0, "Error preparing function!");
	}

	void AngelEngine::SetArgByte(int arg, uint8_t val)
	{
		m_Context->SetArgByte(arg, val);
	}

	void AngelEngine::SetArgWord(int arg, uint16_t val)
	{
		m_Context->SetArgWord(arg, val);
	}

	void AngelEngine::SetArgDWord(int arg, uint32_t val)
	{
		m_Context->SetArgDWord(arg, val);
	}

	void AngelEngine::SetArgQWord(int arg, uint64_t val)
	{
		m_Context->SetArgQWord(arg, val);
	}

	void AngelEngine::SetArgFloat(int arg, float val)
	{
		m_Context->SetArgFloat(arg, val);
	}

	void AngelEngine::SetArgDouble(int arg, double val)
	{
		m_Context->SetArgDouble(arg, val);
	}

	void AngelEngine::SetArgObject(int arg, void* obj)
	{
		m_Context->SetArgObject(arg, obj);
	}

	void AngelEngine::SetArgAddress(int arg, void* addr)
	{
		m_Context->SetArgAddress(arg, addr);
	}

	void AngelEngine::Execute()
	{
		int result = m_Context->Execute();
		if (result == asEXECUTION_EXCEPTION)
			PrintExceptionInfo(m_Context);
	}

	uint8_t AngelEngine::GetReturnByte()
	{
		return m_Context->GetReturnByte();
	}

	uint16_t AngelEngine::GetReturnWord()
	{
		return m_Context->GetReturnWord();
	}

	uint32_t AngelEngine::GetReturnDWord()
	{
		return m_Context->GetReturnDWord();
	}

	uint64_t AngelEngine::GetReturnQWord()
	{
		return m_Context->GetReturnQWord();
	}

	float AngelEngine::GetReturnFloat()
	{
		return m_Context->GetReturnFloat();
	}

	double AngelEngine::GetReturnDouble()
	{
		return m_Context->GetReturnDouble();
	}

	void* AngelEngine::GetReturnObject()
	{
		return m_Context->GetReturnObject();
	}

	void* AngelEngine::GetReturnAddress()
	{
		return m_Context->GetReturnAddress();
	}

	void AngelEngine::SetObject(void* obj)
	{
		m_Context->SetObject(obj);
	}

	int AngelEngine::GetTypeIDByName(const char* typeName)
	{
		return AngelUtils::StringToAngelType(m_Engine, typeName);
	}

	asITypeInfo* AngelEngine::GetTypeInfoByName(const char* typeName)
	{
		return m_Engine->GetTypeInfoByDecl(typeName);
	}
}