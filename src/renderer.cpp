#include "pch.h"
#include "renderer.h"
#include "config.h"
#include "defines.h"

namespace Renderer {

	namespace {

		std::vector<const char*> getEnabledLayers() {
			uint32_t layerCount = 0;
			vkEnumerateInstanceLayerProperties(&layerCount, nullptr);
			std::vector<VkLayerProperties> availableLayers(layerCount);
			vkEnumerateInstanceLayerProperties(&layerCount, availableLayers.data());

			std::vector<const char*> enabledLayers;
			if (Config::ENABLE_VALIDATION_LAYERS) {
				for (const char* targetName : Config::REQUIRED_VK_LAYERS) {
					bool found = false;
					for (const auto& layerProperties : availableLayers) {
						if (strcmp(targetName, layerProperties.layerName) == 0) {
							found = true;
							break;
						}
					}
					if (found) {
						enabledLayers.push_back(targetName);
					}
				}
			}
			return enabledLayers;
		}

		void setupApplicationInfo(VkApplicationInfo& appInfo) {
			appInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
			appInfo.pApplicationName = "Raytracer";
			appInfo.applicationVersion = VK_MAKE_VERSION(1, 0, 0);
			appInfo.pEngineName = "No Engine";
			appInfo.engineVersion = VK_MAKE_VERSION(1, 0, 0);
			appInfo.apiVersion = VK_API_VERSION_1_3;
		}

		void setupInstanceCreateInfo(VkInstanceCreateInfo& info, VkApplicationInfo& appInfo) {
			std::vector<const char*> enabledLayers = getEnabledLayers();

			info.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
			info.enabledExtensionCount = static_cast<u32>(Config::REQUIRED_VK_EXTENSIONS.size());
			info.enabledLayerCount = static_cast<u32>(enabledLayers.size());
			info.ppEnabledExtensionNames = Config::REQUIRED_VK_EXTENSIONS.data();
			info.ppEnabledLayerNames = enabledLayers.data();
			info.pApplicationInfo = &appInfo;
		}
	}

	void createInstance(VulkanContext& ctx) {
		VkApplicationInfo appInfo{};
		setupApplicationInfo(appInfo);

		VkInstanceCreateInfo info{};
		setupInstanceCreateInfo(info, appInfo);

		if (vkCreateInstance(&info, nullptr, &ctx.instance) != VK_SUCCESS) {
			throw std::runtime_error("Failed to create Vulkan instance!");
		}
	}

	bool init(VulkanContext& ctx)
	{
		createInstance(ctx);

		return false;
	}

	void cleanup(VulkanContext& ctx) {
		vkDestroyInstance(ctx.instance, nullptr);
	}

}
