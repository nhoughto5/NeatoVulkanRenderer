#pragma once
#include "Common.h"
class PhysicalDevice
{
public:
	PhysicalDevice();
	~PhysicalDevice();

	static bool isDeviceSuitable(VkPhysicalDevice device);
	void setPhysicalDevice(VkPhysicalDevice device);
	VkPhysicalDevice getPhysicalDevice();
private:
	VkPhysicalDevice m_PhysicalDevice;
};

