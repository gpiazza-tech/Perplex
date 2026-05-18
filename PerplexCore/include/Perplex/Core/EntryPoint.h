#pragma once

#include <Perplex/Core/Application.h>
#include <Perplex/Core/Log.h>
#include <Perplex/Debug/Instrumentor.h>

#ifdef HW_PLATFORM_WINDOWS

extern Perplex::Application* Perplex::CreateApplication();

int main(int argc, char** argv)
{
	Perplex::Log::Init();

	HW_PROFILE_BEGIN_SESSION("Startup", "profiling/HollowareProfile-Startup.json");
	auto app = Perplex::CreateApplication();
	HW_PROFILE_END_SESSION();

	HW_PROFILE_BEGIN_SESSION("Runtime", "profiling/HollowareProfile-Runtime.json");
	app->Run();
	HW_PROFILE_END_SESSION();

	HW_PROFILE_BEGIN_SESSION("Startup", "profiling/HollowareProfile-Shutdown.json");
	delete app;
	HW_PROFILE_END_SESSION();
}

#endif
