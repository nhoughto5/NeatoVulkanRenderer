#pragma once
#include "Instance.h";
class Surface
{
public:
	Surface(const Instance* _parent, VkPhysicalDevice physicalDevice, VkDevice logicalDevice, void* window);
	Surface(Surface&& other) noexcept;
	Surface& operator=(Surface&& other) noexcept;
	~Surface();

	void Recreate();
	const VkSurfaceKHR& getSurface();
private:
	GLFWwindow* m_Window{ nullptr };
	const Instance* m_Instance{nullptr};
	VkPhysicalDevice m_PhysicalDevice{ VK_NULL_HANDLE };
	VkDevice m_LogicalDevice{ VK_NULL_HANDLE };
	VkSurfaceKHR m_Surface{ VK_NULL_HANDLE };
	std::vector<VkImageView> swapChainImageViews;

	void createSurface(VkFormat swapChainImageFormat);
};

