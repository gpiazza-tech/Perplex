#pragma once

#include <Holloware.h>

#include "Panels/Dockspace.h"
#include "Panels/ContentBrowserPanel.h"
#include "Panels/SceneHierarchyPanel.h"

#include <imgui/imgui.h>
#include <imgui/imgui_internal.h>

namespace Holloware
{
	class EditorLayer : public Layer
	{
	public:
		EditorLayer();
		virtual ~EditorLayer() {}

		virtual void OnAttach() override;
		virtual void OnDetach() override;

		void OnUpdate(Timestep ts) override;
		virtual void OnImGuiRender() override;
		void OnEvent(Event& e) override;
	private:
		bool OnMouseButtonPressed(MouseButtonPressedEvent& e);

		void OnSceneLoad();
		void OnScenePlay();
		void OnSceneStop();

		void OnResize();

		// UI Panels
		void UI_MenuBar();
		void UI_Popups();

		void UI_Viewport();
		void UI_Viewport_OnAssetDrop(Asset asset);
		void UI_Stats();
		void UI_Toolbar();

		void OnAssetImported(Asset asset);
	private:
		std::filesystem::path m_AssetsPath{};

		Ref<pxr::TextureBuffer> m_PlayIcon{};
		Ref<pxr::TextureBuffer> m_StopIcon{};

		float m_FrameMS = 0.0f;
		float m_FPS = 0.0f;

		Ref<Scene> m_ActiveScene{};

		bool m_ViewportFocused = false;
		bool m_ViewportHovered = false;
		ImVec2 m_ViewportPanelSize = ImVec2(1.0f, 1.0f);
		glm::vec2 m_ViewportSize = glm::vec2(1.0f, 1.0f);
		glm::vec2 m_ViewportBounds[2];

		EditorCamera m_EditorCamera{};
		
		// Panels
		Dockspace m_Dockspace{};
		SceneHierarchyPanel m_SceneHierarchyPanel{};
		ContentBrowserPanel m_ContentBrowserPanel{};

		enum class SceneState
		{
			Edit = 0, Play = 1
		};

		SceneState m_SceneState = SceneState::Edit;
	};
}