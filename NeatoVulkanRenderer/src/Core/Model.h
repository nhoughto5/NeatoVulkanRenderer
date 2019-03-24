#pragma once
#include "Common.h"
#include "PhysicalDevice.h"
#include "LogicalDevice.h"
#include "SwapChain.h"
#include "CommandBus.h"

const std::string MODEL_PATH = "models/chalet.obj";
const std::string TEXTURE_PATH = "textures/chalet.jpg";

class Model
{
public:
	Model(PhysicalDevice* physicalDevice, LogicalDevice* logicalDevice, SwapChain* swapChain, CommandBus* commandBus);
	~Model();

	void Cleanup();

	void createIndexBuffer();

	void createUniformBuffers();

	void createVertexBuffer();

	void loadModel();

	void createTextureImageView();
	void createTextureImage();
	void createDepthResources();
	void createColorResources();

	void generateMipmaps(VkImage image, VkFormat imageFormat, int32_t texWidth, int32_t texHeight, uint32_t mipLevels);

	void createTextureSampler();
	VkImageView getTextureImageView();
	VkImageView getColorImageView();
	VkImageView getDepthImageView();
	VkSampler getTextureSampler();
	VkBuffer getVertexBuffer();
	VkBuffer getIndexBuffer();
	std::vector<uint32_t> getIndices();
	std::vector<VkBuffer> getUniformBuffers();
	std::vector<VkDeviceMemory> getUniformBuffersMemory();
	int getNumIndicies();

private:
	VkImageView m_ColorImageView;
	VkImage m_ColorImage;
	VkDeviceMemory m_ColorImageMemory;
	VkImageView m_DepthImageView;
	VkImage m_DepthImage;
	VkDeviceMemory m_DepthImageMemory;
	VkImageView m_TextureImageView;
	VkImage m_TextureImage;
	VkDeviceMemory m_TextureImageMemory;
	uint32_t mipLevels;
	VkSampler m_TextureSampler;
	VkBuffer m_VertexBuffer;
	VkDeviceMemory m_VertexBufferMemory;
	VkBuffer m_IndexBuffer;
	VkDeviceMemory m_IndexBufferMemory;
	std::vector<VkBuffer> m_UniformBuffers;
	std::vector<VkDeviceMemory> m_UniformBuffersMemory;
	std::vector<Vertex> m_Vertices;
	std::vector<uint32_t> m_Indices;

	PhysicalDevice* m_PhysicalDevice;
	LogicalDevice* m_LogicalDevice;
	SwapChain* m_SwapChain;
	CommandBus* m_CommandBus;
};

