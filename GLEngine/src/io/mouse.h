#pragma once

#include "core/coredef.h"
#include <glad/glad.h>
#include <GLFW/glfw3.h>

class Mouse
{
public:
	static void CursorPosCallback(GLFWwindow* window, f64 posX, f64 posY);
	static void MouseButtonCallback(GLFWwindow* window, MouseCode button, i32 action, i32 mods);
	static void MouseWheelCallback(GLFWwindow* window, f64 dx, f64 dy);

	static f64 GetMouseX();
	static f64 GetMouseY();

	static f64 GetDX();
	static f64 GetDY();

	static f64 GetScrollDX();
	static f64 GetScrollDY();

	static bool Button(MouseCode button);
	static bool ButtonChanged(MouseCode button);
	static bool ButtonWentUp(MouseCode button);
	static bool ButtonWentDown(MouseCode button);
private:
	static f64 x;
	static f64 y;

	static f64 lastX;
	static f64 lastY;

	static f64 dx;
	static f64 dy;

	static f64 scrollDX;
	static f64 scrollDY;

	static bool firstMouse;

	static bool buttons[];
	static bool buttonsChanged[];
};