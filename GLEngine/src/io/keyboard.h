#pragma once

#include "core/coredef.h"

#include <glad/glad.h>
#include <GLFW/glfw3.h>

class Keyboard {
public:
	static void KeyCallback(GLFWwindow* window, KeyCode key, i32 scancode, i32 action, i32 mods);
	static bool Key(KeyCode key);
	static bool KeyChanged(KeyCode key);
	static bool KeyWentUp(KeyCode key);
	static bool KeyWentDown(KeyCode key);
private:
	static bool keys[];
	static bool keysChanged[];
};