#pragma once

#include "Holloware/Core/LayerStack.h"

#include <memory>
#include <string>

namespace Holloware
{
	class Window;
	class Event;
	class WindowCloseEvent;
	class WindowResizeEvent;
	class ImGuiLayer;
	class Project;
	class Layer;

	class Application
	{
	public:
		Application(const std::string& name = "Holloware App");
		virtual ~Application();

		void Run();

		void OnEvent(Event& e);

		void PushLayer(Layer* layer);
		void PushOverlay(Layer* overlay);

		static inline Application& Get() { return *s_Instance; }
		inline Window& GetWindow() { return *m_Window; }
		inline Project& GetCurrentProject() { return *m_Project; }

		void Close();

		ImGuiLayer* GetImGuiLayer() { return m_ImGuiLayer; }
	private:
		bool OnWindowClose(WindowCloseEvent& e);
		bool OnWindowResize(WindowResizeEvent& e);
	private:
		std::unique_ptr<Window> m_Window;
		ImGuiLayer* m_ImGuiLayer;

		bool m_Running = true;
		bool m_Minimized = false;

		LayerStack m_LayerStack;
		float m_LastFrameTime = 0.0f;

		Project* m_Project;
	private:
		static Application* s_Instance;
	};

	// To be defined in CLIENT
	Application* CreateApplication();
}

