#pragma once

#define GLFW_INCLUDE_VULKAN
#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEFAULT_ALIGNED_GENTYPES
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#define STB_IMAGE_IMPLEMENTATION
#define TINYOBJLOADER_IMPLEMENTATION
#define GLM_ENABLE_EXPERIMENTAL

#include <glfw3.h>

#include <iostream>
#include <stdexcept>
#include <unordered_map>
#include <functional>
#include <cstdlib>
#include <vector>
#include <algorithm>
#include <cstring>
#include <set>
#include <array>
#include "Helpers.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/hash.hpp>
#include "Vertex.h"
#include <chrono>
