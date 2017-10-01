#pragma region Include

#pragma once

#include "Header.hpp"

#pragma endregion

namespace GreatVulkan
{
	// TODO: move to other file
	class ApplicationInfo:
		public VkApplicationInfo
	{
	public:
		using APIVersion = decltype(VkApplicationInfo::apiVersion);
	protected:
		const String applicationNameContainer;
		const String engineNameContainer;
	public:
		inline ApplicationInfo() = delete;
		inline ApplicationInfo(const String& applicationName_, const Size& applicationVersion_, const String& engineName_, const Size& engineVersion_, const APIVersion& apiVersion_);
		inline ApplicationInfo(const ApplicationInfo&) = delete;
		inline ~ApplicationInfo() = default;
	public:
		inline ApplicationInfo& operator = (const ApplicationInfo&) = delete;
	};
	class InstanceCreateInfo:
		public VkInstanceCreateInfo
	{
	public:
		inline InstanceCreateInfo() = delete;
		inline InstanceCreateInfo(const Vector<const char*>& enabledLayersNames_, const Vector<const char*>& enabledExtensionsNames_);
		inline InstanceCreateInfo(const VkApplicationInfo& vk_applicationInfo_, const Vector<const char*>& enabledLayersNames_, const Vector<const char*>& enabledExtensionsNames_);
		inline InstanceCreateInfo(const VkApplicationInfo* vk_applicationInfo_, const Vector<const char*>& enabledLayersNames_, const Vector<const char*>& enabledExtensionsNames_);
		inline InstanceCreateInfo(const InstanceCreateInfo&) = delete;
		inline ~InstanceCreateInfo() = default;
	public:
		inline InstanceCreateInfo& operator = (const InstanceCreateInfo&) = delete;
	};
	class DeviceCreateInfo:
		public VkDeviceCreateInfo
	{
	public:
		inline DeviceCreateInfo() = delete;
		inline DeviceCreateInfo(const Vector<VkDeviceQueueCreateInfo>& vk_deviceQueueCreateInfos_, const Vector<const char*>& vk_enabledPhysicalDeviceLayersNames_, const Vector<const char*>& vk_enabledPhysicalDeviceExtensionsNames_, const VkPhysicalDeviceFeatures& vk_physicalDeviceFeatures_);
		inline DeviceCreateInfo(const Vector<VkDeviceQueueCreateInfo>& vk_deviceQueueCreateInfos_, const Vector<const char*>& vk_enabledPhysicalDeviceLayersNames_, const Vector<const char*>& vk_enabledPhysicalDeviceExtensionsNames_, const VkPhysicalDeviceFeatures* vk_physicalDeviceFeatures_);
		inline DeviceCreateInfo(const DeviceCreateInfo&) = delete;
		inline ~DeviceCreateInfo() = default;
	public:
		inline DeviceCreateInfo& operator = (const DeviceCreateInfo&) = delete;
	};
	class DeviceQueueCreateInfo:
		public VkDeviceQueueCreateInfo
	{
	public:
		inline DeviceQueueCreateInfo() = delete;
		inline DeviceQueueCreateInfo(const decltype(VkDeviceQueueCreateInfo::queueFamilyIndex)& queueFamilyIndex_, const Vector<float>& queuePriorities_);
		inline DeviceQueueCreateInfo(const DeviceQueueCreateInfo&) = delete;
		inline ~DeviceQueueCreateInfo() = default;
	public:
		inline DeviceQueueCreateInfo& operator = (const DeviceQueueCreateInfo&) = delete;
	};
	class PhysicalDeviceFeatures:
		public VkPhysicalDeviceFeatures
	{
	public:
		enum class Feature
		{
			RobustBufferAccess,
			FullDrawIndexUint32,
			ImageCubeArray,
			IndependentBlend,
			GeometryShader,
			TessellationShader,
			SampleRateShading,
			DualSrcBlend,
			LogicOp,
			MultiDrawIndirect,
			DrawIndirectFirstInstance,
			DepthClamp,
			DepthBiasClamp,
			FillModeNonSolid,
			DepthBounds,
			WideLines,
			LargePoints,
			AlphaToOne,
			MultiViewport,
			SamplerAnisotropy,
			TextureCompressionETC2,
			TextureCompressionASTC_LDR,
			TextureCompressionBC,
			OcclusionQueryPrecise,
			PipelineStatisticsQuery,
			VertexPipelineStoresAndAtomics,
			FragmentStoresAndAtomics,
			ShaderTessellationAndGeometryPointSize,
			ShaderImageGatherExtended,
			ShaderStorageImageExtendedFormats,
			ShaderStorageImageMultisample,
			ShaderStorageImageReadWithoutFormat,
			ShaderStorageImageWriteWithoutFormat,
			ShaderUniformBufferArrayDynamicIndexing,
			ShaderSampledImageArrayDynamicIndexing,
			ShaderStorageBufferArrayDynamicIndexing,
			ShaderStorageImageArrayDynamicIndexing,
			ShaderClipDistance,
			ShaderCullDistance,
			ShaderFloat64,
			ShaderInt64,
			ShaderInt16,
			ShaderResourceResidency,
			ShaderResourceMinLod,
			SparseBinding,
			SparseResidencyBuffer,
			SparseResidencyImage2D,
			SparseResidencyImage3D,
			SparseResidency2Samples,
			SparseResidency4Samples,
			SparseResidency8Samples,
			SparseResidency16Samples,
			SparseResidencyAliased,
			VariableMultisampleRate,
			InheritedQueries,
		};
	protected:
		inline static VkBool32 Has(const Initializer<Feature>& features_, const Feature& feature_);
	public:
		inline PhysicalDeviceFeatures() = delete;
		inline PhysicalDeviceFeatures(const Initializer<Feature>& features_);
		inline PhysicalDeviceFeatures(const PhysicalDeviceFeatures&) = delete;
		inline ~PhysicalDeviceFeatures() = default;
	public:
		inline PhysicalDeviceFeatures& operator = (const PhysicalDeviceFeatures&) = delete;
	};

#if VK_USE_PLATFORM_WIN32_KHR
	class Win32SurfaceCreateInfoKHR:
		public VkWin32SurfaceCreateInfoKHR
	{
	public:
		inline Win32SurfaceCreateInfoKHR() = delete;
		inline Win32SurfaceCreateInfoKHR(const decltype(VkWin32SurfaceCreateInfoKHR::hinstance)& handleInstance_, const decltype(VkWin32SurfaceCreateInfoKHR::hwnd)& handleWindow_);
		inline Win32SurfaceCreateInfoKHR(const Win32SurfaceCreateInfoKHR&) = delete;
		inline ~Win32SurfaceCreateInfoKHR() = default;
	public:
		inline Win32SurfaceCreateInfoKHR& operator = (const Win32SurfaceCreateInfoKHR&) = delete;
	};
#endif

	
	// Instance
	inline Vector<VkLayerProperties> EnumerateInstanceLayerProperties();
	inline Vector<const char*> ExtractInstanceLayersNames(const Vector<VkLayerProperties>& vk_instanceLayers_);
	inline Vector<VkExtensionProperties> EnumerateInstanceExtensionProperties(const char* vk_instanceLayerName_);
	inline Map<String, Vector<VkExtensionProperties>> EnumerateInstanceExtensionsProperties(const Vector<const char*>& vk_instanceLayerNames_);
	inline Vector<const char*> ExtractInstanceExtensionsNames(const Map<String, Vector<VkExtensionProperties>>& instanceExtensions_);
	inline VkInstance CreateInstance(const VkInstanceCreateInfo& vk_instanceCreateInfo_);
	inline void DestroyInstance(const VkInstance& vk_instance_);

