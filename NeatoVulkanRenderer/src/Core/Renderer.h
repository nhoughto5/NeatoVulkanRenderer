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

const int WIDTH = 800;
const int HEIGHT = 600;

class Renderer {
public:
	void run();
	~Renderer();

private:
	DebugLayer* debugger;
	Instance *instance;
	GLFWwindow * window;
	Surface* surface;
	PhysicalDevice* physicalDevice;
	LogicalDevice* logicalDevice;
	SwapChain* swapChain;
	RenderPass* renderPass;
	GraphicsPipeline* graphicsPipeline;
	CommandBus* commandBus;
	Model* houseModel;

	VkSemaphore imageAvailableSemaphore, renderFinishedSemaphore;
	VkDescriptorPool descriptorPool;
	std::vector<VkDescriptorSet> descriptorSets;

	void initWindow();
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