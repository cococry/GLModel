#include "glpch.h"
#include "texture.h"

#include <stb_image.h>

GLTexture::GLTexture(const std::string& dir, const std::string& path, aiTextureType type, bool flipped)
	: Dir(dir), Path(path), Type(type)
{

	glGenTextures(1, &m_RendererID);
	glBindTexture(GL_TEXTURE_2D, m_RendererID);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);

	stbi_set_flip_vertically_on_load(flipped);

	u8* data = stbi_load((dir + "/" + path).c_str(), &m_Width, &m_Height, &m_NChannels, 0);

	GLenum internalFormat = GL_RGB;
	GLenum format = GL_RGB;

	if (m_NChannels == 4) {
		internalFormat = GL_RGBA;
		format = GL_RGBA;
	}

	if (data) {
		glTexImage2D(GL_TEXTURE_2D, 0, internalFormat, m_Width, m_Height, 0, format, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);

	}
	else {
		std::cout << "[TEXTURE]: Failed to load texture\n";
	}

	stbi_image_free(data);
}

void GLTexture::Bind()
{
	glBindTexture(GL_TEXTURE_2D, m_RendererID);
}

void GLTexture::Activate(i32 slot)
{
	glActiveTexture(GL_TEXTURE0 + slot);
}

void GLTexture::Unbind()
{
	glBindTexture(GL_TEXTURE_2D, 0);
}

void GLTexture::Delete()
{
	glDeleteTextures(1, &m_RendererID);
}
