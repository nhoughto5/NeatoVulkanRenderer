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
	DebugLayer debugger;
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
	std::vector<VkBuffer> uniformBuffers;
	std::vector<VkDeviceMemory> uniformBuffersMemory;
	VkDescriptorPool descriptorPool;
	std::vector<VkDescriptorSet> descriptorSets;

	void initWindow();
	static void onWindowResize(GLFWwindow* window, int width, int height);
	void initVulkan();
	void createColorResources();
	void loadModel();
	bool hasStencilComponent(VkFormat format);
	void createTextureSampler();
	void createDescriptorSets();
	void createDescriptorPool();
	void createUniformBuffers();
	void createIndexBuffer();
	void createBuffer(VkDeviceSize size, VkBufferUsageFlags usage, VkMemoryPropertyFlags properties, VkBuffer& buffer, VkDeviceMemory& bufferMemory);
	void createVertexBuffer();
	void copyBuffer(VkBuffer srcBuffer, VkBuffer dstBuffer, VkDeviceSize size);
	void createSemaphores();
	void transitionImageLayout(VkImage image, VkFormat format, VkImageLayout oldLayout, VkImageLayout newLayout, uint32_t mipLevels);
	void copyBufferToImage(VkBuffer buffer, VkImage image, uint32_t width, uint32_t height);

	void recreateSwapChain();

	void drawFrame();
	void mainLoop();
	void cleanupSwapChain();
	void cleanup();
	void updateUniformBuffer(uint32_t currentImage);
};