#include "nvrpch.h"
#include "Camera.h"
#include <glm/gtx/string_cast.hpp>

Camera::Camera(GLFWwindow* window, SwapChain* swapChain) :
	m_SwapChain(swapChain),
	m_Window(window)
{
	toString();
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
		mouseEvent(e);
	}
	else if (e.IsInCategory(EventCategory::EventCategoryMouseButton)) {
		mouseEvent(e);
	}
}

glm::mat4 Camera::getPerspective() {
	return glm::perspective(glm::radians(45.0f), m_SwapChain->getSwapChainExtent().width / (float)m_SwapChain->getSwapChainExtent().height, 0.1f, 10.0f);
}

glm::mat4 Camera::getLookAt() {
	auto mat = glm::lookAt(m_Position, m_LookAt, m_Up);
	if (print) std::cout << glm::to_string(mat) << "\n";
	print = false;
	return mat;
}

glm::mat4 Camera::getRotate() {
	return glm::rotate(glm::mat4(1.0f), glm::radians(rot), m_Up);
}

glm::vec3 Camera::lookAtDirection()
{
	return glm::normalize(m_Position - m_LookAt);
}

glm::vec3 Camera::getRight()
{
	return glm::normalize(glm::cross(m_Up, lookAtDirection()));
}

void Camera::keyboardEvent(KeyEvent & e)
{
	//std::cout << "Keyboard Event: " << e.ToString() << "\n";
	print = true;
	switch (e.GetKeyCode()) {
		case GLFW_KEY_W:
			m_Position += moveSpeed * m_LookAt;
			break;
		case GLFW_KEY_S:
			m_Position -= moveSpeed * m_LookAt;
			break;
		case GLFW_KEY_A:
			m_Position -= glm::normalize(glm::cross(m_LookAt, m_Up)) * moveSpeed;
			break;
		case GLFW_KEY_D:
			m_Position += glm::normalize(glm::cross(m_LookAt, m_Up)) * moveSpeed;
			break;
		case GLFW_KEY_E:
			rot += 0.5f;
			break;
		case GLFW_KEY_Q:
			rot -= 0.5f;
			break;
		default: 
			break;
	}
	//std::cout << "Position: " << m_Position.x << " " << m_Position.y << " " << m_Position.z << "\n";
}

void Camera::mouseEvent(Event & e)
{
	std::cout << "Mouse Event: " << e.ToString() << "\n";
}

void Camera::mouseButtonEvent(Event & e)
{
	std::cout << "Mouse Button Event: " << e.ToString() << "\n";
}

void Camera::registerKeyInput(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	switch (key) {
	case GLFW_KEY_SPACE:
		std::cout << "Space\n";
		break;
	default:
		break;
	}
}

void Camera::toString()
{
	std::cout << "Position: " << m_Position.x << ", " << m_Position.y << ", " << m_Position.z << "   ";
	std::cout << "Up: " << m_Up.x << ", " << m_Up.y << ", " << m_Up.z << " ";
	std::cout << "Look At: " << m_LookAt.x << ", " << m_LookAt.y << ", " << m_LookAt.z << "\n";
}

void Camera::update() {
	static auto startTime = std::chrono::high_resolution_clock::now();
	auto currentTime = std::chrono::high_resolution_clock::now();
	m_DeltaTime = std::chrono::duration<float, std::chrono::seconds::period>(currentTime - startTime).count();
}