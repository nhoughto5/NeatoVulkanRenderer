#include "nvepch.h"
#include "Surface.h"

Surface::Surface(const Instance * _parent, VkPhysicalDevice physicalDevice, void * _window) : 
	m_Instance(_parent), m_PhysicalDevice(physicalDevice), m_Window(reinterpret_cast<GLFWwindow*>(_window))
{
	createSurface();
}

Surface::Surface(Surface && other) noexcept
{
}

Surface & Surface::operator=(Surface && other) noexcept
{
	// TODO: insert return statement here
}

Surface::~Surface()
{
}

void Surface::createSurface() {
	if (glfwCreateWindowSurface(m_Instance->getInstance(), m_Window, nullptr, &m_Surface) != VK_SUCCESS) {
		throw std::runtime_error("Failed to create window surface");
	}

	for (size_t i = 0; i < swapChainImages.size(); i++) {
		VkImageViewCreateInfo createInfo = {};
		createInfo.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
		createInfo.image = swapChainImages[i];
		createInfo.viewType = VK_IMAGE_VIEW_TYPE_2D;
		createInfo.format = swapChainImageFormat;
		createInfo.components.r = VK_COMPONENT_SWIZZLE_IDENTITY;
		createInfo.components.g = VK_COMPONENT_SWIZZLE_IDENTITY;
		createInfo.components.b = VK_COMPONENT_SWIZZLE_IDENTITY;
		createInfo.components.a = VK_COMPONENT_SWIZZLE_IDENTITY;
		createInfo.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
		createInfo.subresourceRange.baseMipLevel = 0;
		createInfo.subresourceRange.levelCount = 1;
		createInfo.subresourceRange.baseArrayLayer = 0;
		createInfo.subresourceRange.layerCount = 1;
		if (vkCreateImageView(device, &createInfo, nullptr, &swapChainImageViews[i]) != VK_SUCCESS) {
			throw std::runtime_error("failed to create image views!");
		}
	}
}

void Surface::Recreate()
{
}

const VkSurfaceKHR & Surface::getSurface()
{
	return m_Surface;
}
