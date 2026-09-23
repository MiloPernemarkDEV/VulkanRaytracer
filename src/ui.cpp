#include "pch.h"
#include "ui.h"

#include <imgui.h>
#include <imgui_impl_vulkan.h>
#include <imgui_impl_win32.h>
#include <imgui_internal.h>

#include "config.h"
#include "vulkan_core.h"
#include "IconsFontAwesome6.h"
#include "viewport.h"
#include "vec4.h"

namespace UI {
    namespace {
        VkDescriptorPool uiDescPool = VK_NULL_HANDLE;

        void createDescriptorPool(VulkanContext& ctx) {
            constexpr VkDescriptorPoolSize poolSizes[] = {
                { .type = VK_DESCRIPTOR_TYPE_SAMPLED_IMAGE, .descriptorCount = 1000 },
                { .type = VK_DESCRIPTOR_TYPE_SAMPLER, .descriptorCount = 16 },
                { .type = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, .descriptorCount = 1000 },
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

        void setupDocking() {
            ImGuiID dockspaceId = ImGui::GetID("Dockspace");
            ImGuiViewport* viewport = ImGui::GetMainViewport();

            if (ImGui::DockBuilderGetNode(dockspaceId) == nullptr) {
                ImGui::DockBuilderAddNode(dockspaceId, ImGuiDockNodeFlags_DockSpace);
                ImGuiID dockIdLeft = 0;
                ImGuiID dockIdMain = dockspaceId;
                ImGui::DockBuilderSplitNode(dockIdMain, ImGuiDir_Left, 0.20f, &dockIdLeft, &dockIdMain);
                ImGuiID dockIdLeftTop = 0;
                ImGuiID dockIDLeftBottom = 0;
                ImGui::DockBuilderSplitNode(dockIdLeft, ImGuiDir_Up, 0.50f, &dockIdLeftTop, &dockIDLeftBottom);
                ImGui::DockBuilderDockWindow("Settings", dockIdMain);
                ImGui::DockBuilderDockWindow("Properties", dockIdLeftTop);
                ImGui::DockBuilderDockWindow("Viewport", dockIDLeftBottom);
                ImGui::DockBuilderFinish(dockspaceId);
            }

            ImGui::DockSpaceOverViewport(dockspaceId, viewport, ImGuiDockNodeFlags_PassthruCentralNode);
        }

        void submitWindows(ViewportTarget& vp) {
            if (ImGui::Begin("Settings")) {

            }
            ImGui::End();

            ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
            const ImGuiWindowFlags viewportFlags =
                ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoScrollWithMouse;
            if (ImGui::Begin("Viewport", nullptr, viewportFlags)) {
                const ImVec2 size = ImGui::GetContentRegionAvail();
                vp.desiredW = static_cast<u32>(std::max(1.0f, size.x));
                vp.desiredH = static_cast<u32>(std::max(1.0f, size.y));

                if (vp.descSet != VK_NULL_HANDLE) {
                    const ImTextureRef tex(static_cast<ImTextureID>(reinterpret_cast<intptr_t>(vp.descSet)));
                    const ImVec2 imageSize{
                        static_cast<float>(vp.extent.width),
                        static_cast<float>(vp.extent.height)
                    };
                    ImGui::Image(tex, imageSize);
                }
            }
            ImGui::End();
            ImGui::PopStyleVar();
        }

        void setupConfig() {
            ImGuiIO& io = ImGui::GetIO();
            io.ConfigFlags = ImGuiConfigFlags_DockingEnable;
            io.ConfigWindowsMoveFromTitleBarOnly = true;
            io.ConfigWindowsCopyContentsWithCtrlC = true;
            io.Fonts->AddFontDefault();

            ImFontConfig icons_config;
            icons_config.MergeMode = true;
            icons_config.PixelSnapH = true;

            static constexpr ImWchar icons_ranges[] = {
                ICON_MIN_FA,
                ICON_MAX_FA,
                0
            };

            ImFont* font = io.Fonts->AddFontFromFileTTF(
                Config::uiMainFontPath,
                16.0f
            );

            io.Fonts->AddFontFromFileTTF(
                Config::uiMainFontPath,
                0.0f,
                &icons_config,
                icons_ranges
            );

            io.FontDefault = font;
        }

        void setupStyle() {
            ImGuiStyle& style = ImGui::GetStyle();

            style.Alpha = 1.0f;
            style.WindowRounding = 5.0f;
            style.ChildRounding = 4.0f;
            style.FrameRounding = 3.0f;
            style.PopupRounding = 4.0f;
            style.ScrollbarRounding = 6.0f;
            style.GrabRounding = 3.0f;

            style.Colors[ImGuiCol_Text]                  = ImVec4(0.86f, 0.86f, 0.90f, 1.00f);
            style.Colors[ImGuiCol_TextDisabled]          = ImVec4(0.50f, 0.50f, 0.55f, 1.00f);
            style.Colors[ImGuiCol_WindowBg]              = Config::windowBackgroundColorImGui();
            style.Colors[ImGuiCol_ChildBg]               = ImVec4(0.10f, 0.10f, 0.12f, 1.00f);
            style.Colors[ImGuiCol_PopupBg]               = ImVec4(0.14f, 0.14f, 0.17f, 0.98f);
            style.Colors[ImGuiCol_Border]                = ImVec4(0.25f, 0.24f, 0.29f, 0.80f);
            style.Colors[ImGuiCol_BorderShadow]          = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);
            style.Colors[ImGuiCol_FrameBg]               = ImVec4(0.17f, 0.17f, 0.20f, 1.00f);
            style.Colors[ImGuiCol_FrameBgHovered]        = ImVec4(0.22f, 0.20f, 0.27f, 1.00f);
            style.Colors[ImGuiCol_FrameBgActive]         = ImVec4(0.25f, 0.22f, 0.31f, 1.00f);
            style.Colors[ImGuiCol_TitleBg]               = ImVec4(0.10f, 0.10f, 0.12f, 1.00f);
            style.Colors[ImGuiCol_TitleBgCollapsed]      = ImVec4(0.10f, 0.10f, 0.12f, 1.00f);
            style.Colors[ImGuiCol_TitleBgActive]         = ImVec4(0.14f, 0.13f, 0.17f, 1.00f);
            style.Colors[ImGuiCol_MenuBarBg]             = ImVec4(0.10f, 0.10f, 0.12f, 1.00f);
            style.Colors[ImGuiCol_ScrollbarBg]           = ImVec4(0.09f, 0.09f, 0.11f, 1.00f);
            style.Colors[ImGuiCol_ScrollbarGrab]         = ImVec4(0.28f, 0.27f, 0.32f, 1.00f);
            style.Colors[ImGuiCol_ScrollbarGrabHovered]  = ImVec4(0.36f, 0.34f, 0.42f, 1.00f);
            style.Colors[ImGuiCol_ScrollbarGrabActive]   = ImVec4(0.45f, 0.39f, 0.55f, 1.00f);
            style.Colors[ImGuiCol_CheckMark]             = ImVec4(0.67f, 0.49f, 0.95f, 1.00f);
            style.Colors[ImGuiCol_SliderGrab]            = ImVec4(0.55f, 0.39f, 0.80f, 1.00f);
            style.Colors[ImGuiCol_SliderGrabActive]      = ImVec4(0.67f, 0.49f, 0.95f, 1.00f);
            style.Colors[ImGuiCol_Button]                = ImVec4(0.25f, 0.21f, 0.31f, 1.00f);
            style.Colors[ImGuiCol_ButtonHovered]         = ImVec4(0.38f, 0.29f, 0.50f, 1.00f);
            style.Colors[ImGuiCol_ButtonActive]          = ImVec4(0.46f, 0.34f, 0.60f, 1.00f);
            style.Colors[ImGuiCol_Header]                = ImVec4(0.27f, 0.22f, 0.35f, 1.00f);
            style.Colors[ImGuiCol_HeaderHovered]         = ImVec4(0.38f, 0.29f, 0.50f, 1.00f);
            style.Colors[ImGuiCol_HeaderActive]          = ImVec4(0.46f, 0.34f, 0.60f, 1.00f);
            style.Colors[ImGuiCol_Separator]             = ImVec4(0.25f, 0.24f, 0.29f, 1.00f);
            style.Colors[ImGuiCol_SeparatorHovered]      = ImVec4(0.55f, 0.39f, 0.80f, 0.70f);
            style.Colors[ImGuiCol_SeparatorActive]       = ImVec4(0.67f, 0.49f, 0.95f, 1.00f);
            style.Colors[ImGuiCol_TableBorderStrong]     = ImVec4(0.28f, 0.27f, 0.33f, 1.00f);
            style.Colors[ImGuiCol_TableBorderLight]      = ImVec4(0.20f, 0.20f, 0.24f, 1.00f);
            style.Colors[ImGuiCol_TableHeaderBg]         = ImVec4(0.18f, 0.17f, 0.22f, 1.00f);
            style.Colors[ImGuiCol_ResizeGrip]            = ImVec4(0.45f, 0.35f, 0.55f, 0.30f);
            style.Colors[ImGuiCol_ResizeGripHovered]     = ImVec4(0.55f, 0.39f, 0.80f, 0.70f);
            style.Colors[ImGuiCol_ResizeGripActive]      = ImVec4(0.67f, 0.49f, 0.95f, 1.00f);
            style.Colors[ImGuiCol_PlotLines]             = ImVec4(0.55f, 0.50f, 0.62f, 1.00f);
            style.Colors[ImGuiCol_PlotLinesHovered]      = ImVec4(0.67f, 0.49f, 0.95f, 1.00f);
            style.Colors[ImGuiCol_PlotHistogram]         = ImVec4(0.55f, 0.39f, 0.80f, 1.00f);
            style.Colors[ImGuiCol_PlotHistogramHovered]  = ImVec4(0.67f, 0.49f, 0.95f, 1.00f);
            style.Colors[ImGuiCol_TextSelectedBg]        = ImVec4(0.42f, 0.30f, 0.60f, 0.45f);
            style.Colors[ImGuiCol_ModalWindowDimBg]      = ImVec4(0.00f, 0.00f, 0.00f, 0.55f);
        }
    } // Unnamed namespace

#ifdef _WIN32

    void init(HWND hwnd, VulkanContext& ctx) {
        createDescriptorPool(ctx);
        IMGUI_CHECKVERSION();
        ImGui::CreateContext();
        setupConfig();
        setupStyle();

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

    void draw(ViewportTarget &target) {
        setupDocking();
        submitWindows(target);
    }

    void end(VkCommandBuffer commandBuffer) {
        ImGui::Render();

        ImGui_ImplVulkan_RenderDrawData(
            ImGui::GetDrawData(),
            commandBuffer
        );
    }

    void destroy(VulkanContext& ctx) {
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