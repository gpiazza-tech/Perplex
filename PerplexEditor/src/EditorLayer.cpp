#include "EditorLayer.h"

namespace Holloware
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

        m_PlayIcon = Texture2D::Create(m_AssetsPath / "textures/play_icon.png");
        m_StopIcon = Texture2D::Create(m_AssetsPath / "textures/pause_icon.png");

        FramebufferSpecification fbSpec;
        fbSpec.Attachments = { FramebufferTextureFormat::RGBA8, FramebufferTextureFormat::RED_INTEGER, FramebufferTextureFormat::Depth };
        fbSpec.Width = 1280;
        fbSpec.Height = 720;
        m_Framebuffer = Framebuffer::Create(fbSpec);

        m_ActiveScene = CreateRef<Scene>();
        m_ActiveScene->CreateAbstractEntity("Placeholder");
        m_SceneHierarchyPanel.SetContext(m_ActiveScene);
    }

    void EditorLayer::OnDetach()
    {
        HW_PROFILE_FUNCTION();
    }

    void EditorLayer::OnUpdate(Timestep ts)
    {
        HW_PROFILE_FUNCTION();

        m_frameMS = ts.GetMilliseconds();

        // Resizing
        if (m_ViewportSize != *((glm::vec2*)&m_ViewportPanelSize))
        {
            OnResize();
        }

        // Render
        m_Framebuffer->Bind();
        RenderCommand::SetClearColor({ 0.1f, 0.1f, 0.1f, 1 });
        RenderCommand::Clear();

        // Clear our entity ID attachment to -1
        m_Framebuffer->ClearAttachment(1, -1);

        // Update
        if (m_SceneState == SceneState::Edit)
        {
            if (m_ViewportFocused) { m_EditorCamera.OnUpdate(ts); }
            m_ActiveScene->OnUpdateEditor(ts, m_EditorCamera);
        }
        else if (m_SceneState == SceneState::Play)
        {
            m_ActiveScene->OnUpdateRuntime(ts);
        }

        auto [mx, my] = ImGui::GetMousePos();
        mx -= m_ViewportBounds[0].x;
        my -= m_ViewportBounds[0].y;
        glm::vec2 viewportSize = m_ViewportBounds[1] - m_ViewportBounds[0];
        my = viewportSize.y - my;
        int mouseX = (int)mx;
        int mouseY = (int)my;

        if (mouseX >= 0 && mouseY >= 0 && mouseX < (int)viewportSize.x && mouseY < (int)viewportSize.y)
        {
            int pixelData = m_Framebuffer->ReadPixel(1, mouseX, mouseY);
            m_HoveredEntity = (pixelData == -1) ? Entity() : Entity((entt::entity)pixelData, m_ActiveScene.get());
        }

        m_Framebuffer->Unbind();
    }

    void EditorLayer::OnImGuiRender()
    {
        HW_PROFILE_FUNCTION();

        m_Dockspace.Begin();

        UI_MenuBar();

        m_SceneHierarchyPanel.OnImGuiRender();
        m_ContentBrowserPanel.OnImGuiRender();

        UI_Viewport();
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
        if (m_ViewportHovered && e.GetMouseButton() == 0)
        {
            m_SceneHierarchyPanel.SetSelectedEntity(m_HoveredEntity);
        }

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
            ImGui::OpenPopup("SAVE_SCENE");
            savePopup = false;
        }
        if (loadPopup)
        {
            ImGui::OpenPopup("LOAD_SCENE");
            loadPopup = false;
        }

        UI_Popups();
    }

    void EditorLayer::UI_Popups()
    {
        if (ImGui::BeginPopupModal("SAVE_SCENE"))
        {
            ImGui::Text("Load Scene");

            static fs::path filepath = m_AssetsPath / "scenes/scene.hws";
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

        if (ImGui::BeginPopupModal("LOAD_SCENE"))
        {
            ImGui::Text("Load Scene");

            static fs::path filepath = m_AssetsPath / "scenes/scene.hws";
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
                m_ActiveScene = sceneAsset.GetData<Scene>();
                OnSceneLoad();
                ImGui::CloseCurrentPopup();
            }

            ImGui::EndPopup();
        }
    }

    void EditorLayer::UI_Viewport()
    {
        ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0, 0));
        ImGui::Begin("ViewPort");
        auto viewportOffset = ImGui::GetCursorPos();

        m_ViewportPanelSize = ImGui::GetContentRegionAvail();

        m_ViewportFocused = ImGui::IsWindowFocused();
        m_ViewportHovered = ImGui::IsWindowHovered();
        Application::Get().GetImGuiLayer()->BlockEvents(!m_ViewportFocused || !m_ViewportHovered);

        uint32_t textureID = m_Framebuffer->GetColorAttachmentRendererID();
        ImGui::Image((void*)textureID, { m_ViewportSize.x, m_ViewportSize.y }, ImVec2(0, 1), ImVec2(1, 0));

        if (ImGui::BeginDragDropTarget())
        {
            if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("CONTENT_BROWSER_ITEM"))
            {
                const wchar_t* path = (const wchar_t*)payload->Data;
                HW_CORE_INFO(std::filesystem::path(path).string().c_str());
            }
            ImGui::EndDragDropTarget();
        }

        auto windowSize = ImGui::GetWindowSize();
        ImVec2 minBound = ImGui::GetWindowPos();
        minBound.x += viewportOffset.x;
        minBound.y += viewportOffset.y;

        ImVec2 maxBound = { minBound.x + windowSize.x, minBound.y + windowSize.y - viewportOffset.y };
        m_ViewportBounds[0] = { minBound.x, minBound.y };
        m_ViewportBounds[1] = { maxBound.x, maxBound.y };

        ImGui::End();
        ImGui::PopStyleVar();
    }

    void EditorLayer::UI_Stats()
    {
        ImGui::Begin("Stats");
        ImGui::Text("FPS: %.3f", 1000.0f / m_frameMS);

        std::string name = m_HoveredEntity ? m_HoveredEntity.GetComponent<TagComponent>().Tag : "None";

        ImGui::Text("Hovered Entity: %s", name.c_str());
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

        Ref<Texture2D> icon = (m_SceneState == SceneState::Play) ? m_StopIcon : m_PlayIcon;
        ImTextureRef iconTextureRef = ImTextureRef(icon->GetRendererID());

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
        m_SceneHierarchyPanel.SetSelectedEntity(Entity());
        OnResize();
    }

    void EditorLayer::OnScenePlay()
    {
        m_SceneState = SceneState::Play;

        Interpreter::Begin();
        m_ActiveScene->OnStartRuntime();
    }

    void EditorLayer::OnSceneStop()
    {
        m_SceneState = SceneState::Edit;

        m_ActiveScene->OnStopRuntime();
        Interpreter::End();
    }

    void EditorLayer::OnResize()
    {
        m_ViewportSize = { m_ViewportPanelSize.x, m_ViewportPanelSize.y };

        m_Framebuffer->Resize((uint32_t)m_ViewportPanelSize.x, (uint32_t)m_ViewportPanelSize.y);
        m_EditorCamera.OnResize(m_ViewportPanelSize.x, m_ViewportPanelSize.y);

        m_ActiveScene->OnViewportResize((uint32_t)m_ViewportSize.x, (uint32_t)m_ViewportSize.y);
    }

    void EditorLayer::OnAssetImported(Asset asset)
    {
        if (asset.GetPath().extension() == ".c")
        {
            m_ActiveScene->OnScriptAssetReimported(asset);
        }
    }
}
