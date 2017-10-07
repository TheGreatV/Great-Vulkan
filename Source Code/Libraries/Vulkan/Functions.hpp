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
	class RenderPassCreateInfo:
		public VkRenderPassCreateInfo
	{
	public:
		inline RenderPassCreateInfo() = delete;
		inline RenderPassCreateInfo(const Vector<VkAttachmentDescription>& vk_attachmentDescriptions_, const Vector<VkSubpassDescription>& vk_subpassDescriptions_, const Vector<VkSubpassDependency>& vk_subpassDependencies_ = Vector<VkSubpassDependency>());
		inline RenderPassCreateInfo(const RenderPassCreateInfo&) = delete;
		inline ~RenderPassCreateInfo() = default;
	public:
		inline RenderPassCreateInfo& operator = (const RenderPassCreateInfo&) = delete;
	};
	class CommandPoolCreateInfo:
		public VkCommandPoolCreateInfo
	{
	public:
		inline CommandPoolCreateInfo() = delete;
		inline CommandPoolCreateInfo(const VkCommandPoolCreateFlags& flags_, const decltype(queueFamilyIndex)& queueFamilyIndex_);
		inline CommandPoolCreateInfo(const CommandPoolCreateInfo&) = delete;
		inline ~CommandPoolCreateInfo() = default;
	public:
		inline CommandPoolCreateInfo& operator = (const CommandPoolCreateInfo&) = delete;
	};
	
	class CommandBufferAllocateInfo:
		public VkCommandBufferAllocateInfo
	{
	public:
		inline CommandBufferAllocateInfo() = delete;
		inline CommandBufferAllocateInfo(const VkCommandPool& vk_commandPool_, const VkCommandBufferLevel& level_, const decltype(commandBufferCount)& commandBufferCount_);
		inline CommandBufferAllocateInfo(const CommandBufferAllocateInfo&) = delete;
		inline ~CommandBufferAllocateInfo() = default;
	public:
		inline CommandBufferAllocateInfo& operator = (const CommandBufferAllocateInfo&) = delete;
	};

	class CommandBufferBeginInfo:
		public VkCommandBufferBeginInfo
	{
	public:
		inline CommandBufferBeginInfo() = delete;
		inline CommandBufferBeginInfo(const VkCommandBufferUsageFlags& flags_, const VkCommandBufferInheritanceInfo& inheritanceInfo_);
		inline CommandBufferBeginInfo(const VkCommandBufferUsageFlags& flags_, const VkCommandBufferInheritanceInfo* inheritanceInfo_ = nullptr);
		inline CommandBufferBeginInfo(const CommandBufferBeginInfo&) = delete;
		inline ~CommandBufferBeginInfo() = default;
	public:
		inline CommandBufferBeginInfo& operator = (const CommandBufferBeginInfo&) = delete;
	};

	class AttachmentDescription:
		public VkAttachmentDescription
	{
	public:
		inline AttachmentDescription() = delete;
		inline AttachmentDescription( // TODO: add flags
			const VkFormat& format_,
			const VkSampleCountFlagBits& samples_,
			const VkAttachmentLoadOp& loadOp_,
			const VkAttachmentStoreOp& storeOp_,
			const VkAttachmentLoadOp& stencilLoadOp_,
			const VkAttachmentStoreOp& stencilStoreOp_,
			const VkImageLayout& initialLayout_,
			const VkImageLayout& finalLayout_
		);
		inline AttachmentDescription(const AttachmentDescription&) = delete;
		inline ~AttachmentDescription() = default;
	public:
		inline AttachmentDescription& operator = (const AttachmentDescription&) = delete;
	};
	class AttachmentReference:
		public VkAttachmentReference
	{
	public:
		inline AttachmentReference() = delete;
		inline AttachmentReference(const decltype(attachment)& attachment_, const VkImageLayout& layout_);
		inline AttachmentReference(const AttachmentReference&) = delete;
		inline ~AttachmentReference() = default;
	public:
		inline AttachmentReference& operator = (const AttachmentReference&) = delete;
	};
	class SubpassDescription:
		public VkSubpassDescription
	{
	public:
		inline SubpassDescription() = delete;
		inline SubpassDescription(const VkPipelineBindPoint& pipelineBindPoint_, const Vector<VkAttachmentReference>& colorAttachments_);
		inline SubpassDescription(
			const VkPipelineBindPoint& pipelineBindPoint_,
			const Vector<VkAttachmentReference>& inputAttachments_,
			const Vector<VkAttachmentReference>& colorAttachments_,
			const Vector<VkAttachmentReference>& resolveAttachments_,
			const VkAttachmentReference* depthStencilAttachment_,
			const Vector<uint32_t>& preserveAttachments_
		);
		inline SubpassDescription(const SubpassDescription&) = delete;
		inline ~SubpassDescription() = default;
	public:
		inline SubpassDescription& operator = (const SubpassDescription&) = delete;
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

	// Win32SurfaceKHR
#if VK_USE_PLATFORM_WIN32_KHR
	inline VkBool32 GetPhysicalDeviceWin32PresentationSupportKHR(const VkPhysicalDevice& vk_physicalDevice_, const Size& queueFamilyIndex_);
	inline VkSurfaceKHR CreateWin32SurfaceKHR(const VkInstance& vk_instance_, const VkWin32SurfaceCreateInfoKHR& vk_win32SurfaceCreateInfo_);
	inline VkBool32 GetPhysicalDeviceSurfaceSupportKHR(const VkPhysicalDevice& vk_physicalDevice_, const decltype(VkDeviceQueueCreateInfo::queueFamilyIndex)& vk_queueFamilyIndex_, const VkSurfaceKHR& vk_surface_);
	inline VkSurfaceCapabilitiesKHR GetPhysicalDeviceSurfaceCapabilitiesKHR(const VkPhysicalDevice& vk_physicalDevice_, const VkSurfaceKHR& vk_surface_);
	inline Vector<VkSurfaceFormatKHR> GetPhysicalDeviceSurfaceFormatsKHR(const VkPhysicalDevice& vk_physicalDevice_, const VkSurfaceKHR& vk_surface_);
	inline void DestroySurfaceKHR(const VkInstance& vk_instance_, const VkSurfaceKHR& vk_surface_);
#endif

	// Queue
	inline VkQueue GetDeviceQueue(const VkDevice& vk_device_, const uint32_t& vk_queueFamilyIndex_, const uint32_t& vk_queueIndex_);

	// RenderPass
	inline VkRenderPass CreateRenderPass(const VkDevice& vk_device_, const VkRenderPassCreateInfo& vk_renderPassCreateInfo_);
	inline void DestroyRenderPass(const VkDevice& vk_device_, const VkRenderPass& vk_renderPass_);

	// Command Pool
	inline VkCommandPool CreateCommandPool(const VkDevice& vk_device_, const VkCommandPoolCreateInfo &vk_commandPoolCreateInfo_);
	inline void ResetCommandPool(const VkDevice& vk_device_, const VkCommandPool& vk_commandPool_, const VkCommandPoolResetFlags& flags_);
	inline void DestroyCommandPool(const VkDevice& vk_device_, const VkCommandPool& vk_commandPool_);

	// Command Buffer
	inline Vector<VkCommandBuffer> AllocateCommandBuffers(const VkDevice& vk_device_, const VkCommandBufferAllocateInfo& vk_commandBufferAllocateInfo_); // TODO: allocate single buffer
	inline void ResetCommandBuffer(const VkCommandBuffer& vk_commandBuffer_, const VkCommandBufferResetFlags& flags_);
	inline void BeginCommandBuffer(const VkCommandBuffer& vk_commandBuffer_, const VkCommandBufferBeginInfo& vk_commandBufferBeginInfo_);
	inline void EndCommandBuffer(const VkCommandBuffer& vk_commandBuffer_);
	inline void FreeCommandBuffers(const VkDevice& vk_device_, const VkCommandPool& vk_commandPool_, const Vector<VkCommandBuffer>& vk_commandBuffers_); // TODO: free single buffer
}


