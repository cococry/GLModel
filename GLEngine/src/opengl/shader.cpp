#include "glpch.h"
#include "shader.h"

#include <glm/gtc/type_ptr.hpp>

GLShader::GLShader(const std::string& vertexFilepath, const std::string& fragmentFilepath)
{
	u32 vertexShader = Compile(GL_VERTEX_SHADER, vertexFilepath);
	u32 fragmentShader = Compile(GL_FRAGMENT_SHADER, fragmentFilepath);

	Link(vertexShader, fragmentShader);
}

GLShader::~GLShader()
{
	glDeleteProgram(m_RendererID);
}

void GLShader::Bind()
{
	glUseProgram(m_RendererID);
}

void GLShader::Unbind()
{
	glUseProgram(0);
}

void GLShader::Delete()
{
	glDeleteProgram(m_RendererID);
}

void GLShader::UpdloadMat4(const std::string& name, const glm::mat4& value)
{
	
	glUniformMatrix4fv(glGetUniformLocation(m_RendererID, name.c_str()), 1, GL_FALSE, glm::value_ptr(value));
}

void GLShader::UpdloadMat3(const std::string& name, const glm::mat3& value)
{
	
	glUniformMatrix3fv(glGetUniformLocation(m_RendererID, name.c_str()), 1, GL_FALSE, glm::value_ptr(value));
}

void GLShader::UpdloadMat2(const std::string& name, const glm::mat2& value)
{
	
	glUniformMatrix2fv(glGetUniformLocation(m_RendererID, name.c_str()), 1, GL_FALSE, glm::value_ptr(value));
}

void GLShader::UpdloadVec4(const std::string& name, const glm::vec4& value)
{
	
	glUniform4fv(glGetUniformLocation(m_RendererID, name.c_str()), 1, glm::value_ptr(value));
}

void GLShader::UpdloadVec4(const std::string& name, const aiColor4D& value)
{
	glUniform4f(glGetUniformLocation(m_RendererID, name.c_str()), value.r, value.g, value.b, value.a);
}

void GLShader::UpdloadVec3(const std::string& name, const glm::vec3& value)
{
	
	glUniform3fv(glGetUniformLocation(m_RendererID, name.c_str()), 1, glm::value_ptr(value));
}

void GLShader::UpdloadVec2(const std::string& name, const glm::vec2& value)
{
	
	glUniform2fv(glGetUniformLocation(m_RendererID, name.c_str()), 1, glm::value_ptr(value));
}

void GLShader::UpdloadInt(const std::string& name, i32 value)
{
	glUniform1i(glGetUniformLocation(m_RendererID, name.c_str()), value);
}

void GLShader::UpdloadFloat(const std::string& name, f32 value)
{
	
	glUniform1f(glGetUniformLocation(m_RendererID, name.c_str()), value);
}

i32 GLShader::Compile(GLenum type, const std::string& filepath)
{
	int success;
	char infoLog[512];
	u32 shader;
	shader = glCreateShader(type);
	std::string shaderSource = LoadSource(filepath);
	const char* shaderSourceCSTR = shaderSource.c_str();
	glShaderSource(shader, 1, &shaderSourceCSTR, nullptr);
	glCompileShader(shader);

	glGetShaderiv(shader, GL_COMPILE_STATUS, &success);

	if (!success) {
		glGetShaderInfoLog(shader, 512, nullptr, infoLog);
		std::string shaderStr = type == GL_FRAGMENT_SHADER ? "fragment" : "vertex";
		std::cout << "[SHADER COMPILATION]: Failed to compile " << shaderStr << " shader:\n" << infoLog << "\n";
	}
	return shader;
}

void GLShader::Link(i32 vertexShader, i32 fragmentShader)
{
	int success;
	char infoLog[512];

	m_RendererID = glCreateProgram();
	glAttachShader(m_RendererID, vertexShader);
	glAttachShader(m_RendererID, fragmentShader);
	glLinkProgram(m_RendererID);

	glGetProgramiv(m_RendererID, GL_LINK_STATUS, &success);
	if (!success) {
		glGetProgramInfoLog(m_RendererID, 512, nullptr, infoLog);
		std::cout << "[SHADER LINKING]: Failed to link shader program:\n " << infoLog << "\n";
	}

	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);
}

std::string GLShader::LoadSource(const std::string& filepath)
{
	std::ifstream file;
	std::stringstream ss;
	std::string returning = "";

	file.open(filepath);

	if (file.is_open()) {
		ss << file.rdbuf();
		returning = ss.str();
	}
	else {
		std::cout << "[FILE]: Could not open file at '" << filepath << "'\n";
	}
	return returning;
}
