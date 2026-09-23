#pragma once 

#ifdef _WIN32 
#include <windows.h>
#include <windowsx.h>
#include <shobjidl.h>
#include <wrl/client.h>
#include <consoleapi.h>

#endif

#include <vulkan/vulkan.h>
#include <vulkan/vulkan_win32.h>
#include <vk_mem_alloc.h>

#include <array>
#include <vector>
#include <unordered_map>
#include <cstdint>
#include <cstddef>
#include <memory>
#include <algorithm>
#include <stdexcept>
#include <string>
#include <ranges>
#include <fstream>