#pragma region
#pragma endregion

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

#pragma region RenderPassCreateInfo

inline GreatVulkan::RenderPassCreateInfo::RenderPassCreateInfo(const Vector<VkAttachmentDescription>& vk_attachmentDescriptions_, const Vector<VkSubpassDescription>& vk_subpassDescriptions_, const Vector<VkSubpassDependency>& vk_subpassDependencies_)
{
	sType			= VkStructureType::VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO;
	pNext			= nullptr;
	flags			= 0;
	attachmentCount	= vk_attachmentDescriptions_.size();
	pAttachments	= vk_attachmentDescriptions_.data();
	subpassCount	= vk_subpassDescriptions_.size();
	pSubpasses		= vk_subpassDescriptions_.data();
	dependencyCount	= vk_subpassDependencies_.size();
	pDependencies	= vk_subpassDependencies_.data();
}

#pragma endregion

#pragma region RenderPassCreateInfo

inline GreatVulkan::CommandPoolCreateInfo::CommandPoolCreateInfo(const VkCommandPoolCreateFlags& flags_, const decltype(queueFamilyIndex)& queueFamilyIndex_)
{
	sType				= VkStructureType::VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
	pNext				= nullptr;
	flags				= flags_;
	queueFamilyIndex	= queueFamilyIndex_;
}

