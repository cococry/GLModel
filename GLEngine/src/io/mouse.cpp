#include "glpch.h"
#include "mouse.h"

f64 Mouse::x = 0;
f64 Mouse::y = 0;
f64 Mouse::lastX = 0;
f64 Mouse::lastY = 0;
f64 Mouse::dx = 0;
f64 Mouse::dy = 0;
f64 Mouse::scrollDX = 0;
f64 Mouse::scrollDY = 0;

bool Mouse::firstMouse = true;

bool Mouse::buttons[GLFW_MOUSE_BUTTON_LAST] = { 0 };
bool Mouse::buttonsChanged[GLFW_MOUSE_BUTTON_LAST] = { 0 };

void Mouse::CursorPosCallback(GLFWwindow* window, f64 posX, f64 posY)
{
	x = posX;
	y = posY;

	if (firstMouse) {
		lastX = x;
		lastY = y;
		firstMouse = false;
	}

	dx = x - lastX;
	dy = y - lastY;
	lastX = x;
	lastY = y;
}

void Mouse::MouseButtonCallback(GLFWwindow* window, MouseCode button, i32 action, i32 mods)
{
	if (action != GLFW_RELEASE) {
		if (!buttons[button]) {
			buttons[button] = true;
		}
	}
	else {
		buttons[button] = false;
	}

	buttonsChanged[button] = action != GLFW_REPEAT;
}

void Mouse::MouseWheelCallback(GLFWwindow* window, f64 dx, f64 dy)
{
	scrollDX = dx;
	scrollDY = dy;
}

f64 Mouse::GetMouseX()
{
	return x;
}

f64 Mouse::GetMouseY()
{
	return y;
}

f64 Mouse::GetDX()
{
	f64 _dx = dx;
	dx = 0;
	return _dx;
}

f64 Mouse::GetDY()
{
	f64 _dy = dy;
	dy = 0;
	return _dy;
}

f64 Mouse::GetScrollDX()
{
	double dx = scrollDX;
	scrollDX = 0;
	return dx;
}

f64 Mouse::GetScrollDY()
{
	double dy = scrollDY;
	scrollDY = 0;
	return dy;
}

bool Mouse::Button(MouseCode button)
{
	return buttons[button];
}

bool Mouse::ButtonChanged(MouseCode button)
{
	bool returning = buttonsChanged[button];
	buttonsChanged[button] = false;
	return returning;
}

bool Mouse::ButtonWentUp(MouseCode button)
{
	return !buttons[button] && ButtonChanged(button);
}

bool Mouse::ButtonWentDown(MouseCode button)
{
	return buttons[button] && ButtonChanged(button);
}
