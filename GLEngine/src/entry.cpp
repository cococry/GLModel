#include "glpch.h"

#include "core/coredef.h"
#include "opengl/shader.h"
#include "opengl/texture.h"
#include "opengl/modles/cube.hpp"
#include "opengl/modles/lamp.hpp"
#include "opengl/model.h"
#include "opengl/light.h"
#include "io/screen.h"
#include "io/keyboard.h"
#include "io/mouse.h"
#include "io/joystick.h"
#include "io/camera.h"

#include <GLFW/glfw3.h>
#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <stb_image.h>

void process_input(f64 dt);

Joystick mainJ(0);
Camera camera({0.0f, 0.0f, 3.0f});
Screen screen;
bool flashLightOn = true;

u32 SCREEN_WIDTH = 1920, SCREEN_HEIGHT = 1080;
f32 deltaTime = 0.0f;
f32 lastFrame = 0.0f;

int main(){
	
	screen.Init();

	screen.SetGLParams();

	GLShader shader("assets/shaders/object.vert", "assets/shaders/object.frag");
	GLShader lampShader("assets/shaders/object.vert", "assets/shaders/lamp.frag");

	Model m(glm::vec3(0.0f, 0.0f, -5.0f), glm::vec3(0.1), true);
	m.LoadModel("assets/models/rifle/scene.gltf");

	DirLight dirLight = { 
		glm::vec3(-0.2f, -1.0f, -0.3f), 
		glm::vec4(0.1f, 0.1f, 0.1f, 1.0f), 
		glm::vec4(0.4f, 0.4f, 0.4f, 1.0f), 
		glm::vec4(0.75f, 0.75f, 0.75f, 1.0f) };
	
	glm::vec3 pointLightPositions[] = {
			glm::vec3(0.7f,  0.2f,  2.0f),
			glm::vec3(2.3f, -3.3f, -4.0f),
			glm::vec3(-4.0f,  2.0f, -12.0f),
			glm::vec3(0.0f,  0.0f, -3.0f)
	};
	Lamp lamps[4];
	for (unsigned int i = 0; i < 4; i++) {
		lamps[i] = Lamp(
			glm::vec3(1.0f),
			glm::vec4(0.05f, 0.05f, 0.05f, 1.0f),
			glm::vec4(0.8f, 0.8f, 0.8f, 1.0f),
			glm::vec4(1.0f),
			pointLightPositions[i],
			glm::vec3(0.25f),
			1.0f, 0.07f, 0.032f);

		lamps[i].Init();
	}

	SpotLight spotLight = { 
		1.0f, 0.07f, 0.32f, 
		camera.CameraPosition, 
		camera.CameraFront, 
		glm::cos(glm::radians(12.5f)),
		glm::cos(glm::radians(20.0f)),
		glm::vec4(0.0f, 0.0f, 0.0f, 1.0f), 
		glm::vec4(1.0f), 
		glm::vec4(1.0f) };

	mainJ.Update();
	if (mainJ.IsPresent()) {
		std::cout << "[INPUT]: Controller '" << mainJ.GetName() << "' is present.\n";
	}
	else {
		std::cout << "[INPUT]: No controller is present\n";
	}

	while (!screen.ShouldClose()) {
		f64 currentTime = glfwGetTime();
		deltaTime = currentTime - lastFrame;
		lastFrame = currentTime;
		
		process_input(deltaTime);
		glfwPollEvents();

		screen.Update();
		shader.Bind();
		shader.UpdloadVec3("u_ViewPos", camera.CameraPosition);
		
		glm::mat4 view = glm::mat4(1.0f);
		glm::mat4 projection = glm::mat4(1.0f);
		view = camera.GetViewMatrix();
		projection = glm::perspective(glm::radians(camera.Zoom), (float)SCREEN_WIDTH / (float)SCREEN_HEIGHT, 0.1f, 1000.0f);

		m.Render(shader);

		shader.UpdloadMat4("u_View", view);
		shader.UpdloadMat4("u_Projection", projection);

		dirLight.direction = glm::vec3(
			glm::rotate(glm::mat4(1.0f), glm::radians(0.5f), glm::vec3(1.0f, 0.0f, 0.0f)) *
			glm::vec4(dirLight.direction, 1.0f));

		dirLight.Render(shader);

		for (u32 i = 0; i < 4; i++) {
			lamps[i].pointLight.Render(i, shader);
		}
		shader.UpdloadInt("u_NPointLights", 4);
		if (flashLightOn) {
			spotLight.position = camera.CameraPosition;
			spotLight.direction = camera.CameraFront;
			spotLight.Render(0, shader);
			shader.UpdloadInt("u_NSpotLights", 1);
		}
		else {
			shader.UpdloadInt("u_NSpotLights", 0);
		}

		
		lampShader.Bind();
		lampShader.UpdloadMat4("u_View", view);
		lampShader.UpdloadMat4("u_Projection", projection);
		for (u32 i = 0; i < 4; i++) {
			lamps[i].Render(lampShader);
		}
		screen.NewFrame();
	}
	m.CleanUp();
	for (u32 i = 0; i < 4; i++) {
		lamps[i].CleanUp();
	}
	
	return 0;
}

