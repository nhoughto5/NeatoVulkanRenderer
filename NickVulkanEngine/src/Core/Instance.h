#pragma once

const std::vector<const char*> validationLayers = {
	"VK_LAYER_LUNARG_standard_validation"
};

class Instance
{
public:
	Instance();
	~Instance();
	const VkInstance &getInstance() const;
private:
	VkInstance m_Instance{ VK_NULL_HANDLE };
	std::vector<const char*> getRequiredExtensions();
	bool checkValidationLayerSupport();
};

