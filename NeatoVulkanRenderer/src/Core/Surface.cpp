#include "nvrpch.h"
#include "Surface.h"

Surface::Surface(const Instance* _parent, void * _window) :
	m_Instance(_parent), m_Window(reinterpret_cast<GLFWwindow*>(_window))
{
	createSurface();
}

Surface::Surface(Surface && other)
	: m_Instance(std::move(other.getInstance())), m_Surface(std::move(other.getSurface()))
{
}

Surface & Surface::operator=(Surface&& other)
{
	m_Instance = std::move(other.getInstance());
	m_Surface = std::move(other.getSurface());
	other.setSurface(VK_NULL_HANDLE);
	return *this;
}

void Surface::Cleanup() {
	vkDestroySurfaceKHR(m_Instance->getInstance(), m_Surface, nullptr);
}

Surface::~Surface()
{
	Cleanup();
}

void Surface::createSurface() {
	if (glfwCreateWindowSurface(m_Instance->getInstance(), m_Window, nullptr, &m_Surface) != VK_SUCCESS) {
		throw std::runtime_error("Failed to create window surface");
	}
}

void Surface::Recreate()
{
}

VkSurfaceKHR & Surface::getSurface()
{
	return m_Surface;
}

void Surface::setSurface(VkSurfaceKHR surface)
{
	m_Surface = surface;
}

const Instance* Surface::getInstance()
{
	return m_Instance;
}