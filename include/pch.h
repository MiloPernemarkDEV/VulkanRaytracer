#pragma once 

#ifdef _WIN32 
#include <windows.h>
#include <windowsx.h>
#include <shobjidl.h>
#include <wrl/client.h>
#endif

#include <vulkan/vulkan.h>
#include <vulkan/vulkan_win32.h>

#include <glm/gtx/quaternion.hpp>
#include <glm/mat4x4.hpp>
#include <glm/vec4.hpp>
#include <glm/vec3.hpp>
#include <glm/vec2.hpp>

#include <array>
#include <vector>
#include <unordered_map>
#include <cstdint>
#include <cstddef>
#include <memory>
#include <algorithm>
#include <stdexcept>
#include <string>