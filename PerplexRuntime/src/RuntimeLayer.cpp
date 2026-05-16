#include "RuntimeLayer.h"

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

        // TODO: resize should be window level event
        m_ActiveScene->OnViewportResize(1280, 720);
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
    }

    bool RuntimeLayer::OnMouseButtonPressed(MouseButtonPressedEvent& e)
    {
        return false;
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