#include <pch.h>

#include "RuntimeLayer.h"
#include <Perplex/Core/Window.h>

#include <imgui/imgui.h>

namespace Perplex
{
    namespace fs = std::filesystem;

    RuntimeLayer::RuntimeLayer() : Layer("RuntimeLayer") { }

    void AttachSceneSystems(Ref<Scene> scene)
    {
        scene->AddSystem<Simulator>();
        scene->AddSystem<Interpreter>();
    }

    void RuntimeLayer::OnAttach()
    {
        HW_PROFILE_FUNCTION();

        m_AssetsPath = Application::Get().GetCurrentProject().GetAssetsPath();

        // Set Asset imported callback
        AssetManager::SetAssetImportedCallback([this](Asset asset) { OnAssetImported(asset); });

        // Load Scene
        // TODO: should not be absolute path
        Asset sceneAsset = Asset("C:/dev/PerplexProjects/Game/assets/scenes/scene.pxs");
        m_ActiveScene = sceneAsset.GetData<Scene>();
        AttachSceneSystems(m_ActiveScene);
        m_ActiveScene->Start();

        Window& window = Application::Get().GetWindow();
        m_SceneRenderer.Resize(window.GetWidth(), window.GetHeight());
    }

    void RuntimeLayer::OnDetach()
    {
        HW_PROFILE_FUNCTION();
    }

    void RuntimeLayer::OnUpdate(Timestep ts)
    {
        HW_PROFILE_FUNCTION();

        m_ActiveScene->Update(ts);
        m_SceneRenderer.Render(m_ActiveScene);
        m_SceneRenderer.DrawToScreen();
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
        m_SceneRenderer.Resize(e.GetWidth(), e.GetHeight());

        return true;
    }

    bool RuntimeLayer::OnWindowRefresh(WindowRefreshEvent& e)
    {
        m_SceneRenderer.DrawToScreen();
        return true;
    }

    void RuntimeLayer::OnAssetImported(Asset asset)
    {
        //if (asset.GetPath().extension() == ".c")
        //    m_Interpreter.OnScriptAssetReimported(m_ActiveScene, asset);
    }
}