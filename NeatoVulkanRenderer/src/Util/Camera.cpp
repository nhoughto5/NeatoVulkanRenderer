#include "nvrpch.h"
#include "Camera.h"
#include <glm/gtx/string_cast.hpp>
#include <glfw3.h>

Camera::Camera(GLFWwindow* window, SwapChain* swapChain) :
	m_SwapChain(swapChain),
	m_Window(window)
{
	toString();
	std::cout << "Yaw: " << m_Yaw << "  Pitch: " << m_Pitch << "\n";
}

Camera::~Camera()
{
}

void Camera::onEvent(Event& e)
{
	if (e.IsInCategory(EventCategory::EventCategoryKeyboard)) {
		keyboardEvent(dynamic_cast<KeyEvent&>(e));
		toString();
	}
	else if (e.IsInCategory(EventCategory::EventCategoryMouse)) {
		mouseEvent(dynamic_cast<MouseMovedEvent&>(e));
	}
	else if (e.IsInCategory(EventCategory::EventCategoryMouseButton)) {
		mouseButtonEvent(e);
	}
}

glm::mat4 Camera::getPerspectiveMatrix() {
	return glm::perspective(glm::radians(45.0f), m_SwapChain->getSwapChainExtent().width / (float)m_SwapChain->getSwapChainExtent().height, 0.1f, 10.0f);
}

glm::mat4 Camera::getViewMatrix() {
	return glm::lookAt(m_Position, m_Position + m_Forward, m_Up);
}

glm::vec3 Camera::lookAtDirection()
{
	return glm::normalize(m_Position - m_Forward);
}

glm::vec3 Camera::getRight()
{
	return m_Right;
}

void Camera::keyboardEvent(KeyEvent & e)
{
	//std::cout << "Keyboard Event: " << e.ToString() << "\n";
	print = true;
	switch (e.GetKeyCode()) {
		case GLFW_KEY_W:
			m_Position += moveSpeed * m_Forward * m_DeltaTime;
			break;
		case GLFW_KEY_S:
			m_Position -= moveSpeed * m_Forward * m_DeltaTime;
			break;
		case GLFW_KEY_A:
			m_Position -= m_Right * moveSpeed * m_DeltaTime;
			break;
		case GLFW_KEY_D:
			m_Position += m_Right * moveSpeed * m_DeltaTime;
			break;
		case GLFW_KEY_E:
			m_Position += m_Up * moveSpeed * m_DeltaTime;
			break;
		case GLFW_KEY_Q:
			m_Position -= m_Up * moveSpeed * m_DeltaTime;
			break;
		case GLFW_KEY_ESCAPE:
			glfwSetInputMode(m_Window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
			break;
		default: 
			break;
	}
	print = true;
	std::cout << "Yaw: " << m_Yaw << "  Pitch: " << m_Pitch << "\n";
	//std::cout << "Position: " << m_Position.x << " " << m_Position.y << " " << m_Position.z << "\n";
}

void Camera::mouseEvent(MouseMovedEvent & e)
{
	//sreturn;
	//Update pitch yaw and roll
	float newX = lastX - e.GetX();
	float newY = e.GetY() - lastY;
	m_Pitch += newX * m_MouseSensitivity * m_DeltaTime;
	m_Yaw += newY * m_MouseSensitivity * m_DeltaTime;
	lastX = e.GetX();
	lastY = e.GetY();
	//if (m_Pitch > 80.f)
	//	m_Pitch = 80.f;
	//else if (m_Pitch < -80.f)
	//	m_Pitch = -80.f;

	if (m_Pitch > 360.f || m_Pitch < -360.f)
		m_Pitch = 0.f;
	if (m_Yaw > 360.f || m_Yaw < -360.f)
		m_Yaw = 0.f;

	std::cout << "Yaw: " << m_Yaw << "  Pitch: " << m_Pitch << "\n";
}

void Camera::mouseButtonEvent(Event & e)
{
	std::cout << "Mouse Button Event: " << e.ToString() << "\n";
}

void Camera::toString()
{
	std::cout << "Position: " << m_Position.x << ", " << m_Position.y << ", " << m_Position.z << "   ";
	std::cout << "Up: " << m_Up.x << ", " << m_Up.y << ", " << m_Up.z << " ";
	std::cout << "Look At: " << m_Forward.x << ", " << m_Forward.y << ", " << m_Forward.z << "\n";
}

void Camera::update() {
	static auto startTime = std::chrono::high_resolution_clock::now();
	auto currentTime = std::chrono::high_resolution_clock::now();
	m_DeltaTime = std::chrono::duration<float, std::chrono::seconds::period>(currentTime - startTime).count();

	m_Forward.x = cos(glm::radians(m_Yaw)) * cos(glm::radians(m_Pitch));
	m_Forward.y = sin(glm::radians(m_Pitch));
	m_Forward.z = sin(glm::radians(m_Yaw)) * cos(glm::radians(m_Pitch));

	m_Forward = glm::normalize(m_Forward);
	m_Right = glm::normalize(glm::cross(m_Forward, worldUp));
	m_Up = glm::normalize(glm::cross(m_Right, m_Forward));
}