#pragma endregion

#pragma region RenderPassCreateInfo

inline GreatVulkan::CommandBufferAllocateInfo::CommandBufferAllocateInfo(const VkCommandPool& vk_commandPool_, const VkCommandBufferLevel& level_, const decltype(commandBufferCount)& commandBufferCount_)
{
	sType				= VkStructureType::VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
	pNext				= nullptr;
	commandPool			= vk_commandPool_;
	level				= level_;
	commandBufferCount	= commandBufferCount_;
}

#pragma endregion

#pragma region RenderPassCreateInfo

inline GreatVulkan::CommandBufferBeginInfo::CommandBufferBeginInfo(const VkCommandBufferUsageFlags& flags_, const VkCommandBufferInheritanceInfo& inheritanceInfo_):
	CommandBufferBeginInfo(flags_, &inheritanceInfo_)
{
}
inline GreatVulkan::CommandBufferBeginInfo::CommandBufferBeginInfo(const VkCommandBufferUsageFlags& flags_, const VkCommandBufferInheritanceInfo* inheritanceInfo_)
{
	sType				= VkStructureType::VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
	pNext				= nullptr;
	flags				= flags_;
	pInheritanceInfo	= inheritanceInfo_;
}

#pragma endregion

#pragma region AttachmentDescription

inline GreatVulkan::AttachmentDescription::AttachmentDescription(
	const VkFormat& format_,
	const VkSampleCountFlagBits& samples_,
	const VkAttachmentLoadOp& loadOp_,
	const VkAttachmentStoreOp& storeOp_,
	const VkAttachmentLoadOp& stencilLoadOp_,
	const VkAttachmentStoreOp& stencilStoreOp_,
	const VkImageLayout& initialLayout_,
	const VkImageLayout& finalLayout_
) {
	flags			= 0; // VkAttachmentDescriptionFlagBits::VK_ATTACHMENT_DESCRIPTION_MAY_ALIAS_BIT;
	format			= format_;
	samples			= samples_;
	loadOp			= loadOp_;
	storeOp			= storeOp_;
	stencilLoadOp	= stencilLoadOp_;
	stencilStoreOp	= stencilStoreOp_;
	initialLayout	= initialLayout_;
	finalLayout		= finalLayout_;
}

#pragma endregion

#pragma region AttachmentDescription

inline GreatVulkan::AttachmentReference::AttachmentReference(const decltype(attachment)& attachment_, const VkImageLayout& layout_)
{
	attachment	= attachment_;
	layout		= layout_;
}

#pragma endregion

#pragma region AttachmentDescription

inline GreatVulkan::SubpassDescription::SubpassDescription(const VkPipelineBindPoint& pipelineBindPoint_, const Vector<VkAttachmentReference>& colorAttachments_):
	SubpassDescription(pipelineBindPoint_, {}, colorAttachments_, {}, nullptr, {})
{
}
inline GreatVulkan::SubpassDescription::SubpassDescription(
	const VkPipelineBindPoint& pipelineBindPoint_,
	const Vector<VkAttachmentReference>& inputAttachments_,
	const Vector<VkAttachmentReference>& colorAttachments_,
	const Vector<VkAttachmentReference>& resolveAttachments_,
	const VkAttachmentReference* depthStencilAttachment_,
	const Vector<uint32_t>& preserveAttachments_
) {
	// TODO: check resolveAttachments_.size()
	flags						= 0;
	pipelineBindPoint			= pipelineBindPoint_;
	inputAttachmentCount		= inputAttachments_.size();
	pInputAttachments			= inputAttachments_.data();
	colorAttachmentCount		= colorAttachments_.size();
	pColorAttachments			= colorAttachments_.data();
	pResolveAttachments			= resolveAttachments_.data();
	pDepthStencilAttachment		= depthStencilAttachment_;
	preserveAttachmentCount		= preserveAttachments_.size();
	pPreserveAttachments		= preserveAttachments_.data();
}

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

