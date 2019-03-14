#pragma once
#include "LogicalDevice.h"
#include "PhysicalDevice.h"

class CommandBus
{
public:
	CommandBus(PhysicalDevice* physicalDevice, LogicalDevice* logicalDevice);
	~CommandBus();
	void Cleanup();

	void createCommandPool();

	void FreeCommandBuffers();
	void createCommandBuffers();
	VkCommandBuffer beginSingleTimeCommands();
	void endSingleTimeCommands(VkCommandBuffer commandBuffer);

private:
	LogicalDevice* m_LogicalDevice;
	PhysicalDevice* m_PhysicalDevice;
	VkCommandPool m_CommandPool;
};