void process_input(f64 dt)
{
	if (mainJ.IsPresent()) {
		mainJ.Update();

		float lx = mainJ.AxesState(GLFW_JOYSTICK_AXES_LEFT_STICK_X);
		float ly = mainJ.AxesState(GLFW_JOYSTICK_AXES_LEFT_STICK_Y);

		if (std::abs(lx) > 0.5f) {
			camera.UpdateCameraPositionByValue(CameraDirection::LEFT, lx, dt);
		}
		if (std::abs(ly) > 0.5f) {
			camera.UpdateCameraPositionByValue(CameraDirection::BACKWARD, ly, dt);
		}

		if (mainJ.ButtonState(GLFW_JOYSTICK_BTN_DOWN) == GLFW_PRESS) {
			camera.UpdateCameraPositionByValue(CameraDirection::UP, 2.5f, dt);

		}
		if (mainJ.ButtonState(GLFW_JOYSTICK_BTN_UP) == GLFW_PRESS) {
			camera.UpdateCameraPositionByValue(CameraDirection::DOWN, 2.5f, dt);
		}


		float rt = mainJ.AxesState(GLFW_JOYSTICK_AXES_RIGHT_TRIGGER) / 2.0f + 0.5f;
		if (rt > 0.05f) {
			camera.UpdateCameraZoom(0.1f);
		}
		float lt = mainJ.AxesState(GLFW_JOYSTICK_AXES_LEFT_TRIGGER) / 2.0f + 0.5f;
		if (lt > 0.05f) {
			camera.UpdateCameraZoom(-0.1f);
		}

		float rx = mainJ.AxesState(GLFW_JOYSTICK_AXES_RIGHT_STICK_X);
		float ry = mainJ.AxesState(GLFW_JOYSTICK_AXES_RIGHT_STICK_Y);
		if (rx != 0 || ry != 0) {
			camera.UpdateCameraDirection(rx, ry);
		}

		if (mainJ.ButtonState(GLFW_JOYSTICK_BTN_RIGHT) == GLFW_PRESS) {
			screen.SetShouldClose(true);
		}
	}
	else {
		if (Keyboard::Key(GLFW_KEY_W)) {
			camera.UpdateCameraPosition(CameraDirection::FORWARD, dt);
		}
		if (Keyboard::Key(GLFW_KEY_S)) {
			camera.UpdateCameraPosition(CameraDirection::BACKWARD, dt);
		}
		if (Keyboard::Key(GLFW_KEY_A)) {
			camera.UpdateCameraPosition(CameraDirection::LEFT, dt);
		}
		if (Keyboard::Key(GLFW_KEY_D)) {
			camera.UpdateCameraPosition(CameraDirection::RIGHT, dt);
		}
		if (Keyboard::Key(GLFW_KEY_SPACE)) {
			camera.UpdateCameraPosition(CameraDirection::UP, dt);
		}
		if (Keyboard::Key(GLFW_KEY_LEFT_SHIFT)) {
			camera.UpdateCameraPosition(CameraDirection::DOWN, dt);
		}
		if (Keyboard::Key(GLFW_KEY_ESCAPE)) {
			screen.SetShouldClose(true);
		}
		if (Keyboard::KeyWentDown(GLFW_KEY_L)) {
			flashLightOn = !flashLightOn;
		}

		double dx = Mouse::GetDX(), dy = Mouse::GetDY();
		if (dx != 0 || dy != 0) {
			camera.UpdateCameraDirection(dx * dt * 8.0f, -dy * dt * 8.0f);
		}

		double scrollDX = Mouse::GetScrollDX(), scrollDY = Mouse::GetScrollDY();
		if (scrollDY != 0) {
			camera.UpdateCameraZoom(scrollDY);
		}
	}
	
}
