#include "nvrpch.h"
#include "Renderer.h"

void Renderer::run()
{
	initWindow();
	initVulkan();
	mainLoop();
	cleanup();
}

Renderer::~Renderer()
{
	delete instance;
	delete surface;
	delete renderPass;
	delete logicalDevice;
	delete physicalDevice;
	delete swapChain;
	delete renderPass;
	delete graphicsPipeline;
	delete commandBus;
	delete houseModel;
}

void Renderer::initWindow() {
	glfwInit();
	glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
	glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);
	window = glfwCreateWindow(WIDTH, HEIGHT, "Vulkan", nullptr, nullptr);
	glfwSetWindowUserPointer(window, this);
	glfwSetWindowSizeCallback(window, Renderer::onWindowResize);
	debugger = new DebugLayer();
}

void Renderer::onWindowResize(GLFWwindow* window, int width, int height) {
	Renderer* app = reinterpret_cast<Renderer*>(glfwGetWindowUserPointer(window));
	app->recreateSwapChain();
}

void Renderer::initVulkan() {
	instance = new Instance();
	debugger->setupDebugCallback(instance->getInstance());

	surface = new Surface(instance, window);
	surface = new Surface(instance, window);
	physicalDevice = new PhysicalDevice(instance, surface);
	logicalDevice = new LogicalDevice(physicalDevice);
	swapChain = new SwapChain(window, logicalDevice, physicalDevice, surface);
	renderPass = new RenderPass(physicalDevice, logicalDevice, swapChain);
	graphicsPipeline = new GraphicsPipeline(physicalDevice, logicalDevice, swapChain, renderPass);
	commandBus = new CommandBus(physicalDevice, logicalDevice, swapChain, renderPass, graphicsPipeline);
	houseModel = new Model(physicalDevice, logicalDevice, swapChain, commandBus);

	houseModel->createDepthResources();
	houseModel->createColorResources();

	swapChain->createFrameBuffers(renderPass->getRenderPass(), houseModel->getColorImageView(), houseModel->getDepthImageView());
	houseModel->createTextureImage();
	houseModel->createTextureImageView();
	houseModel->createTextureSampler();
	houseModel->loadModel();
	houseModel->createVertexBuffer();
	houseModel->createIndexBuffer();
	houseModel->createUniformBuffers();
	descriptorPool = graphicsPipeline->createDescriptorPool();
	createDescriptorSets();
	commandBus->createCommandBuffers(
		houseModel->getVertexBuffer(),
		houseModel->getIndexBuffer(),
		houseModel->getNumIndicies(),
		descriptorSets
	);
	createSemaphores();
}

void Renderer::createDescriptorSets() {
	std::vector<VkDescriptorSetLayout> layouts(swapChain->getSwapChainImages().size(), graphicsPipeline->getDescriptorSetLayout());

	VkDescriptorSetAllocateInfo allocInfo = {};
	allocInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO;
	allocInfo.descriptorPool = descriptorPool;
	allocInfo.descriptorSetCount = static_cast<uint32_t>(swapChain->getSwapChainImages().size());
	allocInfo.pSetLayouts = layouts.data();

	descriptorSets.resize(swapChain->getSwapChainImages().size());
	if (vkAllocateDescriptorSets(logicalDevice->getLogicalDevice(), &allocInfo, descriptorSets.data()) != VK_SUCCESS) {
		throw std::runtime_error("Failed to allocate descriptor sets");
	}

	auto uniformBuffers = houseModel->getUniformBuffers();

	for (size_t i = 0; i < swapChain->getSwapChainImages().size(); i++) {
		VkDescriptorBufferInfo bufferInfo = {};
		bufferInfo.buffer = uniformBuffers[i];
		bufferInfo.offset = 0;
		bufferInfo.range = sizeof(UniformBufferObject);

		VkDescriptorImageInfo imageInfo = {};
		imageInfo.imageLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
		imageInfo.imageView = houseModel->getTextureImageView();
		imageInfo.sampler = houseModel->getTextureSampler();

		std::array<VkWriteDescriptorSet, 2> descriptorWrites = {};
		descriptorWrites[0].sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
		descriptorWrites[0].dstSet = descriptorSets[i];
		descriptorWrites[0].dstBinding = 0;
		descriptorWrites[0].dstArrayElement = 0;
		descriptorWrites[0].descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
		descriptorWrites[0].descriptorCount = 1;
		descriptorWrites[0].pBufferInfo = &bufferInfo;

		descriptorWrites[1].sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
		descriptorWrites[1].dstSet = descriptorSets[i];
		descriptorWrites[1].dstBinding = 1;
		descriptorWrites[1].dstArrayElement = 0;
		descriptorWrites[1].descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
		descriptorWrites[1].descriptorCount = 1;
		descriptorWrites[1].pImageInfo = &imageInfo;

		vkUpdateDescriptorSets(logicalDevice->getLogicalDevice(), static_cast<uint32_t>(descriptorWrites.size()), descriptorWrites.data(), 0, nullptr);
	}
}

