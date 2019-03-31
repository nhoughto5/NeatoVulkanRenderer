#include "nvrpch.h"
#include "Camera.h"

Camera::Camera(GLFWwindow* window, SwapChain* swapChain) :
	m_SwapChain(swapChain),
	m_Window(window)
{
	//glfwSetKeyCallback(m_Window, registerKeyInput);
	//Camera::angle = 0.0f;
}

Camera::~Camera()
{
}

glm::mat4 Camera::getPerspective() {
	//return glm::perspective(glm::radians(angle), m_SwapChain->getSwapChainExtent().width / (float)m_SwapChain->getSwapChainExtent().height, 0.1f, 10.0f);
	return glm::perspective(glm::radians(45.0f), m_SwapChain->getSwapChainExtent().width / (float)m_SwapChain->getSwapChainExtent().height, 0.1f, 10.0f);
}

glm::mat4 Camera::getLookAt() {
	return glm::lookAt(glm::vec3(2.0f, 2.0f, 2.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, 1.0f));
}

glm::mat4 Camera::getRotate() {
	return glm::rotate(glm::mat4(1.0f), glm::radians(45.0f * m_DeltaTime), glm::vec3(0.0f, 0.0f, 1.0f));
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