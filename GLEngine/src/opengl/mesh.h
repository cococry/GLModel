#pragma once

#include "core/coredef.h"

#include "shader.h"
#include "texture.h"

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <vector>
#include <assimp/scene.h>

struct Vertex
{
	glm::vec3 Position;
	glm::vec3 Normal;
	glm::vec2 TexCoord;

	static std::vector<Vertex> GenerateList(f32* vertices, i32 nVertices);
};

class Mesh
{
public:
	std::vector<Vertex> Vertices;
	std::vector<u32> Indices;
	u32 Vao;

	std::vector<GLTexture> Textures;
	aiColor4D Diffuse, Specular;

	Mesh() = default;
	Mesh(const std::vector<Vertex>& vertices, const std::vector<u32>& indices, const std::vector<GLTexture>& textures = {});
	Mesh(const std::vector<Vertex>& vertices, const std::vector<u32>& indices, const aiColor4D& diffuse, const aiColor4D& specular);
	void Render(GLShader& shader);

	void CleanUp();
private:
	bool m_NoTextures;
	u32 m_Vbo, m_Ibo;

	void Setup();
};