void Renderer::createSemaphores() {
	VkSemaphoreCreateInfo semaphoreInfo = {};
	semaphoreInfo.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO;

	if (vkCreateSemaphore(logicalDevice->getLogicalDevice(), &semaphoreInfo, nullptr, &imageAvailableSemaphore) != VK_SUCCESS ||
		vkCreateSemaphore(logicalDevice->getLogicalDevice(), &semaphoreInfo, nullptr, &renderFinishedSemaphore) != VK_SUCCESS) {
		throw std::runtime_error("Failed to create semaphore");
	}
}


void Renderer::recreateSwapChain() {
	int width, height;
	glfwGetWindowSize(window, &width, &height);
	if (width == 0 || height == 0) return;

	vkDeviceWaitIdle(logicalDevice->getLogicalDevice());
	cleanupSwapChain();
	
	swapChain = new SwapChain(window, logicalDevice, physicalDevice, surface);
	renderPass = new RenderPass(physicalDevice, logicalDevice, swapChain);
	graphicsPipeline = new GraphicsPipeline(physicalDevice, logicalDevice, swapChain, renderPass);

	houseModel->createColorResources();
	houseModel->createDepthResources();
	swapChain->createFrameBuffers(renderPass->getRenderPass(), houseModel->getColorImageView(), houseModel->getDepthImageView());
	commandBus->createCommandBuffers(
		houseModel->getVertexBuffer(),
		houseModel->getIndexBuffer(),
		houseModel->getNumIndicies(),
		descriptorSets
	);
}

void Renderer::drawFrame() {
	uint32_t imageIndex;
	VkResult result = vkAcquireNextImageKHR(logicalDevice->getLogicalDevice(), swapChain->getSwapChain(), std::numeric_limits<uint64_t>::max(), imageAvailableSemaphore, VK_NULL_HANDLE, &imageIndex);
	if (result == VK_ERROR_OUT_OF_DATE_KHR) {
		recreateSwapChain();
	} 
	else if (result != VK_SUCCESS && result != VK_SUBOPTIMAL_KHR) {
		std::runtime_error("Failed to acquire swap chain image");
	}

	update(imageIndex);

	VkSubmitInfo submitInfo = {};
	submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;

	VkSemaphore waitSemaphores[] = { imageAvailableSemaphore };
	VkPipelineStageFlags waitStages[] = { VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT };
	submitInfo.waitSemaphoreCount = 1;
	submitInfo.pWaitSemaphores = waitSemaphores;
	submitInfo.pWaitDstStageMask = waitStages;
	submitInfo.commandBufferCount = 1;
	submitInfo.pCommandBuffers = commandBus->getCommandBufferByIndex(imageIndex);

	// Which semaphore to signal once the command buffer has finished execution
	VkSemaphore signalSemaphores[] = { renderFinishedSemaphore };
	submitInfo.signalSemaphoreCount = 1;
	submitInfo.pSignalSemaphores = signalSemaphores;

	if (vkQueueSubmit(logicalDevice->getGraphicsQueue(), 1, &submitInfo, VK_NULL_HANDLE) != VK_SUCCESS) {
		throw std::runtime_error("Faile to submit draw command buffer!");
	}

	VkPresentInfoKHR presentInfo = {};
	presentInfo.sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR;
	presentInfo.waitSemaphoreCount = 1;
	presentInfo.pWaitSemaphores = signalSemaphores;

	VkSwapchainKHR swapChains[] = { swapChain->getSwapChain() };
	presentInfo.swapchainCount = 1;
	presentInfo.pSwapchains = swapChains;
	presentInfo.pImageIndices = &imageIndex;
	presentInfo.pResults = nullptr;

	result = vkQueuePresentKHR(logicalDevice->getPresentQueue(), &presentInfo);

	if (result == VK_ERROR_OUT_OF_DATE_KHR || result == VK_SUBOPTIMAL_KHR) {
		recreateSwapChain();
	}
	else if (result != VK_SUCCESS) {
		throw std::runtime_error("failed to present swap chain image!");
	}
	//Sync with GPU to prevent memory leak in validation layers
	vkQueueWaitIdle(logicalDevice->getPresentQueue());
}

