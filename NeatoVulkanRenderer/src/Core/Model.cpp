#include "nvrpch.h"
#include "Model.h"
#include <tiny_obj_loader.h>
#include <stb_image.h>

Model::Model(PhysicalDevice* physicalDevice, LogicalDevice* logicalDevice, SwapChain* swapChain, CommandBus* commandBus) :
	m_PhysicalDevice(physicalDevice),
	m_LogicalDevice(logicalDevice),
	m_SwapChain(swapChain),
	m_CommandBus(commandBus)
{
}

Model::~Model()
{
}

glm::mat4 Model::getModelMatrix() {
	return glm::mat4(1.0f);
}

void Model::Cleanup() {
	vkDestroySampler(m_LogicalDevice->getLogicalDevice(), m_TextureSampler, nullptr);
	vkDestroyImageView(m_LogicalDevice->getLogicalDevice(), m_TextureImageView, nullptr);

	vkDestroyImageView(m_LogicalDevice->getLogicalDevice(), m_TextureImageView, nullptr);
	vkDestroyImage(m_LogicalDevice->getLogicalDevice(), m_TextureImage, nullptr);
	vkFreeMemory(m_LogicalDevice->getLogicalDevice(), m_TextureImageMemory, nullptr);

	vkDestroyImageView(m_LogicalDevice->getLogicalDevice(), m_ColorImageView, nullptr);
	vkDestroyImage(m_LogicalDevice->getLogicalDevice(), m_ColorImage, nullptr);
	vkFreeMemory(m_LogicalDevice->getLogicalDevice(), m_ColorImageMemory, nullptr);

	vkDestroyImageView(m_LogicalDevice->getLogicalDevice(), m_DepthImageView, nullptr);
	vkDestroyImage(m_LogicalDevice->getLogicalDevice(), m_DepthImage, nullptr);
	vkFreeMemory(m_LogicalDevice->getLogicalDevice(), m_DepthImageMemory, nullptr);
	
	vkDestroyBuffer(m_LogicalDevice->getLogicalDevice(), m_IndexBuffer, nullptr);
	vkFreeMemory(m_LogicalDevice->getLogicalDevice(), m_IndexBufferMemory, nullptr);
	vkDestroyBuffer(m_LogicalDevice->getLogicalDevice(), m_VertexBuffer, nullptr);
	vkFreeMemory(m_LogicalDevice->getLogicalDevice(), m_VertexBufferMemory, nullptr);

	for (size_t i = 0; i < m_SwapChain->getSwapChainImages().size(); i++) {
		vkDestroyBuffer(m_LogicalDevice->getLogicalDevice(), m_UniformBuffers[i], nullptr);
		vkFreeMemory(m_LogicalDevice->getLogicalDevice(), m_UniformBuffersMemory[i], nullptr);
	}
}

void Model::createIndexBuffer() {
	VkDeviceSize bufferSize = sizeof(m_Indices[0]) * m_Indices.size();
	VkBuffer stagingBuffer;
	VkDeviceMemory stagingBufferMemory;
	m_LogicalDevice->createBuffer(bufferSize, VK_BUFFER_USAGE_TRANSFER_SRC_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, stagingBuffer, stagingBufferMemory);

	void* data;
	vkMapMemory(m_LogicalDevice->getLogicalDevice(), stagingBufferMemory, 0, bufferSize, 0, &data);
	memcpy(data, m_Indices.data(), (size_t)bufferSize);
	vkUnmapMemory(m_LogicalDevice->getLogicalDevice(), stagingBufferMemory);

	m_LogicalDevice->createBuffer(bufferSize, VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_INDEX_BUFFER_BIT, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, m_IndexBuffer, m_IndexBufferMemory);

	m_CommandBus->copyBuffer(stagingBuffer, m_IndexBuffer, bufferSize);
	vkDestroyBuffer(m_LogicalDevice->getLogicalDevice(), stagingBuffer, nullptr);
	vkFreeMemory(m_LogicalDevice->getLogicalDevice(), stagingBufferMemory, nullptr);
}

void Model::createUniformBuffers() {
	VkDeviceSize bufferSize = sizeof(UniformBufferObject);
	m_UniformBuffers.resize(m_SwapChain->getSwapChainImages().size());
	m_UniformBuffersMemory.resize(m_SwapChain->getSwapChainImages().size());

	for (size_t i = 0; i < m_SwapChain->getSwapChainImages().size(); i++) {
		m_LogicalDevice->createBuffer(bufferSize, VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, m_UniformBuffers[i], m_UniformBuffersMemory[i]);
	}
}

