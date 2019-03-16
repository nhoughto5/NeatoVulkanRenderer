#pragma once
#include "LogicalDevice.h"
#include "PhysicalDevice.h"
#include "RenderPass.h"
#include "SwapChain.h"
#include "GraphicsPipeline.h"

class CommandBus
{
public:
	CommandBus(PhysicalDevice* physicalDevice, LogicalDevice* logicalDevice, SwapChain* swapChain, RenderPass* renderPass, GraphicsPipeline* graphicsPipeline);
	~CommandBus();
	void Cleanup();

	void createCommandPool();

	void createCommandBuffers(int numFrameBuffers);
	VkCommandBuffer beginSingleTimeCommands();
	void endSingleTimeCommands(VkCommandBuffer commandBuffer);
	VkCommandBuffer* getCommandBufferByIndex(int index);
	int commandBuffersCount();
	VkCommandPool getPool();

private:
	LogicalDevice* m_LogicalDevice;
	PhysicalDevice* m_PhysicalDevice;
	RenderPass* m_RenderPass;
	SwapChain* m_SwapChain;
	GraphicsPipeline* m_GraphicsPipeline;
	VkCommandPool m_CommandPool;
	std::vector<VkCommandBuffer> m_CommandBuffers;
};

