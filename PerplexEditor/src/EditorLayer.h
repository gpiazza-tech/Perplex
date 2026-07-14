#pragma once

#include <Perplex/Perplex.h>

#include "Panels/Dockspace.h"
#include "Panels/ContentBrowserPanel.h"
#include "Panels/SceneHierarchyPanel.h"
#include "Panels/PerpixelPanel.h"
#include "Panels/ViewportPanel.h"
#include <Perplex/ImGui/StringPopup.h>
#include <Perplex/Core/RenderSettings.h>

#include <imgui/imgui.h>
#include <imgui/imgui_internal.h>

namespace Perplex
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

		void OnScenePlay();
		void OnSceneStop();

		void OnResize();

		// UI Panels
		void UI_MenuBar();

		void UI_Stats();
		void UI_Toolbar();
		void UI_RenderSettings();

		void OnAssetImported(Asset asset);
	private:
		std::filesystem::path m_AssetsPath{};

		Ref<pxr::TextureBuffer> m_PlayIcon{};
		Ref<pxr::TextureBuffer> m_StopIcon{};

		float m_FrameMS = 0.0f;
		float m_FPS = 0.0f;

		SceneRenderer m_SceneRenderer{ 1, 1, 16 };
		AudioEngine m_AudioEngine{};

		EditorCamera m_EditorCamera{};

		RenderSettings m_RenderSettings{};
		
		// Panels
		Dockspace m_Dockspace{};
		ViewportPanel m_ViewportPanel{};
		SceneHierarchyPanel m_SceneHierarchyPanel{};
		ContentBrowserPanel m_ContentBrowserPanel{};
		PerpixelPanel m_PerpixelPanel{};

		StringPopup m_SavePopup{ "Save Scene" };
		StringPopup m_LoadPopup{ "Load Scene" };

		enum class SceneState
		{
			Edit = 0, Play = 1
		};

		SceneState m_SceneState = SceneState::Edit;
	};
}