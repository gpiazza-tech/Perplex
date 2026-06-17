#include <pch.h>
#include "EditorLayer.h"

#include <Perplex/Perpixel/PerpixelSystem.h>

#include <glm/fwd.hpp>
#include <imgui/imgui.h>

#include <string.h>
#include <cstdint>
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

        m_ActiveScene = CreateRef<Scene>();

        m_ActiveScene->CreateAbstractEntity("Placeholder");
        m_SceneHierarchyPanel.SetContext(m_ActiveScene);

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
            m_ActiveScene->Update(ts);
            // m_SceneRenderer.Render(m_ActiveScene);
        }

        glm::vec2 mousePos = m_SceneRenderer.ScreenToWorldPosition(m_ViewportPanel.GetMousePosition());
        Input::SetMouseWorldPosition({ mousePos.x, mousePos.y });
    }

    void EditorLayer::OnImGuiRender()
    {
        HW_PROFILE_FUNCTION();

        // Scene
        switch (m_SceneState)
        {
        case SceneState::Edit:
            m_SceneRenderer.RenderEditor(m_ActiveScene, m_EditorCamera);
            break;
        case SceneState::Play:
            m_SceneRenderer.Render(m_ActiveScene);
            break;
        default:
            break;
        }

        // ImGui
        m_Dockspace.Begin();

        UI_MenuBar();

        m_SceneHierarchyPanel.OnImGuiRender();
        m_ContentBrowserPanel.OnImGuiRender();
        m_PerpixelPanel.OnImGuiRender(m_ActiveScene, m_SceneHierarchyPanel.GetSelectedEntity());
        m_ViewportPanel.OnImGuiRender(m_SceneRenderer.GetMainFramebufferTexture());

        UI_Stats();
        UI_Toolbar();

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
        static bool savePopup = false;
        static bool loadPopup = false;

        // MENU BAR
        if (ImGui::BeginMenuBar())
        {
            if (ImGui::BeginMenu("File"))
            {
                if (ImGui::MenuItem("Exit")) { Application::Get().Close(); }
                if (ImGui::MenuItem("Save")) { savePopup = true; }
                if (ImGui::MenuItem("Load")) { loadPopup = true; }

                ImGui::EndMenu();
            }
            ImGui::EndMenuBar();
        }

        // POPUPS
        if (savePopup)
        {
            ImGui::OpenPopup("Save Scene");
            savePopup = false;
        }
        if (loadPopup)
        {
            ImGui::OpenPopup("Load Scene");
            loadPopup = false;
        }

        UI_Popups();
    }

    void EditorLayer::UI_Popups()
    {
        if (ImGui::BeginPopupModal("Save Scene"))
        {
            static fs::path filepath = m_AssetsPath / "scenes/scene.pxs";
            std::string filepathString = filepath.string();

            char buffer[64];
            memset(buffer, 0, sizeof(buffer));
            strcpy_s(buffer, sizeof(buffer), filepathString.c_str());
            if (ImGui::InputText("path", buffer, sizeof(buffer)))
            {
                filepath = std::string(buffer);
            }

            if (ImGui::Button("Save"))
            {
                SceneSerializer::Serialize(m_ActiveScene, filepathString);

                ImGui::CloseCurrentPopup();
            }

            ImGui::EndPopup();
        }

        if (ImGui::BeginPopupModal("Load Scene"))
        {
            static fs::path filepath = m_AssetsPath / "scenes/scene.pxs";
            std::string filepathString = filepath.string();

            char buffer[64];
            memset(buffer, 0, sizeof(buffer));
            strcpy_s(buffer, sizeof(buffer), filepathString.c_str());
            if (ImGui::InputText("path", buffer, sizeof(buffer)))
            {
                filepath = std::string(buffer);
            }

            if (ImGui::Button("Load"))
            {
                Asset sceneAsset = Asset(filepathString);
                m_ActiveScene = sceneAsset.LoadData<Scene>();

                OnSceneLoad();
                ImGui::CloseCurrentPopup();
            }

            ImGui::EndPopup();
        }
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

    void EditorLayer::OnSceneLoad()
    {
        m_SceneHierarchyPanel.SetContext(m_ActiveScene);
        OnResize();

        if (m_SceneState == SceneState::Play)
            OnScenePlay();
    }

    void EditorLayer::OnScenePlay()
    {
        ImGuiUtilities::SetRuntimeStyles();

        m_SceneState = SceneState::Play;
        m_ActiveScene->Start();
    }

    void EditorLayer::OnSceneStop()
    {
        ImGuiUtilities::SetEditorStyles();

        m_SceneState = SceneState::Edit;
        m_ActiveScene->Stop();
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
