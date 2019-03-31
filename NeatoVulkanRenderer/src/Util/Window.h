#pragma once
const int WIDTH = 800;
const int HEIGHT = 600;

class Window
{
public:
	Window();
	~Window();

	GLFWwindow* getWindow();
private:

	GLFWwindow* m_Window;
};

