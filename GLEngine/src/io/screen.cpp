#include "glpch.h"
#include "screen.h"
#include "io/keyboard.h"
#include "io/mouse.h"

u32 Screen::WINDOW_WIDTH = 1920;
u32 Screen::WINDOW_HEIGHT = 1080;

void Screen::ResizeViewportCllbc(GLFWwindow* window, i32 width, i32 height)
{
	glViewport(0, 0, width, height);

	WINDOW_WIDTH = width;
	WINDOW_HEIGHT = height;
}

Screen::Screen()
{

}

Screen::~Screen()
{
	glfwTerminate();
}

bool Screen::Init()
{
	glfwInit();

	m_Window = glfwCreateWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "OpenGL Application", nullptr, nullptr);

	if (!m_Window) {
		std::cout << "[Windowing]: ERROR: Could not create GLFW Window!\n"; 
		return false;    
	}
	glfwMakeContextCurrent(m_Window);
	return true;
}

void Screen::SetGLParams()
{
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
		std::cout << "[ERROR]: Glad could not be intialized successfully.";
		glfwTerminate();
	}
	glViewport(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT);
	glfwSetFramebufferSizeCallback(m_Window, Screen::ResizeViewportCllbc);
	glfwSetKeyCallback(m_Window, Keyboard::KeyCallback);
	glfwSetCursorPosCallback(m_Window, Mouse::CursorPosCallback);
	glfwSetScrollCallback(m_Window, Mouse::MouseWheelCallback);

	glfwSetInputMode(m_Window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	glEnable(GL_DEPTH_TEST);
}

void Screen::Update()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glClearColor(COLOR_RGB_4(99, 150, 166, 255));
}

void Screen::NewFrame()
{
	glfwSwapBuffers(m_Window);

}

bool Screen::ShouldClose()
{
	return glfwWindowShouldClose(m_Window);
}

void Screen::SetShouldClose(bool arg)
{
	glfwSetWindowShouldClose(m_Window, arg); 
}