	// PhysicalDevices
	inline Vector<VkPhysicalDevice> EnumeratePhysicalDevices(const VkInstance& vk_instance_);
	inline VkPhysicalDeviceProperties GetPhysicalDeviceProperties(const VkPhysicalDevice& vk_physicalDevice_);
	inline Vector<VkQueueFamilyProperties> GetPhysicalDeviceQueueFamilyProperties(const VkPhysicalDevice& vk_physicalDevice_);
	inline VkPhysicalDeviceMemoryProperties GetPhysicalDeviceMemoryProperties(const VkPhysicalDevice& vk_physicalDevice_);

	// Device
	inline Vector<VkLayerProperties> EnumerateDeviceLayerProperties(const VkPhysicalDevice& vk_physicalDevice_);
	inline Vector<const char*> ExtractDeviceLayersNames(const Vector<VkLayerProperties>& vk_deviceLayers_);
	inline Vector<VkExtensionProperties> EnumerateDeviceExtensionProperties(const VkPhysicalDevice& vk_physicalDevice_, const char* vk_deviceLayerName_);
	inline Map<String, Vector<VkExtensionProperties>> EnumerateDeviceExtensionsProperties(const VkPhysicalDevice& vk_physicalDevice_, const Vector<const char*>& vk_deviceLayerNames_);
	inline Vector<const char*> ExtractDeviceExtensionsNames(const Map<String, Vector<VkExtensionProperties>>& deviceExtensions_);
	inline VkDevice CreateDevice(const VkPhysicalDevice& vk_physicalDevice_, const VkDeviceCreateInfo& vk_deviceCreateInfo_);
	inline void DestroyDevice(const VkDevice& vk_device_);

#if VK_USE_PLATFORM_WIN32_KHR
	// Win32SurfaceKHR
	inline VkBool32 GetPhysicalDeviceWin32PresentationSupportKHR(const VkPhysicalDevice& vk_physicalDevice_, const Size& queueFamilyIndex_);
	inline VkSurfaceKHR CreateWin32SurfaceKHR(const VkInstance& vk_instance_, const VkWin32SurfaceCreateInfoKHR& vk_win32SurfaceCreateInfo_);
	inline VkBool32 GetPhysicalDeviceSurfaceSupportKHR(const VkPhysicalDevice& vk_physicalDevice_, const decltype(VkDeviceQueueCreateInfo::queueFamilyIndex)& vk_queueFamilyIndex_, const VkSurfaceKHR& vk_surface_);
	inline VkSurfaceCapabilitiesKHR GetPhysicalDeviceSurfaceCapabilitiesKHR(const VkPhysicalDevice& vk_physicalDevice_, const VkSurfaceKHR& vk_surface_);
	inline Vector<VkSurfaceFormatKHR> GetPhysicalDeviceSurfaceFormatsKHR(const VkPhysicalDevice& vk_physicalDevice_, const VkSurfaceKHR& vk_surface_);
#endif
}


