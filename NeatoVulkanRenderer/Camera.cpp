#include "nvrpch.h"
#include "Camera.h"

Camera::Camera(SwapChain* swapChain) :
	m_SwapChain(swapChain)
{
}

Camera::~Camera()
{
}

glm::mat4 Camera::getPerspective() {
	return glm::perspective(glm::radians(45.0f), m_SwapChain->getSwapChainExtent().width / (float)m_SwapChain->getSwapChainExtent().height, 0.1f, 10.0f);
}

glm::mat4 Camera::getLookAt() {
	return glm::lookAt(glm::vec3(2.0f, 2.0f, 2.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, 1.0f));
}

glm::mat4 Camera::getRotate() {
	return glm::rotate(glm::mat4(1.0f), m_DeltaTime * glm::radians(45.0f), glm::vec3(0.0f, 0.0f, 1.0f));
}

void Camera::update() {
	static auto startTime = std::chrono::high_resolution_clock::now();
	auto currentTime = std::chrono::high_resolution_clock::now();
	m_DeltaTime = std::chrono::duration<float, std::chrono::seconds::period>(currentTime - startTime).count();
}