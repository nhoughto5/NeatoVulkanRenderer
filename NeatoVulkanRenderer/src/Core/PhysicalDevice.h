#pragma once
#include "Common.h"
#include "Surface.h"

const std::vector<const char*> deviceExtensions = {
	VK_KHR_SWAPCHAIN_EXTENSION_NAME
};

class PhysicalDevice
{
public:
	PhysicalDevice(Instance* instance, Surface* surface);
	~PhysicalDevice();

	QueueFamilyIndices findQueueFamilies(VkPhysicalDevice device);
	bool checkDeviceExtensionSupport(VkPhysicalDevice device);
	SwapChainSupportDetails querySwapChainSupport(VkPhysicalDevice device);
	bool isDeviceSuitable(VkPhysicalDevice device);
	void setPhysicalDevice(VkPhysicalDevice device);
	VkPhysicalDevice getPhysicalDevice();
	VkSampleCountFlagBits getMaxUsableSampleCount();

private:

	Surface* m_Surface;
	VkPhysicalDevice m_PhysicalDevice;
	QueueFamilyIndices m_QueueFamilyIndices;
	Instance* m_Instance;
	bool maxUsableSampleCountFound{ false };
	VkSampleCountFlagBits msaaSamples = VK_SAMPLE_COUNT_1_BIT;
};

