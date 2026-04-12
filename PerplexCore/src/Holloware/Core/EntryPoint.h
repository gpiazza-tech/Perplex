#pragma once

#include "Holloware/Core/Application.h"
#include "Holloware/Core/Log.h"
#include "Holloware/Debug/Instrumentor.h"

#ifdef HW_PLATFORM_WINDOWS

extern Holloware::Application* Holloware::CreateApplication();

int main(int argc, char** argv)
{
	Holloware::Log::Init();

	HW_PROFILE_BEGIN_SESSION("Startup", "profiling/HollowareProfile-Startup.json");
	auto app = Holloware::CreateApplication();
	HW_PROFILE_END_SESSION();

	HW_PROFILE_BEGIN_SESSION("Runtime", "profiling/HollowareProfile-Runtime.json");
	app->Run();
	HW_PROFILE_END_SESSION();

	HW_PROFILE_BEGIN_SESSION("Startup", "profiling/HollowareProfile-Shutdown.json");
	delete app;
	HW_PROFILE_END_SESSION();
}

#endif
