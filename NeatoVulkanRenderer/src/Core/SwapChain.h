#pragma once
#include "Common.h"
#include "LogicalDevice.h"
#include "PhysicalDevice.h"
#include "Surface.h"
#include "RenderPass.h"

class SwapChain
{
public:
	SwapChain(GLFWwindow* window, LogicalDevice *logicalDevice, PhysicalDevice* physicalDevice, Surface* surface);

	~SwapChain();

	VkFormat getSwapChainImageFormat();
	VkExtent2D getSwapChainExtent();
	void createFrameBuffers(RenderPass* renderPass);

	const std::vector<VkImage>& getSwapChainImages() const;
	const std::vector<VkImageView>& getSwapChainImageViews() const;
	const std::vector<VkFramebuffer>& getSwapChainFrameBuffers() const;
	const VkFramebuffer getFrameBufferByIndex(int i) const;

	VkSwapchainKHR getSwapChain();

	void Cleanup();
private:
	VkPresentModeKHR chooseSwapPresentMode(const std::vector<VkPresentModeKHR> availablePresentModes);
	VkSurfaceFormatKHR chooseSwapSurfaceFormat(const std::vector<VkSurfaceFormatKHR>& availableFormats);
	void createImageViews();
	VkExtent2D chooseSwapExtent(const VkSurfaceCapabilitiesKHR & capabilities);

	LogicalDevice *m_LogicalDevice;
	PhysicalDevice *m_PhysicalDevice;
	Surface* m_Surface;
	GLFWwindow* m_Window;

	VkFormat m_SwapChainImageFormat;
	VkSwapchainKHR m_SwapChain;
	VkExtent2D m_SwapChainExtent;
	std::vector<VkImage> m_SwapChainImages;
	std::vector<VkImageView> m_SwapChainImageViews;
	std::vector<VkFramebuffer> m_SwapChainFrameBuffers;
};