void Model::createVertexBuffer() {
	VkDeviceSize bufferSize = sizeof(m_Vertices[0]) * m_Vertices.size();

	VkBuffer stagingBuffer;
	VkDeviceMemory stagingBufferMemory;
	m_LogicalDevice->createBuffer(bufferSize, VK_BUFFER_USAGE_TRANSFER_SRC_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, stagingBuffer, stagingBufferMemory);

	void* data;
	vkMapMemory(m_LogicalDevice->getLogicalDevice(), stagingBufferMemory, 0, bufferSize, 0, &data);
	memcpy(data, m_Vertices.data(), (size_t)bufferSize);
	vkUnmapMemory(m_LogicalDevice->getLogicalDevice(), stagingBufferMemory);

	m_LogicalDevice->createBuffer(bufferSize, VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_VERTEX_BUFFER_BIT, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, m_VertexBuffer, m_VertexBufferMemory);

	m_CommandBus->copyBuffer(stagingBuffer, m_VertexBuffer, bufferSize);

	vkDestroyBuffer(m_LogicalDevice->getLogicalDevice(), stagingBuffer, nullptr);
	vkFreeMemory(m_LogicalDevice->getLogicalDevice(), stagingBufferMemory, nullptr);
}

void Model::loadModel() {
	tinyobj::attrib_t attrib;
	std::vector<tinyobj::shape_t> shapes;
	std::vector<tinyobj::material_t> materials;
	std::string warn, err;

	if (!tinyobj::LoadObj(&attrib, &shapes, &materials, &warn, &err, MODEL_PATH.c_str())) {
		throw std::runtime_error(warn + err);
	}

	std::unordered_map<Vertex, uint32_t> uniqueVertices = {};

	for (const auto& shape : shapes) {
		for (const auto& index : shape.mesh.indices) {
			Vertex vertex = {};

			vertex.pos = {
				attrib.vertices[3 * index.vertex_index + 0],
				attrib.vertices[3 * index.vertex_index + 1],
				attrib.vertices[3 * index.vertex_index + 2]
			};
			vertex.texCoord = {
				attrib.texcoords[2 * index.texcoord_index + 0],
				attrib.texcoords[2 * index.texcoord_index + 1],
			};

			vertex.texCoord = {
				attrib.texcoords[2 * index.texcoord_index + 0],
				1.0f - attrib.texcoords[2 * index.texcoord_index + 1]
			};

			if (uniqueVertices.count(vertex) == 0) {
				uniqueVertices[vertex] = static_cast<uint32_t>(m_Vertices.size());
				m_Vertices.push_back(vertex);
			}

			m_Indices.push_back(uniqueVertices[vertex]);
		}
	}
}

void Model::createTextureImageView() {
	m_TextureImageView = createImageView(m_TextureImage, VK_FORMAT_R8G8B8A8_UNORM, VK_IMAGE_ASPECT_COLOR_BIT, mipLevels, m_LogicalDevice->getLogicalDevice());
}

void Model::createTextureImage() {
	int texWidth, texHeight, texChannels;
	stbi_uc* pixels = stbi_load(TEXTURE_PATH.c_str(), &texWidth, &texHeight, &texChannels, STBI_rgb_alpha);
	VkDeviceSize imageSize = texWidth * texHeight * 4;

	mipLevels = static_cast<uint32_t>(std::floor(std::log2(std::max(texWidth, texHeight)))) + 1;

	if (!pixels) {
		throw std::runtime_error("Failed to load texture image");
	}

	VkBuffer stagingBuffer;
	VkDeviceMemory stagingBufferMemory;
	m_LogicalDevice->createBuffer(imageSize, VK_BUFFER_USAGE_TRANSFER_SRC_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, stagingBuffer, stagingBufferMemory);

	void* data;
	vkMapMemory(m_LogicalDevice->getLogicalDevice(), stagingBufferMemory, 0, imageSize, 0, &data);
	memcpy(data, pixels, static_cast<size_t>(imageSize));
	vkUnmapMemory(m_LogicalDevice->getLogicalDevice(), stagingBufferMemory);
	stbi_image_free(pixels);

	createImage(m_PhysicalDevice->getPhysicalDevice(), m_LogicalDevice->getLogicalDevice(), texWidth, texHeight, mipLevels, VK_SAMPLE_COUNT_1_BIT, VK_FORMAT_R8G8B8A8_UNORM, VK_IMAGE_TILING_OPTIMAL, VK_IMAGE_USAGE_TRANSFER_SRC_BIT | VK_IMAGE_USAGE_TRANSFER_DST_BIT | VK_IMAGE_USAGE_SAMPLED_BIT, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, m_TextureImage, m_TextureImageMemory);

	m_CommandBus->transitionImageLayout(m_TextureImage, VK_FORMAT_R8G8B8A8_UNORM, VK_IMAGE_LAYOUT_UNDEFINED, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, mipLevels);
	m_CommandBus->copyBufferToImage(stagingBuffer, m_TextureImage, static_cast<uint32_t>(texWidth), static_cast<uint32_t>(texHeight));

	vkDestroyBuffer(m_LogicalDevice->getLogicalDevice(), stagingBuffer, nullptr);
	vkFreeMemory(m_LogicalDevice->getLogicalDevice(), stagingBufferMemory, nullptr);

	generateMipmaps(m_TextureImage, VK_FORMAT_R8G8B8A8_UNORM, texWidth, texHeight, mipLevels);
}