void Renderer::mainLoop() {
	while (!glfwWindowShouldClose(window)) {
		glfwPollEvents();
		drawFrame();
	}
	vkDeviceWaitIdle(logicalDevice->getLogicalDevice());
}

void Renderer::cleanupSwapChain() {

	for (size_t i = 0; i < swapChain->getSwapChainFrameBuffers().size(); i++) {
		vkDestroyFramebuffer(logicalDevice->getLogicalDevice(), swapChain->getFrameBufferByIndex(i), nullptr);
	}

	vkFreeCommandBuffers(logicalDevice->getLogicalDevice(), commandBus->getCommandPool(), static_cast<uint32_t>(commandBus->commandBuffersCount()), commandBus->getCommandBuffers().data());

	graphicsPipeline->Cleanup();
	swapChain->Cleanup();
}

void Renderer::cleanup() {
	cleanupSwapChain();

	houseModel->Cleanup();
	vkDestroyDescriptorSetLayout(logicalDevice->getLogicalDevice(), graphicsPipeline->getDescriptorSetLayout(), nullptr);
	
	auto uniformBuffers = houseModel->getUniformBuffers();
	auto uniformBuffersMemory = houseModel->getUniformBuffersMemory();

	for (size_t i = 0; i < swapChain->getSwapChainImages().size(); i++) {
		vkDestroyBuffer(logicalDevice->getLogicalDevice(), uniformBuffers[i], nullptr);
		vkFreeMemory(logicalDevice->getLogicalDevice(), uniformBuffersMemory[i], nullptr);
	}

	vkDestroyDescriptorSetLayout(logicalDevice->getLogicalDevice(), graphicsPipeline->getDescriptorSetLayout(), nullptr);
	vkDestroySemaphore(logicalDevice->getLogicalDevice(), renderFinishedSemaphore, nullptr);
	vkDestroySemaphore(logicalDevice->getLogicalDevice(), imageAvailableSemaphore, nullptr);

	commandBus->Cleanup();

	logicalDevice->Cleanup();
	debugger->DestroyDebugReportCallbackEXT(instance->getInstance(), nullptr);

	surface->Cleanup();
	instance->Cleanup();

	glfwDestroyWindow(window);

	glfwTerminate();
}

void Renderer::update(uint32_t currentImage) {
	static auto startTime = std::chrono::high_resolution_clock::now();
	auto currentTime = std::chrono::high_resolution_clock::now();
	float time = std::chrono::duration<float, std::chrono::seconds::period>(currentTime - startTime).count();

	UniformBufferObject ubo = {};
	ubo.model = glm::rotate(glm::mat4(1.0f), time * glm::radians(45.0f), glm::vec3(0.0f, 0.0f, 1.0f));
	ubo.view = glm::lookAt(glm::vec3(2.0f, 2.0f, 2.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, 1.0f));
	ubo.proj = glm::perspective(glm::radians(45.0f), swapChain->getSwapChainExtent().width / (float)swapChain->getSwapChainExtent().height, 0.1f, 10.0f);
	ubo.proj[1][1] *= -1;

	void* data;
	auto uniformBuffersMemory = houseModel->getUniformBuffersMemory();
	vkMapMemory(logicalDevice->getLogicalDevice(), uniformBuffersMemory[currentImage], 0, sizeof(ubo), 0, &data);
	memcpy(data, &ubo, sizeof(ubo));
	vkUnmapMemory(logicalDevice->getLogicalDevice(), uniformBuffersMemory[currentImage]);
}