#include "nvrpch.h"
#include "Window.h"

Window::Window()
{
	glfwInit();
	glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
	glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);
	m_Window = glfwCreateWindow(WIDTH, HEIGHT, "Vulkan", nullptr, nullptr);
	glfwSetWindowUserPointer(m_Window, this);
}

Window::~Window()
{
}

GLFWwindow * Window::getWindow()
{
	return m_Window;
}