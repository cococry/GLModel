#pragma once

#include "core/coredef.h"

#include <glad/glad.h>
#include <GLFW/glfw3.h>

class Screen 
{
public:
	static u32 WINDOW_WIDTH, WINDOW_HEIGHT;

	static void ResizeViewportCllbc(GLFWwindow* window, i32 width, i32 height);

	Screen();
	~Screen();

	bool Init();

	void SetGLParams();

	void Update();
	void NewFrame();

	bool ShouldClose();
	void SetShouldClose(bool arg); 

	inline GLFWwindow* GetNativeWindow() const { return m_Window; }
private:
	GLFWwindow* m_Window;
};