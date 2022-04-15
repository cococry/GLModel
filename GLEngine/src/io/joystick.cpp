#include "glpch.h"
#include "joystick.h"

Joystick::Joystick(int i)
{
	id = GetID(i);

	Update();
}

void Joystick::Update()
{
	present = glfwJoystickPresent(id);

	if (present) {
		name = glfwGetJoystickName(id);
		axes = glfwGetJoystickAxes(id, &axesCount);
		buttons = glfwGetJoystickButtons(id, &buttonCount);
	}
}

f32 Joystick::AxesState(i32 axis)
{
	if (present) {
		return axes[axis];
	}
	return 0.0f;
}

u8 Joystick::ButtonState(KeyCode button)
{
	if (present) {
		return buttons[button];
	}
	return GLFW_RELEASE;
}

i32 Joystick::GetAxesCount()
{
	return axesCount;
}

i32 Joystick::GetButtonCount()
{
	return buttonCount;
}

bool Joystick::IsPresent()
{
	return present;
}

const char* Joystick::GetName()
{
	return name;
}

i32 Joystick::GetID(int i)
{
	return GLFW_JOYSTICK_1 + i;
}
