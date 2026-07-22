#include <pch.h>
#include "EditorLayer.h"

#include <Perplex/Scene/SceneManager.h>

#include <glm/fwd.hpp>
#include <imgui/imgui.h>

#include <filesystem>
#include <string>

namespace Perplex
{
    namespace fs = std::filesystem;

    EditorLayer::EditorLayer()
        : Layer("EditorLayer")
    {
    }

    void EditorLayer::OnAttach()
    {
        HW_PROFILE_FUNCTION();

        m_Dockspace = Dockspace();
        m_EditorCamera = EditorCamera();
        m_AssetsPath = Application::Get().GetCurrentProject().GetAssetsPath();

        // Set Asset imported callback
        AssetManager::SetAssetImportedCallback([this](Asset asset) { OnAssetImported(asset); });

        m_PlayIcon = CreateRef<pxr::TextureBuffer>(m_AssetsPath / "textures/play_icon.png");
        m_StopIcon = CreateRef<pxr::TextureBuffer>(m_AssetsPath / "textures/pause_icon.png");

        Ref<Scene> emptyScene = CreateRef<Scene>();

        emptyScene->CreateAbstractEntity("Placeholder");
        SceneManager::Get().LoadScene(emptyScene);

        ImGuiUtilities::SetGlobalStyles();
    }

    void EditorLayer::OnDetach()
    {
        HW_PROFILE_FUNCTION();
    }

    void EditorLayer::OnUpdate(Timestep ts)
    {
        HW_PROFILE_FUNCTION();

        m_FrameMS = ts.GetMilliseconds();

        m_ViewportPanel.OnUpdate();

        // Resizing
        if (m_ViewportPanel.IsResizing())
            OnResize();

        // Update
        if (m_SceneState == SceneState::Edit)
        {
            if (m_ViewportPanel.IsFocused()) { m_EditorCamera.OnUpdate(ts); }
            // m_SceneRenderer.RenderEditor(m_ActiveScene, m_EditorCamera);
        }
        else if (m_SceneState == SceneState::Play)
        {
            SceneManager::Get().ActiveScene()->Update(ts);
            // m_SceneRenderer.Render(m_ActiveScene);
        }

        glm::vec2 mousePos = m_SceneRenderer.ScreenToWorldPosition(m_ViewportPanel.GetMousePosition());
        Input::SetMouseWorldPosition({ mousePos.x, mousePos.y });

        SceneManager::Get().OnUpdateEnd();
    }

    void EditorLayer::OnImGuiRender()
    {
        HW_PROFILE_FUNCTION();

        Ref<Scene> activeScene = SceneManager::Get().ActiveScene();

        // Scene
        switch (m_SceneState)
        {
        case SceneState::Edit:
            m_SceneRenderer.RenderEditor(activeScene, m_EditorCamera, m_RenderSettings);
            break;
        case SceneState::Play:
            m_SceneRenderer.Render(activeScene, m_RenderSettings);
            break;
        default:
            break;
        }

        // ImGui
        m_Dockspace.Begin();

        UI_MenuBar();

        m_SceneHierarchyPanel.OnImGuiRender();
        m_ContentBrowserPanel.OnImGuiRender();
        m_PerpixelPanel.OnImGuiRender(activeScene, m_SceneHierarchyPanel.GetSelectedEntity());
        m_ViewportPanel.OnImGuiRender(m_SceneRenderer.GetMainFramebufferTexture());

        UI_Stats();
        UI_Toolbar();
        UI_RenderSettings();

        m_Dockspace.End();
    }

    void EditorLayer::OnEvent(Event& e)
    {
        m_EditorCamera.OnEvent(e);

        EventDispatcher dispatcher(e);
        dispatcher.Dispatch<MouseButtonPressedEvent>(HW_BIND_EVENT_FN(EditorLayer::OnMouseButtonPressed));
    }

    bool EditorLayer::OnMouseButtonPressed(MouseButtonPressedEvent& e)
    {
        /* Entity Selection
        if (m_ViewportHovered && e.GetMouseButton() == 0)
            m_SceneHierarchyPanel.SetSelectedEntity(m_HoveredEntity);
        */

        return false;
    }

