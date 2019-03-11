#pragma once
#include "SwapChain.h"
#include "PhysicalDevice.h"
#include "LogicalDevice.h"

class RenderPass
{
public:
	RenderPass(PhysicalDevice* physicalDevice, LogicalDevice* logicalDevice, SwapChain* swapChain);
	~RenderPass();

	VkRenderPass getRenderPass();
private:
	VkRenderPass m_RenderPass;
	SwapChain* m_SwapChain;
	PhysicalDevice* m_PhysicalDevice;
	LogicalDevice* m_LogicalDevice;
};

