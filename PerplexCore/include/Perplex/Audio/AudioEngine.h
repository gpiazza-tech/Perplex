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

		static inline AudioEngine& Get() { return *s_Instance; };
		
		void PlaySound(const std::filesystem::path& audioFile);
		Sound* StartLoop(const std::filesystem::path& audioFile);
		void EndLoop(Sound* sound);
	private:
		static AudioEngine* s_Instance;

		ma_engine* m_Engine{};
	};
}