#pragma region GreatVulkan

#pragma region ApplicationInfo

inline GreatVulkan::ApplicationInfo::ApplicationInfo(const String& applicationName_, const Size& applicationVersion_, const String& engineName_, const Size& engineVersion_, const APIVersion& apiVersion_):
	applicationNameContainer(applicationName_),
	engineNameContainer(engineName_)
{
	// TODO: move to initialization
	sType				= VkStructureType::VK_STRUCTURE_TYPE_APPLICATION_INFO;
	pNext				= nullptr;
	pApplicationName	= applicationNameContainer.c_str();
	applicationVersion	= applicationVersion_;
	pEngineName			= engineNameContainer.c_str();
	engineVersion		= engineVersion_;
	apiVersion			= apiVersion_;
}

#pragma endregion

#pragma region InstanceCreateInfo

inline GreatVulkan::InstanceCreateInfo::InstanceCreateInfo(const Vector<const char*>& enabledLayersNames_, const Vector<const char*>& enabledExtensionsNames_):
	InstanceCreateInfo(nullptr, enabledLayersNames_, enabledExtensionsNames_)
{
}
inline GreatVulkan::InstanceCreateInfo::InstanceCreateInfo(const VkApplicationInfo& vk_applicationInfo_, const Vector<const char*>& enabledLayersNames_, const Vector<const char*>& enabledExtensionsNames_) :
	InstanceCreateInfo(&vk_applicationInfo_, enabledLayersNames_, enabledExtensionsNames_)
{
}
inline GreatVulkan::InstanceCreateInfo::InstanceCreateInfo(const VkApplicationInfo* vk_applicationInfo_, const Vector<const char*>& enabledLayersNames_, const Vector<const char*>& enabledExtensionsNames_)
{
	// TODO: move to initialization
	sType						= VkStructureType::VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
	pNext						= nullptr;
	flags						= 0;
	pApplicationInfo			= vk_applicationInfo_;
	enabledLayerCount			= enabledLayersNames_.size();
	ppEnabledLayerNames			= enabledLayersNames_.data();
	enabledExtensionCount		= enabledExtensionsNames_.size();
	ppEnabledExtensionNames		= enabledExtensionsNames_.data();
}

#pragma endregion

#pragma region DeviceCreateInfo

