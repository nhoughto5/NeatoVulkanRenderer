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

	void createVertexBuffer();

	void loadModel();

	void createTextureImageView();
	void createTextureImage();
	void createDepthResources();
	void createColorResources();

	void generateMipmaps(VkImage image, VkFormat imageFormat, int32_t texWidth, int32_t texHeight, uint32_t mipLevels);

	void createTextureSampler();
	VkImageView getTextureImageView();
	VkSampler getTextureSampler();
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
	VkBuffer vertexBuffer;
	VkDeviceMemory vertexBufferMemory;
	VkBuffer indexBuffer;
	VkDeviceMemory indexBufferMemory;
	std::vector<Vertex> vertices;
	std::vector<uint32_t> indices;

	PhysicalDevice* m_PhysicalDevice;
	LogicalDevice* m_LogicalDevice;
	SwapChain* m_SwapChain;
	CommandBus* m_CommandBus;
};

