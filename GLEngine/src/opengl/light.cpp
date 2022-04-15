#include "glpch.h"
#include "light.h"

void PointLight::Render(int i, GLShader& shader)
{
	std::string name = "u_PointLights[" + std::to_string(i) + "]";

	shader.UpdloadFloat(name + ".k0", k0);
	shader.UpdloadFloat(name + ".k1", k1);
	shader.UpdloadFloat(name + ".k2", k2);

	shader.UpdloadVec3(name + ".position", position);
	shader.UpdloadVec4(name + ".ambient", ambient);
	shader.UpdloadVec4(name + ".diffuse", diffuse);
	shader.UpdloadVec4(name + ".specular", specular);
}

void DirLight::Render(GLShader& shader)
{
	std::string name = "u_DirLight";

	shader.UpdloadVec3(name + ".direction", direction);
	shader.UpdloadVec4(name + ".ambient", ambient);
	shader.UpdloadVec4(name + ".diffuse", diffuse);
	shader.UpdloadVec4(name + ".specular", specular);
}

void SpotLight::Render(int i, GLShader& shader)
{
	std::string name = "u_SpotLights[" + std::to_string(i) + "]";

	shader.UpdloadFloat(name + ".k0", k0);
	shader.UpdloadFloat(name + ".k1", k1);
	shader.UpdloadFloat(name + ".k2", k2);

	shader.UpdloadVec3(name + ".position", position);
	shader.UpdloadVec3(name + ".direction", direction);

	shader.UpdloadFloat(name + ".cutOff", cutOff);
	shader.UpdloadFloat(name + ".outerCutOff", outerCutOff);

	shader.UpdloadVec3(name + ".direction", direction);
	shader.UpdloadVec4(name + ".ambient", ambient);
	shader.UpdloadVec4(name + ".diffuse", diffuse);
	shader.UpdloadVec4(name + ".specular", specular);
}
