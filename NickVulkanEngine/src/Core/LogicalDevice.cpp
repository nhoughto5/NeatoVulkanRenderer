#include "nvepch.h"
#include "LogicalDevice.h"

LogicalDevice::LogicalDevice(PhysicalDevice* physicalDevice)
{
	// Specifying the queues to be created
	QueueFamilyIndices indices = physicalDevice->findQueueFamilies(physicalDevice->getPhysicalDevice());

	std::vector<VkDeviceQueueCreateInfo> queueCreateInfos;
	std::set<int> uniqueQueueFamilies = { indices.graphicsFamily, indices.presentFamily };

	float queuePriority = 1.0f;
	for (int queueFamily : uniqueQueueFamilies) {
		VkDeviceQueueCreateInfo queueCreateInfo = {};
		queueCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
		queueCreateInfo.queueFamilyIndex = queueFamily;
		queueCreateInfo.queueCount = 1;
		queueCreateInfo.pQueuePriorities = &queuePriority;
		queueCreateInfos.push_back(queueCreateInfo);
	}

	// Specifying used device features
	VkPhysicalDeviceFeatures deviceFeatures = {};
	deviceFeatures.samplerAnisotropy = VK_TRUE;
	deviceFeatures.sampleRateShading = VK_TRUE;
	// Creating the logical device
	VkDeviceCreateInfo createInfo = {};
	createInfo.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;

	createInfo.queueCreateInfoCount = static_cast<uint32_t>(queueCreateInfos.size());
	createInfo.pQueueCreateInfos = queueCreateInfos.data();

	createInfo.pEnabledFeatures = &deviceFeatures;

	createInfo.enabledExtensionCount = static_cast<uint32_t>(deviceExtensions.size());
	createInfo.ppEnabledExtensionNames = deviceExtensions.data();

	if (enableValidationLayers) {
		createInfo.enabledLayerCount = static_cast<uint32_t>(validationLayers.size());
		createInfo.ppEnabledLayerNames = validationLayers.data();
	}
	else {
		createInfo.enabledLayerCount = 0;
	}

	if (vkCreateDevice(physicalDevice->getPhysicalDevice(), &createInfo, nullptr, &m_LogicalDevice) != VK_SUCCESS) {
		throw std::runtime_error("failed to create logical device!");
	}

	// Retrieving queue handles
	vkGetDeviceQueue(m_LogicalDevice, indices.graphicsFamily, 0, &graphicsQueue);
	vkGetDeviceQueue(m_LogicalDevice, indices.presentFamily, 0, &presentQueue);
}

LogicalDevice::~LogicalDevice()
{
}

VkDevice LogicalDevice::getLogicalDevice()
{
	return m_LogicalDevice;
}

VkQueue LogicalDevice::getGraphicsQueue()
{
	return graphicsQueue;
}

VkQueue LogicalDevice::getPresentQueue()
{
	return presentQueue;
}
