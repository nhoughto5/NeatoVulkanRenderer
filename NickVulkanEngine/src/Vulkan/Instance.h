#pragma once

const std::vector<const char*> validationLayers = {
	"VK_LAYER_LUNARG_standard_validation"
};

class Instance
{
public:

	static VkInstance& getInstance();
private:
	static std::vector<const char*> getRequiredExtensions();
	static bool checkValidationLayerSupport();
};

