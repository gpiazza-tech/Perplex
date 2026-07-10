#include <Perplex/pch.h>
#include <Perplex/Audio/AudioEngine.h>

#include <Perplex/Core/Core.h>
#include <Perplex/Core/Log.h>
#include <Perplex/Core/Application.h>
#include <Perplex/Core/Project.h>

#define MA_IMPLEMENTATION
#include <../vendor/miniaudio/include/miniaudio/miniaudio.h>

#include <filesystem>

namespace fs = std::filesystem;

namespace Perplex
{
	AudioEngine::AudioEngine()
	{
		m_Engine = new ma_engine();
		ma_result initResult = ma_engine_init(NULL, m_Engine);

		if (initResult != MA_SUCCESS)
		{
			HW_CORE_ASSERT(false, "Failed to initialize audio engine!");
		}
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

		ma_result playResult = ma_engine_play_sound(m_Engine, absPath.string().c_str(), NULL);

		switch (playResult)
		{
		case MA_SUCCESS:
			return;
		case MA_DOES_NOT_EXIST:
			HW_WARN("Audio file {} does not exist!", audioFile.string());
			break;
		default:
			HW_WARN("Failed to play audio file {}", audioFile.string());
			break;
		}
	}

	Sound* AudioEngine::StartLoop(const fs::path& audioFile)
	{
		Project proj = Application::Get().GetCurrentProject();
		fs::path absPath = proj.GetAssetsPath() / audioFile;

		ma_sound* sound = new ma_sound{};
		ma_result initResult;
		initResult = ma_sound_init_from_file(m_Engine, absPath.string().c_str(), 0, NULL, NULL, sound);
		switch (initResult)
		{
		case MA_SUCCESS:
			break;
		case MA_DOES_NOT_EXIST:
			HW_WARN("Audio file {} does not exist!", audioFile.string());
			break;
		default:
			HW_WARN("Failed to init audio file {}", audioFile.string());
			break;
		}

		ma_sound_set_looping(sound, MA_TRUE);

		ma_result startResult = ma_sound_start(sound);
		if (startResult != MA_SUCCESS)
			HW_WARN("Failed to start audio file {}", audioFile.string());

		return sound;
	}

	void AudioEngine::EndLoop(Sound* sound)
	{
		ma_sound_uninit(sound);
		delete sound;
	}

	void AudioEngine::Stop()
	{
		ma_engine_stop(m_Engine);
	}

	void AudioEngine::Start()
	{
		ma_engine_start(m_Engine);
	}
}