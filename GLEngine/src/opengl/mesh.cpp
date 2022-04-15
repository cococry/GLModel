#include "glpch.h"
#include "mesh.h"

std::vector<Vertex> Vertex::GenerateList(f32* vertices, i32 nVertices)
{
	std::vector<Vertex> returning(nVertices);

	i32 stride = sizeof(Vertex) / sizeof(float);

	for (u32 i = 0; i < nVertices; i++) {
		returning[i].Position = glm::vec3(
			vertices[i * stride + 0],
			vertices[i * stride + 1],
			vertices[i * stride + 2]
		);

		returning[i].Normal = glm::vec3(
			vertices[i * stride + 3],
			vertices[i * stride + 4],
			vertices[i * stride + 5]
		);
		returning[i].TexCoord = glm::vec2(
			vertices[i * stride + 6],
			vertices[i * stride + 7]
		);
	}
	return returning;
}

Mesh::Mesh(const std::vector<Vertex>& vertices, const std::vector<u32>& indices, const std::vector<GLTexture>& textures)
	: Vertices(vertices), Indices(indices), Textures(textures), m_NoTextures(false)
{
	Setup();
}

Mesh::Mesh(const std::vector<Vertex>& vertices, const std::vector<u32>& indices, const aiColor4D& diffuse, const aiColor4D& specular)
	: Vertices(vertices), Indices(indices), Diffuse(diffuse), Specular(specular), m_NoTextures(true)
{
	Setup();
}

void Mesh::Render(GLShader& shader)
{
	if (m_NoTextures) {
		shader.UpdloadVec4("u_Material.diffuse", Diffuse);
		shader.UpdloadVec4("u_Material.specular", Specular);
		shader.UpdloadInt("u_NoTex", 1);
	}
	else {
		u32 diffuseI = 0;
		u32 specularI = 0;

		for (u32 i = 0; i < Textures.size(); i++) {
			glActiveTexture(GL_TEXTURE0 + i);

			std::string name;
			switch (Textures[i].Type) {
			case aiTextureType_DIFFUSE:
				name = "diffuse" + std::to_string(diffuseI++);
				break;
			case aiTextureType_SPECULAR:
				name = "specular" + std::to_string(specularI++);
				break;
			}
			shader.UpdloadInt(name, i);
			Textures[i].Bind();
		}
	}
	
	glBindVertexArray(Vao);
	glDrawElements(GL_TRIANGLES, Indices.size(), GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);

	glActiveTexture(GL_TEXTURE0);
}

void Mesh::CleanUp()
{
	glDeleteVertexArrays(1, &Vao);
	glDeleteBuffers(1, &m_Vbo);
	glDeleteBuffers(1, &m_Ibo);
}

void Mesh::Setup()
{
	glCreateVertexArrays(1, &Vao);
	glBindVertexArray(Vao);

	glCreateBuffers(1, &m_Vbo);
	glBindBuffer(GL_ARRAY_BUFFER, m_Vbo);
	glBufferData(GL_ARRAY_BUFFER, Vertices.size() * sizeof(Vertex), &Vertices[0], GL_STATIC_DRAW);

	glCreateBuffers(1, &m_Ibo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_Ibo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, Indices.size() * sizeof(u32), &Indices[0], GL_STATIC_DRAW);

	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, Position));

	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, Normal));

	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, TexCoord));



	glBindVertexArray(0);
}
