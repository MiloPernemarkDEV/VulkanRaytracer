#pragma once

#include <vector>
#include <string>
#include <vulkan/vulkan.h>


namespace Shader {
    std::vector<char> readFile(const std::string& filename);
    VkShaderModule createModule(VkDevice device, const std::vector<char>& code);
}