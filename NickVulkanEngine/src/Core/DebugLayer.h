#pragma once
#include "Common.h"

class DebugLayer
{
public:
	void setupDebugCallback(const VkInstance& instance);
	void DestroyDebugReportCallbackEXT(VkInstance instance, const VkAllocationCallbacks* pAllocator);

private:
	VkDebugReportCallbackEXT callback;
	VkResult CreateDebugReportCallbackEXT(VkInstance instance, const VkDebugReportCallbackCreateInfoEXT* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkDebugReportCallbackEXT* pCallback);
};