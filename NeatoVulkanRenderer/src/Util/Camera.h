#pragma once
#include "Core/Common.h"
#include "Core/SwapChain.h"
#include "Events/Event.h"
#include "Events/KeyEvent.h"
#include "Events/MouseEvent.h"

class Camera
{
public:
	Camera(GLFWwindow* window, SwapChain* swapChain);
	~Camera();

	void onEvent(Event& e);
	void update();

	glm::mat4 getPerspectiveMatrix();
	glm::mat4 getViewMatrix();
	glm::vec3 lookAtDirection();
	glm::vec3 getRight();

	void toString();
private:

	void keyboardEvent(KeyEvent& e);
	void mouseEvent(MouseMovedEvent& e);
	void mouseButtonEvent(Event& e);
	bool print{ false };

	glm::vec3 m_Position{ glm::vec3(2.0f, 0.0f, 1.0f) };
	glm::vec3 m_Right{ glm::vec3(0.0f, 0.0f, 0.0f) };
	glm::vec3 m_Forward{ glm::vec3(0.0f, 0.0f, 0.0f) };
	glm::vec3 m_Up{ glm::vec3(0.0f, 0.0f, 1.0f) };
	const glm::vec3 worldUp{ glm::vec3(0.0f, 0.0f, 1.0f) };

	float m_Pitch{ 180.0f };
	float m_Yaw{ 0.0f };
	float m_Roll{ 0.0f };
	float m_MouseSensitivity{ .00001f };

	GLFWwindow* m_Window;
	SwapChain* m_SwapChain;
	float m_DeltaTime{ 0.0f };
	float moveSpeed{ 0.005f };
};

