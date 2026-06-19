#pragma once

#include <Perplex/Core/Core.h>

namespace Perplex
{
	class Scene;
	class Asset;

	class SceneManager
	{
	public:
		SceneManager();
		static SceneManager& Get()
		{
			static SceneManager instance{};
			return instance;
		}

		void LoadScene(Asset asset);
		void LoadScene(Ref<Scene> scene);
		void SaveScene(const std::filesystem::path& path);
		Ref<Scene> ActiveScene() { return m_ActiveScene; }

		void OnUpdateEnd();
		bool JustLoaded() const { return m_JustLoaded; }
	private:
		Ref<Scene> m_ActiveScene{};
		Ref<Scene> m_NextScene{};

		bool m_JustLoaded{ false };
	};
}