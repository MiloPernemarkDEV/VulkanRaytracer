#include "pch.h"
#include "vulkan_core.h"
#include "config.h"
#include "defines.h"
#include <iostream>
#include <ranges>

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
				for (const char* targetLayer : Config::REQUIRED_VK_LAYERS) {
					bool found = false;
					for (const auto& layerProperties : availableLayers) {
						if (strcmp(targetLayer, layerProperties.layerName) == 0) {
							found = true;
							break;
						}
					}
					if (found) {
						enabledLayers.push_back(targetLayer);
					}
				}
			}
			return enabledLayers;
		}

		void setupApplicationInfo(VkApplicationInfo& appInfo) {
			appInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
			appInfo.pApplicationName = "Raytracer";
			appInfo.applicationVersion = Config::APPLICATION_VERSION;
			appInfo.pEngineName = "No Engine";
			appInfo.engineVersion = Config::APPLICATION_VERSION;
			appInfo.apiVersion = Config::REQUIRED_VULKAN_VERSION;
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

		bool checkHasFeatures(VkPhysicalDevice physicalDevice) {
				VkPhysicalDeviceBufferDeviceAddressFeatures bufferAddressFeatures{};
				bufferAddressFeatures.sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_BUFFER_DEVICE_ADDRESS_FEATURES;

				VkPhysicalDeviceAccelerationStructureFeaturesKHR accelFeatures{};
				accelFeatures.sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_ACCELERATION_STRUCTURE_FEATURES_KHR;
				accelFeatures.pNext = &bufferAddressFeatures;

				VkPhysicalDeviceRayTracingPipelineFeaturesKHR rtPipelineFeatures{};
				rtPipelineFeatures.sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_RAY_TRACING_PIPELINE_FEATURES_KHR;
				rtPipelineFeatures.pNext = &accelFeatures;

				VkPhysicalDeviceFeatures2 deviceFeatures2{};
				deviceFeatures2.sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_FEATURES_2;
				deviceFeatures2.pNext = &rtPipelineFeatures;

				vkGetPhysicalDeviceFeatures2(physicalDevice, &deviceFeatures2);

				return (bufferAddressFeatures.bufferDeviceAddress == VK_TRUE) &&
					    (accelFeatures.accelerationStructure == VK_TRUE) &&
						(rtPipelineFeatures.rayTracingPipeline == VK_TRUE);
		}

		bool isDeviceSuitable(VkPhysicalDevice physicalDevice) {
			VkPhysicalDeviceProperties2 properties{VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_PROPERTIES_2};
			vkGetPhysicalDeviceProperties2(physicalDevice, &properties);

			const bool hasDiscrete = properties.properties.deviceType == VK_PHYSICAL_DEVICE_TYPE_DISCRETE_GPU;
			const bool hasVersion = properties.properties.apiVersion >= Config::REQUIRED_VULKAN_VERSION;
			bool hasFeatures = checkHasFeatures(physicalDevice);

			u32 count{};
			vkGetPhysicalDeviceQueueFamilyProperties2(physicalDevice, &count, nullptr);

			std::vector<VkQueueFamilyProperties2> queueFamilies(count, {VK_STRUCTURE_TYPE_QUEUE_FAMILY_PROPERTIES_2});
			vkGetPhysicalDeviceQueueFamilyProperties2(physicalDevice, &count, queueFamilies.data());

			bool hasQueues{false};
			for (size_t i = 0; i < queueFamilies.size(); i++) {
				const VkQueueFlags flags = queueFamilies[i].queueFamilyProperties.queueFlags;

				// graphics queue ensures we have transfer queue too
				constexpr VkQueueFlags required = VK_QUEUE_GRAPHICS_BIT | VK_QUEUE_COMPUTE_BIT;
				if ((flags & required) == required) {
					hasQueues = true;
					break; // Found it, we can stop looking
				}
			}
			return hasDiscrete && hasVersion && hasQueues && hasFeatures;
		}


		void pickPhysicalDevice(VulkanContext& ctx) {
			u32 count{};
			vkEnumeratePhysicalDevices(ctx.instance, &count, nullptr);

			if (count == 0) {
				throw std::runtime_error("Failed to find any gpu at all!");
			}

			std::vector<VkPhysicalDevice> physicalDevices(count);
			vkEnumeratePhysicalDevices(ctx.instance, &count, physicalDevices.data());

			for (auto device : physicalDevices) {
				if (isDeviceSuitable(device)) {
					ctx.physicalDevice = device;
					return;
				}
			}

			throw std::runtime_error("Failed to find a suitable gpu for the application!");
		}


	} // namespace

	bool init(VulkanContext& ctx)
	{
		createInstance(ctx);
		createValidationLayers(ctx.instance, ctx.debugMessenger);
		pickPhysicalDevice(ctx);

		return true;
	}

	void cleanup(VulkanContext& ctx) {

		destroyValidationLayers(ctx.instance, ctx.debugMessenger);
		vkDestroyInstance(ctx.instance, nullptr);
	}

}