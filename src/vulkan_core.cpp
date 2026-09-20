#include "pch.h"
#include "vulkan_core.h"
#include "config.h"
#include "window.h"

#include "defines.h"
#include <iostream>

static VKAPI_ATTR VkBool32 VKAPI_CALL debugCallback(
			VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity,
			VkDebugUtilsMessageTypeFlagsEXT messageType,
			const VkDebugUtilsMessengerCallbackDataEXT* pCallbackData,
			void* pUserData) {

	if (messageSeverity & (VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT)) {
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
			if constexpr (Config::enableValidationLayers) {
				for (const char* targetLayer : Config::requiredVulkanLayers) {
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

		constexpr VkApplicationInfo makeApplicationInfo() {
			VkApplicationInfo appInfo{};
			appInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
			appInfo.pApplicationName = Config::appName;
			appInfo.applicationVersion = Config::applicationVersion;
			appInfo.pEngineName = Config::engineName;
			appInfo.engineVersion = Config::applicationVersion;
			appInfo.apiVersion = Config::requiredVulkanVersion;
			return appInfo;
		}

		void setupInstanceCreateInfo(VkInstanceCreateInfo& info, const VkApplicationInfo& appInfo, std::vector<const char*>& enabledLayers) {
			info.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
			info.enabledExtensionCount = static_cast<u32>(Config::requiredInstanceExtensions.size());
			info.enabledLayerCount = static_cast<u32>(enabledLayers.size());
			info.ppEnabledExtensionNames = Config::requiredInstanceExtensions.data();
			info.ppEnabledLayerNames = enabledLayers.data();
			info.pApplicationInfo = &appInfo;
		}

		constexpr void populateDebugMessengerCreateInfo(VkDebugUtilsMessengerCreateInfoEXT& info) {
			info = {};
			info.sType = VK_STRUCTURE_TYPE_DEBUG_UTILS_MESSENGER_CREATE_INFO_EXT;
			info.messageSeverity = VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_SEVERITY_INFO_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT;
			info.messageType = VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT;
			info.pfnUserCallback = debugCallback;
		}

		void createValidationLayers(VkInstance instance, VkDebugUtilsMessengerEXT& debugMessenger) {
			if constexpr (!Config::enableValidationLayers) return;

			VkDebugUtilsMessengerCreateInfoEXT info{};
			populateDebugMessengerCreateInfo(info);

			const auto func = reinterpret_cast<PFN_vkCreateDebugUtilsMessengerEXT>(vkGetInstanceProcAddr(instance, "vkCreateDebugUtilsMessengerEXT"));
			if (func != nullptr) {
				VK_CHECK(func(instance, &info, nullptr, &debugMessenger), "Failed to setup debug messenger when creating validation layers!");
			}
		}

		void destroyValidationLayers(VkInstance instance, VkDebugUtilsMessengerEXT debugMessenger) {
			if (debugMessenger == VK_NULL_HANDLE) return;

			const auto func = reinterpret_cast<PFN_vkDestroyDebugUtilsMessengerEXT>(
				vkGetInstanceProcAddr(instance, "vkDestroyDebugUtilsMessengerEXT"));
			if (func != nullptr) {
				func(instance, debugMessenger, nullptr);
			}
		}

		void createInstance(VulkanContext& ctx) {
			constexpr VkApplicationInfo appInfo = makeApplicationInfo();

			VkInstanceCreateInfo info{};
			std::vector<const char*> enabledLayers = getEnabledLayers();
			setupInstanceCreateInfo(info, appInfo, enabledLayers);

			VkDebugUtilsMessengerCreateInfoEXT debugInfo{};
			if constexpr (Config::enableValidationLayers) {
				populateDebugMessengerCreateInfo(debugInfo);
				info.pNext = &debugInfo;
			}
			else {
				info.pNext = nullptr;
			}

			VK_CHECK(vkCreateInstance(&info, nullptr, &ctx.instance), "Failed to create Vulkan instance!");
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

		bool isDeviceSuitable(VkPhysicalDevice physicalDevice, u32& queueFamilyIndex) {
			VkPhysicalDeviceProperties2 properties{VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_PROPERTIES_2};
			vkGetPhysicalDeviceProperties2(physicalDevice, &properties);

			const bool hasDiscrete = properties.properties.deviceType == VK_PHYSICAL_DEVICE_TYPE_DISCRETE_GPU;
			const bool hasVersion = properties.properties.apiVersion >= Config::requiredVulkanVersion;
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
					queueFamilyIndex = i;
					break;
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

			u32 queueFamilyIndex{};

			for (auto device : physicalDevices) {
				if (isDeviceSuitable(device, queueFamilyIndex)) {
					ctx.physicalDevice = device;
					ctx.queueFamilyIndex = queueFamilyIndex;
					return;
				}
			}

			throw std::runtime_error("Failed to find a suitable gpu for the application!");
		}

		void createLogicalDevice(VulkanContext& ctx) {
			float queuePriority = 1.0f;

			VkDeviceQueueCreateInfo queueCreateInfo = {};
			queueCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
			queueCreateInfo.queueFamilyIndex = ctx.queueFamilyIndex;
			queueCreateInfo.queueCount = 1;
			queueCreateInfo.pQueuePriorities = &queuePriority;

			VkPhysicalDeviceRayTracingPipelineFeaturesKHR rtPipelineFeatures{};
			rtPipelineFeatures.sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_RAY_TRACING_PIPELINE_FEATURES_KHR;
			rtPipelineFeatures.rayTracingPipeline = VK_TRUE;

			VkPhysicalDeviceAccelerationStructureFeaturesKHR accelFeatures{};
			accelFeatures.sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_ACCELERATION_STRUCTURE_FEATURES_KHR;
			accelFeatures.accelerationStructure = VK_TRUE;
			accelFeatures.pNext = &rtPipelineFeatures;

			VkPhysicalDeviceVulkan13Features features13{};
			features13.sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_VULKAN_1_3_FEATURES;
			features13.dynamicRendering = VK_TRUE;
			features13.synchronization2 = VK_TRUE;
			features13.pNext = &accelFeatures;

			VkPhysicalDeviceVulkan12Features features12{};
			features12.sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_VULKAN_1_2_FEATURES;
			features12.bufferDeviceAddress = VK_TRUE;
			features12.descriptorBindingPartiallyBound = VK_TRUE;
			features12.runtimeDescriptorArray = VK_TRUE;
			features12.pNext = &features13;

			VkPhysicalDeviceFeatures2 features2{};
			features2.sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_FEATURES_2;
			features2.features.samplerAnisotropy = VK_TRUE;
			features2.pNext = &features12;

			VkDeviceCreateInfo createInfo{};
			createInfo.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
			createInfo.pNext = &features2;
			createInfo.queueCreateInfoCount = 1;
			createInfo.pQueueCreateInfos = &queueCreateInfo;
			createInfo.enabledExtensionCount = static_cast<uint32_t>(Config::requiredDeviceExtensions.size());
			createInfo.ppEnabledExtensionNames = Config::requiredDeviceExtensions.data();
			createInfo.pEnabledFeatures = nullptr; // handled by pMext chain

			if (vkCreateDevice(ctx.physicalDevice, &createInfo, nullptr, &ctx.device) != VK_SUCCESS) {
				throw std::runtime_error("Failed to create logical device!");
			}

			vkGetDeviceQueue(ctx.device, ctx.queueFamilyIndex, 0, &ctx.graphicsQueue);
		}

		VkSurfaceFormatKHR chooseSurfaceFormatAndColorSpace(const std::vector<VkSurfaceFormatKHR>& surfaceFormats) {
			for (int i = 0; i < surfaceFormats.size(); i++) {
				if ((surfaceFormats[i].format == VK_FORMAT_B8G8R8A8_SRGB) &&
					surfaceFormats[i].colorSpace == VK_COLOR_SPACE_SRGB_NONLINEAR_KHR) {
					return surfaceFormats[i];
				}
			}
			return surfaceFormats[0];
		}

		VkPresentModeKHR choosePresentMode(const std::vector<VkPresentModeKHR>& presentModes) {
			for (int i = 0; i < presentModes.size(); i++) {
				if (presentModes[i] == VK_PRESENT_MODE_MAILBOX_KHR) {
					return presentModes[i];
				}
			}
			return presentModes[0];
		}

		constexpr u32 chooseNumImages(const VkSurfaceCapabilitiesKHR& capabilities) {
			const u32 requestedCount = capabilities.minImageCount + 1;
			u32 result = 0;
			if ((capabilities.maxImageCount > 0) && (requestedCount > capabilities.maxImageCount)) {
				result = capabilities.maxImageCount;
			}
			else {
				result = requestedCount;
			}
			return result;
		}

		VkImageView createImageView(VkDevice device, VkImage image, VkFormat format, VkImageAspectFlags aspectFlags,
			VkImageViewType viewType, u32 layerCount, u32 mipLevels) {

			VkImageViewCreateInfo info{};
			info.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
			info.pNext = nullptr;
			info.flags = 0;
			info.image = image;
			info.viewType = viewType;
			info.format = format;
			info.components.r = VK_COMPONENT_SWIZZLE_IDENTITY;
			info.components.g = VK_COMPONENT_SWIZZLE_IDENTITY;
			info.components.b = VK_COMPONENT_SWIZZLE_IDENTITY;
			info.components.a = VK_COMPONENT_SWIZZLE_IDENTITY;
			info.subresourceRange.aspectMask = aspectFlags;
			info.subresourceRange.baseMipLevel = 0;
			info.subresourceRange.levelCount = mipLevels;
			info.subresourceRange.baseArrayLayer = 0;
			info.subresourceRange.layerCount = layerCount;

			VkImageView view;
			VK_CHECK(vkCreateImageView(device, &info, nullptr, &view), "Failed to create image view!");

			return view;
		}

		void destroyImageViews(VulkanContext& ctx);

		void createSwapchain(VulkanContext& ctx) {
			VkSurfaceCapabilitiesKHR capabilities{};
			vkGetPhysicalDeviceSurfaceCapabilitiesKHR(ctx.physicalDevice, ctx.surface, &capabilities);

			const u32 numImages = chooseNumImages(capabilities);

			u32 presentModeCount{};
			vkGetPhysicalDeviceSurfacePresentModesKHR(ctx.physicalDevice, ctx.surface, &presentModeCount, nullptr);
			std::vector<VkPresentModeKHR> presentModes(presentModeCount);
			vkGetPhysicalDeviceSurfacePresentModesKHR(ctx.physicalDevice, ctx.surface, &presentModeCount, presentModes.data());

			const VkPresentModeKHR presentMode = choosePresentMode(presentModes);

			u32 surfaceFormatCount{};
			vkGetPhysicalDeviceSurfaceFormatsKHR(ctx.physicalDevice, ctx.surface, &surfaceFormatCount, nullptr);
			std::vector<VkSurfaceFormatKHR> surfaceFormats(surfaceFormatCount);
			vkGetPhysicalDeviceSurfaceFormatsKHR(ctx.physicalDevice, ctx.surface, &surfaceFormatCount, surfaceFormats.data());

			const VkSurfaceFormatKHR surfaceFormat = chooseSurfaceFormatAndColorSpace(surfaceFormats);
			const VkExtent2D extent = Window::getExtent2D(capabilities, Window::getHandle());
			if (extent.width == 0 || extent.height == 0) {
				return;
			}
			ctx.swapchainState.extent = extent;

			VkSwapchainKHR oldSwapchain = ctx.swapchainState.swapchain;

			VkSwapchainCreateInfoKHR info{};
			info.sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR;
			info.pNext = nullptr;
			info.flags = 0;
			info.surface = ctx.surface;
			info.minImageCount = numImages;
			info.imageFormat = surfaceFormat.format;
			info.imageColorSpace = surfaceFormat.colorSpace;
			info.imageExtent = extent;
			info.imageArrayLayers = 1;
			info.imageUsage = (VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT | VK_IMAGE_USAGE_TRANSFER_DST_BIT);
			info.imageSharingMode = VK_SHARING_MODE_EXCLUSIVE;
			info.queueFamilyIndexCount = 0;
			info.pQueueFamilyIndices = nullptr;
			info.preTransform = capabilities.currentTransform;
			info.compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR;
			info.presentMode = presentMode;
			info.clipped = VK_TRUE;
			info.oldSwapchain = oldSwapchain;

			VkSwapchainKHR newSwapchain = VK_NULL_HANDLE;
			VK_CHECK(vkCreateSwapchainKHR(ctx.device, &info, nullptr, &newSwapchain), "Failed to create swapchain!");

			if (oldSwapchain != VK_NULL_HANDLE) {
				destroyImageViews(ctx);
				vkDestroySwapchainKHR(ctx.device, oldSwapchain, nullptr);
			}

			ctx.swapchainState.swapchain = newSwapchain;
			ctx.swapchainState.imageFormat = surfaceFormat.format;

			u32 numSwapChainImages{};
			VK_CHECK(vkGetSwapchainImagesKHR(ctx.device, ctx.swapchainState.swapchain, &numSwapChainImages, nullptr), "Failed to get swap chain images!");

			ctx.swapchainState.images.resize(numSwapChainImages);

			VK_CHECK(vkGetSwapchainImagesKHR(ctx.device, ctx.swapchainState.swapchain,
				&numSwapChainImages, ctx.swapchainState.images.data()), "Failed to get swap chain images!");

			ctx.swapchainState.views.resize(numSwapChainImages);

			const s32 layerCount = 1;
			const s32 mipLevels = 1;

			for (size_t i = 0; i < ctx.swapchainState.views.size(); ++i) {
				ctx.swapchainState.views[i] = createImageView(ctx.device, ctx.swapchainState.images[i], surfaceFormat.format,
					VK_IMAGE_ASPECT_COLOR_BIT, VK_IMAGE_VIEW_TYPE_2D, layerCount, mipLevels);
			}
		}

		void destroyImageViews(VulkanContext& ctx) {
			for (size_t i = 0; i < ctx.swapchainState.views.size(); ++i) {
				vkDestroyImageView(ctx.device, ctx.swapchainState.views[i], nullptr);
			}
		}

	} // unnamed namespace

	void init(VulkanContext& ctx)
	{
		createInstance(ctx);
		Window::createSurface(ctx);

		createValidationLayers(ctx.instance, ctx.debugMessenger);
		pickPhysicalDevice(ctx);
		createLogicalDevice(ctx);
		createSwapchain(ctx);

	}

	void cleanup(VulkanContext& ctx) {

		destroyImageViews(ctx);
		vkDestroySwapchainKHR(ctx.device, ctx.swapchainState.swapchain, nullptr);
		vkDestroyDevice(ctx.device, nullptr);
		destroyValidationLayers(ctx.instance, ctx.debugMessenger);

		Window::destroySurface(ctx);
		vkDestroyInstance(ctx.instance, nullptr);
	}

	void recreateSwapchain(VulkanContext& ctx) {
		vkDeviceWaitIdle(ctx.device);
		createSwapchain(ctx);
	}

	FrameState& getCurrentFrame(VulkanContext& ctx) {
		return ctx.frameStates[ctx.currentFrame % Config::frameOverlap];
	}

} // namespace VulkanCore