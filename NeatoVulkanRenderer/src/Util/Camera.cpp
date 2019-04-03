#include "nvrpch.h"
#include "Camera.h"

Camera::Camera(GLFWwindow* window, SwapChain* swapChain) :
	m_SwapChain(swapChain),
	m_Window(window)
{
}

Camera::~Camera()
{
}

void Camera::onEvent(Event& e)
{
	if (e.IsInCategory(EventCategory::EventCategoryKeyboard)) {
		keyboardEvent(dynamic_cast<KeyEvent&>(e));
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
	return glm::lookAt(position, lookAt, up);
}

glm::mat4 Camera::getRotate() {
	return glm::rotate(glm::mat4(1.0f), glm::radians(rot), up);
}

glm::vec3 Camera::lookAtDirection()
{
	return glm::normalize(position - lookAt);
}

glm::vec3 Camera::getRight()
{
	return glm::normalize(glm::cross(up, lookAtDirection()));
}

void Camera::keyboardEvent(KeyEvent & e)
{
	std::cout << "Keyboard Event: " << e.ToString() << "\n";
	switch (e.GetKeyCode()) {
		case GLFW_KEY_S:
			position.y += 1;
			break;
		case GLFW_KEY_W:
			position.y -= 1;
			break;
	}
	std::cout << "Position: " << position.y << "\n";
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

void Camera::update() {
	static auto startTime = std::chrono::high_resolution_clock::now();
	auto currentTime = std::chrono::high_resolution_clock::now();
	m_DeltaTime = std::chrono::duration<float, std::chrono::seconds::period>(currentTime - startTime).count();
}