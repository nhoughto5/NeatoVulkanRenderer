#pragma once
#include "Events/Event.h"
#include "Events/KeyEvent.h"
#include "Events/ApplicationEvent.h"
#include "Events/MouseEvent.h"

const int WIDTH = 800;
const int HEIGHT = 600;

class Window
{
public:
	using EventCallbackFn = std::function<void(Event&)>;
	Window();
	~Window();

	GLFWwindow* getWindow();
	inline void SetEventCallback(const EventCallbackFn& callback) { 
		m_Data.EventCallback = callback; 
	}
private:

	GLFWwindow* m_Window;

	struct WindowData
	{
		std::string Title;
		unsigned int Width, Height;
		bool VSync;

		EventCallbackFn EventCallback;
	};
	WindowData m_Data;
};