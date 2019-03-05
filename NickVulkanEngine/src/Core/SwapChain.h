#pragma once
class SwapChain
{
public:
	SwapChain();
	~SwapChain();

private:
	VkExtent2D swapChainExtent;
	std::vector<VkImage> swapChainImages;
	std::vector<VkImageView> swapChainImageViews;
};

