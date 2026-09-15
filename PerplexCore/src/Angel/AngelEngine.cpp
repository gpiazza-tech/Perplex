#include <Perplex/pch.h>
#include <Perplex/Angel/AngelEngine.h>

#include <Perplex/Angel/AngelAPIBinder.h>
#include <Perplex/Angel/AngelModule.h>
#include <Perplex/Core/Core.h>

#include <angelscript.h>
#include <scriptstdstring/scriptstdstring.h>
#include <scriptbuilder/scriptbuilder.h>

#include <string>

namespace Perplex
{
	void MessageCallback(const asSMessageInfo* msg, void* param)
	{
		if (msg->type == asMSGTYPE_ERROR)
			HW_ERROR("angelscript error: {0} ({1}, {2}): {3}", msg->section, msg->row, msg->col, msg->message);
		else if (msg->type == asMSGTYPE_WARNING)
			HW_WARN("angelscript error: {0} ({1}, {2}): {3}", msg->section, msg->row, msg->col, msg->message);
		else if (msg->type == asMSGTYPE_INFORMATION)
			HW_INFO("angelscript error: {0} ({1}, {2}): {3}", msg->section, msg->row, msg->col, msg->message);
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
		HW_CORE_ASSERT(r >= 0, "Failed to add section from file!");

		r = builder.AddSectionFromMemory(name.c_str(), source.c_str());
		HW_CORE_ASSERT(r >= 0, "Failed to add section from file!");

		r = builder.BuildModule();
		HW_CORE_ASSERT(r >= 0, "Compilation error!");

		asIScriptModule* mod = m_Engine->GetModule(name.c_str());
		return AngelModule{ mod, this, name };
	}

	void AngelEngine::Prepare(asIScriptFunction* func)
	{
		m_Context->Prepare(func);
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
}