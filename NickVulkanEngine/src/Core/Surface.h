#pragma once
#include "Instance.h"
class Surface
{
public:
	Surface(const Instance* _parent, void* window);
	Surface(Surface&& other);
	Surface& operator=(Surface&& other);
	~Surface();

	void Recreate();
	VkSurfaceKHR& getSurface();
	void setSurface(VkSurfaceKHR surface);
	const Instance* getInstance();
private:
	GLFWwindow* m_Window{ nullptr };
	const Instance* m_Instance{nullptr};
	VkSurfaceKHR m_Surface{ VK_NULL_HANDLE };

	void createSurface();
};

