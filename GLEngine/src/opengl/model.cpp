#include "glpch.h"
#include "model.h"

Model::Model(const glm::vec3& position, const glm::vec3& size, bool noTextures)
	: Position(position), Size(size), p_NoTextures(noTextures)
{

}

void Model::LoadModel(const std::string& filepath)
{
	Assimp::Importer importer;
	const aiScene* scene = importer.ReadFile(filepath, aiProcess_Triangulate | aiProcess_FlipUVs);

	if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) {
		std::cout << "[IMPORT]: ERROR: Could not load model at location '" << filepath << "'.\n" << importer.GetErrorString() << "\n";
		return;
	}

	p_Directory = filepath.substr(0, filepath.find_last_of("/"));
	ProcessNode(scene->mRootNode, scene);
}

void Model::Render(GLShader& shader)
{
	glm::mat4 model = glm::mat4(1.0f);
	model = glm::translate(model, Position);
	model = glm::scale(model, Size);
	shader.UpdloadMat4("u_Model", model);

	shader.UpdloadFloat("u_Material.shininess", 0.5f);
	for (Mesh& mesh : p_Meshes) {
		mesh.Render(shader);
	}
}

void Model::CleanUp()
{
	for (Mesh& mesh : p_Meshes) {
		mesh.CleanUp();
	}
}

void Model::ProcessNode(aiNode* node, const aiScene* scene)
{
	// process meshes
	for (u32 i = 0; i < node->mNumMeshes; i++) {
		aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
		p_Meshes.push_back(ProcessMesh(mesh, scene));
	}

	// process child nodes
	for (u32 i = 0; i < node->mNumChildren; i++) {
		ProcessNode(node->mChildren[i], scene);
	}
}

Mesh Model::ProcessMesh(aiMesh* mesh, const aiScene* scene)
{
	std::vector<Vertex> vertices;
	std::vector<u32> indices;
	std::vector<GLTexture> textures;

	// vertices
	for (u32 i = 0; i < mesh->mNumVertices; i++) {
		Vertex vertex;

		vertex.Position = glm::vec3(
			mesh->mVertices[i].x,
			mesh->mVertices[i].y,
			mesh->mVertices[i].z
		);
		vertex.Normal = glm::vec3(
			mesh->mNormals[i].x,
			mesh->mNormals[i].y,
			mesh->mNormals[i].z
		);

		if (mesh->mTextureCoords[0]) {
			vertex.TexCoord = glm::vec2(
				mesh->mTextureCoords[0][i].x,
				mesh->mTextureCoords[0][i].y
			);
		}
		else {
			vertex.TexCoord = glm::vec2(0.0f);
		}

		vertices.push_back(vertex);
	}

	// indices
	for (u32 i = 0; i < mesh->mNumFaces; i++) {
		aiFace face = mesh->mFaces[i];
		for (u32 j = 0; j < face.mNumIndices; j++) {
			indices.push_back(face.mIndices[j]);
		}
	}

	// material
	if (mesh->mMaterialIndex >= 0) {
		aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];

		if (p_NoTextures) {
			aiColor4D diff(1.0f);
			aiGetMaterialColor(material, AI_MATKEY_COLOR_DIFFUSE, &diff);

			aiColor4D spec(1.0f);
			aiGetMaterialColor(material, AI_MATKEY_COLOR_SPECULAR, &spec);

			return Mesh(vertices, indices, diff, spec);
		}

		std::vector<GLTexture> diffuseMaps = LoadTextures(material, aiTextureType_DIFFUSE);
		textures.insert(textures.end(), diffuseMaps.begin(), diffuseMaps.end());

		std::vector<GLTexture> specularMaps = LoadTextures(material, aiTextureType_SPECULAR);
		textures.insert(textures.end(), specularMaps.begin(), specularMaps.end());
	}
	return Mesh(vertices, indices, textures);
}

std::vector<GLTexture> Model::LoadTextures(aiMaterial* mat, aiTextureType type)
{
	std::vector<GLTexture> textures;

	for (u32 i = 0; i < mat->GetTextureCount(type); i++) {
		aiString str;
		mat->GetTexture(type, i, &str);
		std::cout << str.C_Str() << "\n";

		bool skip = false;
		for (u32 j = 0; j < p_TexturesLoaded.size(); j++) {
			if (std::strcmp(p_TexturesLoaded[j].Path.data(), str.C_Str()) == 0) {
				textures.push_back(p_TexturesLoaded[j]);
				skip = true;
				break;
			}
		}

		if (!skip) {
			GLTexture tex(p_Directory, str.C_Str(), type, false);
			textures.push_back(tex);
			p_TexturesLoaded.push_back(tex);
		}
	}

	return textures;
}
