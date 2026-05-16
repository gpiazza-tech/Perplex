#pragma once

#include <Holloware.h>

#include <imgui/imgui.h>
#include <imgui/imgui_internal.h>

namespace Holloware
{
	class RuntimeLayer : public Layer
	{
	public:
		RuntimeLayer();
		virtual ~RuntimeLayer() {}

		virtual void OnAttach() override;
		virtual void OnDetach() override;

		void OnUpdate(Timestep ts) override;
		virtual void OnImGuiRender() override {}
		void OnEvent(Event& e) override;
	private:
		bool OnMouseButtonPressed(MouseButtonPressedEvent& e);
		bool OnWindowResize(WindowResizeEvent& e);
		bool OnWindowRefresh(WindowRefreshEvent& e);

		void OnScenePlay();
		void OnSceneStop();

		void OnAssetImported(Asset asset);
	private:
		std::filesystem::path m_AssetsPath{};
		Ref<Scene> m_ActiveScene{};
	};
}