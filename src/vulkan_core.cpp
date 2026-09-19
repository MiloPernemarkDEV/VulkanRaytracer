#include "pch.h"
#include "vulkan_core.h"
#include "config.h"
#include "defines.h"
#include <iostream>

static VKAPI_ATTR VkBool32 VKAPI_CALL debugCallback(
			VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity,
			VkDebugUtilsMessageTypeFlagsEXT messageType,
			const VkDebugUtilsMessengerCallbackDataEXT* pCallbackData,
			void* pUserData) {

	if (messageSeverity >= VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT) {
		std::cerr << "[Vulkan Validation]: " << pCallbackData->pMessage << std::endl;
	}

	return VK_FALSE;
}

namespace VulkanCore {
	namespace 
	{
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

		void setupInstanceCreateInfo(VkInstanceCreateInfo& info, const VkApplicationInfo& appInfo, std::vector<const char*>& enabledLayers) {
			info.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
			info.enabledExtensionCount = static_cast<u32>(Config::REQUIRED_VK_EXTENSIONS.size());
			info.enabledLayerCount = static_cast<u32>(enabledLayers.size());
			info.ppEnabledExtensionNames = Config::REQUIRED_VK_EXTENSIONS.data();
			info.ppEnabledLayerNames = enabledLayers.data();
			info.pApplicationInfo = &appInfo;
		}

		void populateDebugMessengerCreateInfo(VkDebugUtilsMessengerCreateInfoEXT& info) {
			info = {};
			info.sType = VK_STRUCTURE_TYPE_DEBUG_UTILS_MESSENGER_CREATE_INFO_EXT;
			info.messageSeverity = VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT;
			info.messageType = VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT;
			info.pfnUserCallback = debugCallback;
		}

		void createValidationLayers(const VkInstance instance, VkDebugUtilsMessengerEXT& debugMessenger) {
			if (!Config::ENABLE_VALIDATION_LAYERS) return;

			VkDebugUtilsMessengerCreateInfoEXT info{};
			populateDebugMessengerCreateInfo(info);

			const auto func = reinterpret_cast<PFN_vkCreateDebugUtilsMessengerEXT>(vkGetInstanceProcAddr(instance, "vkCreateDebugUtilsMessengerEXT"));
			if (func != nullptr) {
				func(instance, &info, nullptr, &debugMessenger);
			}
		}

		void destroyValidationLayers(const VkInstance instance, const VkDebugUtilsMessengerEXT debugMessenger) {
			if (debugMessenger == VK_NULL_HANDLE) return;

			const auto func = reinterpret_cast<PFN_vkDestroyDebugUtilsMessengerEXT>(
				vkGetInstanceProcAddr(instance, "vkDestroyDebugUtilsMessengerEXT"));
			if (func != nullptr) {
				func(instance, debugMessenger, nullptr);
			}
		}

		void createInstance(VulkanContext& ctx) {
			VkApplicationInfo appInfo{};
			setupApplicationInfo(appInfo);

			VkInstanceCreateInfo info{};
			std::vector<const char*> enabledLayers = getEnabledLayers();
			setupInstanceCreateInfo(info, appInfo, enabledLayers);


			VkDebugUtilsMessengerCreateInfoEXT debugInfo{};
			if (Config::ENABLE_VALIDATION_LAYERS) {
				populateDebugMessengerCreateInfo(debugInfo);
				info.pNext = &debugInfo;
			}
			else {
				info.pNext = nullptr;
			}

			if (vkCreateInstance(&info, nullptr, &ctx.instance) != VK_SUCCESS) {
				throw std::runtime_error("Failed to create Vulkan instance!");
			}
		}

	} // namespace

	bool init(VulkanContext& ctx)
	{
		createInstance(ctx);
		createValidationLayers(ctx.instance, ctx.debugMessenger);

		return true;
	}

	void cleanup(VulkanContext& ctx) {

		destroyValidationLayers(ctx.instance, ctx.debugMessenger);
		vkDestroyInstance(ctx.instance, nullptr);
	}

}