#include <iostream>
#include <fstream>

#define VK_USE_PLATFORM_WIN32_KHR 1

#include <Common/Common.hpp>
#include <Vulkan/Vulkan.hpp>

#include <Windows.h>
using namespace GreatVulkan;


void func()
{
	/*
	+		[0]	0x00648528 "VK_LAYER_LUNARG_core_validation"	const char *
	+		[1]	0x006485d0 "VK_LAYER_LUNARG_monitor"	const char *
	+		[2]	0x00648678 "VK_LAYER_LUNARG_object_tracker"	const char *
	+		[3]	0x00648748 "VK_LAYER_LUNARG_parameter_validation"	const char *
	+		[4]	0x00648800 "VK_LAYER_LUNARG_screenshot"	const char *
	+		[5]	0x006488a8 "VK_LAYER_GOOGLE_threading"	const char *
	+		[6]	0x00648950 "VK_LAYER_GOOGLE_unique_objects"	const char *
	+		[7]	0x006489f8 "VK_LAYER_RENDERDOC_Capture"	const char *
	*/
	/*
	+		[0]	0x0064b74c "VK_KHR_surface"	const char *
	+		[1]	0x00651278 "VK_KHR_win32_surface"	const char *
	+		[2]	0x0064e758 "VK_EXT_debug_report"	const char *
	*/
	/*
	+		[0]	0x06c7bc70 "VK_LAYER_LUNARG_core_validation"	const char *
	+		[1]	0x06c7bd18 "VK_LAYER_LUNARG_monitor"	const char *
	+		[2]	0x06c7bdc0 "VK_LAYER_LUNARG_object_tracker"	const char *
	+		[3]	0x06c7be90 "VK_LAYER_LUNARG_parameter_validation"	const char *
	+		[4]	0x06c7bf48 "VK_LAYER_LUNARG_screenshot"	const char *
	+		[5]	0x06c7bff0 "VK_LAYER_GOOGLE_threading"	const char *
	+		[6]	0x06c7c098 "VK_LAYER_GOOGLE_unique_objects"	const char *
	+		[7]	0x06c7c140 "VK_LAYER_RENDERDOC_Capture"	const char *
	*/
	/*
	+		[0]	0x06d2d850 "VK_KHR_sampler_mirror_clamp_to_edge"	const char *
	+		[1]	0x06d2d960 "VK_KHR_swapchain"	const char *
	+		[2]	0x06d2da80 "VK_EXT_debug_marker"	const char *
	*/

	auto loadShader = [](const String& filename)
	{
		FILE* file = nullptr;

		auto loadResult = fopen_s(&file, filename.c_str(), "rb");

		if (loadResult != 0)
		{
			throw std::exception("failed to load file");
		}

		fseek(file, 0, FILE_END);
		
		auto size = ftell(file);
		
		if (size % 4 != 0)
		{
			throw Exception(); // TODO
		}

		rewind(file);

		std::vector<uint32_t> result(size);
		fread((void*)result.data(), 1, size, file);

		fclose(file);

		return Move(result);
	};

	VkExtent2D windowSize;
	{
		windowSize.width = 800;
		windowSize.height = 600;
	}

	auto handleInstance = GetModuleHandleA(nullptr);
	auto handleWindow = [&]()
	{
		auto windowClassName = "window class";
		{
			WNDCLASSA windowClassInfo;
			{
				windowClassInfo.style = CS_HREDRAW | CS_VREDRAW | CS_OWNDC;
				windowClassInfo.lpfnWndProc = DefWindowProcA;
				windowClassInfo.cbClsExtra = 0;
				windowClassInfo.cbWndExtra = 0;
				windowClassInfo.hInstance = handleInstance;
				windowClassInfo.hIcon = nullptr;
				windowClassInfo.hCursor = nullptr;
				windowClassInfo.hbrBackground = nullptr;
				windowClassInfo.lpszMenuName = nullptr;
				windowClassInfo.lpszClassName = windowClassName;
			}
			if (!RegisterClassA(&windowClassInfo))
			{
				throw std::exception("failed to register window class");
			}
		}

		// DWORD style = WS_OVERLAPPEDWINDOW | WS_VISIBLE;
		DWORD style =
			WS_OVERLAPPED |
			WS_CAPTION |
			WS_SYSMENU |
			WS_VISIBLE;

		RECT rect;
		{
			rect.top = 100;
			rect.bottom = rect.top + windowSize.height;
			rect.left = 100;
			rect.right = rect.left + windowSize.width;
		}
		AdjustWindowRect(&rect, style, false);

		auto handle = CreateWindowA(
			windowClassName,
			"window",
			style,
			rect.left, rect.top, rect.right - rect.left, rect.bottom - rect.top, // 0, 0, 800, 600,
			nullptr,
			nullptr,
			handleInstance,
			nullptr
		);

		if (!handle)
		{
			throw std::exception("failed to create window");
		}

		return handle;
	}();

	uint32_t vk_instanceLayersPropertiesCount;
	{
		if (auto result = Result(vkEnumerateInstanceLayerProperties(&vk_instanceLayersPropertiesCount, nullptr)))
		{
			std::cout << "Vulkan instance layers properties succesfully enumerated!" << std::endl;
			std::cout << "There is " << vk_instanceLayersPropertiesCount << " vulkan layer properties" << std::endl;
		}
		else
		{
			throw Exception();
		}
	}
	Vector<VkLayerProperties> vk_instanceLayersProperties(vk_instanceLayersPropertiesCount);
	{
		if (auto result = Result(vkEnumerateInstanceLayerProperties(&vk_instanceLayersPropertiesCount, vk_instanceLayersProperties.data())))
		{
			std::cout << "Vulkan instance layers properties succesfully obtained!" << std::endl;

			for (auto vk_layerProperties : vk_instanceLayersProperties)
			{
				std::cout << "Vulkan layer properties:" <<
					"\tlayerName: "				<< vk_layerProperties.layerName				<< std::endl <<
					"\tspecVersion: "			<< vk_layerProperties.specVersion			<< std::endl <<
					"\timplementationVersion: "	<< vk_layerProperties.implementationVersion	<< std::endl <<
					"\tdescription: "			<< vk_layerProperties.description			<< std::endl;
			}
		}
		else
		{
			throw Exception();
		}
	}
	Vector<String> vk_instanceLayersNames;
	Vector<const char*> vk_enabledInstanceLayersNames;
	{
#if _DEBUG
		vk_instanceLayersNames.reserve(vk_instanceLayersProperties.size());

		for (auto &vk_layerProperties : vk_instanceLayersProperties)
		{
			auto name = static_cast<String>(vk_layerProperties.layerName);

			// "VK_LAYER_LUNARG_api_dump"
			// "VK_LAYER_LUNARG_core_validation"	
			// "VK_LAYER_LUNARG_monitor"	
			// "VK_LAYER_LUNARG_object_tracker"	
			// "VK_LAYER_LUNARG_parameter_validation"	
			// "VK_LAYER_LUNARG_screenshot"	
			// "VK_LAYER_LUNARG_standard_validation"	
			// "VK_LAYER_GOOGLE_threading"	
			// "VK_LAYER_GOOGLE_unique_objects"	
			// "VK_LAYER_RENDERDOC_Capture"	
			if (
				(name != "VK_LAYER_LUNARG_vktrace") &&
				(name != "VK_LAYER_LUNARG_api_dump") &&
				// (name != "VK_LAYER_RENDERDOC_Capture") && // debug print on screen
				(name != "VK_LAYER_LUNARG_standard_validation")
			)
			{
				vk_instanceLayersNames.push_back(name);
			}
		}

		for (auto &vk_layerName : vk_instanceLayersNames)
		{
			vk_enabledInstanceLayersNames.push_back(vk_layerName.data());
		}
#endif
	}

	Map<String, Vector<VkExtensionProperties>> vk_instanceLayersExtensionsProperties;
	{
		for (auto &vk_layerName : vk_instanceLayersNames)
		{
			uint32_t vk_instanceLayerExtensionsPropertiesCount;
			{
				if (auto result = Result(vkEnumerateInstanceExtensionProperties(vk_layerName.c_str(), &vk_instanceLayerExtensionsPropertiesCount, nullptr)))
				{
					std::cout << "Vulkan instance layer extensions properties, for layer " << vk_layerName << " succesfully enumerated!" << std::endl;
				}
				else
				{
					throw Exception();
				}
			}
			Vector<VkExtensionProperties> vk_layerExtensionsProperties(vk_instanceLayerExtensionsPropertiesCount);
			{
				if (auto result = Result(vkEnumerateInstanceExtensionProperties(vk_layerName.c_str(), &vk_instanceLayerExtensionsPropertiesCount, vk_layerExtensionsProperties.data())))
				{
					std::cout << "Vulkan instance layers extensions properties for layer " << vk_layerName << " succesfully obtained!" << std::endl;
				}
				else
				{
					throw Exception();
				}
			}

			vk_instanceLayersExtensionsProperties[vk_layerName] = Move(vk_layerExtensionsProperties);
		}

		String emptyName = "";
		{
			uint32_t vk_instanceLayerExtensionsPropertiesCount;
			{
				if (auto result = Result(vkEnumerateInstanceExtensionProperties(emptyName.c_str(), &vk_instanceLayerExtensionsPropertiesCount, nullptr)))
				{
					std::cout << "Vulkan instance layer extensions properties, for layer empty succesfully enumerated!" << std::endl;
				}
				else
				{
					throw Exception();
				}
			}
			Vector<VkExtensionProperties> vk_layerExtensionsProperties(vk_instanceLayerExtensionsPropertiesCount);
			{
				if (auto result = Result(vkEnumerateInstanceExtensionProperties(emptyName.c_str(), &vk_instanceLayerExtensionsPropertiesCount, vk_layerExtensionsProperties.data())))
				{
					std::cout << "Vulkan instance layers extensions properties for layer empty succesfully obtained!" << std::endl;
				}
				else
				{
					throw Exception();
				}
			}

			vk_instanceLayersExtensionsProperties[emptyName] = Move(vk_layerExtensionsProperties);
		}
	}
	Vector<String> vk_instanceExtensionsNames;
	Vector<const char*> vk_enabledInstanceExtensionsNames;
	{
		for (auto &vk_layerExtensionsProperties : vk_instanceLayersExtensionsProperties)
		{
			for (auto &vk_layerExtensionProperties : vk_layerExtensionsProperties.second)
			{
				auto name = static_cast<String>(vk_layerExtensionProperties.extensionName);

				if (std::find(vk_instanceExtensionsNames.begin(), vk_instanceExtensionsNames.end(), name) == vk_instanceExtensionsNames.end())
				{
					vk_instanceExtensionsNames.push_back(name);
				}
			}
		}

		for (auto &vk_extensionName : vk_instanceExtensionsNames)
		{
			vk_enabledInstanceExtensionsNames.push_back(vk_extensionName.data());
		}
	}

	VkApplicationInfo vk_applicationInfo;
	{
		vk_applicationInfo.sType				= VkStructureType::VK_STRUCTURE_TYPE_APPLICATION_INFO;
		vk_applicationInfo.pNext				= nullptr;
		vk_applicationInfo.pApplicationName		= "application name";
		vk_applicationInfo.applicationVersion	= 0;
		vk_applicationInfo.pEngineName			= "engine name";
		vk_applicationInfo.engineVersion		= 0;
		vk_applicationInfo.apiVersion			= VK_MAKE_VERSION(1, 0, 56);
	}
	VkInstanceCreateInfo vk_instanceCreateInfo;
	{
		vk_instanceCreateInfo.sType						= VkStructureType::VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
		vk_instanceCreateInfo.pNext						= nullptr;
		vk_instanceCreateInfo.flags						= 0;
		vk_instanceCreateInfo.pApplicationInfo			= &vk_applicationInfo;
		vk_instanceCreateInfo.enabledLayerCount			= vk_enabledInstanceLayersNames.size();
		vk_instanceCreateInfo.ppEnabledLayerNames		= vk_enabledInstanceLayersNames.data();
		vk_instanceCreateInfo.enabledExtensionCount		= vk_enabledInstanceExtensionsNames.size();
		vk_instanceCreateInfo.ppEnabledExtensionNames	= vk_enabledInstanceExtensionsNames.data();
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

	uint32_t vk_physicalDeviceLayersPropertiesCount;
	{
		if (auto result = Result(vkEnumerateDeviceLayerProperties(vk_physicalDevice, &vk_physicalDeviceLayersPropertiesCount, nullptr)))
		{
			std::cout << "Device properties successfully enumerated!" << std::endl;
		}
		else
		{
			throw Exception(); // TODO
		}
	}
	Vector<VkLayerProperties> vk_physicalDeviceLayersProperties(vk_physicalDeviceLayersPropertiesCount);
	{
		if (auto result = Result(vkEnumerateDeviceLayerProperties(vk_physicalDevice, &vk_physicalDeviceLayersPropertiesCount, vk_physicalDeviceLayersProperties.data())))
		{
			std::cout << "Device properties successfully obtained!" << std::endl;
		}
		else
		{
			throw Exception(); // TODO
		}
	}
	Vector<String> vk_physicalDeviceLayersNames;
	Vector<const char*> vk_enabledPhysicalDeviceLayersNames;
	{
#if _DEBUG
		vk_physicalDeviceLayersNames.reserve(vk_physicalDeviceLayersProperties.size());

		for (auto &vk_physicalDeviceLayerProperties : vk_physicalDeviceLayersProperties)
		{
			auto name = static_cast<String>(vk_physicalDeviceLayerProperties.layerName);

			if (name != "VK_LAYER_LUNARG_vktrace")
			{
				vk_physicalDeviceLayersNames.push_back(name);
			}
		}

		for (auto &vk_layerName : vk_physicalDeviceLayersNames)
		{
			vk_enabledPhysicalDeviceLayersNames.push_back(vk_layerName.data());
		}
#endif
	}

	Map<String, Vector<VkExtensionProperties>> vk_physicalDeviceLayersExtensionsProperties;
	{
		for (auto &vk_layerName : vk_physicalDeviceLayersNames)
		{
			uint32_t vk_physicalDeviceLayerExtensionsPropertiesCount;
			{
				if (auto result = Result(vkEnumerateDeviceExtensionProperties(vk_physicalDevice, vk_layerName.c_str(), &vk_physicalDeviceLayerExtensionsPropertiesCount, nullptr)))
				{
					std::cout << "Vulkan physical device layer extensions properties, for layer " << vk_layerName << " succesfully enumerated!" << std::endl;
				}
				else
				{
					throw Exception();
				}
			}
			Vector<VkExtensionProperties> vk_physicalDeviceExtensionsProperties(vk_physicalDeviceLayerExtensionsPropertiesCount);
			{
				if (auto result = Result(vkEnumerateDeviceExtensionProperties(vk_physicalDevice, vk_layerName.c_str(), &vk_physicalDeviceLayerExtensionsPropertiesCount, vk_physicalDeviceExtensionsProperties.data())))
				{
					std::cout << "Vulkan physical device layer extensions properties for layer " << vk_layerName << " succesfully obtained!" << std::endl;
				}
				else
				{
					throw Exception();
				}
			}

			vk_physicalDeviceLayersExtensionsProperties[vk_layerName] = Move(vk_physicalDeviceExtensionsProperties);
		}

		String emptyName = "";
		{
			uint32_t vk_physicalDeviceLayerExtensionsPropertiesCount;
			{
				if (auto result = Result(vkEnumerateDeviceExtensionProperties(vk_physicalDevice, emptyName.c_str(), &vk_physicalDeviceLayerExtensionsPropertiesCount, nullptr)))
				{
					std::cout << "Vulkan instance layer extensions properties, for layer empty succesfully enumerated!" << std::endl;
				}
				else
				{
					throw Exception();
				}
			}
			Vector<VkExtensionProperties> vk_physicalDeviceExtensionsProperties(vk_physicalDeviceLayerExtensionsPropertiesCount);
			{
				if (auto result = Result(vkEnumerateDeviceExtensionProperties(vk_physicalDevice, emptyName.c_str(), &vk_physicalDeviceLayerExtensionsPropertiesCount, vk_physicalDeviceExtensionsProperties.data())))
				{
					std::cout << "Vulkan instance layer extensions properties for layer empty succesfully obtained!" << std::endl;
				}
				else
				{
					throw Exception();
				}
			}

			vk_physicalDeviceLayersExtensionsProperties[emptyName] = Move(vk_physicalDeviceExtensionsProperties);
		}
	}
	Vector<String> vk_physicalDeviceExtensionsNames;
	Vector<const char*> vk_enabledPhysicalDeviceExtensionsNames;
	{
		for (auto &vk_physicalDeviceExtensionsProperties : vk_physicalDeviceLayersExtensionsProperties)
		{
			for (auto &vk_physicalDeviceExtensionProperties : vk_physicalDeviceExtensionsProperties.second)
			{
				auto name = static_cast<String>(vk_physicalDeviceExtensionProperties.extensionName);

				if (std::find(vk_physicalDeviceExtensionsNames.begin(), vk_physicalDeviceExtensionsNames.end(), name) == vk_physicalDeviceExtensionsNames.end())
				{
					vk_physicalDeviceExtensionsNames.push_back(Move(name));
				}
			}
		}

		for (auto &vk_extensionName : vk_physicalDeviceExtensionsNames)
		{
			vk_enabledPhysicalDeviceExtensionsNames.push_back(vk_extensionName.data());
		}
	}

	uint32_t queueFamilyIndex = 0;
	uint32_t queueIndex = 0;
	Vector<float> vk_deviceQueuesPriorities = {1.0f};
	Vector<VkDeviceQueueCreateInfo> vk_deviceQueueCreateInfos(1);
	{
		for (auto &vk_deviceQueueCreateInfo : vk_deviceQueueCreateInfos)
		{
			vk_deviceQueueCreateInfo.sType				= VkStructureType::VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
			vk_deviceQueueCreateInfo.pNext				= nullptr;
			vk_deviceQueueCreateInfo.flags				= 0;
			vk_deviceQueueCreateInfo.queueFamilyIndex	= 0;
			vk_deviceQueueCreateInfo.queueCount			= vk_deviceQueuesPriorities.size();
			vk_deviceQueueCreateInfo.pQueuePriorities	= vk_deviceQueuesPriorities.data();
		}
	}
	VkPhysicalDeviceFeatures vk_physicalDeviceFeatures;
	{
		std::memset(&vk_physicalDeviceFeatures, 0, sizeof(vk_physicalDeviceFeatures)); // just to ensure that new fields will be nulled

		vk_physicalDeviceFeatures.robustBufferAccess						= VK_FALSE;
		vk_physicalDeviceFeatures.fullDrawIndexUint32						= VK_FALSE;
		vk_physicalDeviceFeatures.imageCubeArray							= VK_FALSE;
		vk_physicalDeviceFeatures.independentBlend							= VK_FALSE;
		vk_physicalDeviceFeatures.geometryShader							= VK_FALSE;
		vk_physicalDeviceFeatures.tessellationShader						= VK_FALSE;
		vk_physicalDeviceFeatures.sampleRateShading							= VK_FALSE;
		vk_physicalDeviceFeatures.dualSrcBlend								= VK_FALSE;
		vk_physicalDeviceFeatures.logicOp									= VK_FALSE;
		vk_physicalDeviceFeatures.multiDrawIndirect							= VK_FALSE;
		vk_physicalDeviceFeatures.drawIndirectFirstInstance					= VK_FALSE;
		vk_physicalDeviceFeatures.depthClamp								= VK_FALSE;
		vk_physicalDeviceFeatures.depthBiasClamp							= VK_FALSE;
		vk_physicalDeviceFeatures.fillModeNonSolid							= VK_FALSE;
		vk_physicalDeviceFeatures.depthBounds								= VK_FALSE;
		vk_physicalDeviceFeatures.wideLines									= VK_FALSE;
		vk_physicalDeviceFeatures.largePoints								= VK_FALSE;
		vk_physicalDeviceFeatures.alphaToOne								= VK_FALSE;
		vk_physicalDeviceFeatures.multiViewport								= VK_FALSE;
		vk_physicalDeviceFeatures.samplerAnisotropy							= VK_FALSE;
		vk_physicalDeviceFeatures.textureCompressionETC2					= VK_FALSE;
		vk_physicalDeviceFeatures.textureCompressionASTC_LDR				= VK_FALSE;
		vk_physicalDeviceFeatures.textureCompressionBC						= VK_FALSE;
		vk_physicalDeviceFeatures.occlusionQueryPrecise						= VK_FALSE;
		vk_physicalDeviceFeatures.pipelineStatisticsQuery					= VK_FALSE;
		vk_physicalDeviceFeatures.vertexPipelineStoresAndAtomics			= VK_FALSE;
		vk_physicalDeviceFeatures.fragmentStoresAndAtomics					= VK_FALSE;
		vk_physicalDeviceFeatures.shaderTessellationAndGeometryPointSize	= VK_FALSE;
		vk_physicalDeviceFeatures.shaderImageGatherExtended					= VK_FALSE;
		vk_physicalDeviceFeatures.shaderStorageImageExtendedFormats			= VK_FALSE;
		vk_physicalDeviceFeatures.shaderStorageImageMultisample				= VK_FALSE;
		vk_physicalDeviceFeatures.shaderStorageImageReadWithoutFormat		= VK_FALSE;
		vk_physicalDeviceFeatures.shaderStorageImageWriteWithoutFormat		= VK_FALSE;
		vk_physicalDeviceFeatures.shaderUniformBufferArrayDynamicIndexing	= VK_FALSE;
		vk_physicalDeviceFeatures.shaderSampledImageArrayDynamicIndexing	= VK_FALSE;
		vk_physicalDeviceFeatures.shaderStorageBufferArrayDynamicIndexing	= VK_FALSE;
		vk_physicalDeviceFeatures.shaderStorageImageArrayDynamicIndexing	= VK_FALSE;
		vk_physicalDeviceFeatures.shaderClipDistance						= VK_FALSE;
		vk_physicalDeviceFeatures.shaderCullDistance						= VK_FALSE;
		vk_physicalDeviceFeatures.shaderFloat64								= VK_FALSE;
		vk_physicalDeviceFeatures.shaderInt64								= VK_FALSE;
		vk_physicalDeviceFeatures.shaderInt16								= VK_FALSE;
		vk_physicalDeviceFeatures.shaderResourceResidency					= VK_FALSE;
		vk_physicalDeviceFeatures.shaderResourceMinLod						= VK_FALSE;
		vk_physicalDeviceFeatures.sparseBinding								= VK_FALSE;
		vk_physicalDeviceFeatures.sparseResidencyBuffer						= VK_FALSE;
		vk_physicalDeviceFeatures.sparseResidencyImage2D					= VK_FALSE;
		vk_physicalDeviceFeatures.sparseResidencyImage3D					= VK_FALSE;
		vk_physicalDeviceFeatures.sparseResidency2Samples					= VK_FALSE;
		vk_physicalDeviceFeatures.sparseResidency4Samples					= VK_FALSE;
		vk_physicalDeviceFeatures.sparseResidency8Samples					= VK_FALSE;
		vk_physicalDeviceFeatures.sparseResidency16Samples					= VK_FALSE;
		vk_physicalDeviceFeatures.sparseResidencyAliased					= VK_FALSE;
		vk_physicalDeviceFeatures.variableMultisampleRate					= VK_FALSE;
		vk_physicalDeviceFeatures.inheritedQueries							= VK_FALSE;
	}
	VkPhysicalDeviceMemoryProperties vk_physicalDeviceMemoryProperties;
	{
		vkGetPhysicalDeviceMemoryProperties(vk_physicalDevice, &vk_physicalDeviceMemoryProperties);
	}

	VkDevice vk_device;
	{
		VkDeviceCreateInfo vk_deviceCreateInfo;
		{
			vk_deviceCreateInfo.sType					= VkStructureType::VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
			vk_deviceCreateInfo.pNext					= nullptr;
			vk_deviceCreateInfo.flags					= 0;
			vk_deviceCreateInfo.queueCreateInfoCount	= vk_deviceQueueCreateInfos.size();
			vk_deviceCreateInfo.pQueueCreateInfos		= vk_deviceQueueCreateInfos.data();
			vk_deviceCreateInfo.enabledLayerCount		= vk_enabledPhysicalDeviceLayersNames.size();
			vk_deviceCreateInfo.ppEnabledLayerNames		= vk_enabledPhysicalDeviceLayersNames.data();
			vk_deviceCreateInfo.enabledExtensionCount	= vk_enabledPhysicalDeviceExtensionsNames.size();
			vk_deviceCreateInfo.ppEnabledExtensionNames	= vk_enabledPhysicalDeviceExtensionsNames.data();
			vk_deviceCreateInfo.pEnabledFeatures		= &vk_physicalDeviceFeatures;
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

	VkSurfaceKHR vk_surface;
	{
		if (auto isPresentationSupport = Boolean(vkGetPhysicalDeviceWin32PresentationSupportKHR(vk_physicalDevice, queueFamilyIndex)))
		{
			std::cout << "Presentation supported!" << std::endl;
		}
		else
		{
			throw Exception(); // TODO
		}

		VkWin32SurfaceCreateInfoKHR vk_win32SurfaceCreateInfo;
		{
			vk_win32SurfaceCreateInfo.sType		= VkStructureType::VK_STRUCTURE_TYPE_WIN32_SURFACE_CREATE_INFO_KHR;
			vk_win32SurfaceCreateInfo.pNext		= nullptr;
			vk_win32SurfaceCreateInfo.flags		= 0;
			vk_win32SurfaceCreateInfo.hinstance	= handleInstance;
			vk_win32SurfaceCreateInfo.hwnd		= handleWindow;
		}

		if (auto result = Result(vkCreateWin32SurfaceKHR(vk_instance, &vk_win32SurfaceCreateInfo, nullptr, &vk_surface)))
		{
			std::cout << "Surface successfully created!" << std::endl;
		}
		else
		{
			throw Exception(); // TODO
		}

		VkBool32 vk_isSufraceSupported;
		{
			if (auto result = Result(vkGetPhysicalDeviceSurfaceSupportKHR(vk_physicalDevice, queueFamilyIndex, vk_surface, &vk_isSufraceSupported)))
			{
				std::cout << "Sufrace support is sucessfully checked!" << std::endl;
			}
			else
			{
				throw Exception(); // TODO
			}
		}
		
		if (auto isSufraceSupported = Boolean(vk_isSufraceSupported))
		{
			std::cout << "Surface is supported!" << std::endl;
		}
		else
		{
			throw Exception();
		}
	}
	VkSurfaceCapabilitiesKHR vk_surfaceCapabilities;
	{
		if (auto result = Result(vkGetPhysicalDeviceSurfaceCapabilitiesKHR(vk_physicalDevice, vk_surface, &vk_surfaceCapabilities)))
		{
			std::cout << "Surface capabilities successfully obtained!" << std::endl;
		}
		else
		{
			throw Exception(); // TODO
		}
	}
	Vector<VkSurfaceFormatKHR> vk_surfaceFormats;
	{
		uint32_t vk_surfaceFormatsCount;
		if (auto result = Result(vkGetPhysicalDeviceSurfaceFormatsKHR(vk_physicalDevice, vk_surface, &vk_surfaceFormatsCount, nullptr)))
		{
			std::cout << "Surface formats succesfully enumerated!" << std::endl;
		}
		else
		{
			throw Exception(); // TODO
		}

		vk_surfaceFormats.resize(vk_surfaceFormatsCount);
		if (auto result = Result(vkGetPhysicalDeviceSurfaceFormatsKHR(vk_physicalDevice, vk_surface, &vk_surfaceFormatsCount, vk_surfaceFormats.data())))
		{
			std::cout << "Surface formats succesfully obtained!" << std::endl;
		}
		else
		{
			throw Exception(); // TODO
		}
	}
	auto vk_surfaceFormat = vk_surfaceFormats[0];

	VkSwapchainKHR vk_swapchain;
	{
		VkSwapchainCreateInfoKHR vk_swapchainCreateInfo;
		{
			vk_swapchainCreateInfo.sType					= VkStructureType::VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR;
			vk_swapchainCreateInfo.pNext					= nullptr;
			vk_swapchainCreateInfo.flags					= 0; // VkSwapchainCreateFlagBitsKHR::VK_SWAPCHAIN_CREATE_BIND_SFR_BIT_KHX;
			vk_swapchainCreateInfo.surface					= vk_surface;
			vk_swapchainCreateInfo.minImageCount			= std::max<uint32_t>(vk_surfaceCapabilities.minImageCount, 2);
			vk_swapchainCreateInfo.imageFormat				= vk_surfaceFormat.format;
			vk_swapchainCreateInfo.imageColorSpace			= vk_surfaceFormat.colorSpace;
			vk_swapchainCreateInfo.imageExtent				= windowSize;
			vk_swapchainCreateInfo.imageArrayLayers			= 1;
			vk_swapchainCreateInfo.imageUsage				= VkImageUsageFlagBits::VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;
			vk_swapchainCreateInfo.imageSharingMode			= VkSharingMode::VK_SHARING_MODE_EXCLUSIVE;
			vk_swapchainCreateInfo.queueFamilyIndexCount	= 0;
			vk_swapchainCreateInfo.pQueueFamilyIndices		= nullptr;
			vk_swapchainCreateInfo.preTransform				= VkSurfaceTransformFlagBitsKHR::VK_SURFACE_TRANSFORM_IDENTITY_BIT_KHR;
			vk_swapchainCreateInfo.compositeAlpha			= VkCompositeAlphaFlagBitsKHR::VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR;
			vk_swapchainCreateInfo.presentMode				= VkPresentModeKHR::VK_PRESENT_MODE_FIFO_KHR;
			vk_swapchainCreateInfo.clipped					= VK_FALSE;
			vk_swapchainCreateInfo.oldSwapchain				= VK_NULL_HANDLE;
		}

		if (auto result = Result(vkCreateSwapchainKHR(vk_device, &vk_swapchainCreateInfo, nullptr, &vk_swapchain)))
		{
			std::cout << "Swapchain successfully created!" << std::endl;
		}
		else
		{
			throw Exception(); // TODO
		}
	}
	uint32_t vk_swapchainImagesCount;
	{
		if (auto result = Result(vkGetSwapchainImagesKHR(vk_device, vk_swapchain, &vk_swapchainImagesCount, nullptr)))
		{
			std::cout << "Swapchain images succesfully enumerated!" << std::endl;
		}
		else
		{
			throw Exception();
		}
	}
	Vector<VkImage> vk_swapchainImages(vk_swapchainImagesCount);
	{
		if (auto result = Result(vkGetSwapchainImagesKHR(vk_device, vk_swapchain, &vk_swapchainImagesCount, vk_swapchainImages.data())))
		{
			std::cout << "Swapchain images succesfully obtained!" << std::endl;
		}
		else
		{
			throw Exception();
		}
	}
	Vector<VkImageView> vk_swapchainImageViews(vk_swapchainImages.size());
	{
		for (Size i = 0; i < vk_swapchainImages.size(); ++i)
		{
			auto &vk_swapchainImage = vk_swapchainImages[i];
			auto &vk_swapchainImageView = vk_swapchainImageViews[i];

			VkComponentMapping vk_componentMapping;
			{
				vk_componentMapping.r	= VkComponentSwizzle::VK_COMPONENT_SWIZZLE_R;
				vk_componentMapping.g	= VkComponentSwizzle::VK_COMPONENT_SWIZZLE_G;
				vk_componentMapping.b	= VkComponentSwizzle::VK_COMPONENT_SWIZZLE_B;
				vk_componentMapping.a	= VkComponentSwizzle::VK_COMPONENT_SWIZZLE_A;
			}
			VkImageSubresourceRange vk_imageSubresourceRange;
			{
				vk_imageSubresourceRange.aspectMask		= VkImageAspectFlagBits::VK_IMAGE_ASPECT_COLOR_BIT;
				vk_imageSubresourceRange.baseMipLevel	= 0;
				vk_imageSubresourceRange.levelCount		= 1;
				vk_imageSubresourceRange.baseArrayLayer	= 0;
				vk_imageSubresourceRange.layerCount		= 1;
			}
			VkImageViewCreateInfo vk_imageViewCreateInfo;
			{
				vk_imageViewCreateInfo.sType			= VkStructureType::VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
				vk_imageViewCreateInfo.pNext			= nullptr;
				vk_imageViewCreateInfo.flags			= 0;
				vk_imageViewCreateInfo.image			= vk_swapchainImage;
				vk_imageViewCreateInfo.viewType			= VkImageViewType::VK_IMAGE_VIEW_TYPE_2D;
				vk_imageViewCreateInfo.format			= vk_surfaceFormat.format;
				vk_imageViewCreateInfo.components		= vk_componentMapping;
				vk_imageViewCreateInfo.subresourceRange = vk_imageSubresourceRange;
			}

			if (auto result = Result(vkCreateImageView(vk_device, &vk_imageViewCreateInfo, nullptr, &vk_swapchainImageView)))
			{
				std::cout << "Swapchain image view successfully created!" << std::endl;
			}
			else
			{
				throw Exception(); // TODO
			}
		}
	}

	Vector<float> vertices = {
		-0.5f, +0.5f,
		+0.0f, -0.5f,
		+0.5f, +0.5f,
	};
	Size verticesMemoryTotalSize = sizeof(float) * vertices.size();
	VkBuffer vk_vertexBuffer;
	{
		VkBufferCreateInfo vk_bufferCreateInfo;
		{
			vk_bufferCreateInfo.sType					= VkStructureType::VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
			vk_bufferCreateInfo.pNext					= nullptr;
			vk_bufferCreateInfo.flags					= 0;
			vk_bufferCreateInfo.size					= verticesMemoryTotalSize;
			vk_bufferCreateInfo.usage					= VkBufferUsageFlagBits::VK_BUFFER_USAGE_VERTEX_BUFFER_BIT;
			vk_bufferCreateInfo.sharingMode				= VkSharingMode::VK_SHARING_MODE_EXCLUSIVE;
			vk_bufferCreateInfo.queueFamilyIndexCount	= 0;
			vk_bufferCreateInfo.pQueueFamilyIndices		= nullptr;
		}

		if (auto result = Result(vkCreateBuffer(vk_device, &vk_bufferCreateInfo, nullptr, &vk_vertexBuffer)))
		{
			std::cout << "Vertex buffer succesfully created!" << std::endl;
		}
		else
		{
			throw Exception(); // TODO
		}
	}
	VkDeviceMemory vk_vertexBufferDeviceMemory;
	{
		VkMemoryRequirements vk_memoryRequirements;
		{
			vkGetBufferMemoryRequirements(vk_device, vk_vertexBuffer, &vk_memoryRequirements);
		}

		VkMemoryAllocateInfo vk_memoryAllocateInfo;
		{
			vk_memoryAllocateInfo.sType				= VkStructureType::VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
			vk_memoryAllocateInfo.pNext				= nullptr;
			vk_memoryAllocateInfo.allocationSize	= vk_memoryRequirements.size;
			vk_memoryAllocateInfo.memoryTypeIndex	= [&](const VkMemoryPropertyFlags& properties) -> uint32_t
			{
				for (uint32_t i = 0; i < vk_physicalDeviceMemoryProperties.memoryTypeCount; ++i)
				{
					auto &memoryType = vk_physicalDeviceMemoryProperties.memoryTypes[i];

					if ((memoryType.propertyFlags & properties) == properties)
					{
						return i;
					}
				}

				throw Exception(); // TODO
			}(VkMemoryPropertyFlagBits::VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT);
		}

		if (auto result = Result(vkAllocateMemory(vk_device, &vk_memoryAllocateInfo, nullptr, &vk_vertexBufferDeviceMemory)))
		{
			std::cout << "Vertex buffer memory succesfully allocated!" << std::endl;
		}
		else
		{
			throw Exception(); // TODO
		}
		
		void* data;
		if (auto result = Result(vkMapMemory(vk_device, vk_vertexBufferDeviceMemory, 0, verticesMemoryTotalSize, 0, &data)))
		{
			std::cout << "Memory successfully mapped!" << std::endl;
		}
		else
		{
			throw Exception(); // TODO
		}

		std::memcpy(data, vertices.data(), verticesMemoryTotalSize);

		vkUnmapMemory(vk_device, vk_vertexBufferDeviceMemory);

		if (auto result = Result(vkBindBufferMemory(vk_device, vk_vertexBuffer, vk_vertexBufferDeviceMemory, 0)))
		{
			std::cout << "Memory successfully binded to the buffer!" << std::endl;
		}
		else
		{
			throw Exception(); // TODO
		}
	}

	VkRenderPass vk_renderPass;
	{
		Vector<VkAttachmentDescription> vk_attachmentDescriptions(1);
		{
			for (auto &vk_attachmentDescription : vk_attachmentDescriptions)
			{
				vk_attachmentDescription.flags			= 0; // VkAttachmentDescriptionFlagBits::VK_ATTACHMENT_DESCRIPTION_MAY_ALIAS_BIT;
				vk_attachmentDescription.format			= vk_surfaceFormat.format;
				vk_attachmentDescription.samples		= VkSampleCountFlagBits::VK_SAMPLE_COUNT_1_BIT;
				vk_attachmentDescription.loadOp			= VkAttachmentLoadOp::VK_ATTACHMENT_LOAD_OP_CLEAR;
				vk_attachmentDescription.storeOp		= VkAttachmentStoreOp::VK_ATTACHMENT_STORE_OP_STORE;
				vk_attachmentDescription.stencilLoadOp	= VkAttachmentLoadOp::VK_ATTACHMENT_LOAD_OP_DONT_CARE;
				vk_attachmentDescription.stencilStoreOp	= VkAttachmentStoreOp::VK_ATTACHMENT_STORE_OP_DONT_CARE;
				vk_attachmentDescription.initialLayout	= VkImageLayout::VK_IMAGE_LAYOUT_UNDEFINED;
				vk_attachmentDescription.finalLayout	= VkImageLayout::VK_IMAGE_LAYOUT_PRESENT_SRC_KHR;
			}
		}
		Vector<VkAttachmentReference> vk_attachmentReferences(1);
		{
			for (auto &vk_attachmentReference : vk_attachmentReferences)
			{
				vk_attachmentReference.attachment	= 0;
				vk_attachmentReference.layout		= VkImageLayout::VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;
			}
		}
		Vector<VkSubpassDescription> vk_subpassDescriptions(1);
		{
			for (auto &vk_subpassDescription : vk_subpassDescriptions)
			{
				vk_subpassDescription.flags						= 0;
				vk_subpassDescription.pipelineBindPoint			= VkPipelineBindPoint::VK_PIPELINE_BIND_POINT_GRAPHICS;
				vk_subpassDescription.inputAttachmentCount		= 0;
				vk_subpassDescription.pInputAttachments			= nullptr;
				vk_subpassDescription.colorAttachmentCount		= vk_attachmentReferences.size();
				vk_subpassDescription.pColorAttachments			= vk_attachmentReferences.data();
				vk_subpassDescription.pResolveAttachments		= nullptr;
				vk_subpassDescription.pDepthStencilAttachment	= nullptr;
				vk_subpassDescription.preserveAttachmentCount	= 0;
				vk_subpassDescription.pPreserveAttachments		= nullptr;
			}
		}
		VkRenderPassCreateInfo vk_renderPassCreateInfo;
		{
			vk_renderPassCreateInfo.sType			= VkStructureType::VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO;
			vk_renderPassCreateInfo.pNext			= nullptr;
			vk_renderPassCreateInfo.flags			= 0;
			vk_renderPassCreateInfo.attachmentCount	= vk_attachmentDescriptions.size();
			vk_renderPassCreateInfo.pAttachments	= vk_attachmentDescriptions.data();
			vk_renderPassCreateInfo.subpassCount	= vk_subpassDescriptions.size();
			vk_renderPassCreateInfo.pSubpasses		= vk_subpassDescriptions.data();
			vk_renderPassCreateInfo.dependencyCount = 0;
			vk_renderPassCreateInfo.pDependencies	= nullptr;
		}

		if (auto result = Result(vkCreateRenderPass(vk_device, &vk_renderPassCreateInfo, nullptr, &vk_renderPass)))
		{
			std::cout << "Render pass successfully created!" << std::endl;
		}
		else
		{
			throw Exception();
		}
	}

	VkShaderModule vk_vertexShaderModule;
	{
		auto source = loadShader("../../../../../Media/Shaders/Triangle Test/1.spir-v.vs");

		VkShaderModuleCreateInfo vk_shaderModuleCreateInfo;
		{
			vk_shaderModuleCreateInfo.sType		= VkStructureType::VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
			vk_shaderModuleCreateInfo.pNext		= nullptr;
			vk_shaderModuleCreateInfo.flags		= 0;
			vk_shaderModuleCreateInfo.codeSize	= source.size();
			vk_shaderModuleCreateInfo.pCode		= source.data();
		}

		if (auto result = Result(vkCreateShaderModule(vk_device, &vk_shaderModuleCreateInfo, nullptr, &vk_vertexShaderModule)))
		{
			std::cout << "Vertex shader successfully created!" << std::endl;
		}
		else
		{
			throw Exception();
		}
	}
	VkShaderModule vk_fragmentShaderModule;
	{
		auto source = loadShader("../../../../../Media/Shaders/Triangle Test/1.spir-v.fs");

		VkShaderModuleCreateInfo vk_shaderModuleCreateInfo;
		{
			vk_shaderModuleCreateInfo.sType		= VkStructureType::VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
			vk_shaderModuleCreateInfo.pNext		= nullptr;
			vk_shaderModuleCreateInfo.flags		= 0;
			vk_shaderModuleCreateInfo.codeSize	= source.size();
			vk_shaderModuleCreateInfo.pCode		= source.data();
		}

		if (auto result = Result(vkCreateShaderModule(vk_device, &vk_shaderModuleCreateInfo, nullptr, &vk_fragmentShaderModule)))
		{
			std::cout << "Fragment shader successfully created!" << std::endl;
		}
		else
		{
			throw Exception();
		}
	}

	VkPipelineLayout vk_pipelineLayout;
	{
		VkPipelineLayoutCreateInfo vk_pipelineLayoutCreateInfo;
		{
			vk_pipelineLayoutCreateInfo.sType					= VkStructureType::VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
			vk_pipelineLayoutCreateInfo.pNext					= nullptr;
			vk_pipelineLayoutCreateInfo.flags					= 0;
			vk_pipelineLayoutCreateInfo.setLayoutCount			= 0;
			vk_pipelineLayoutCreateInfo.pSetLayouts				= nullptr;
			vk_pipelineLayoutCreateInfo.pushConstantRangeCount	= 0;
			vk_pipelineLayoutCreateInfo.pPushConstantRanges		= nullptr;
		}

		if (auto result = Result(vkCreatePipelineLayout(vk_device, &vk_pipelineLayoutCreateInfo, nullptr, &vk_pipelineLayout)))
		{
			std::cout << "Pipeline layout successfully created!" << std::endl;
		}
		else
		{
			throw Exception(); // TODO
		}
	}
	VkPipeline vk_pipeline;
	{
		Array<VkPipelineShaderStageCreateInfo, 2> vk_shaderStageCreateInfos;
		{
			vk_shaderStageCreateInfos[0];
			{
				vk_shaderStageCreateInfos[0].sType					= VkStructureType::VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
				vk_shaderStageCreateInfos[0].pNext					= nullptr;
				vk_shaderStageCreateInfos[0].flags					= 0;
				vk_shaderStageCreateInfos[0].stage					= VkShaderStageFlagBits::VK_SHADER_STAGE_VERTEX_BIT;
				vk_shaderStageCreateInfos[0].module					= vk_vertexShaderModule;
				vk_shaderStageCreateInfos[0].pName					= "main";
				vk_shaderStageCreateInfos[0].pSpecializationInfo	= nullptr; // may be NULL
			}
			vk_shaderStageCreateInfos[1];
			{
				vk_shaderStageCreateInfos[1].sType					= VkStructureType::VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
				vk_shaderStageCreateInfos[1].pNext					= nullptr;
				vk_shaderStageCreateInfos[1].flags					= 0;
				vk_shaderStageCreateInfos[1].stage					= VkShaderStageFlagBits::VK_SHADER_STAGE_FRAGMENT_BIT;
				vk_shaderStageCreateInfos[1].module					= vk_fragmentShaderModule;
				vk_shaderStageCreateInfos[1].pName					= "main";
				vk_shaderStageCreateInfos[1].pSpecializationInfo	= nullptr; // may be NULL
			}
		}
		Array<VkVertexInputBindingDescription, 1> vk_vertexBindingDescriptions;
		{
			vk_vertexBindingDescriptions[0];
			{
				vk_vertexBindingDescriptions[0].binding		= 0;
				vk_vertexBindingDescriptions[0].stride		= sizeof(float) * 2;
				vk_vertexBindingDescriptions[0].inputRate	= VkVertexInputRate::VK_VERTEX_INPUT_RATE_VERTEX;
			}
		}
		Array<VkVertexInputAttributeDescription, 1> vk_vertexAttributeDescriptions;
		{
			vk_vertexAttributeDescriptions[0];
			{
				vk_vertexAttributeDescriptions[0].location	= 0;
				vk_vertexAttributeDescriptions[0].binding	= 0;
				vk_vertexAttributeDescriptions[0].format	= VkFormat::VK_FORMAT_R32G32_SFLOAT;
				vk_vertexAttributeDescriptions[0].offset	= 0;
			}
		}
		VkPipelineVertexInputStateCreateInfo vk_vertexInputStateCreateInfo;
		{
			vk_vertexInputStateCreateInfo.sType								= VkStructureType::VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO;
			vk_vertexInputStateCreateInfo.pNext								= nullptr;
			vk_vertexInputStateCreateInfo.flags								= 0;
			vk_vertexInputStateCreateInfo.vertexBindingDescriptionCount		= vk_vertexBindingDescriptions.size();
			vk_vertexInputStateCreateInfo.pVertexBindingDescriptions		= vk_vertexBindingDescriptions.data();
			vk_vertexInputStateCreateInfo.vertexAttributeDescriptionCount	= vk_vertexAttributeDescriptions.size();
			vk_vertexInputStateCreateInfo.pVertexAttributeDescriptions		= vk_vertexAttributeDescriptions.data();
		}
		VkPipelineInputAssemblyStateCreateInfo vk_inputAssemblyStateCreateInfo;
		{
			vk_inputAssemblyStateCreateInfo.sType					= VkStructureType::VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO;
			vk_inputAssemblyStateCreateInfo.pNext					= nullptr;
			vk_inputAssemblyStateCreateInfo.flags					= 0;
			vk_inputAssemblyStateCreateInfo.topology				= VkPrimitiveTopology::VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST;
			vk_inputAssemblyStateCreateInfo.primitiveRestartEnable	= VK_FALSE;
		}
		Array<VkViewport, 1> vk_viewports;
		{
			vk_viewports[0];
			{
				vk_viewports[0].x			= 0;
				vk_viewports[0].y			= 0;
				vk_viewports[0].width		= static_cast<float>(vk_surfaceCapabilities.currentExtent.width);
				vk_viewports[0].height		= static_cast<float>(vk_surfaceCapabilities.currentExtent.height);
				vk_viewports[0].minDepth	= 0.0f;
				vk_viewports[0].maxDepth	= 1.0f;
			}
		}
		Array<VkRect2D, 1> vk_scissors;
		{
			vk_scissors[0];
			{
				vk_scissors[0].offset.x			= 0;
				vk_scissors[0].offset.y			= 0;
				vk_scissors[0].extent.width		= vk_surfaceCapabilities.currentExtent.width;
				vk_scissors[0].extent.height	= vk_surfaceCapabilities.currentExtent.height;
			}
		}
		VkPipelineViewportStateCreateInfo vk_viewportStateCreateInfo;
		{
			vk_viewportStateCreateInfo.sType			= VkStructureType::VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO;
			vk_viewportStateCreateInfo.pNext			= nullptr;
			vk_viewportStateCreateInfo.flags			= 0;
			vk_viewportStateCreateInfo.viewportCount	= vk_viewports.size();
			vk_viewportStateCreateInfo.pViewports		= vk_viewports.data();
			vk_viewportStateCreateInfo.scissorCount		= vk_scissors.size();
			vk_viewportStateCreateInfo.pScissors		= vk_scissors.data();
		}
		VkPipelineRasterizationStateCreateInfo vk_rasterizationStateCreateInfo;
		{
			vk_rasterizationStateCreateInfo.sType					= VkStructureType::VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO;
			vk_rasterizationStateCreateInfo.pNext					= nullptr;
			vk_rasterizationStateCreateInfo.flags					= 0;
			vk_rasterizationStateCreateInfo.depthClampEnable		= VK_FALSE;
			vk_rasterizationStateCreateInfo.rasterizerDiscardEnable	= VK_FALSE;
			vk_rasterizationStateCreateInfo.polygonMode				= VkPolygonMode::VK_POLYGON_MODE_FILL;
			vk_rasterizationStateCreateInfo.cullMode				= VkCullModeFlagBits::VK_CULL_MODE_NONE;
			vk_rasterizationStateCreateInfo.frontFace				= VkFrontFace::VK_FRONT_FACE_COUNTER_CLOCKWISE;
			vk_rasterizationStateCreateInfo.depthBiasEnable			= VK_FALSE;
			vk_rasterizationStateCreateInfo.depthBiasConstantFactor	= 0.0f;
			vk_rasterizationStateCreateInfo.depthBiasClamp			= 0.0f;
			vk_rasterizationStateCreateInfo.depthBiasSlopeFactor	= 0.0f;
			vk_rasterizationStateCreateInfo.lineWidth				= 1.0f;
		}
		VkPipelineMultisampleStateCreateInfo vk_multisampleStateCreateInfo;
		{
			vk_multisampleStateCreateInfo.sType					= VkStructureType::VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO;
			vk_multisampleStateCreateInfo.pNext					= nullptr;
			vk_multisampleStateCreateInfo.flags					= 0;
			vk_multisampleStateCreateInfo.rasterizationSamples	= VkSampleCountFlagBits::VK_SAMPLE_COUNT_1_BIT;
			vk_multisampleStateCreateInfo.sampleShadingEnable	= VK_FALSE;
			vk_multisampleStateCreateInfo.minSampleShading		= 0.0f;
			vk_multisampleStateCreateInfo.pSampleMask			= nullptr;
			vk_multisampleStateCreateInfo.alphaToCoverageEnable	= VK_FALSE;
			vk_multisampleStateCreateInfo.alphaToOneEnable		= VK_FALSE;
		}
		VkStencilOpState stencilFront;
		{
			stencilFront.failOp			= VkStencilOp::VK_STENCIL_OP_KEEP;
			stencilFront.passOp			= VkStencilOp::VK_STENCIL_OP_KEEP;
			stencilFront.depthFailOp	= VkStencilOp::VK_STENCIL_OP_KEEP;
			stencilFront.compareOp		= VkCompareOp::VK_COMPARE_OP_ALWAYS;
			stencilFront.compareMask	= 0;
			stencilFront.writeMask		= 0;
			stencilFront.reference		= 0;
		}
		VkStencilOpState stencilBack;
		{
			stencilBack.failOp			= VkStencilOp::VK_STENCIL_OP_KEEP;
			stencilBack.passOp			= VkStencilOp::VK_STENCIL_OP_KEEP;
			stencilBack.depthFailOp		= VkStencilOp::VK_STENCIL_OP_KEEP;
			stencilBack.compareOp		= VkCompareOp::VK_COMPARE_OP_ALWAYS;
			stencilBack.compareMask		= 0;
			stencilBack.writeMask		= 0;
			stencilBack.reference		= 0;
		}
		VkPipelineDepthStencilStateCreateInfo vk_depthStencilStateCreateInfo;
		{
			vk_depthStencilStateCreateInfo.sType					= VkStructureType::VK_STRUCTURE_TYPE_PIPELINE_DEPTH_STENCIL_STATE_CREATE_INFO;
			vk_depthStencilStateCreateInfo.pNext					= nullptr;
			vk_depthStencilStateCreateInfo.flags					= 0;
			vk_depthStencilStateCreateInfo.depthTestEnable			= VK_FALSE;
			vk_depthStencilStateCreateInfo.depthWriteEnable			= VK_FALSE;
			vk_depthStencilStateCreateInfo.depthCompareOp			= VkCompareOp::VK_COMPARE_OP_ALWAYS;
			vk_depthStencilStateCreateInfo.depthBoundsTestEnable	= VK_FALSE;
			vk_depthStencilStateCreateInfo.stencilTestEnable		= VK_FALSE;
			vk_depthStencilStateCreateInfo.front					= stencilFront;
			vk_depthStencilStateCreateInfo.back						= stencilBack;
			vk_depthStencilStateCreateInfo.minDepthBounds			= 0.0f;
			vk_depthStencilStateCreateInfo.maxDepthBounds			= 0.0f;
		}
		Array<VkPipelineColorBlendAttachmentState, 1> vk_colorBlendAttachmentStates;
		{
			vk_colorBlendAttachmentStates[0];
			{
				vk_colorBlendAttachmentStates[0].blendEnable			= VK_FALSE;
				vk_colorBlendAttachmentStates[0].srcColorBlendFactor	= VkBlendFactor::VK_BLEND_FACTOR_ONE;
				vk_colorBlendAttachmentStates[0].dstColorBlendFactor	= VkBlendFactor::VK_BLEND_FACTOR_ONE;
				vk_colorBlendAttachmentStates[0].colorBlendOp			= VkBlendOp::VK_BLEND_OP_ADD;
				vk_colorBlendAttachmentStates[0].srcAlphaBlendFactor	= VkBlendFactor::VK_BLEND_FACTOR_ONE;
				vk_colorBlendAttachmentStates[0].dstAlphaBlendFactor	= VkBlendFactor::VK_BLEND_FACTOR_ONE;
				vk_colorBlendAttachmentStates[0].alphaBlendOp			= VkBlendOp::VK_BLEND_OP_ADD;
				vk_colorBlendAttachmentStates[0].colorWriteMask			=
					VkColorComponentFlagBits::VK_COLOR_COMPONENT_R_BIT |
					VkColorComponentFlagBits::VK_COLOR_COMPONENT_G_BIT |
					VkColorComponentFlagBits::VK_COLOR_COMPONENT_B_BIT;
			}
		}
		VkPipelineColorBlendStateCreateInfo vk_pipelineColorBlendStateCreateInfo;
		{
			vk_pipelineColorBlendStateCreateInfo.sType					= VkStructureType::VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO;
			vk_pipelineColorBlendStateCreateInfo.pNext					= nullptr;
			vk_pipelineColorBlendStateCreateInfo.flags					= 0;
			vk_pipelineColorBlendStateCreateInfo.logicOpEnable			= VK_FALSE;
			vk_pipelineColorBlendStateCreateInfo.logicOp				= VkLogicOp::VK_LOGIC_OP_CLEAR;
			vk_pipelineColorBlendStateCreateInfo.attachmentCount		= vk_colorBlendAttachmentStates.size();
			vk_pipelineColorBlendStateCreateInfo.pAttachments			= vk_colorBlendAttachmentStates.data();
			vk_pipelineColorBlendStateCreateInfo.blendConstants;
			{
				vk_pipelineColorBlendStateCreateInfo.blendConstants[0]	= 0.0f;
				vk_pipelineColorBlendStateCreateInfo.blendConstants[1]	= 0.0f;
				vk_pipelineColorBlendStateCreateInfo.blendConstants[2]	= 0.0f;
				vk_pipelineColorBlendStateCreateInfo.blendConstants[3]	= 0.0f;
			}
		}
		VkGraphicsPipelineCreateInfo vk_graphicsPipelineCreateInfo;
		{
			vk_graphicsPipelineCreateInfo.sType					= VkStructureType::VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO;
			vk_graphicsPipelineCreateInfo.pNext					= nullptr;
			vk_graphicsPipelineCreateInfo.flags					= 0;
			vk_graphicsPipelineCreateInfo.stageCount			= vk_shaderStageCreateInfos.size();
			vk_graphicsPipelineCreateInfo.pStages				= vk_shaderStageCreateInfos.data();
			vk_graphicsPipelineCreateInfo.pVertexInputState		= &vk_vertexInputStateCreateInfo;
			vk_graphicsPipelineCreateInfo.pInputAssemblyState	= &vk_inputAssemblyStateCreateInfo;
			vk_graphicsPipelineCreateInfo.pTessellationState	= nullptr;
			vk_graphicsPipelineCreateInfo.pViewportState		= &vk_viewportStateCreateInfo;
			vk_graphicsPipelineCreateInfo.pRasterizationState	= &vk_rasterizationStateCreateInfo;
			vk_graphicsPipelineCreateInfo.pMultisampleState		= &vk_multisampleStateCreateInfo;
			vk_graphicsPipelineCreateInfo.pDepthStencilState	= &vk_depthStencilStateCreateInfo;
			vk_graphicsPipelineCreateInfo.pColorBlendState		= &vk_pipelineColorBlendStateCreateInfo;
			vk_graphicsPipelineCreateInfo.pDynamicState			= nullptr;
			vk_graphicsPipelineCreateInfo.layout				= vk_pipelineLayout;
			vk_graphicsPipelineCreateInfo.renderPass			= vk_renderPass;
			vk_graphicsPipelineCreateInfo.subpass				= 0;
			vk_graphicsPipelineCreateInfo.basePipelineHandle	= VK_NULL_HANDLE;
			vk_graphicsPipelineCreateInfo.basePipelineIndex		= -1;
		}

		if (auto result = Result(vkCreateGraphicsPipelines(
			vk_device,
			VK_NULL_HANDLE, // may be NULL
			1, &vk_graphicsPipelineCreateInfo,
			nullptr,
			&vk_pipeline
		)))
		{
			std::cout << "Pipeline successfully created!" << std::endl;
		}
		else
		{
			throw Exception(); // TODO
		}
	}

	vkDestroyShaderModule(vk_device, vk_vertexShaderModule, nullptr);
	vkDestroyShaderModule(vk_device, vk_fragmentShaderModule, nullptr);

	VkQueue vk_queue = VK_NULL_HANDLE;
	{
		vkGetDeviceQueue(vk_device, 0, 0, &vk_queue);
	}

	VkCommandPool vk_commandPool;
	{
		VkCommandPoolCreateInfo vk_commandPoolCreateInfo;
		{
			vk_commandPoolCreateInfo.sType				= VkStructureType::VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
			vk_commandPoolCreateInfo.pNext				= nullptr;
			vk_commandPoolCreateInfo.flags				= VkCommandPoolCreateFlagBits::VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT;
			vk_commandPoolCreateInfo.queueFamilyIndex	= 0;
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

	Vector<VkCommandBuffer> vk_commandBuffers(vk_swapchainImages.size());
	Vector<VkFramebuffer> vk_framebuffers(vk_swapchainImages.size());
	{
		VkCommandBufferAllocateInfo vk_commandBufferAllocateInfo;
		{
			vk_commandBufferAllocateInfo.sType				= VkStructureType::VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
			vk_commandBufferAllocateInfo.pNext				= nullptr;
			vk_commandBufferAllocateInfo.commandPool		= vk_commandPool;
			vk_commandBufferAllocateInfo.level				= VkCommandBufferLevel::VK_COMMAND_BUFFER_LEVEL_PRIMARY;
			vk_commandBufferAllocateInfo.commandBufferCount	= vk_commandBuffers.size();
		}

		if (auto result = Result(vkAllocateCommandBuffers(vk_device, &vk_commandBufferAllocateInfo, vk_commandBuffers.data())))
		{
			std::cout << "Command buffers successfully allocated!" << std::endl;
		}
		else
		{
			throw Exception(); // TODO
		}
		
		for (Size i = 0; i < vk_swapchainImages.size(); ++i)
		{
			auto &vk_commandBuffer = vk_commandBuffers[i];
			{
				if (auto result = Result(vkResetCommandBuffer(vk_commandBuffer, VkCommandBufferResetFlagBits::VK_COMMAND_BUFFER_RESET_RELEASE_RESOURCES_BIT)))
				{
					std::cout << "Command buffer successfully reset!" << std::endl;
				}
				else
				{
					throw Exception(); // TODO
				}
			}

			auto &vk_swapchainImageView = vk_swapchainImageViews[i];
			auto &vk_framebuffer = vk_framebuffers[i];
			{
				VkFramebufferCreateInfo vk_framebufferCreateInfo;
				{
					vk_framebufferCreateInfo.sType				= VkStructureType::VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
					vk_framebufferCreateInfo.pNext				= nullptr;
					vk_framebufferCreateInfo.flags				= 0;
					vk_framebufferCreateInfo.renderPass			= vk_renderPass;
					vk_framebufferCreateInfo.attachmentCount	= 1;
					vk_framebufferCreateInfo.pAttachments		= &vk_swapchainImageView; // TODO
					vk_framebufferCreateInfo.width				= vk_surfaceCapabilities.currentExtent.width;
					vk_framebufferCreateInfo.height				= vk_surfaceCapabilities.currentExtent.height;
					vk_framebufferCreateInfo.layers				= 1;
				}

				if (auto result = Result(vkCreateFramebuffer(vk_device, &vk_framebufferCreateInfo, nullptr, &vk_framebuffer)))
				{
					std::cout << "Framebuffer successfully created!" << std::endl;
				}
				else
				{
					throw Exception(); // TODO
				}
			}

			VkCommandBufferBeginInfo vk_commandBufferBeginInfo;
			{
				vk_commandBufferBeginInfo.sType				= VkStructureType::VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
				vk_commandBufferBeginInfo.pNext				= nullptr;
				vk_commandBufferBeginInfo.flags				= 0;
				vk_commandBufferBeginInfo.pInheritanceInfo	= nullptr;
			}

			if (auto result = Result(vkBeginCommandBuffer(vk_commandBuffer, &vk_commandBufferBeginInfo)))
			{
				std::cout << "Command buffer recording successfully begin!" << std::endl;
			}
			else
			{
				throw Exception(); // TODO
			}

			VkRect2D vk_renderArea;
			{
				vk_renderArea.offset.x = 0;
				vk_renderArea.offset.y = 0;
				vk_renderArea.extent = vk_surfaceCapabilities.currentExtent;
			}
			VkClearValue vk_clearValue;
			{
				// vk_clearValue.color.uint32[0] = 0xFFFFFFFF;
				// vk_clearValue.color.uint32[1] = 0xEFFFFFFF;
				// vk_clearValue.color.uint32[2] = 0x00000000;
				// vk_clearValue.color.uint32[3] = 0xFFFFFFFF;
				vk_clearValue.color.float32[0] = 1.0f;
				vk_clearValue.color.float32[1] = 0.5f;
				vk_clearValue.color.float32[2] = 0.0f;
				vk_clearValue.color.float32[3] = 1.0f;
			}
			VkRenderPassBeginInfo vk_renderPassBeginInfo;
			{
				vk_renderPassBeginInfo.sType			= VkStructureType::VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
				vk_renderPassBeginInfo.pNext			= nullptr;
				vk_renderPassBeginInfo.renderPass		= vk_renderPass;
				vk_renderPassBeginInfo.framebuffer		= vk_framebuffer;
				vk_renderPassBeginInfo.renderArea		= vk_renderArea;
				vk_renderPassBeginInfo.clearValueCount	= 1;
				vk_renderPassBeginInfo.pClearValues		= &vk_clearValue;
			}

			vkCmdBeginRenderPass(vk_commandBuffer, &vk_renderPassBeginInfo, VkSubpassContents::VK_SUBPASS_CONTENTS_INLINE);

			vkCmdBindPipeline(vk_commandBuffer, VkPipelineBindPoint::VK_PIPELINE_BIND_POINT_GRAPHICS, vk_pipeline);

			VkDeviceSize offset = 0;
			vkCmdBindVertexBuffers(vk_commandBuffer, 0, 1, &vk_vertexBuffer, &offset);

			vkCmdDraw(vk_commandBuffer, 3, 1, 0, 0);

			vkCmdEndRenderPass(vk_commandBuffer);

			if (auto result = Result(vkEndCommandBuffer(vk_commandBuffer)))
			{
				std::cout << "Command buffer recording successfully end!" << std::endl;
			}
			else
			{
				throw Exception(); // TODO
			}
		}
	}

	VkFence vk_fence;
	{
		VkFenceCreateInfo vk_fenceCreateInfo;
		{
			vk_fenceCreateInfo.sType	= VkStructureType::VK_STRUCTURE_TYPE_FENCE_CREATE_INFO;
			vk_fenceCreateInfo.pNext	= nullptr;
			vk_fenceCreateInfo.flags	= 0; // VkFenceCreateFlagBits::VK_FENCE_CREATE_SIGNALED_BIT;
		}

		if (auto result = Result(vkCreateFence(vk_device, &vk_fenceCreateInfo, nullptr, &vk_fence)))
		{
			std::cout << "Fence successfully created!" << std::endl;
		}
		else
		{
			throw Exception(); // TODO
		}
	}

	while (!GetAsyncKeyState(VK_ESCAPE))
	{
		MSG msg;
		{
			while (PeekMessage(&msg, handleWindow, 0, 0, PM_REMOVE))
			{
				TranslateMessage(&msg);
				DispatchMessage(&msg);
			}
		}

		uint32_t vk_nextImageIndex;
		if (auto result = Result(vkAcquireNextImageKHR(vk_device, vk_swapchain, UINT64_MAX, VK_NULL_HANDLE, vk_fence, &vk_nextImageIndex)))
		{
			std::cout << "Query for next image is successfull!" << std::endl;
		}
		else
		{
			throw Exception(); // TODO
		}

		if (auto result = Result(vkWaitForFences(vk_device, 1, &vk_fence, VK_FALSE, UINT64_MAX)))
		{
			std::cout << "Wait for fence is successfull!" << std::endl;
		}
		else
		{
			throw Exception(); // TODO
		}

		if (auto result = Result(vkResetFences(vk_device, 1, &vk_fence)))
		{
			std::cout << "Fence successfully reset!" << std::endl;
		}
		else
		{
			throw Exception(); // TODO
		}

		auto &vk_commandBuffer = vk_commandBuffers[vk_nextImageIndex];

		VkSubmitInfo vk_submitInfo;
		{
			vk_submitInfo.sType = VkStructureType::VK_STRUCTURE_TYPE_SUBMIT_INFO;
			vk_submitInfo.pNext = nullptr;
			vk_submitInfo.waitSemaphoreCount = 0;
			vk_submitInfo.pWaitSemaphores = nullptr;
			vk_submitInfo.pWaitDstStageMask = nullptr;
			vk_submitInfo.commandBufferCount = 1;
			vk_submitInfo.pCommandBuffers = &vk_commandBuffer;
			vk_submitInfo.signalSemaphoreCount = 0;
			vk_submitInfo.pSignalSemaphores = nullptr;
		}
		if (auto result = Result(vkQueueSubmit(vk_queue, 1, &vk_submitInfo, VK_NULL_HANDLE)))
		{
			std::cout << "Command successfully submitted!" << std::endl;
		}
		else
		{
			throw Exception(); // TODO
		}

		if (auto result = Result(vkQueueWaitIdle(vk_queue)))
		{
			std::cout << "Wait for queue is successfull" << std::endl;
		}
		else
		{
			throw Exception(); // TODO
		}

		VkPresentInfoKHR vk_presentInfoKHR;
		{
			vk_presentInfoKHR.sType = VkStructureType::VK_STRUCTURE_TYPE_PRESENT_INFO_KHR;
			vk_presentInfoKHR.pNext = nullptr;
			vk_presentInfoKHR.waitSemaphoreCount = 0;
			vk_presentInfoKHR.pWaitSemaphores = nullptr;
			vk_presentInfoKHR.swapchainCount = 1;
			vk_presentInfoKHR.pSwapchains = &vk_swapchain;
			vk_presentInfoKHR.pImageIndices = &vk_nextImageIndex;
			vk_presentInfoKHR.pResults = nullptr; // may be NULL
		}
		if (auto result = Result(vkQueuePresentKHR(vk_queue, &vk_presentInfoKHR)))
		{
			std::cout << "Presentation is successfull!" << std::endl;
		}
		else
		{
			throw Exception(); // TODO
		}

		break;
	}

	vkDestroyPipeline(vk_device, vk_pipeline, nullptr);
	vkDestroyPipelineLayout(vk_device, vk_pipelineLayout, nullptr);
	vkDestroyBuffer(vk_device, vk_vertexBuffer, nullptr);
	vkFreeMemory(vk_device, vk_vertexBufferDeviceMemory, nullptr);
	for (auto &vk_framebuffer : vk_framebuffers)
	{
		vkDestroyFramebuffer(vk_device, vk_framebuffer, nullptr);
	}
	for (auto &vk_swapchainImageView : vk_swapchainImageViews)
	{
		vkDestroyImageView(vk_device, vk_swapchainImageView, nullptr);
	}
	vkFreeCommandBuffers(vk_device, vk_commandPool, vk_commandBuffers.size(), vk_commandBuffers.data());
	vkDestroyCommandPool(vk_device, vk_commandPool, nullptr);
	vkDestroyRenderPass(vk_device, vk_renderPass, nullptr);
	vkDestroyFence(vk_device, vk_fence, nullptr);
	vkDestroySwapchainKHR(vk_device, vk_swapchain, nullptr);
	vkDestroySurfaceKHR(vk_instance, vk_surface, nullptr);
	vkDestroyDevice(vk_device, nullptr);
	vkDestroyInstance(vk_instance, nullptr);

	DestroyWindow(handleWindow);
}

void main()
{
	func();

	std::system("pause");
}