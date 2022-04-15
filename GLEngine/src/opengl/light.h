#pragma once

#include "core/coredef.h"
#include <glm/glm.hpp>

#include "shader.h"

struct PointLight 
{
	float k0, k1, k2;

	glm::vec3 position;

	glm::vec4 ambient;
	glm::vec4 diffuse;
	glm::vec4 specular;

	void Render(int i, GLShader& shader);
};

struct DirLight
{
	glm::vec3 direction;

	glm::vec4 ambient;
	glm::vec4 diffuse;
	glm::vec4 specular;

	void Render(GLShader& shader);
};

struct SpotLight
{
	float k0, k1, k2;

	glm::vec3 position;
	glm::vec3 direction;

	f32 cutOff;
	f32 outerCutOff;
	glm::vec4 ambient;
	glm::vec4 diffuse;
	glm::vec4 specular;

	void Render(int i, GLShader& shader);
};