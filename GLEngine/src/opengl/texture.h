#pragma once
#include "core/coredef.h"

#include <glad/glad.h>
#include <string>
#include <assimp/scene.h>

class GLTexture
{
public:
	GLTexture() = default;
	GLTexture(const std::string& dir, const std::string& path, aiTextureType type, bool flipped = true);

	void Bind();
	void Activate(i32 slot = 0);
	void Unbind();

	void Delete();

	inline u32 GetID() const { return m_RendererID; }
	inline i32 GetWidth() const { return m_Width; }
	inline i32 GetHeight() const { return m_Height; }
	inline i32 GetChannels() const { return m_NChannels; }

	aiTextureType Type;
	std::string Dir, Path;

private:
	u32 m_RendererID;
	i32 m_Width, m_Height, m_NChannels;
};