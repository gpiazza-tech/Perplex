#pragma once

#include "Holloware/Renderer/Shader.h"

typedef unsigned int GLenum;

namespace Holloware
{
	class OpenGLShader : public Shader
	{
	public:
		OpenGLShader(const std::string& filepath);
		OpenGLShader(const std::string& name, const std::string& vertexSrc, const std::string& fragmentSrc);
		~OpenGLShader();

		virtual void Bind() const override;
		virtual void Unbind() const override;

		virtual void SetInt(const std::string name, int value) override;
		virtual void SetIntArray(const std::string name, int* values, uint32_t count) override;
		virtual void SetFloat(const std::string name, float value) override;
		virtual void SetFloat2(const std::string name, const glm::vec2& value) override;
		virtual void SetFloat3(const std::string name, const glm::vec3& value) override;
		virtual void SetFloat4(const std::string name, const glm::vec4& value) override;
		virtual void SetMat4(const std::string name, const glm::mat4& value) override;

		virtual const std::string& GetName() const override { return m_Name; }

		virtual void UploadUniformInt(const std::string& name, int value) const;
		virtual void UploadUniformIntArray(const std::string name, int* values, uint32_t count) const;

		virtual void UploadUniformFloat(const std::string& name, float value) const;
		virtual void UploadUniformFloat2(const std::string& name, const glm::vec2& value) const;
		virtual void UploadUniformFloat3(const std::string& name, const glm::vec3& value) const;
		virtual void UploadUniformFloat4(const std::string& name, const glm::vec4& value) const;

		virtual void UploadUniformMat3(const std::string& name, const glm::mat3& matrix) const;
		virtual void UploadUniformMat4(const std::string& name, const glm::mat4& matrix) const;
	private:
		std::string ReadFile(const std::string& filepath);
		std::unordered_map<GLenum, std::string> PreProcess(const std::string& source);
		void Compile(const std::unordered_map<GLenum, std::string>& shaderSources);
	private:
		uint32_t m_RendererID;
		std::string m_Name;
	};
}