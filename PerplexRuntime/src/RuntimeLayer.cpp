#include "RuntimeLayer.h"

#include <Holloware/Core/Window.h>

#include <imgui/imgui.h>

namespace Holloware
{
    namespace fs = std::filesystem;

    RuntimeLayer::RuntimeLayer()
        : Layer("RuntimeLayer")
    {
    }

    void RuntimeLayer::OnAttach()
    {
        HW_PROFILE_FUNCTION();

        m_AssetsPath = Application::Get().GetCurrentProject().GetAssetsPath();

        // Set Asset imported callback
        AssetManager::SetAssetImportedCallback([this](Asset asset) { OnAssetImported(asset); });

        // Load Scene
        // TODO: should not be absolute path
        Asset sceneAsset = Asset("C:/dev/PerplexProjects/Game/assets/scenes/scene.hws");
        m_ActiveScene = sceneAsset.GetData<Scene>();
        OnScenePlay();

        Window& window = Application::Get().GetWindow();
        m_ActiveScene->OnViewportResize(window.GetWidth(), window.GetHeight());
    }

    void RuntimeLayer::OnDetach()
    {
        HW_PROFILE_FUNCTION();
    }

    void RuntimeLayer::OnUpdate(Timestep ts)
    {
        HW_PROFILE_FUNCTION();

        m_ActiveScene->OnUpdateRuntime(ts);
        SceneRenderer::DrawToScreen();
    }

    void RuntimeLayer::OnEvent(Event& e)
    {
        EventDispatcher dispatcher(e);
        dispatcher.Dispatch<MouseButtonPressedEvent>(HW_BIND_EVENT_FN(RuntimeLayer::OnMouseButtonPressed));
        dispatcher.Dispatch<WindowResizeEvent>(HW_BIND_EVENT_FN(RuntimeLayer::OnWindowResize));
        dispatcher.Dispatch<WindowRefreshEvent>(HW_BIND_EVENT_FN(RuntimeLayer::OnWindowRefresh));
    }

    bool RuntimeLayer::OnMouseButtonPressed(MouseButtonPressedEvent& e)
    {
        return false;
    }

    bool RuntimeLayer::OnWindowResize(WindowResizeEvent& e)
    {
        m_ActiveScene->OnViewportResize(e.GetWidth(), e.GetHeight());

        return true;
    }

    bool RuntimeLayer::OnWindowRefresh(WindowRefreshEvent& e)
    {
        SceneRenderer::DrawToScreen();
        return true;
    }

    void RuntimeLayer::OnScenePlay()
    {
        Interpreter::Begin();
        m_ActiveScene->OnStartRuntime();
    }

    void RuntimeLayer::OnSceneStop()
    {
        m_ActiveScene->OnStopRuntime();
        Interpreter::End();
    }

    void RuntimeLayer::OnAssetImported(Asset asset)
    {
        if (asset.GetPath().extension() == ".c")
        {
            m_ActiveScene->OnScriptAssetReimported(asset);
        }
    }
}