#pragma once
#include "Core/Common.h"
#include "Core/SwapChain.h"

class Camera
{
public:
	Camera(GLFWwindow* window, SwapChain* swapChain);
	~Camera();

	void upFactor();

	static void registerKeyInput(GLFWwindow* window, int key, int scancode, int action, int mods);
	void update();

	glm::mat4 getPerspective();
	glm::mat4 getLookAt();
	glm::mat4 getRotate();

private:
	GLFWwindow* m_Window;
	SwapChain* m_SwapChain;
	float m_DeltaTime{ 0.0f };
	float factor{ 1.0f };
};

