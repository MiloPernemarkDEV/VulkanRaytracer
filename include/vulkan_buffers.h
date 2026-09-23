#pragma once

struct VulkanContext;

namespace VulkanBuffers {

    struct AllocatedBuffer {
        VmaAllocationInfo allocationInfo    {};
        VmaAllocation     allocation        = nullptr;
        VkBuffer          buffer            = VK_NULL_HANDLE;
    };

    void createVertexBuffer(VulkanContext& ctx);
}