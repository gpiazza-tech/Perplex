#include <Perplex/pch.h>
#include <Perplex/Core/Application.h>

#include <Perplex/Core/Core.h>
#include <Perplex/Core/Window.h>
#include <Perplex/Core/Timestep.h>
#include <Perplex/Core/FileIO.h>
#include <Perplex/Debug/Instrumentor.h>
#include <Perplex/Events/Event.h>
#include <Perplex/Events/ApplicationEvent.h>
#include <Perplex/Scene/SceneRenderer.h>
#include <Perplex/Scene/SceneManager.h>
#include <Perplex/ImGui/ImGuiLayer.h>
#include <Perplex/Assets/AssetManager.h>
#include <Perplex/Serialization/JsonHelper.h>
#include <Perplex/Platform/SystemUtils.h>
#include <Perplex/Core/Game.h>
#include <Perplex/Angel/AngelEngine.h>
#include <pxr/pxr.h>

#include <filesystem>
#include <functional>
#include <memory>
#include <string>

namespace fs = std::filesystem;

// This function is for dev purposes only. It tries to find the source res folder in PerplexCore,
// and if it exists it copies it into the game root directory. If it's not found, the operation is skipped.
static void TryCopyResFolder(const std::filesystem::path& target)
{
	if (!Perplex::FileIO::TryCreateDirectories(target))
		HW_CORE_ERROR("Failed to create engine res path!");

	fs::path resPath = std::filesystem::current_path() / "../PerplexCore/res";
	if (fs::exists(resPath))
	{
		fs::copy(resPath, target,
			fs::copy_options::recursive | fs::copy_options::overwrite_existing);
	}

	else
		HW_CORE_INFO("Res folder not found, continuing launch...");
}

namespace Perplex
{
#define BIND_EVENT_FN(x) std::bind(&Application::x, this, std::placeholders::_1)

	Application* Application::s_Instance = nullptr;

	Application::Application(const std::filesystem::path& gameDirectory, const std::string& name)
	{
		HW_PROFILE_FUNCTION();

		HW_CORE_ASSERT(!s_Instance, "Application already exists!");
		s_Instance = this;

		// Launch game
		LoadGame(gameDirectory);

		// Create window
		m_Window = std::unique_ptr<Window>(Window::Create(WindowProps(name)));
		m_Window->SetEventCallback(BIND_EVENT_FN(OnEvent));
		m_Window->SetVSync(false);

		// Create asset manager
		m_AssetManager = std::make_unique<AssetManager>(m_Game.AssetsDirectory);
		m_AssetManager->SetEventCallback(BIND_EVENT_FN(OnEvent));

		// Create angel engine
		m_AngelEngine = std::make_unique<AngelEngine>();

		// Initialize Renderer and Resources
		TryCopyResFolder(m_Game.RootDirectory / "engine/res");
		pxr::SetResourceFolder(m_Game.RootDirectory / "engine/res");
		pxr::Renderer::Init(16);

		if (m_Game.StartScene)
			SceneManager::Get().LoadScene(m_Game.StartScene);

		// Create ImGui Layer
		m_ImGuiLayer = new ImGuiLayer();
		PushOverlay(m_ImGuiLayer);
	}

	std::filesystem::path Application::EngineRes(const std::filesystem::path& relative) const
	{
		return m_Game.RootDirectory / "engine/res" / relative;
	}

	Application::~Application()
	{
		for (auto it = m_LayerStack.end(); it != m_LayerStack.begin(); )
			(*--it)->OnDetach();

		SaveGame();
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

	void Application::LoadGame(const fs::path& gameDirectory)
	{
		auto pxgamePath = FileIO::FindFileWithExtension(".pxgame", gameDirectory);
		HW_CORE_ASSERT(pxgamePath.has_value(), ".pxgame file not found in directory {0}!", gameDirectory.string().c_str());

		JsonHelper::ObjectFromFile(m_Game, pxgamePath.value());

		m_Game.RootDirectory = pxgamePath.value().parent_path();
		m_Game.AssetsDirectory = m_Game.RootDirectory / "assets";
		m_Game.Title = pxgamePath.value().stem().string();
	}

	void Application::SaveGame() const
	{
		JsonHelper::ObjectToFile(m_Game, m_Game.RootDirectory / (m_Game.Title + ".pxgame"));
	}

	void Application::Update()
	{
		float time = GetTime();
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