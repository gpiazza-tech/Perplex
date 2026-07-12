#include <Perplex/pch.h>
#include <Perplex/Core/Application.h>

#include <Perplex/Core/Core.h>
#include <Perplex/Core/Window.h>
#include <Perplex/Core/Timestep.h>
#include <Perplex/Core/Project.h>
#include <Perplex/Debug/Instrumentor.h>
#include <Perplex/Events/Event.h>
#include <Perplex/Events/ApplicationEvent.h>
#include <Perplex/Scene/SceneRenderer.h>
#include <Perplex/ImGui/ImGuiLayer.h>
#include <Perplex/Assets/AssetManager.h>
#include <pxr/pxr.h>

#include <functional>
#include <memory>
#include <string>

// TODO: move to WindowsWindow
#include <GLFW/glfw3.h>

namespace Perplex
{

#define BIND_EVENT_FN(x) std::bind(&Application::x, this, std::placeholders::_1)

	Application* Application::s_Instance = nullptr;

	Application::Application(const std::string& name)
	{
		HW_PROFILE_FUNCTION();

		HW_CORE_ASSERT(!s_Instance, "Application already exists!");
		s_Instance = this;

		// Set Current Project
		m_Project = new Project("C:/dev/PerplexProjects/Game");
		pxr::SetResourceFolder(m_Project->GetProjectPath() / "engine/res");

		// Create window
		m_Window = std::unique_ptr<Window>(Window::Create(WindowProps(name)));
		m_Window->SetEventCallback(BIND_EVENT_FN(OnEvent));
		m_Window->SetVSync(false);

		// Initialize renderers
		// Renderer::Init();
		// Renderer2D::Init();
		pxr::Renderer::Init(16);
		
		// Initialize Asset Manager
		AssetManager::Init();

		// Create ImGui Layer
		m_ImGuiLayer = new ImGuiLayer();
		PushOverlay(m_ImGuiLayer);
	}

	Application::~Application()
	{
		AssetManager::Cleanup();
	}

	void Application::PushLayer(Layer* layer)
	{
		HW_PROFILE_FUNCTION();

		m_LayerStack.PushLayer(layer);
		layer->OnAttach();
	}

	void Application::PushOverlay(Layer* overlay)
	{
		HW_PROFILE_FUNCTION();

		m_LayerStack.PushOverlay(overlay);
		overlay->OnAttach();
	}

	void Application::OnEvent(Event& e)
	{
		HW_PROFILE_FUNCTION();

		EventDispatcher dispatcher(e);
		dispatcher.Dispatch<WindowCloseEvent>(BIND_EVENT_FN(OnWindowClose));
		dispatcher.Dispatch<WindowResizeEvent>(BIND_EVENT_FN(OnWindowResize));
		dispatcher.Dispatch<WindowRefreshEvent>(BIND_EVENT_FN(OnWindowRefresh));

		for (auto it = m_LayerStack.end(); it != m_LayerStack.begin(); )
		{
			(*--it)->OnEvent(e);
			if (e.Handled())
				break;
		}
	}

	void Application::Update()
	{
		float time = (float)glfwGetTime(); // Platform::GetTime
		Timestep timestep = (time - m_LastFrameTime) * m_Timescale;
		m_LastFrameTime = time;

		if (!m_Minimized)
		{
			{
				HW_PROFILE_SCOPE("LayerStack OnUpdate");

				for (Layer* layer : m_LayerStack)
					layer->OnUpdate(timestep);
			}

			m_ImGuiLayer->Begin();
			{
				HW_PROFILE_SCOPE("LayerStack OnImGuiRender");

				for (Layer* layer : m_LayerStack)
					layer->OnImGuiRender();
			}
			m_ImGuiLayer->End();
		}
	}

	void Application::Run()
	{
		HW_PROFILE_FUNCTION();

		while (m_Running)
		{
			HW_PROFILE_SCOPE("Run Loop");

			Update();
			m_Window->OnUpdate();
		}
	}

	void Application::Close()
	{
		m_Running = false;
	}

	float Application::GetTimescale() const
	{
		return m_Timescale;
	}

	void Application::SetTimescale(float timescale)
	{
		m_Timescale = timescale;
	}

	bool Application::OnWindowClose(WindowCloseEvent& e)
	{
		HW_PROFILE_FUNCTION();

		m_Running = false;
		return true;
	}

	bool Application::OnWindowResize(WindowResizeEvent& e)
	{
		HW_PROFILE_FUNCTION();

		if (e.GetWidth() == 0 || e.GetHeight() == 0)
		{
			m_Minimized = true;
			return false;
		}

		m_Minimized = false;
		return false;
	}

	bool Application::OnWindowRefresh(WindowRefreshEvent& e)
	{
		Update();
		return false;
	}
}