inline GreatVulkan::DeviceCreateInfo::DeviceCreateInfo(const Vector<VkDeviceQueueCreateInfo>& vk_deviceQueueCreateInfos_, const Vector<const char*>& vk_enabledPhysicalDeviceLayersNames_, const Vector<const char*>& vk_enabledPhysicalDeviceExtensionsNames_, const VkPhysicalDeviceFeatures& vk_physicalDeviceFeatures_):
	DeviceCreateInfo(vk_deviceQueueCreateInfos_, vk_enabledPhysicalDeviceLayersNames_, vk_enabledPhysicalDeviceExtensionsNames_, &vk_physicalDeviceFeatures_)
{
}
inline GreatVulkan::DeviceCreateInfo::DeviceCreateInfo(const Vector<VkDeviceQueueCreateInfo>& vk_deviceQueueCreateInfos_, const Vector<const char*>& vk_enabledPhysicalDeviceLayersNames_, const Vector<const char*>& vk_enabledPhysicalDeviceExtensionsNames_, const VkPhysicalDeviceFeatures* vk_physicalDeviceFeatures_)
{
	sType					= VkStructureType::VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
	pNext					= nullptr;
	flags					= 0;
	queueCreateInfoCount	= vk_deviceQueueCreateInfos_.size();
	pQueueCreateInfos		= vk_deviceQueueCreateInfos_.data();
	enabledLayerCount		= vk_enabledPhysicalDeviceLayersNames_.size();
	ppEnabledLayerNames		= vk_enabledPhysicalDeviceLayersNames_.data();
	enabledExtensionCount	= vk_enabledPhysicalDeviceExtensionsNames_.size();
	ppEnabledExtensionNames	= vk_enabledPhysicalDeviceExtensionsNames_.data();
	pEnabledFeatures		= vk_physicalDeviceFeatures_;
}

#pragma endregion

#pragma region PhysicalDeviceFeatures

inline VkBool32 GreatVulkan::PhysicalDeviceFeatures::Has(const Initializer<Feature>& features_, const Feature& feature_)
{
	auto it = std::find(features_.begin(), features_.end(), feature_);

	return it != features_.end();
}

inline GreatVulkan::PhysicalDeviceFeatures::PhysicalDeviceFeatures(const Initializer<Feature>& features_)
{
	robustBufferAccess						= Has(features_, Feature::RobustBufferAccess);
    fullDrawIndexUint32						= Has(features_, Feature::FullDrawIndexUint32);
    imageCubeArray							= Has(features_, Feature::ImageCubeArray);
    independentBlend						= Has(features_, Feature::IndependentBlend);
    geometryShader							= Has(features_, Feature::GeometryShader);
    tessellationShader						= Has(features_, Feature::TessellationShader);
    sampleRateShading						= Has(features_, Feature::SampleRateShading);
    dualSrcBlend							= Has(features_, Feature::DualSrcBlend);
    logicOp									= Has(features_, Feature::LogicOp);
    multiDrawIndirect						= Has(features_, Feature::MultiDrawIndirect);
    drawIndirectFirstInstance				= Has(features_, Feature::DrawIndirectFirstInstance);
    depthClamp								= Has(features_, Feature::DepthClamp);
    depthBiasClamp							= Has(features_, Feature::DepthBiasClamp);
    fillModeNonSolid						= Has(features_, Feature::FillModeNonSolid);
    depthBounds								= Has(features_, Feature::DepthBounds);
    wideLines								= Has(features_, Feature::WideLines);
    largePoints								= Has(features_, Feature::LargePoints);
    alphaToOne								= Has(features_, Feature::AlphaToOne);
    multiViewport							= Has(features_, Feature::MultiViewport);
    samplerAnisotropy						= Has(features_, Feature::SamplerAnisotropy);
    textureCompressionETC2					= Has(features_, Feature::TextureCompressionETC2);
    textureCompressionASTC_LDR				= Has(features_, Feature::TextureCompressionASTC_LDR);
    textureCompressionBC					= Has(features_, Feature::TextureCompressionBC);
    occlusionQueryPrecise					= Has(features_, Feature::OcclusionQueryPrecise);
    pipelineStatisticsQuery					= Has(features_, Feature::PipelineStatisticsQuery);
    vertexPipelineStoresAndAtomics			= Has(features_, Feature::VertexPipelineStoresAndAtomics);
    fragmentStoresAndAtomics				= Has(features_, Feature::FragmentStoresAndAtomics);
    shaderTessellationAndGeometryPointSize	= Has(features_, Feature::ShaderTessellationAndGeometryPointSize);
    shaderImageGatherExtended				= Has(features_, Feature::ShaderImageGatherExtended);
    shaderStorageImageExtendedFormats		= Has(features_, Feature::ShaderStorageImageExtendedFormats);
    shaderStorageImageMultisample			= Has(features_, Feature::ShaderStorageImageMultisample);
    shaderStorageImageReadWithoutFormat		= Has(features_, Feature::ShaderStorageImageReadWithoutFormat);
    shaderStorageImageWriteWithoutFormat	= Has(features_, Feature::ShaderStorageImageWriteWithoutFormat);
    shaderUniformBufferArrayDynamicIndexing	= Has(features_, Feature::ShaderUniformBufferArrayDynamicIndexing);
    shaderSampledImageArrayDynamicIndexing	= Has(features_, Feature::ShaderSampledImageArrayDynamicIndexing);
    shaderStorageBufferArrayDynamicIndexing	= Has(features_, Feature::ShaderStorageBufferArrayDynamicIndexing);
    shaderStorageImageArrayDynamicIndexing	= Has(features_, Feature::ShaderStorageImageArrayDynamicIndexing);
    shaderClipDistance						= Has(features_, Feature::ShaderClipDistance);
    shaderCullDistance						= Has(features_, Feature::ShaderCullDistance);
    shaderFloat64							= Has(features_, Feature::ShaderFloat64);
    shaderInt64								= Has(features_, Feature::ShaderInt64);
    shaderInt16								= Has(features_, Feature::ShaderInt16);
    shaderResourceResidency					= Has(features_, Feature::ShaderResourceResidency);
    shaderResourceMinLod					= Has(features_, Feature::ShaderResourceMinLod);
    sparseBinding							= Has(features_, Feature::SparseBinding);
    sparseResidencyBuffer					= Has(features_, Feature::SparseResidencyBuffer);
    sparseResidencyImage2D					= Has(features_, Feature::SparseResidencyImage2D);
    sparseResidencyImage3D					= Has(features_, Feature::SparseResidencyImage3D);
    sparseResidency2Samples					= Has(features_, Feature::SparseResidency2Samples);
    sparseResidency4Samples					= Has(features_, Feature::SparseResidency4Samples);
    sparseResidency8Samples					= Has(features_, Feature::SparseResidency8Samples);
    sparseResidency16Samples				= Has(features_, Feature::SparseResidency16Samples);
    sparseResidencyAliased					= Has(features_, Feature::SparseResidencyAliased);
    variableMultisampleRate					= Has(features_, Feature::VariableMultisampleRate);
    inheritedQueries						= Has(features_, Feature::InheritedQueries);
}

