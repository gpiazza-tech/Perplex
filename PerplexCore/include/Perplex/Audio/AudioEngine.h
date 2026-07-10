#pragma once

#include <filesystem>

struct ma_engine;
struct ma_sound;
using Sound = ma_sound;

namespace Perplex
{
	class AudioEngine
	{
	public:
		AudioEngine();
		~AudioEngine();

		void PlaySound(const std::filesystem::path& audioFile);
		Sound* StartLoop(const std::filesystem::path& audioFile);
		void EndLoop(Sound* sound);

		void Stop();
		void Start();
	private:
		ma_engine* m_Engine{};
	};
}