#pragma once
#include "SwapChain.h"
#include "PhysicalDevice.h"
#include "LogicalDevice.h"
#include "RenderPass.h"

class GraphicsPipeline {
public:
	GraphicsPipeline(PhysicalDevice* physicalDevice, LogicalDevice* logicalDevice, SwapChain* swapChain, RenderPass* renderPass);
	~GraphicsPipeline();

	void Cleanup();
	
	VkPipeline getGraphicsPipeline();
	VkPipelineLayout getGraphicsPipelineLayout();
	VkDescriptorSetLayout getDescriptorSetLayout();

private:
	PhysicalDevice* m_PhysicalDevice;
	SwapChain* m_SwapChain;
	LogicalDevice* m_LogicalDevice;
	RenderPass* m_RenderPass;
	VkPipeline m_GraphicsPipeline;
	VkDescriptorSetLayout m_DescriptorSetLayout;
	VkPipelineLayout m_PipelineLayout;

	VkShaderModule createShaderModule(const std::vector<char>& code);
	void createDescriptorSetLayout();
};