#pragma endregion

#pragma region DeviceQueueCreateInfo

inline GreatVulkan::DeviceQueueCreateInfo::DeviceQueueCreateInfo(const decltype(VkDeviceQueueCreateInfo::queueFamilyIndex)& queueFamilyIndex_, const Vector<float>& queuePriorities_)
{
	sType				= VkStructureType::VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
	pNext				= nullptr;
	flags				= 0;
	queueFamilyIndex	= queueFamilyIndex_;
	queueCount			= queuePriorities_.size();
	pQueuePriorities	= queuePriorities_.data();
}

#pragma endregion

#pragma region Win32SurfaceCreateInfoKHR

#if VK_USE_PLATFORM_WIN32_KHR

inline GreatVulkan::Win32SurfaceCreateInfoKHR::Win32SurfaceCreateInfoKHR(const decltype(VkWin32SurfaceCreateInfoKHR::hinstance)& handleInstance_, const decltype(VkWin32SurfaceCreateInfoKHR::hwnd)& handleWindow_)
{
	sType		= VkStructureType::VK_STRUCTURE_TYPE_WIN32_SURFACE_CREATE_INFO_KHR;
	pNext		= nullptr;
	flags		= 0;
	hinstance	= handleInstance_;
	hwnd		= handleWindow_;
}

#endif

#pragma endregion

