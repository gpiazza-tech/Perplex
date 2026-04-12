#pragma once

// Standard
#include <iostream>
#include <memory>
#include <utility>
#include <algorithm>
#include <functional>
#include <filesystem>
#include <fstream>
#include <sstream>
#include <cstring>
#include <cstdint>

#include <string>
#include <array>
#include <vector>
#include <unordered_map>
#include <unordered_set>
#include <any>

#include <random>
#include <xhash>
#include <chrono>
#include <thread>

// Vendor
#include <entt.hpp>

#include <glm/glm.hpp>

#include <spdlog/spdlog.h>

#include <nlohmann/json_fwd.hpp>
#include <nlohmann/json.hpp>

// Engine
#include "Holloware/Core/Input.h"
#include "Holloware/Core/KeyCodes.h"
#include "Holloware/Core/MouseButtonCodes.h"

#include "Holloware/Core/Log.h"
#include "Holloware/Debug/Instrumentor.h"

#ifdef HW_PLATFORM_WINDOWS
	#include <Windows.h>
#endif