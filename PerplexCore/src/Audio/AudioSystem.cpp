#include <Perplex/pch.h>
#include <Perplex/Audio/AudioSystem.h>

#include <Perplex/Audio/AudioEngine.h>
#include <Perplex/Scene/Entity.h>
#include <Perplex/Scene/SceneSystem.h>
#include <Perplex/Scene/Scene.h>
#include <Perplex/Core/Core.h>
#include <Perplex/Core/Timestep.h>

namespace Perplex
{
	AudioSystem::AudioSystem(Ref<Scene> scene) : SceneSystem(scene) {}
	AudioSystem::~AudioSystem() {}

	void AudioSystem::OnSceneStart()
	{
		m_AudioEngine.Start();
	}

	void AudioSystem::OnSceneUpdate(Timestep ts)
	{

	}

	void AudioSystem::OnSceneStop()
	{
		m_AudioEngine.Stop();
	}

	void AudioSystem::OnComponentAdded(Entity entity)
	{

	}

	void AudioSystem::OnComponentRemoved(Entity entity)
	{

	}

	void AudioSystem::PlaySound(const std::filesystem::path& audioFile)
	{
		m_AudioEngine.PlaySound(audioFile);
	}

	Sound* AudioSystem::StartLoop(const std::filesystem::path& audioFile)
	{
		return m_AudioEngine.StartLoop(audioFile);
	}

	void AudioSystem::EndLoop(Sound* sound)
	{
		m_AudioEngine.EndLoop(sound);
	}
}