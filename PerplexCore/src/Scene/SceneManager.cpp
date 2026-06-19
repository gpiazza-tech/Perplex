#include <Perplex/pch.h>
#include <Perplex/Scene/SceneManager.h>

#include <Perplex/Scene/Scene.h>
#include <Perplex/Core/Log.h>
#include <Perplex/Scene/SceneSerializer.h>
#include <Perplex/Assets/Asset.h>
#include <Perplex/Assets/AssetType.h>

namespace Perplex
{
	SceneManager::SceneManager()
		: m_ActiveScene{ nullptr } {}

	void SceneManager::LoadScene(Asset asset)
	{
		if (asset.GetType() != AssetType::SceneAsset)
			HW_CORE_ERROR("Tried to load Scene from invalid asset type!");

		m_NextScene = asset.LoadData<Scene>();
	}

	void SceneManager::LoadScene(Ref<Scene> scene)
	{ 
		m_NextScene = scene;
	};

	void SceneManager::SaveScene(const std::filesystem::path& path)
	{
		SceneSerializer::Serialize(m_ActiveScene, path);
	}

	void SceneManager::OnUpdateEnd()
	{
		if (m_JustLoaded)
			m_JustLoaded = false;

		if (m_ActiveScene != m_NextScene)
		{
			bool playing = m_ActiveScene != nullptr && m_ActiveScene->IsPlaying();

			if (playing)
				m_ActiveScene->Stop();

			m_ActiveScene = m_NextScene;

			if (playing)
				m_ActiveScene->Start();

			m_JustLoaded = true;
		}
	}
}