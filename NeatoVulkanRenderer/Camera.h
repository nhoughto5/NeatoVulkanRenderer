#pragma once
#include "Core/Common.h"
#include "Core/SwapChain.h"

class Camera
{
public:
	Camera(SwapChain* swapChain);
	~Camera();
	void update();

	glm::mat4 getPerspective();
	glm::mat4 getLookAt();
	glm::mat4 getRotate();

private:
	SwapChain* m_SwapChain;
	float m_DeltaTime{ 0.0f };
};

