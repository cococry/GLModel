#pragma once

#include "core/coredef.h"
#include "mesh.h"
#include "shader.h"

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <vector>

class Model
{
public:
	glm::vec3 Position;
	glm::vec3 Size;

	Model(const glm::vec3& position = glm::vec3(0.0f), const glm::vec3& size = glm::vec3(1.0f), bool noTextures = false);

	virtual void Init() {};
	void LoadModel(const std::string& filepath);
	virtual void Render(GLShader& shader);
	void CleanUp();
protected:
	bool p_NoTextures;
	std::vector<Mesh> p_Meshes;
	std::string p_Directory;
	std::vector<GLTexture> p_TexturesLoaded;

	void ProcessNode(aiNode* node, const aiScene* scene);
	Mesh ProcessMesh(aiMesh* mesh, const aiScene* scene);
	std::vector<GLTexture> LoadTextures(aiMaterial* mat, aiTextureType type);
};