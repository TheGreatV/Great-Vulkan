#include <iostream>
#include <fstream>

#define VK_USE_PLATFORM_WIN32_KHR 1

#include <Common/Common.hpp>
#include <Vulkan/Vulkan.hpp>

#include <Windows.h>
using namespace GreatVulkan;


void func()
{
	Vector<const char*> vk_enabledInstanceLayersNames = {
		"VK_LAYER_LUNARG_api_dump",
	};

	VkApplicationInfo vk_applicationInfo;
	{
		vk_applicationInfo.sType = VkStructureType::VK_STRUCTURE_TYPE_APPLICATION_INFO;
		vk_applicationInfo.pNext = nullptr;
		vk_applicationInfo.pApplicationName = "application name";
		vk_applicationInfo.applicationVersion = 0;
		vk_applicationInfo.pEngineName = "engine name";
		vk_applicationInfo.engineVersion = 0;
		vk_applicationInfo.apiVersion = VK_MAKE_VERSION(1, 0, 61);
	}
	VkInstanceCreateInfo vk_instanceCreateInfo;
	{
		vk_instanceCreateInfo.sType = VkStructureType::VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
		vk_instanceCreateInfo.pNext = nullptr;
		vk_instanceCreateInfo.flags = 0;
		vk_instanceCreateInfo.pApplicationInfo = &vk_applicationInfo;
		vk_instanceCreateInfo.enabledLayerCount = vk_enabledInstanceLayersNames.size();
		vk_instanceCreateInfo.ppEnabledLayerNames = vk_enabledInstanceLayersNames.data();
		vk_instanceCreateInfo.enabledExtensionCount = 0;
		vk_instanceCreateInfo.ppEnabledExtensionNames = nullptr;
	}

	VkInstance vk_instance;
	if (auto result = Result(vkCreateInstance(&vk_instanceCreateInfo, nullptr, &vk_instance)))
	{
		std::cout << "Instance successfully created!" << std::endl;
	}
	else
	{
		throw Exception();
	}

	uint32_t vk_physicalDevicesCount;
	{
		if (auto result = Result(vkEnumeratePhysicalDevices(vk_instance, &vk_physicalDevicesCount, nullptr)))
		{
			std::cout << "Vulkan physical devices succesfully enumerated!" << std::endl;
		}
		else
		{
			throw Exception();
		}
	}
	Vector<VkPhysicalDevice> vk_physicalDevices(vk_physicalDevicesCount);
	{
		if (auto result = Result(vkEnumeratePhysicalDevices(vk_instance, &vk_physicalDevicesCount, vk_physicalDevices.data())))
		{
			std::cout << "Vulkan physical devices succesfully obtained!" << std::endl;
		}
		else
		{
			throw Exception();
		}
	}
	VkPhysicalDevice vk_physicalDevice = vk_physicalDevices[0];

	VkPhysicalDeviceProperties vk_physicalDeviceProperties;
	vkGetPhysicalDeviceProperties(vk_physicalDevice, &vk_physicalDeviceProperties);
	{
		std::cout
			<< "Physical device properties:" << std::endl
			<< "apiVersion: "
			<< VK_VERSION_MAJOR(vk_physicalDeviceProperties.apiVersion) << "."
			<< VK_VERSION_MINOR(vk_physicalDeviceProperties.apiVersion) << "."
			<< VK_VERSION_PATCH(vk_physicalDeviceProperties.apiVersion) << std::endl
			<< "driverVersion: " << vk_physicalDeviceProperties.driverVersion << std::endl;
		// uint32_t                            apiVersion;
		// uint32_t                            driverVersion;
		// uint32_t                            vendorID;
		// uint32_t                            deviceID;
		// VkPhysicalDeviceType                deviceType;
		// char                                deviceName[VK_MAX_PHYSICAL_DEVICE_NAME_SIZE];
		// uint8_t                             pipelineCacheUUID[VK_UUID_SIZE];
		// VkPhysicalDeviceLimits              limits;
		// VkPhysicalDeviceSparseProperties    sparseProperties;
	}

	uint32_t vk_physicalDeviceQueueFamilyPropertiesCount = 0;
	{
		vkGetPhysicalDeviceQueueFamilyProperties(vk_physicalDevice, &vk_physicalDeviceQueueFamilyPropertiesCount, nullptr);
	}
	Vector<VkQueueFamilyProperties> vk_physicalDeviceQueueFamilyProperties(vk_physicalDeviceQueueFamilyPropertiesCount);
	{
		vkGetPhysicalDeviceQueueFamilyProperties(vk_physicalDevice, &vk_physicalDeviceQueueFamilyPropertiesCount, vk_physicalDeviceQueueFamilyProperties.data());
	}

	VkPhysicalDeviceMemoryProperties vk_physicalDeviceMemoryProperties;
	{
		vkGetPhysicalDeviceMemoryProperties(vk_physicalDevice, &vk_physicalDeviceMemoryProperties);
	}

	uint32_t queueFamilyIndex = 0;
	uint32_t queueIndex = 0;
	Vector<float> vk_deviceQueuesPriorities = {1.0f};
	Vector<VkDeviceQueueCreateInfo> vk_deviceQueueCreateInfos(1);
	{
		for (auto &vk_deviceQueueCreateInfo : vk_deviceQueueCreateInfos)
		{
			vk_deviceQueueCreateInfo.sType = VkStructureType::VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
			vk_deviceQueueCreateInfo.pNext = nullptr;
			vk_deviceQueueCreateInfo.flags = 0;
			vk_deviceQueueCreateInfo.queueFamilyIndex = 0;
			vk_deviceQueueCreateInfo.queueCount = vk_deviceQueuesPriorities.size();
			vk_deviceQueueCreateInfo.pQueuePriorities = vk_deviceQueuesPriorities.data();
		}
	}
	VkPhysicalDeviceFeatures vk_physicalDeviceFeatures;
	{
		std::memset(&vk_physicalDeviceFeatures, 0, sizeof(vk_physicalDeviceFeatures)); // just to ensure that new fields will be nulled

		vk_physicalDeviceFeatures.robustBufferAccess = VK_FALSE;
		vk_physicalDeviceFeatures.fullDrawIndexUint32 = VK_FALSE;
		vk_physicalDeviceFeatures.imageCubeArray = VK_FALSE;
		vk_physicalDeviceFeatures.independentBlend = VK_FALSE;
		vk_physicalDeviceFeatures.geometryShader = VK_FALSE;
		vk_physicalDeviceFeatures.tessellationShader = VK_FALSE;
		vk_physicalDeviceFeatures.sampleRateShading = VK_FALSE;
		vk_physicalDeviceFeatures.dualSrcBlend = VK_FALSE;
		vk_physicalDeviceFeatures.logicOp = VK_FALSE;
		vk_physicalDeviceFeatures.multiDrawIndirect = VK_FALSE;
		vk_physicalDeviceFeatures.drawIndirectFirstInstance = VK_FALSE;
		vk_physicalDeviceFeatures.depthClamp = VK_FALSE;
		vk_physicalDeviceFeatures.depthBiasClamp = VK_FALSE;
		vk_physicalDeviceFeatures.fillModeNonSolid = VK_FALSE;
		vk_physicalDeviceFeatures.depthBounds = VK_FALSE;
		vk_physicalDeviceFeatures.wideLines = VK_FALSE;
		vk_physicalDeviceFeatures.largePoints = VK_FALSE;
		vk_physicalDeviceFeatures.alphaToOne = VK_FALSE;
		vk_physicalDeviceFeatures.multiViewport = VK_FALSE;
		vk_physicalDeviceFeatures.samplerAnisotropy = VK_FALSE;
		vk_physicalDeviceFeatures.textureCompressionETC2 = VK_FALSE;
		vk_physicalDeviceFeatures.textureCompressionASTC_LDR = VK_FALSE;
		vk_physicalDeviceFeatures.textureCompressionBC = VK_FALSE;
		vk_physicalDeviceFeatures.occlusionQueryPrecise = VK_FALSE;
		vk_physicalDeviceFeatures.pipelineStatisticsQuery = VK_FALSE;
		vk_physicalDeviceFeatures.vertexPipelineStoresAndAtomics = VK_FALSE;
		vk_physicalDeviceFeatures.fragmentStoresAndAtomics = VK_FALSE;
		vk_physicalDeviceFeatures.shaderTessellationAndGeometryPointSize = VK_FALSE;
		vk_physicalDeviceFeatures.shaderImageGatherExtended = VK_FALSE;
		vk_physicalDeviceFeatures.shaderStorageImageExtendedFormats = VK_FALSE;
		vk_physicalDeviceFeatures.shaderStorageImageMultisample = VK_FALSE;
		vk_physicalDeviceFeatures.shaderStorageImageReadWithoutFormat = VK_FALSE;
		vk_physicalDeviceFeatures.shaderStorageImageWriteWithoutFormat = VK_FALSE;
		vk_physicalDeviceFeatures.shaderUniformBufferArrayDynamicIndexing = VK_FALSE;
		vk_physicalDeviceFeatures.shaderSampledImageArrayDynamicIndexing = VK_FALSE;
		vk_physicalDeviceFeatures.shaderStorageBufferArrayDynamicIndexing = VK_FALSE;
		vk_physicalDeviceFeatures.shaderStorageImageArrayDynamicIndexing = VK_FALSE;
		vk_physicalDeviceFeatures.shaderClipDistance = VK_FALSE;
		vk_physicalDeviceFeatures.shaderCullDistance = VK_FALSE;
		vk_physicalDeviceFeatures.shaderFloat64 = VK_FALSE;
		vk_physicalDeviceFeatures.shaderInt64 = VK_FALSE;
		vk_physicalDeviceFeatures.shaderInt16 = VK_FALSE;
		vk_physicalDeviceFeatures.shaderResourceResidency = VK_FALSE;
		vk_physicalDeviceFeatures.shaderResourceMinLod = VK_FALSE;
		vk_physicalDeviceFeatures.sparseBinding = VK_FALSE;
		vk_physicalDeviceFeatures.sparseResidencyBuffer = VK_FALSE;
		vk_physicalDeviceFeatures.sparseResidencyImage2D = VK_FALSE;
		vk_physicalDeviceFeatures.sparseResidencyImage3D = VK_FALSE;
		vk_physicalDeviceFeatures.sparseResidency2Samples = VK_FALSE;
		vk_physicalDeviceFeatures.sparseResidency4Samples = VK_FALSE;
		vk_physicalDeviceFeatures.sparseResidency8Samples = VK_FALSE;
		vk_physicalDeviceFeatures.sparseResidency16Samples = VK_FALSE;
		vk_physicalDeviceFeatures.sparseResidencyAliased = VK_FALSE;
		vk_physicalDeviceFeatures.variableMultisampleRate = VK_FALSE;
		vk_physicalDeviceFeatures.inheritedQueries = VK_FALSE;
	}

	VkDevice vk_device;
	{
		VkDeviceCreateInfo vk_deviceCreateInfo;
		{
			vk_deviceCreateInfo.sType = VkStructureType::VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
			vk_deviceCreateInfo.pNext = nullptr;
			vk_deviceCreateInfo.flags = 0;
			vk_deviceCreateInfo.queueCreateInfoCount = vk_deviceQueueCreateInfos.size();
			vk_deviceCreateInfo.pQueueCreateInfos = vk_deviceQueueCreateInfos.data();
			vk_deviceCreateInfo.enabledLayerCount = 0;
			vk_deviceCreateInfo.ppEnabledLayerNames = nullptr;
			vk_deviceCreateInfo.enabledExtensionCount = 0;
			vk_deviceCreateInfo.ppEnabledExtensionNames = nullptr;
			vk_deviceCreateInfo.pEnabledFeatures = &vk_physicalDeviceFeatures;
		}

		if (auto result = Result(vkCreateDevice(vk_physicalDevice, &vk_deviceCreateInfo, nullptr, &vk_device)))
		{
			std::cout << "Device succesfully created!" << std::endl;
		}
		else
		{
			throw Exception();
		}
	}

	VkCommandPool vk_commandPool;
	{
		VkCommandPoolCreateInfo vk_commandPoolCreateInfo;
		{
			vk_commandPoolCreateInfo.sType = VkStructureType::VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
			vk_commandPoolCreateInfo.pNext = nullptr;
			vk_commandPoolCreateInfo.flags = VkCommandPoolCreateFlagBits::VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT;
			vk_commandPoolCreateInfo.queueFamilyIndex = 0;
		}

		if (auto result = Result(vkCreateCommandPool(vk_device, &vk_commandPoolCreateInfo, nullptr, &vk_commandPool)))
		{
			std::cout << "Command pool succesfully created!" << std::endl;
		}
		else
		{
			throw Exception(); // TODO
		}

		if (auto result = Result(vkResetCommandPool(vk_device, vk_commandPool, VkCommandPoolResetFlagBits::VK_COMMAND_POOL_RESET_RELEASE_RESOURCES_BIT)))
		{
			std::cout << "Command pool successfully reset!" << std::endl;
		}
		else
		{
			throw Exception();
		}
	}

	vkDestroyCommandPool(vk_device, vk_commandPool, nullptr);
	vkDestroyDevice(vk_device, nullptr);
	vkDestroyInstance(vk_instance, nullptr);
}

void main()
{
	func();

	std::system("pause");
}