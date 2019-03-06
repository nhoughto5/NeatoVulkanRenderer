#pragma once
#include "PhysicalDevice.h"

class LogicalDevice
{
public:
	LogicalDevice(PhysicalDevice* physicalDevice);
	~LogicalDevice();

	VkDevice getLogicalDevice();
	VkQueue getGraphicsQueue();
	VkQueue getPresentQueue();
private:
	VkQueue graphicsQueue;
	VkQueue presentQueue;
	VkDevice m_LogicalDevice;
	PhysicalDevice* m_PhysicalDevice;
};