// Instance
inline GreatVulkan::Vector<VkLayerProperties> GreatVulkan::EnumerateInstanceLayerProperties()
{
	uint32_t vk_instanceLayersPropertiesCount;

	if (auto result = Result(vkEnumerateInstanceLayerProperties(&vk_instanceLayersPropertiesCount, nullptr)))
	{
		Vector<VkLayerProperties> vk_instanceLayersProperties(vk_instanceLayersPropertiesCount);

		if (auto result = Result(vkEnumerateInstanceLayerProperties(&vk_instanceLayersPropertiesCount, vk_instanceLayersProperties.data())))
		{
			return Move(vk_instanceLayersProperties);
		}
		else
		{
			throw Exception(); // TODO
		}
	}
	else
	{
		throw Exception(); // TODO
	}
}
inline GreatVulkan::Vector<const char*> GreatVulkan::ExtractInstanceLayersNames(const Vector<VkLayerProperties>& vk_instanceLayers_)
{
	Vector<const char*> layersNames(vk_instanceLayers_.size());

	for (Size i = 0; i < layersNames.size(); ++i)
	{
		layersNames[i] = vk_instanceLayers_[i].layerName;
	}

	return Move(layersNames);
}
inline GreatVulkan::Vector<VkExtensionProperties> GreatVulkan::EnumerateInstanceExtensionProperties(const char* vk_instanceLayerName_)
{
	uint32_t vk_instanceLayerExtensionsPropertiesCount;

	if (auto result = Result(vkEnumerateInstanceExtensionProperties(vk_instanceLayerName_, &vk_instanceLayerExtensionsPropertiesCount, nullptr)))
	{
		Vector<VkExtensionProperties> vk_instanceLayerExtensionsProperties(vk_instanceLayerExtensionsPropertiesCount);

		if (auto result = Result(vkEnumerateInstanceExtensionProperties(vk_instanceLayerName_, &vk_instanceLayerExtensionsPropertiesCount, vk_instanceLayerExtensionsProperties.data())))
		{
			return Move(vk_instanceLayerExtensionsProperties);
		}
		else
		{
			throw Exception(); // TODO
		}
	}
	else
	{
		throw Exception(); // TODO
	}
}
inline GreatVulkan::Map<GreatVulkan::String, GreatVulkan::Vector<VkExtensionProperties>> GreatVulkan::EnumerateInstanceExtensionsProperties(const Vector<const char*>& vk_instanceLayerNames_)
{
	Map<String, Vector<VkExtensionProperties>> vk_instanceLayersExtensionsProperties;

	for (auto &vk_layerName : vk_instanceLayerNames_)
	{
		String layerName = vk_layerName;

		auto layerExtensionProperties = EnumerateInstanceExtensionProperties(vk_layerName);

		vk_instanceLayersExtensionsProperties[layerName] = Move(layerExtensionProperties);
	}

	return Move(vk_instanceLayersExtensionsProperties);
}
inline GreatVulkan::Vector<const char*> GreatVulkan::ExtractInstanceExtensionsNames(const Map<String, Vector<VkExtensionProperties>>& instanceExtensions_)
{
	Size extensionsCount = 0;
	{
		for (auto &i : instanceExtensions_)
		{
			extensionsCount += i.second.size();
		}
	}

	Vector<const char*> extensionsNames(extensionsCount);
	{
		Size i = 0;
		for (auto &j : instanceExtensions_)
		{
			for (auto &k : j.second)
			{
				extensionsNames[i] = k.extensionName;

				++i;
			}
		}
	}

	return Move(extensionsNames);
}
inline VkInstance GreatVulkan::CreateInstance(const VkInstanceCreateInfo& vk_instanceCreateInfo_)
{
	VkInstance vk_instance;

	if (auto result = Result(vkCreateInstance(&vk_instanceCreateInfo_, nullptr, &vk_instance)))
	{
		return vk_instance;
	}
	else
	{
		throw Exception(); // TODO
	}
}
inline void GreatVulkan::DestroyInstance(const VkInstance& vk_instance_)
{
	vkDestroyInstance(vk_instance_, nullptr);
}

// PhysicalDevices
inline GreatVulkan::Vector<VkPhysicalDevice> GreatVulkan::EnumeratePhysicalDevices(const VkInstance& vk_instance_)
{
	uint32_t vk_physicalDevicesCount;

	if (auto result = Result(vkEnumeratePhysicalDevices(vk_instance_, &vk_physicalDevicesCount, nullptr)))
	{
		Vector<VkPhysicalDevice> vk_physicalDevices(vk_physicalDevicesCount);
		{
			if (auto result = Result(vkEnumeratePhysicalDevices(vk_instance_, &vk_physicalDevicesCount, vk_physicalDevices.data())))
			{
				return Move(vk_physicalDevices);
			}
			else
			{
				throw Exception(); // TODO
			}
		}
	}
	else
	{
		throw Exception(); // TODO
	}
}
inline VkPhysicalDeviceProperties GreatVulkan::GetPhysicalDeviceProperties(const VkPhysicalDevice& vk_physicalDevice_)
{
	VkPhysicalDeviceProperties vk_physicalDeviceProperties;
	
	vkGetPhysicalDeviceProperties(vk_physicalDevice_, &vk_physicalDeviceProperties);

	return vk_physicalDeviceProperties;
}
inline GreatVulkan::Vector<VkQueueFamilyProperties> GreatVulkan::GetPhysicalDeviceQueueFamilyProperties(const VkPhysicalDevice& vk_physicalDevice_)
{
	uint32_t vk_physicalDeviceQueueFamilyPropertiesCount = 0;

	vkGetPhysicalDeviceQueueFamilyProperties(vk_physicalDevice_, &vk_physicalDeviceQueueFamilyPropertiesCount, nullptr);

	Vector<VkQueueFamilyProperties> vk_physicalDeviceQueueFamilyProperties(vk_physicalDeviceQueueFamilyPropertiesCount);

	vkGetPhysicalDeviceQueueFamilyProperties(vk_physicalDevice_, &vk_physicalDeviceQueueFamilyPropertiesCount, vk_physicalDeviceQueueFamilyProperties.data());

	return Move(vk_physicalDeviceQueueFamilyProperties);
}
inline VkPhysicalDeviceMemoryProperties GreatVulkan::GetPhysicalDeviceMemoryProperties(const VkPhysicalDevice& vk_physicalDevice_)
{
	VkPhysicalDeviceMemoryProperties vk_physicalDeviceMemoryProperties;

	vkGetPhysicalDeviceMemoryProperties(vk_physicalDevice_, &vk_physicalDeviceMemoryProperties);

	return Move(vk_physicalDeviceMemoryProperties);
}