void Model::createDepthResources() {
	VkFormat depthFormat = findDepthFormat(m_PhysicalDevice->getPhysicalDevice());
	createImage(m_PhysicalDevice->getPhysicalDevice(), m_LogicalDevice->getLogicalDevice(), m_SwapChain->getSwapChainExtent().width, m_SwapChain->getSwapChainExtent().height, 1, m_PhysicalDevice->getMaxUsableSampleCount(), depthFormat, VK_IMAGE_TILING_OPTIMAL, VK_IMAGE_USAGE_DEPTH_STENCIL_ATTACHMENT_BIT, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, m_DepthImage, m_DepthImageMemory);
	m_DepthImageView = createImageView(m_DepthImage, depthFormat, VK_IMAGE_ASPECT_DEPTH_BIT, 1, m_LogicalDevice->getLogicalDevice());

	m_CommandBus->transitionImageLayout(m_DepthImage, depthFormat, VK_IMAGE_LAYOUT_UNDEFINED, VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL, 1);
}

void Model::createColorResources() {
	VkFormat colorFormat = m_SwapChain->getSwapChainImageFormat();
	createImage(m_PhysicalDevice->getPhysicalDevice(), m_LogicalDevice->getLogicalDevice(), m_SwapChain->getSwapChainExtent().width, m_SwapChain->getSwapChainExtent().height, 1, m_PhysicalDevice->getMaxUsableSampleCount(), colorFormat, VK_IMAGE_TILING_OPTIMAL, VK_IMAGE_USAGE_TRANSIENT_ATTACHMENT_BIT | VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, m_ColorImage, m_ColorImageMemory);
	m_ColorImageView = createImageView(m_ColorImage, colorFormat, VK_IMAGE_ASPECT_COLOR_BIT, 1, m_LogicalDevice->getLogicalDevice());

	m_CommandBus->transitionImageLayout(m_ColorImage, colorFormat, VK_IMAGE_LAYOUT_UNDEFINED, VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL, 1);
}