    void EditorLayer::UI_MenuBar()
    {
        // MENU BAR
        if (ImGui::BeginMenuBar())
        {
            if (ImGui::BeginMenu("File"))
            {
                if (ImGui::MenuItem("Exit"))
                {
                    Application::Get().Close();
                }

                if (ImGui::MenuItem("Save"))
                {
                    static fs::path filepath = m_AssetsPath / "scenes/scene.pxs";
                    m_SavePopup.Open(filepath.string(), [](const std::string& newStr)
                        {
                            SceneManager::Get().SaveScene(fs::path{ newStr });
                            ImGui::CloseCurrentPopup();
                        });
                }

                if (ImGui::MenuItem("Load"))
                {
                    static fs::path filepath = m_AssetsPath / "scenes/scene.pxs";
                    m_LoadPopup.Open(filepath.string(), [](const std::string& newStr) 
                        {
                            Asset sceneAsset = Asset(fs::path{ newStr });
                            SceneManager::Get().LoadScene(sceneAsset);
                        });
                }

                ImGui::EndMenu();
            }
            ImGui::EndMenuBar();
        }

        m_SavePopup.Update();
        m_LoadPopup.Update();
    }

    void EditorLayer::UI_Stats()
    {
        ImGui::Begin("Stats");
        ImGui::Text("FPS: %.3f", 1000.0f / m_FrameMS);
        ImGui::Dummy({ 0.0f, 10.0f });

        const pxr::RenderStats& renderStats = pxr::Renderer::GetStats();
        ImGui::Text("Quads: %i", renderStats.Quads);
        ImGui::Text("Draw Calls: %i", renderStats.DrawCalls);

        ImGui::End();
    }

    void EditorLayer::UI_Toolbar()
    {
        ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0, 2));
        ImGui::PushStyleVar(ImGuiStyleVar_ItemInnerSpacing, ImVec2(0, 0));
        ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0, 0, 0, 0));
        ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0.3f, 0.3f, 0.3f, 0.5f));
        ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(0.3f, 0.3f, 0.3f, 0.3f));

        ImGui::Begin("Tool Bar", nullptr, ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoScrollWithMouse);

        float size = ImGui::GetWindowHeight() - 5.0f;

        Ref<pxr::TextureBuffer> icon = (m_SceneState == SceneState::Play) ? m_StopIcon : m_PlayIcon;
        ImTextureRef iconTextureRef = ImTextureRef(icon->GetID());

        ImGui::SetCursorPosX((ImGui::GetWindowContentRegionMax().x * 0.5f) - (size * 0.5f));
        ImGui::SetCursorPosY((ImGui::GetWindowContentRegionMax().y * 0.5f) - (size * 0.5f));
        if (ImGui::ImageButton("12309487", iconTextureRef, { size, size }))
        {
            if (m_SceneState == SceneState::Edit)
                OnScenePlay();
            else if (m_SceneState == SceneState::Play)
                OnSceneStop();
        }

        ImGui::End();

        ImGui::PopStyleVar(2);
        ImGui::PopStyleColor(3);
    }

    void EditorLayer::UI_RenderSettings()
    {
        ImGui::Begin("Render Settings");

        ImGui::Checkbox("Postprocessing", &m_RenderSettings.Postprocessing);

        ImGui::Dummy({ 0.0f, 20.0f });

        ImGui::Checkbox("Bloom", &m_RenderSettings.Bloom);
        ImGui::DragFloat("Blooom Threshold", &m_RenderSettings.BloomThreshold, 0.1f);
        ImGui::DragFloat("Blooom Filter Radius", &m_RenderSettings.BloomFilterRadius, 0.001f);

        ImGui::Dummy({ 0.0f, 20.0f });

        ImGui::Checkbox("Tonemapping", &m_RenderSettings.Tonemapping);
        ImGui::Checkbox("Pixelate", &m_RenderSettings.Pixelate);

        ImGui::End();
    }

    void EditorLayer::OnScenePlay()
    {
        ImGuiUtilities::SetRuntimeStyles();

        m_SceneState = SceneState::Play;
        SceneManager::Get().Play();
    }

    void EditorLayer::OnSceneStop()
    {
        ImGuiUtilities::SetEditorStyles();

        m_SceneState = SceneState::Edit;
        SceneManager::Get().Stop();
    }

    void EditorLayer::OnResize()
    {
        const glm::vec2& viewportSize = m_ViewportPanel.GetSize();
        m_EditorCamera.OnResize(viewportSize.x, viewportSize.y);
        m_SceneRenderer.Resize((int)viewportSize.x, (int)viewportSize.y);
    }

    void EditorLayer::OnAssetImported(Asset asset)
    {
        //if (asset.GetPath().extension() == ".c")
        //    m_Interpreter.OnScriptAssetReimported(m_ActiveScene, asset);
    }
}
