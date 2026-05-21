#include <Perplex/pch.h>
#include <Perplex/Audio/AudioEngine.h>

#include <Perplex/Core/Core.h>
#include <Perplex/Core/Application.h>
#include <Perplex/Core/Project.h>

#define MA_IMPLEMENTATION
#include <../vendor/miniaudio/include/miniaudio/miniaudio.h>

#include <filesystem>

namespace fs = std::filesystem;

namespace Perplex
{
	AudioEngine* AudioEngine::s_Instance = nullptr;

	AudioEngine::AudioEngine()
	{
		m_Engine = new ma_engine();
		ma_result initResult = ma_engine_init(NULL, m_Engine);

		if (initResult != MA_SUCCESS)
		{
			HW_CORE_ASSERT(false, "Failed to initialize audio engine!");
		}

		s_Instance = this;
	}

	AudioEngine::~AudioEngine()
	{
		ma_engine_uninit(m_Engine);
		delete m_Engine;
	}

	void AudioEngine::PlaySound(const fs::path& audioFile)
	{
		Project proj = Application::Get().GetCurrentProject();
		fs::path absPath = proj.GetAssetsPath() / audioFile;

		ma_engine_play_sound(m_Engine, absPath.string().c_str(), NULL);
	}
}