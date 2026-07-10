#pragma once

#include <Perplex/Audio/AudioEngine.h>
#include <Perplex/Scene/SceneSystem.h>
#include <Perplex/Components/Components.h>
#include <Perplex/Core/TypeID.h>
#include <Perplex/Core/Core.h>
#include <Perplex/Core/Timestep.h>

#include <filesystem>

namespace Perplex
{
	class Scene;
	class Entity;

	class AudioSystem : SceneSystem
	{
	public:
		AudioSystem(Ref<Scene> scene);
		~AudioSystem();

		TypeID GetComponentTypeID() override { return GetTypeID<AudioComponent>(); }

		void OnSceneStart() override;
		void OnSceneUpdate(Timestep ts) override;
		void OnSceneStop() override;

		void OnComponentAdded(Entity entity) override;
		void OnComponentRemoved(Entity entity) override;

		void PlaySound(const std::filesystem::path& audioFile);
		Sound* StartLoop(const std::filesystem::path& audioFile);
		void EndLoop(Sound* sound);
	private:
		AudioEngine m_AudioEngine{};
	};
}