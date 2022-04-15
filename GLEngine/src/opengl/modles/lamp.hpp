#pragma once

#include "cube.hpp"
#include "opengl/light.h"

class Lamp : public Cube {
public:
	glm::vec3 LightColor;

	PointLight pointLight;

	Lamp() = default;
	Lamp(
		const glm::vec3& lightColor,
		const glm::vec4& ambient,
		const glm::vec4& diffuse,
		const glm::vec4& specular,
		const glm::vec3& pos,
		const glm::vec3& size,
		float k0,
		float k1,
		float k2)	
			:	
		LightColor(lightColor), pointLight({k0, k1, k2, pos, ambient, diffuse, specular}),
		Cube(pos, size) {}

	void Render(GLShader& shader) override
	{
		shader.UpdloadVec3("u_LightColor", LightColor);

		Cube::Render(shader);
	}
};