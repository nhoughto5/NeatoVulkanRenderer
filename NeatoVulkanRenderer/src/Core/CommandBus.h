#pragma once
#include "LogicalDevice.h"
#include "PhysicalDevice.h"
#include "RenderPass.h"
#include "SwapChain.h"
#include "GraphicsPipeline.h"

class CommandBus
{
public:
	CommandBus(PhysicalDevice* physicalDevice, LogicalDevice* logicalDevice, SwapChain* swapChain, RenderPass* renderPass, GraphicsPipeline* graphicsPipeline);
	~CommandBus();
	void Cleanup();

	void copyBuffer(VkBuffer srcBuffer, VkBuffer dstBuffer, VkDeviceSize size);

	void copyBufferToImage(VkBuffer buffer, VkImage image, uint32_t width, uint32_t height);

	void transitionImageLayout(VkImage image, VkFormat format, VkImageLayout oldLayout, VkImageLayout newLayout, uint32_t mipLevels);

	void createCommandPool();

	void createCommandBuffers(VkBuffer vertexBuffer, VkBuffer indexBuffer, int numIndices, std::vector<VkDescriptorSet> descriptorSets);

	VkCommandBuffer beginSingleTimeCommands();
	void endSingleTimeCommands(VkCommandBuffer commandBuffer);
	VkCommandBuffer* getCommandBufferByIndex(int index);
	std::vector<VkCommandBuffer> getCommandBuffers();
	int commandBuffersCount();
	VkCommandPool getCommandPool();

private:
	LogicalDevice* m_LogicalDevice;
	PhysicalDevice* m_PhysicalDevice;
	RenderPass* m_RenderPass;
	SwapChain* m_SwapChain;
	GraphicsPipeline* m_GraphicsPipeline;
	VkCommandPool m_CommandPool;
	std::vector<VkCommandBuffer> m_CommandBuffers;
};

