#include "glpch.h"
#include "keyboard.h"

bool Keyboard::keys[GLFW_KEY_LAST] = { 0 };
bool Keyboard::keysChanged[GLFW_KEY_LAST] = { 0 };

void Keyboard::KeyCallback(GLFWwindow* window, KeyCode key, i32 scancode, i32 action, i32 mods)
{
	if (action != GLFW_RELEASE) {
		if (!keys[key]) {
			keys[key] = true;
		}
	}
	else {
		keys[key] = false;
	}

	keysChanged[key] = action != GLFW_REPEAT;
}

bool Keyboard::Key(KeyCode key)
{
	return keys[key];
}

bool Keyboard::KeyChanged(KeyCode key)
{
	bool returning = keysChanged[key];
	keysChanged[key] = false;
	return returning;
}

bool Keyboard::KeyWentUp(KeyCode key)
{
	return !keys[key] && KeyChanged(key);
}

bool Keyboard::KeyWentDown(KeyCode key)
{
	return keys[key] && KeyChanged(key);
}
