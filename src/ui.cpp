#include "pch.h"
#include "ui.h"

#include <imgui.h>
#include <imgui_impl_vulkan.h>
#include <imgui_impl_win32.h>

#include "config.h"
#include "vulkan_core.h"

namespace UI {
    namespace {
        VkDescriptorPool uiDescPool = VK_NULL_HANDLE;

        void createDescriptorPool(VulkanContext& ctx) {
            VkDescriptorPoolSize poolSizes[] = {
                { VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, 1000 }
            };

            VkDescriptorPoolCreateInfo info{};
            info.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO;
            info.flags = VK_DESCRIPTOR_POOL_CREATE_FREE_DESCRIPTOR_SET_BIT;
            info.maxSets = 1000;
            info.poolSizeCount = 1;
            info.pPoolSizes = poolSizes;

            VK_CHECK(
                vkCreateDescriptorPool(ctx.device, &info, nullptr, &uiDescPool),
                "Failed to create UI descriptor pool"
            );
        }
    }

#ifdef _WIN32

    void init(HWND hwnd, VulkanContext& ctx) {
        createDescriptorPool(ctx);

        IMGUI_CHECKVERSION();
        ImGui::CreateContext();

        ImGuiIO& io = ImGui::GetIO();
        io.ConfigFlags = ImGuiConfigFlags_DockingEnable;

        // ImGui only creates its main pipeline when this is filled.
        // With UseDynamicRendering and a zeroed PipelineRenderingCreateInfo,
        // Init skips pipeline creation and RenderDrawData binds VK_NULL_HANDLE.
        const VkFormat colorFormat = ctx.swapchainState.imageFormat;
        VkPipelineRenderingCreateInfo renderingInfo{};
        renderingInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_RENDERING_CREATE_INFO;
        renderingInfo.colorAttachmentCount = 1;
        renderingInfo.pColorAttachmentFormats = &colorFormat;

        ImGui_ImplVulkan_InitInfo initInfo{};
        initInfo.ApiVersion = Config::requiredVulkanVersion;
        initInfo.Instance = ctx.instance;
        initInfo.PhysicalDevice = ctx.physicalDevice;
        initInfo.Device = ctx.device;
        initInfo.QueueFamily = ctx.queueFamilyIndex;
        initInfo.Queue = ctx.graphicsQueue;
        initInfo.DescriptorPool = uiDescPool;
        initInfo.MinImageCount = 2;
        initInfo.ImageCount = static_cast<uint32_t>(ctx.swapchainState.images.size());
        initInfo.UseDynamicRendering = true;
        initInfo.PipelineInfoMain.PipelineRenderingCreateInfo = renderingInfo;

        if (!ImGui_ImplVulkan_Init(&initInfo)) {
            throw std::runtime_error("Failed to initialize ImGui Vulkan backend");
        }
        ImGui_ImplWin32_Init(hwnd);

    }

    void begin() {
        ImGui_ImplVulkan_NewFrame();
        ImGui_ImplWin32_NewFrame();
        ImGui::NewFrame();
    }

    void draw() {
        if (ImGui::Begin("Raytracer")) {
            ImGui::Text("Hello from ImGui!");
        }

        ImGui::End();
    }

    void end(VkCommandBuffer commandBuffer) {
        ImGui::Render();

        ImGui_ImplVulkan_RenderDrawData(
            ImGui::GetDrawData(),
            commandBuffer
        );
    }

    void shutdown(VulkanContext& ctx) {
        ImGui_ImplVulkan_Shutdown();
        ImGui_ImplWin32_Shutdown();
        ImGui::DestroyContext();

        vkDestroyDescriptorPool(ctx.device, uiDescPool, nullptr);
        uiDescPool = VK_NULL_HANDLE;
    }

#else

    void init(HWND, VulkanContext&) {}
    void begin() {}
    void draw() {}
    void end(VkCommandBuffer) {}
    void shutdown(VulkanContext&) {}

#endif
}