#pragma once
#include "Core/Common.h"
#include "Core/SwapChain.h"
#include "Events/Event.h"
#include "Events/KeyEvent.h"

class Camera
{
public:
	Camera(GLFWwindow* window, SwapChain* swapChain);
	~Camera();

	void onEvent(Event& e);

	static void registerKeyInput(GLFWwindow* window, int key, int scancode, int action, int mods);
	void update();

	glm::mat4 getPerspective();
	glm::mat4 getLookAt();
	glm::mat4 getRotate();
	glm::vec3 lookAtDirection();
	glm::vec3 getRight();

	void toString();
private:

	void keyboardEvent(KeyEvent& e);
	void mouseEvent(Event& e);
	void mouseButtonEvent(Event& e);
	bool print{ false };

	glm::vec3 m_Position{ glm::vec3(2.0f, 2.0f, 2.0f) };
	glm::vec3 m_LookAt{ glm::vec3(0.0f, 0.0f, 1.0f) };
	glm::vec3 m_Up{ glm::vec3(0.0f, 0.0f, 1.0f) };

	GLFWwindow* m_Window;
	SwapChain* m_SwapChain;
	float m_DeltaTime{ 0.0f };
	float rot{ 0.0f };
	float moveSpeed{ 0.05f };
};

