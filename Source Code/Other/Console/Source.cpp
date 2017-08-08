#include <iostream>

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
	+		[0]	0x06c7bc70 "VK_LAYER_LUNARG_core_validation"	const char *
	+		[1]	0x06c7bd18 "VK_LAYER_LUNARG_monitor"	const char *
	+		[2]	0x06c7bdc0 "VK_LAYER_LUNARG_object_tracker"	const char *
	+		[3]	0x06c7be90 "VK_LAYER_LUNARG_parameter_validation"	const char *
	+		[4]	0x06c7bf48 "VK_LAYER_LUNARG_screenshot"	const char *
	+		[5]	0x06c7bff0 "VK_LAYER_GOOGLE_threading"	const char *
	+		[6]	0x06c7c098 "VK_LAYER_GOOGLE_unique_objects"	const char *
	+		[7]	0x06c7c140 "VK_LAYER_RENDERDOC_Capture"	const char *
	*/

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

		DWORD style = WS_OVERLAPPEDWINDOW | WS_VISIBLE;

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
#if _DEBUG
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
#endif
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
#if _DEBUG
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
#endif
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

	VkDevice vk_device;
	{
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
	}

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
}

void main()
{
	func();

	// std::system("pause");
}