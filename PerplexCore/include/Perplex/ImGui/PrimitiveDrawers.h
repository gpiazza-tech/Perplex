#pragma once

#include <glm/fwd.hpp>

#include <string>

namespace Perplex
{
	void Draw(int& value, const char* label = "");
	void Draw(float& value, const char* label = "");
	void Draw(double& value, const char* label = "");
	void Draw(bool& value, const char* label = "");
	void Draw(std::string& value, const char* label = "");
	void Draw(glm::vec2& value, const char* label = "");
	void Draw(glm::vec3& value, const char* label = "");
}