// Device
inline GreatVulkan::Vector<VkLayerProperties> GreatVulkan::EnumerateDeviceLayerProperties(const VkPhysicalDevice& vk_physicalDevice_)
{
	uint32_t vk_physicalDeviceLayersPropertiesCount;
	
	if (auto result = Result(vkEnumerateDeviceLayerProperties(vk_physicalDevice_, &vk_physicalDeviceLayersPropertiesCount, nullptr)))
	{
		Vector<VkLayerProperties> vk_physicalDeviceLayersProperties(vk_physicalDeviceLayersPropertiesCount);

		if (auto result = Result(vkEnumerateDeviceLayerProperties(vk_physicalDevice_, &vk_physicalDeviceLayersPropertiesCount, vk_physicalDeviceLayersProperties.data())))
		{
			return Move(vk_physicalDeviceLayersProperties);
		}
		else
		{
			throw Exception(); // TODO
		}
	}
	else
	{
		throw Exception(); // TODO
	}
}
inline GreatVulkan::Vector<const char*> GreatVulkan::ExtractDeviceLayersNames(const Vector<VkLayerProperties>& vk_deviceLayers_)
{
	Vector<const char*> layersNames(vk_deviceLayers_.size());

	for (Size i = 0; i < layersNames.size(); ++i)
	{
		layersNames[i] = vk_deviceLayers_[i].layerName;
	}

	return Move(layersNames);
}
inline GreatVulkan::Vector<VkExtensionProperties> GreatVulkan::EnumerateDeviceExtensionProperties(const VkPhysicalDevice& vk_physicalDevice_, const char* vk_deviceLayerName_)
{
	uint32_t vk_deviceLayerExtensionsPropertiesCount;

	if (auto result = Result(vkEnumerateDeviceExtensionProperties(vk_physicalDevice_, vk_deviceLayerName_, &vk_deviceLayerExtensionsPropertiesCount, nullptr)))
	{
		Vector<VkExtensionProperties> vk_deviceLayerExtensionsProperties(vk_deviceLayerExtensionsPropertiesCount);

		if (auto result = Result(vkEnumerateDeviceExtensionProperties(vk_physicalDevice_, vk_deviceLayerName_, &vk_deviceLayerExtensionsPropertiesCount, vk_deviceLayerExtensionsProperties.data())))
		{
			return Move(vk_deviceLayerExtensionsProperties);
		}
		else
		{
			throw Exception(); // TODO
		}
	}
	else
	{
		throw Exception(); // TODO
	}
}
inline GreatVulkan::Map<GreatVulkan::String, GreatVulkan::Vector<VkExtensionProperties>> GreatVulkan::EnumerateDeviceExtensionsProperties(const VkPhysicalDevice& vk_physicalDevice_, const Vector<const char*>& vk_deviceLayerNames_)
{
	Map<String, Vector<VkExtensionProperties>> vk_deviceLayersExtensionsProperties;

	for (auto &vk_layerName : vk_deviceLayerNames_)
	{
		String layerName = vk_layerName;

		auto layerExtensionProperties = EnumerateDeviceExtensionProperties(vk_physicalDevice_, vk_layerName);

		vk_deviceLayersExtensionsProperties[layerName] = Move(layerExtensionProperties);
	}

	return Move(vk_deviceLayersExtensionsProperties);
}
inline GreatVulkan::Vector<const char*> GreatVulkan::ExtractDeviceExtensionsNames(const Map<String, Vector<VkExtensionProperties>>& deviceExtensions_)
{
	Size extensionsCount = 0;
	{
		for (auto &i : deviceExtensions_)
		{
			extensionsCount += i.second.size();
		}
	}

	Vector<const char*> extensionsNames(extensionsCount);
	{
		Size i = 0;
		for (auto &j : deviceExtensions_)
		{
			for (auto &k : j.second)
			{
				extensionsNames[i] = k.extensionName;

				++i;
			}
		}
	}

	return Move(extensionsNames);
}
inline VkDevice GreatVulkan::CreateDevice(const VkPhysicalDevice& vk_physicalDevice_, const VkDeviceCreateInfo& vk_deviceCreateInfo_)
{
	VkDevice vk_device;

	if (auto result = Result(vkCreateDevice(vk_physicalDevice_, &vk_deviceCreateInfo_, nullptr, &vk_device)))
	{
		return vk_device;
	}
	else
	{
		throw Exception();
	}
}
inline void GreatVulkan::DestroyDevice(const VkDevice& vk_device_)
{
	vkDestroyDevice(vk_device_, nullptr);
}

