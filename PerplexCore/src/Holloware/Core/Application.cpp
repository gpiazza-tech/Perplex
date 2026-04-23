#include "pch.h"
#include "Application.h"

#include "Holloware/Core/Core.h"
#include "Holloware/Core/Window.h"
#include "Holloware/Core/Timestep.h"
#include "Holloware/Core/Project.h"
#include "Holloware/Core/Input.h"
#include "Holloware/Debug/Instrumentor.h"
#include "Holloware/Events/Event.h"
#include "Holloware/Events/ApplicationEvent.h"
#include "Holloware/Renderer/Renderer.h"
#include "Holloware/Scene/SceneRenderer.h"
#include "Holloware/ImGui/ImGuiLayer.h"
#include "Holloware/Assets/AssetManager.h"
#include <pxr/pxr.h>

#include <GLFW/glfw3.h>

namespace Holloware
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
		pxr::SetResourceFolder(m_Project->GetProjectPath() / "renderer");

		// Create window
		m_Window = std::unique_ptr<Window>(Window::Create(WindowProps(name)));
		m_Window->SetEventCallback(BIND_EVENT_FN(OnEvent));
		m_Window->SetVSync(false);

		// Initialize renderers
		// Renderer::Init();
		// Renderer2D::Init();
		SceneRenderer::Init(m_Window.get()->GetWidth(), m_Window.get()->GetHeight(), 16);
		
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

		for (auto it = m_LayerStack.end(); it != m_LayerStack.begin(); )
		{
			(*--it)->OnEvent(e);
			if (e.Handled())
				break;
		}
	}

	void Application::Run()
	{
		HW_PROFILE_FUNCTION();

		while (m_Running)
		{
			HW_PROFILE_SCOPE("Run Loop");

			float time = (float)glfwGetTime(); // Platform::GetTime
			Timestep timestep = time - m_LastFrameTime;
			m_LastFrameTime = time;

			Input::OnUpdate(timestep);

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

			m_Window->OnUpdate();
		}
	}

	void Application::Close()
	{
		m_Running = false;
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
		Renderer::OnWindowResize(e.GetWidth(), e.GetHeight());

		return false;
	}
}