// Win32SurfaceKHR
#if VK_USE_PLATFORM_WIN32_KHR

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
inline void GreatVulkan::DestroySurfaceKHR(const VkInstance& vk_instance_, const VkSurfaceKHR& vk_surface_)
{
	vkDestroySurfaceKHR(vk_instance_, vk_surface_, nullptr);
}

#endif

// Queue
inline VkQueue GreatVulkan::GetDeviceQueue(const VkDevice& vk_device_, const uint32_t& vk_queueFamilyIndex_, const uint32_t& vk_queueIndex_)
{
	VkQueue vk_queue = VK_NULL_HANDLE; // TODO: should we really need to initialize this value?

	vkGetDeviceQueue(vk_device_, vk_queueFamilyIndex_, vk_queueIndex_, &vk_queue);

	return vk_queue;
}

// RenderPass
inline VkRenderPass GreatVulkan::CreateRenderPass(const VkDevice& vk_device_, const VkRenderPassCreateInfo& vk_renderPassCreateInfo_)
{
	VkRenderPass vk_renderPass;

	if (auto result = Result(vkCreateRenderPass(vk_device_, &vk_renderPassCreateInfo_, nullptr, &vk_renderPass)))
	{
		return vk_renderPass;
	}
	else
	{
		throw Exception(); // TODO
	}
}
inline void GreatVulkan::DestroyRenderPass(const VkDevice& vk_device_, const VkRenderPass& vk_renderPass_)
{
	vkDestroyRenderPass(vk_device_, vk_renderPass_, nullptr);
}

// Command Pool
inline VkCommandPool GreatVulkan::CreateCommandPool(const VkDevice& vk_device_, const VkCommandPoolCreateInfo &vk_commandPoolCreateInfo_)
{
	VkCommandPool vk_commandPool;

	if (auto result = Result(vkCreateCommandPool(vk_device_, &vk_commandPoolCreateInfo_, nullptr, &vk_commandPool)))
	{
		return vk_commandPool;
	}
	else
	{
		throw Exception(); // TODO
	}
}
inline void GreatVulkan::ResetCommandPool(const VkDevice& vk_device_, const VkCommandPool& vk_commandPool_, const VkCommandPoolResetFlags& flags_)
{
	if (auto result = Result(vkResetCommandPool(vk_device_, vk_commandPool_, flags_))); else
	{
		throw Exception();
	}
}
inline void GreatVulkan::DestroyCommandPool(const VkDevice& vk_device_, const VkCommandPool& vk_commandPool_)
{
	vkDestroyCommandPool(vk_device_, vk_commandPool_, nullptr);
}

// Command Buffer
inline GreatVulkan::Vector<VkCommandBuffer> GreatVulkan::AllocateCommandBuffers(const VkDevice& vk_device_, const VkCommandBufferAllocateInfo& vk_commandBufferAllocateInfo_)
{
	Vector<VkCommandBuffer> vk_commandBuffers(vk_commandBufferAllocateInfo_.commandBufferCount);

	if (auto result = Result(vkAllocateCommandBuffers(vk_device_, &vk_commandBufferAllocateInfo_, vk_commandBuffers.data())))
	{
		return Move(vk_commandBuffers);
	}
	else
	{
		throw Exception(); // TODO
	}
}
inline void GreatVulkan::ResetCommandBuffer(const VkCommandBuffer& vk_commandBuffer_, const VkCommandBufferResetFlags& flags_)
{
	if (auto result = Result(vkResetCommandBuffer(vk_commandBuffer_, flags_))); else
	{
		throw Exception(); // TODO
	}
}
inline void GreatVulkan::BeginCommandBuffer(const VkCommandBuffer& vk_commandBuffer_, const VkCommandBufferBeginInfo& vk_commandBufferBeginInfo_)
{
	if (auto result = Result(vkBeginCommandBuffer(vk_commandBuffer_, &vk_commandBufferBeginInfo_)));  else
	{
		throw Exception(); // TODO
	}
}
inline void GreatVulkan::EndCommandBuffer(const VkCommandBuffer& vk_commandBuffer_)
{
	if (auto result = Result(vkEndCommandBuffer(vk_commandBuffer_))); else
	{
		throw Exception(); // TODO
	}
}
inline void GreatVulkan::FreeCommandBuffers(const VkDevice& vk_device_, const VkCommandPool& vk_commandPool_, const Vector<VkCommandBuffer>& vk_commandBuffers_)
{
	vkFreeCommandBuffers(vk_device_, vk_commandPool_, vk_commandBuffers_.size(), vk_commandBuffers_.data());
}

#pragma endregion


#pragma region
#pragma endregion


