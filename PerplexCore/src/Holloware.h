#pragma once

// For use by Holloware applications

#include <filesystem>

#include "Holloware/Core/Application.h"
#include "Holloware/Core/Layer.h"
#include "Holloware/Core/Log.h"

#include "Holloware/Core/Timestep.h"

#include "Holloware/Core/Input.h"
#include "Holloware/Core/KeyCodes.h"
#include "Holloware/Core/MouseButtonCodes.h"

#include "Holloware/Core/Project.h"

#include "Holloware/Events/Event.h"
#include "Holloware/Events/MouseEvent.h"
#include "Holloware/Events/KeyEvent.h"
#include "Holloware/Events/ApplicationEvent.h"

#include "Holloware/ImGui/ImGuiLayer.h"

#include "Holloware/Scene/Scene.h"
#include "Holloware/Scene/Entity.h"
#include "Holloware/Scene/Components.h"
#include "Holloware/Scene/SceneSerializer.h"
#include "Holloware/Scene/EditorCamera.h"

#include "Holloware/Scripting/Interpreter.h"

#include "Holloware/Debug/Instrumentor.h"

// ---Renderer------------------------------
#include "Holloware/Renderer/Renderer.h"
#include "Holloware/Renderer/Renderer2D.h"
#include "Holloware/Renderer/RenderCommand.h"

#include "Holloware/Renderer/Buffer.h"
#include "Holloware/Renderer/Shader.h"
#include "Holloware/Renderer/FrameBuffer.h"
#include "Holloware/Renderer/Texture.h"
#include "Holloware/Renderer/SubTexture2D.h"
#include "Holloware/Renderer/VertexArray.h"

#include "Holloware/Renderer/GameObject.h"
// -----------------------------------------