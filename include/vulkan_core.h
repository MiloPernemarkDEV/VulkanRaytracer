#pragma once

#include "defines.h"
#include "vulkan_commands.h"
#include "config.h"
#include <vk_mem_alloc.h>

struct ViewportTarget {
    VkImage         image      = VK_NULL_HANDLE;
    VkDeviceMemory  memory     = VK_NULL_HANDLE;
    VkImageView     view       = VK_NULL_HANDLE;
    VkSampler       sampler    = VK_NULL_HANDLE;
    VkDescriptorSet descSet    = VK_NULL_HANDLE;
    VkExtent2D      extent     = {.width = 1, .height = 1};
    VkFormat        format     = VK_FORMAT_R8G8B8A8_UNORM;
    u32             desiredW   = 1;
    u32             desiredH   = 1;
};

struct DynamicStates {
    VkViewport viewport{};
    VkRect2D   scissor{};
};

struct Pipelines {
    VkPipelineLayout layout    = VK_NULL_HANDLE;
    VkPipeline       filled    = VK_NULL_HANDLE;
    VkPipeline       wireframe = VK_NULL_HANDLE;
    VkPipeline       vertex    = VK_NULL_HANDLE;
};

struct FrameState {
    VkCommandPool   commandPool        = VK_NULL_HANDLE;
    VkCommandBuffer mainCommandBuffer  = VK_NULL_HANDLE;
    VkSemaphore     swapchainSemaphore = VK_NULL_HANDLE;
    VkSemaphore     renderSemaphore    = VK_NULL_HANDLE;
    VkFence         renderFence        = VK_NULL_HANDLE;
};

struct SwapchainState {
    VkSwapchainKHR           swapchain   = VK_NULL_HANDLE;
    std::vector<VkImage>     images;
    std::vector<VkImageView> views;
    VkExtent2D               extent{};
    VkFormat                 imageFormat = VK_FORMAT_UNDEFINED;
};

struct VulkanContext {
    VkInstance                                   instance            = VK_NULL_HANDLE;
    VkDebugUtilsMessengerEXT                     debugMessenger      = VK_NULL_HANDLE;
    VkPhysicalDevice                             physicalDevice      = VK_NULL_HANDLE;
    VkDevice                                     device              = VK_NULL_HANDLE;
    u32                                          queueFamilyIndex    = UINT32_MAX;
    VkQueue                                      graphicsQueue       = VK_NULL_HANDLE;
    VkSurfaceKHR                                 surface             = VK_NULL_HANDLE;
    std::array<FrameState, Config::frameOverlap> frameStates         {};
    Pipelines                                    pipelines           {};
    DynamicStates                                dynamicStates       {};
    ViewportTarget                               viewport            {};
    SwapchainState                               swapchainState      {};
    u32                                          swapchainImageIndex = UINT32_MAX ;
    u32                                          currentFrame        = 0;
    VmaAllocator                                 allocator           = nullptr;
};

namespace VulkanCore {
	void init(VulkanContext& ctx);
	void destroy(VulkanContext& ctx);
	void recreateSwapchain(VulkanContext& ctx);

	[[nodiscard]] FrameState& getCurrentFrame(VulkanContext& ctx);
    u32 findMemoryType(VkPhysicalDevice physicalDevice, u32 typeBits, VkMemoryPropertyFlags required);

    FORCE_INLINE constexpr VkClearValue vec4ToClearValue(const Math::vec4& color) {
        return VkClearValue{
            .color = {{ color.x, color.y, color.z, color.w }}
        };
    }
}