void Model::generateMipmaps(VkImage image, VkFormat imageFormat, int32_t texWidth, int32_t texHeight, uint32_t mipLevels) {

	VkFormatProperties formatProperties;
	vkGetPhysicalDeviceFormatProperties(m_PhysicalDevice->getPhysicalDevice(), imageFormat, &formatProperties);

	if (!(formatProperties.optimalTilingFeatures & VK_FORMAT_FEATURE_SAMPLED_IMAGE_FILTER_LINEAR_BIT)) {
		throw std::runtime_error("Texture image format does not support linear filtering");
	}

	VkCommandBuffer commandBuffer = m_CommandBus->beginSingleTimeCommands();

	VkImageMemoryBarrier barrier = {};
	barrier.sType = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER;
	barrier.image = image;
	barrier.srcQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
	barrier.dstQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
	barrier.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
	barrier.subresourceRange.baseArrayLayer = 0;
	barrier.subresourceRange.layerCount = 1;
	barrier.subresourceRange.levelCount = 1;

	int32_t mipWidth = texWidth;
	int32_t mipHeight = texHeight;

	for (uint32_t i = 1; i < mipLevels; i++) {
		barrier.subresourceRange.baseMipLevel = i - 1;
		barrier.oldLayout = VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL;
		barrier.newLayout = VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL;
		barrier.srcAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;
		barrier.dstAccessMask = VK_ACCESS_TRANSFER_READ_BIT;

		vkCmdPipelineBarrier(commandBuffer,
			VK_PIPELINE_STAGE_TRANSFER_BIT, VK_PIPELINE_STAGE_TRANSFER_BIT, 0,
			0, nullptr,
			0, nullptr,
			1, &barrier);

		VkImageBlit blit = {};
		blit.srcOffsets[0] = { 0, 0, 0 };
		blit.srcOffsets[1] = { mipWidth, mipHeight, 1 };
		blit.srcSubresource.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
		blit.srcSubresource.mipLevel = i - 1;
		blit.srcSubresource.baseArrayLayer = 0;
		blit.srcSubresource.layerCount = 1;
		blit.dstOffsets[0] = { 0, 0, 0 };
		blit.dstOffsets[1] = { mipWidth > 1 ? mipWidth / 2 : 1, mipHeight > 1 ? mipHeight / 2 : 1, 1 };
		blit.dstSubresource.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
		blit.dstSubresource.mipLevel = i;
		blit.dstSubresource.baseArrayLayer = 0;
		blit.dstSubresource.layerCount = 1;

		vkCmdBlitImage(commandBuffer,
			image, VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL,
			image, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL,
			1, &blit,
			VK_FILTER_LINEAR);

		barrier.oldLayout = VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL;
		barrier.newLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
		barrier.srcAccessMask = VK_ACCESS_TRANSFER_READ_BIT;
		barrier.dstAccessMask = VK_ACCESS_SHADER_READ_BIT;

		vkCmdPipelineBarrier(commandBuffer,
			VK_PIPELINE_STAGE_TRANSFER_BIT, VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT, 0,
			0, nullptr,
			0, nullptr,
			1, &barrier);

		if (mipWidth > 1) mipWidth /= 2;
		if (mipHeight > 1) mipHeight /= 2;
	}

	barrier.subresourceRange.baseMipLevel = mipLevels - 1;
	barrier.oldLayout = VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL;
	barrier.newLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
	barrier.srcAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;
	barrier.dstAccessMask = VK_ACCESS_SHADER_READ_BIT;

	vkCmdPipelineBarrier(commandBuffer,
		VK_PIPELINE_STAGE_TRANSFER_BIT, VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT, 0,
		0, nullptr,
		0, nullptr,
		1, &barrier);

	m_CommandBus->endSingleTimeCommands(commandBuffer);
}

void Model::createTextureSampler() {
	VkSamplerCreateInfo samplerInfo = {};
	samplerInfo.sType = VK_STRUCTURE_TYPE_SAMPLER_CREATE_INFO;
	samplerInfo.magFilter = VK_FILTER_LINEAR;
	samplerInfo.minFilter = VK_FILTER_LINEAR;
	samplerInfo.addressModeU = VK_SAMPLER_ADDRESS_MODE_REPEAT;
	samplerInfo.addressModeV = VK_SAMPLER_ADDRESS_MODE_REPEAT;
	samplerInfo.addressModeW = VK_SAMPLER_ADDRESS_MODE_REPEAT;
	samplerInfo.anisotropyEnable = VK_TRUE;
	samplerInfo.maxAnisotropy = 16;
	samplerInfo.borderColor = VK_BORDER_COLOR_INT_OPAQUE_BLACK;
	samplerInfo.unnormalizedCoordinates = VK_FALSE;
	samplerInfo.compareEnable = VK_FALSE;
	samplerInfo.compareOp = VK_COMPARE_OP_ALWAYS;
	samplerInfo.mipmapMode = VK_SAMPLER_MIPMAP_MODE_LINEAR;
	samplerInfo.minLod = 0;
	samplerInfo.maxLod = static_cast<float>(mipLevels);
	samplerInfo.mipLodBias = 0;

	if (vkCreateSampler(m_LogicalDevice->getLogicalDevice(), &samplerInfo, nullptr, &m_TextureSampler) != VK_SUCCESS) {
		throw std::runtime_error("failed to create texture sampler!");
	}
}

VkBuffer Model::getVertexBuffer() {
	return m_VertexBuffer;
}

VkBuffer Model::getIndexBuffer() {
	return m_IndexBuffer;
}

std::vector<uint32_t> Model::getIndices() {
	return m_Indices;
}

std::vector<VkBuffer> Model::getUniformBuffers()
{
	return m_UniformBuffers;
}

std::vector<VkDeviceMemory> Model::getUniformBuffersMemory()
{
	return m_UniformBuffersMemory;
}

int Model::getNumIndicies()
{
	return m_Indices.size();
}

VkImageView Model::getTextureImageView()
{
	return m_TextureImageView;
}

VkImageView Model::getColorImageView()
{
	return m_ColorImageView;
}

VkImageView Model::getDepthImageView()
{
	return m_DepthImageView;
}

VkSampler Model::getTextureSampler()
{
	return m_TextureSampler;
}