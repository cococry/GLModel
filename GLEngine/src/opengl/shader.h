#pragma once

#include "core/coredef.h"

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <assimp/scene.h>
#include <string>

class GLShader
{
public:
	GLShader() = default;
	GLShader(const std::string& vertexFilepath, const std::string& fragmentFilepath);
	~GLShader();

	void Bind();
	void Unbind();
	void Delete();

	void UpdloadMat4(const std::string& name, const glm::mat4& value);
	void UpdloadMat3(const std::string& name, const glm::mat3& value);
	void UpdloadMat2(const std::string& name, const glm::mat2& value);
	void UpdloadVec4(const std::string& name, const glm::vec4& value);
	void UpdloadVec4(const std::string& name, const aiColor4D& value);
	void UpdloadVec3(const std::string& name, const glm::vec3& value);
	void UpdloadVec2(const std::string& name, const glm::vec2& value);
	void UpdloadInt(const std::string& name, i32 value);
	void UpdloadFloat(const std::string& name, f32 value);

	inline i32 GetID() const { return m_RendererID; }
private:
	i32 Compile(GLenum type, const std::string& filepath);
	void Link(i32 vertexShader, i32 fragmentShader);
	std::string LoadSource(const std::string& filepath);

	u32 m_RendererID;
};