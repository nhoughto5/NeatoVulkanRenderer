#include "nvepch.h"
#include "PhysicalDevice.h"

PhysicalDevice::PhysicalDevice()
{
}

PhysicalDevice::~PhysicalDevice()
{
}

bool PhysicalDevice::isDeviceSuitable(VkPhysicalDevice device) {
	QueueFamilyIndices indices = findQueueFamilies(device);

	// Checking for swap chain support
	bool extensionsSupported = checkDeviceExtensionSupport(device);

	bool swapChainAdequate = false;
	if (extensionsSupported) {
		SwapChainSupportDetails swapChainSupport = querySwapChainSupport(device);
		swapChainAdequate = !swapChainSupport.formats.empty() && !swapChainSupport.presentModes.empty();
	}

	VkPhysicalDeviceFeatures supportedFeatures;
	vkGetPhysicalDeviceFeatures(device, &supportedFeatures);

	return indices.isComplete() && extensionsSupported && swapChainAdequate && supportedFeatures.samplerAnisotropy;
}

void PhysicalDevice::setPhysicalDevice(VkPhysicalDevice device)
{
	m_PhysicalDevice = device;
}

VkPhysicalDevice PhysicalDevice::getPhysicalDevice()
{
	return m_PhysicalDevice;
}
