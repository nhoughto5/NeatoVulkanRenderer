#pragma once
#include "DebugLayer.h"
#include "Instance.h"
#include "Common.h"
#include "Surface.h"
#include "LogicalDevice.h"
#include "PhysicalDevice.h"
#include "SwapChain.h"
#include "RenderPass.h"
#include "GraphicsPipeline.h"
#include "CommandBus.h"
#include "Model.h"
#include "Util/Camera.h"
#include "Util/Window.h"

class Renderer {
public:
	void run();
	~Renderer();

private:
	Window* window;
	DebugLayer* debugger;
	Instance *instance;
	Surface* surface;
	PhysicalDevice* physicalDevice;
	LogicalDevice* logicalDevice;
	SwapChain* swapChain;
	RenderPass* renderPass;
	GraphicsPipeline* graphicsPipeline;
	CommandBus* commandBus;
	Model* houseModel;
	Camera* camera;

	void onEvent(Event& e);
	VkSemaphore imageAvailableSemaphore, renderFinishedSemaphore;
	VkDescriptorPool descriptorPool;
	std::vector<VkDescriptorSet> descriptorSets;

	static void onWindowResize(GLFWwindow* window, int width, int height);
	void initVulkan();
	void createDescriptorSets();

	void recreateSwapChain();
	void createSemaphores();
	void drawFrame();
	void mainLoop();
	void cleanupSwapChain();
	void cleanup();
	void update(uint32_t currentImage);
};