#if VK_USE_PLATFORM_WIN32_KHR

// Win32SurfaceKHR
inline VkBool32 GreatVulkan::GetPhysicalDeviceWin32PresentationSupportKHR(const VkPhysicalDevice& vk_physicalDevice_, const Size& queueFamilyIndex_)
{
	auto isPresentationSupport = vkGetPhysicalDeviceWin32PresentationSupportKHR(vk_physicalDevice_, queueFamilyIndex_);

	return isPresentationSupport;
}
inline VkSurfaceKHR GreatVulkan::CreateWin32SurfaceKHR(const VkInstance& vk_instance_, const VkWin32SurfaceCreateInfoKHR& vk_win32SurfaceCreateInfo_)
{
	VkSurfaceKHR vk_surface;

	if (auto result = Result(vkCreateWin32SurfaceKHR(vk_instance_, &vk_win32SurfaceCreateInfo_, nullptr, &vk_surface)))
	{
		return vk_surface;
	}
	else
	{
		throw Exception(); // TODO
	}
}
inline VkBool32 GreatVulkan::GetPhysicalDeviceSurfaceSupportKHR(const VkPhysicalDevice& vk_physicalDevice_, const decltype(VkDeviceQueueCreateInfo::queueFamilyIndex)& vk_queueFamilyIndex_, const VkSurfaceKHR& vk_surface_)
{
	VkBool32 vk_isSufraceSupported;

	if (auto result = Result(vkGetPhysicalDeviceSurfaceSupportKHR(vk_physicalDevice_, vk_queueFamilyIndex_, vk_surface_, &vk_isSufraceSupported)))
	{
		return vk_isSufraceSupported;
	}
	else
	{
		throw Exception(); // TODO
	}
}
inline VkSurfaceCapabilitiesKHR GreatVulkan::GetPhysicalDeviceSurfaceCapabilitiesKHR(const VkPhysicalDevice& vk_physicalDevice_, const VkSurfaceKHR& vk_surface_)
{
	VkSurfaceCapabilitiesKHR vk_surfaceCapabilities;

	if (auto result = Result(vkGetPhysicalDeviceSurfaceCapabilitiesKHR(vk_physicalDevice_, vk_surface_, &vk_surfaceCapabilities)))
	{
		return vk_surfaceCapabilities;
	}
	else
	{
		throw Exception(); // TODO
	}
}
inline GreatVulkan::Vector<VkSurfaceFormatKHR> GreatVulkan::GetPhysicalDeviceSurfaceFormatsKHR(const VkPhysicalDevice& vk_physicalDevice_, const VkSurfaceKHR& vk_surface_)
{
	uint32_t vk_surfaceFormatsCount;

	if (auto result = Result(vkGetPhysicalDeviceSurfaceFormatsKHR(vk_physicalDevice_, vk_surface_, &vk_surfaceFormatsCount, nullptr)))
	{
		Vector<VkSurfaceFormatKHR> vk_surfaceFormats(vk_surfaceFormatsCount);

		if (auto result = Result(vkGetPhysicalDeviceSurfaceFormatsKHR(vk_physicalDevice_, vk_surface_, &vk_surfaceFormatsCount, vk_surfaceFormats.data())))
		{
			return Move(vk_surfaceFormats);
		}
		else
		{
			throw Exception(); // TODO
		}
	}
	else
	{
		throw Exception(); // TODO
	}
}

#endif

#pragma endregion


#pragma region
#pragma endregion


