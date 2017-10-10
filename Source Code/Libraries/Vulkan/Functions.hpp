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
	class ImageViewCreateInfo:
		public VkImageViewCreateInfo
	{
	public:
		inline ImageViewCreateInfo() = delete;
		inline ImageViewCreateInfo(
			const VkImageViewCreateFlags& flags_,
			const VkImage& vk_image_,
			const VkImageViewType& vk_imageViewType_,
			const VkFormat& format_,
			const VkComponentMapping& vk_componentMapping_,
			const VkImageSubresourceRange& vk_imageSubresourceRange_
		);
		inline ImageViewCreateInfo(const ImageViewCreateInfo&) = delete;
		inline ~ImageViewCreateInfo() = default;
	public:
		inline ImageViewCreateInfo& operator = (const ImageViewCreateInfo&) = delete;
	};
	class FramebufferCreateInfo:
		public VkFramebufferCreateInfo
	{
	public:
		inline FramebufferCreateInfo() = delete;
		inline FramebufferCreateInfo(
			const VkRenderPass& vk_renderPass_,
			const Vector<VkImageView>& attachments_,
			const decltype(width)& width_,
			const decltype(height)& height_,
			const decltype(layers)& layers_
		);
		inline FramebufferCreateInfo(const FramebufferCreateInfo&) = delete;
		inline ~FramebufferCreateInfo() = default;
	public:
		inline FramebufferCreateInfo& operator = (const FramebufferCreateInfo&) = delete;
	};
	class FenceCreateInfo:
		public VkFenceCreateInfo
	{
	public:
		inline FenceCreateInfo() = delete;
		inline FenceCreateInfo(const VkFenceCreateFlags& flags_);
		inline FenceCreateInfo(const FenceCreateInfo&) = delete;
		inline ~FenceCreateInfo() = default;
	public:
		inline FenceCreateInfo& operator = (const FenceCreateInfo&) = delete;
	};
	class BufferCreateInfo:
		public VkBufferCreateInfo
	{
	public:
		inline BufferCreateInfo() = delete;
		inline BufferCreateInfo(
			const VkBufferCreateFlags& flags_,
			const VkDeviceSize size_,
			const VkBufferUsageFlags& usage_,
			const VkSharingMode& sharingMode_,
			const Vector<uint32_t>& queueFamilyIndices_ = Vector<uint32_t>()
		);
		inline BufferCreateInfo(const BufferCreateInfo&) = delete;
		inline ~BufferCreateInfo() = default;
	public:
		inline BufferCreateInfo& operator = (const BufferCreateInfo&) = delete;
	};
	class ShaderModuleCreateInfo:
		public VkShaderModuleCreateInfo
	{
	public:
		inline ShaderModuleCreateInfo() = delete;
		inline explicit ShaderModuleCreateInfo(const Vector<uint32_t>& sourceCode_);
		inline ShaderModuleCreateInfo(const ShaderModuleCreateInfo&) = delete;
		inline ~ShaderModuleCreateInfo() = default;
	public:
		inline ShaderModuleCreateInfo& operator = (const ShaderModuleCreateInfo&) = delete;
	};
	class PipelineLayoutCreateInfo:
		public VkPipelineLayoutCreateInfo
	{
	public:
		inline PipelineLayoutCreateInfo() = delete;
		inline PipelineLayoutCreateInfo(const Vector<VkDescriptorSetLayout>& vk_descriptorSetLayouts_, const Vector<VkPushConstantRange>& vk_pushConstantRanges_);
		inline PipelineLayoutCreateInfo(const PipelineLayoutCreateInfo&) = delete;
		inline ~PipelineLayoutCreateInfo() = default;
	public:
		inline PipelineLayoutCreateInfo& operator = (const PipelineLayoutCreateInfo&) = delete;
	};
	class GraphicsPipelineCreateInfo:
		public VkGraphicsPipelineCreateInfo
	{
	public:
		inline GraphicsPipelineCreateInfo() = delete;
		inline GraphicsPipelineCreateInfo(
			const VkPipelineCreateFlags& flags_,
			const Vector<VkPipelineShaderStageCreateInfo>& vk_stages_,
			const VkPipelineVertexInputStateCreateInfo& vk_vertexInputState_,
			const VkPipelineInputAssemblyStateCreateInfo& vk_inputAssemblyState_,
			const VkPipelineViewportStateCreateInfo& vk_viewportState_,
			const VkPipelineRasterizationStateCreateInfo& vk_rasterizationState_,
			const VkPipelineMultisampleStateCreateInfo& vk_multisampleState_,
			const VkPipelineDepthStencilStateCreateInfo& vk_depthStencilState_,
			const VkPipelineColorBlendStateCreateInfo& vk_colorBlendState_,
			const VkPipelineLayout& vk_pipelineLayout_,
			const VkRenderPass& vk_renderPass_,
			const uint32_t subpass_
		);
		inline GraphicsPipelineCreateInfo(
			const VkPipelineCreateFlags& flags_,
			const Vector<VkPipelineShaderStageCreateInfo>& vk_stages_,
			const VkPipelineVertexInputStateCreateInfo* vk_vertexInputState_,
			const VkPipelineInputAssemblyStateCreateInfo* vk_inputAssemblyState_,
			const VkPipelineTessellationStateCreateInfo* vk_tessellationState_,
			const VkPipelineViewportStateCreateInfo* vk_viewportState_,
			const VkPipelineRasterizationStateCreateInfo* vk_rasterizationState_,
			const VkPipelineMultisampleStateCreateInfo* vk_multisampleState_,
			const VkPipelineDepthStencilStateCreateInfo* vk_depthStencilState_,
			const VkPipelineColorBlendStateCreateInfo* vk_colorBlendState_,
			const VkPipelineDynamicStateCreateInfo* vk_dynamicState_,
			const VkPipelineLayout& vk_pipelineLayout_,
			const VkRenderPass& vk_renderPass_,
			const uint32_t subpass_,
			const VkPipeline vk_basePipeline_,
			const int32_t basePipelineIndex_
		);
		inline GraphicsPipelineCreateInfo(const GraphicsPipelineCreateInfo&) = delete;
		inline ~GraphicsPipelineCreateInfo() = default;
	public:
		inline GraphicsPipelineCreateInfo& operator = (const GraphicsPipelineCreateInfo&) = delete;
	};
	
	class PipelineShaderStageCreateInfo:
		public VkPipelineShaderStageCreateInfo
	{
	protected:
		const String nameContainer;
	public:
		inline PipelineShaderStageCreateInfo() = delete;
		inline PipelineShaderStageCreateInfo(
			const VkShaderStageFlagBits& stage_,
			const VkShaderModule& vk_shaderModule_,
			const String& name_,
			const VkSpecializationInfo* vk_specializationInfo_ = nullptr
		);
		inline PipelineShaderStageCreateInfo(const PipelineShaderStageCreateInfo&) = delete;
		inline ~PipelineShaderStageCreateInfo() = default;
	public:
		inline PipelineShaderStageCreateInfo& operator = (const PipelineShaderStageCreateInfo&) = delete;
	};
	class PipelineVertexInputStateCreateInfo:
		public VkPipelineVertexInputStateCreateInfo
	{
	public:
		inline PipelineVertexInputStateCreateInfo() = delete;
		inline PipelineVertexInputStateCreateInfo(const Vector<VkVertexInputBindingDescription>& bindings_, const Vector<VkVertexInputAttributeDescription>& attributes_);
		inline PipelineVertexInputStateCreateInfo(const PipelineVertexInputStateCreateInfo&) = delete;
		inline ~PipelineVertexInputStateCreateInfo() = default;
	public:
		inline PipelineVertexInputStateCreateInfo& operator = (const PipelineVertexInputStateCreateInfo&) = delete;
	};
	class PipelineInputAssemblyStateCreateInfo:
		public VkPipelineInputAssemblyStateCreateInfo
	{
	public:
		inline PipelineInputAssemblyStateCreateInfo() = delete;
		inline PipelineInputAssemblyStateCreateInfo(const VkPrimitiveTopology& topology_, const VkBool32& isPrimitiveRestartEnabled_);
		inline PipelineInputAssemblyStateCreateInfo(const PipelineInputAssemblyStateCreateInfo&) = delete;
		inline ~PipelineInputAssemblyStateCreateInfo() = default;
	public:
		inline PipelineInputAssemblyStateCreateInfo& operator = (const PipelineInputAssemblyStateCreateInfo&) = delete;
	};
	class PipelineViewportStateCreateInfo:
		public VkPipelineViewportStateCreateInfo
	{
	public:
		inline PipelineViewportStateCreateInfo() = delete;
		inline PipelineViewportStateCreateInfo(const Vector<VkViewport>& vk_viewports_, const Vector<VkRect2D>& vk_scissors_);
		inline PipelineViewportStateCreateInfo(const PipelineViewportStateCreateInfo&) = delete;
		inline ~PipelineViewportStateCreateInfo() = default;
	public:
		inline PipelineViewportStateCreateInfo& operator = (const PipelineViewportStateCreateInfo&) = delete;
	};
	class PipelineRasterizationStateCreateInfo:
		public VkPipelineRasterizationStateCreateInfo
	{
	public:
		inline PipelineRasterizationStateCreateInfo() = delete;
		inline PipelineRasterizationStateCreateInfo(
			const VkBool32& isDepthClampEnabled_,
			const VkBool32& isRasterizationDiscardEnabled_,
			const VkPolygonMode& polygonMode_,
			const VkCullModeFlags& cullMode_,
			const VkFrontFace& frontFace_,
			const VkBool32& isDepthBiasEnabled_,
			const float& depthBiasConstantFactor_,
			const float& depthBiasClamp_,
			const float& depthBiasSlopeFactor_,
			const float& lineWidth_
		);
		inline PipelineRasterizationStateCreateInfo(const PipelineRasterizationStateCreateInfo&) = delete;
		inline ~PipelineRasterizationStateCreateInfo() = default;
	public:
		inline PipelineRasterizationStateCreateInfo& operator = (const PipelineRasterizationStateCreateInfo&) = delete;
	};
	class PipelineMultisampleStateCreateInfo:
		public VkPipelineMultisampleStateCreateInfo
	{
	public:
		inline PipelineMultisampleStateCreateInfo() = delete;
		inline PipelineMultisampleStateCreateInfo(
			const VkSampleCountFlagBits& rasterizationSamples_,
			const VkBool32& isSampleShadingEnable_,
			const decltype(minSampleShading)& minSampleShading_,
			const VkSampleMask* vk_sampleMask_,
			const VkBool32& isAlphaToCoverageEnable_,
			const VkBool32& isAlphaToOneEnable_
		);
		inline PipelineMultisampleStateCreateInfo(const PipelineMultisampleStateCreateInfo&) = delete;
		inline ~PipelineMultisampleStateCreateInfo() = default;
	public:
		inline PipelineMultisampleStateCreateInfo& operator = (const PipelineMultisampleStateCreateInfo&) = delete;
	};
	class PipelineDepthStencilStateCreateInfo:
		public VkPipelineDepthStencilStateCreateInfo
	{
	public:
		inline PipelineDepthStencilStateCreateInfo() = delete;
		inline PipelineDepthStencilStateCreateInfo(
			const VkBool32& isDepthTestEnabled_,
			const VkBool32& isDepthWriteEnabled_,
			const VkCompareOp& depthCompareOp_,
			const VkBool32& isDepthBoundTestEnabled_,
			const VkBool32& isStencilTestEnabled_,
			const VkStencilOpState& front_,
			const VkStencilOpState& back_,
			const float& minDepthBounds_,
			const float& maxDepthBounds_
		);
		inline PipelineDepthStencilStateCreateInfo(const PipelineDepthStencilStateCreateInfo&) = delete;
		inline ~PipelineDepthStencilStateCreateInfo() = default;
	public:
		inline PipelineDepthStencilStateCreateInfo& operator = (const PipelineDepthStencilStateCreateInfo&) = delete;
	};
	class PipelineColorBlendStateCreateInfo:
		public VkPipelineColorBlendStateCreateInfo
	{
	public:
		inline PipelineColorBlendStateCreateInfo() = delete;
		inline PipelineColorBlendStateCreateInfo(
			const VkBool32& isLogicOpEnabled_,
			const VkLogicOp& logicOp_,
			const Vector<VkPipelineColorBlendAttachmentState>& vk_colorBlendAttachmentStates_,
			const Initializer<float>& blendConstants_ // size should be equal 4
		);
		inline PipelineColorBlendStateCreateInfo(const PipelineColorBlendStateCreateInfo&) = delete;
		inline ~PipelineColorBlendStateCreateInfo() = default;
	public:
		inline PipelineColorBlendStateCreateInfo& operator = (const PipelineColorBlendStateCreateInfo&) = delete;
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
	class MemoryAllocateInfo:
		public VkMemoryAllocateInfo
	{
	public:
		inline MemoryAllocateInfo() = delete;
		inline MemoryAllocateInfo(const decltype(allocationSize)& allocationSize_, const decltype(memoryTypeIndex)& memoryTypeIndex_);
		inline MemoryAllocateInfo(const MemoryAllocateInfo&) = delete;
		inline ~MemoryAllocateInfo() = default;
	public:
		inline MemoryAllocateInfo& operator = (const MemoryAllocateInfo&) = delete;
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
	class RenderPassBeginInfo:
		public VkRenderPassBeginInfo
	{
	public:
		inline RenderPassBeginInfo() = delete;
		inline RenderPassBeginInfo(const VkRenderPass& vk_renderPass_, const VkFramebuffer& vk_framebuffer_, const VkRect2D& vk_renderArea_, const Vector<VkClearValue>& vk_clearValues_);
		inline RenderPassBeginInfo(const RenderPassBeginInfo&) = delete;
		inline ~RenderPassBeginInfo() = default;
	public:
		inline RenderPassBeginInfo& operator = (const RenderPassBeginInfo&) = delete;
	};

	class SubmitInfo:
		public VkSubmitInfo
	{
	public:
		inline SubmitInfo() = delete;
		inline SubmitInfo(const Vector<VkCommandBuffer>& vk_commandBuffers_, const Vector<VkSemaphore>& vk_signalSemaphores_ = Vector<VkSemaphore>());
		inline SubmitInfo(const SubmitInfo&) = delete;
		inline ~SubmitInfo() = default;
	public:
		inline SubmitInfo& operator = (const SubmitInfo&) = delete;
	};

	class PresentInfoKHR:
		public VkPresentInfoKHR
	{
	public:
		inline PresentInfoKHR() = delete;
		inline PresentInfoKHR(const Vector<VkSemaphore>& vk_waitSemaphores_, const Vector<VkSwapchainKHR>& vk_swapchains_, const Vector<uint32_t>& vk_nextImageIndices_); // TODO: vk_swapchains_ and vk_nextImageIndices_ should have the same size!
		inline PresentInfoKHR(const PresentInfoKHR&) = delete;
		inline ~PresentInfoKHR() = default;
	public:
		inline PresentInfoKHR& operator = (const PresentInfoKHR&) = delete;
	};

	class Offset2D:
		public VkOffset2D
	{
	public:
		using X = decltype(x);
		using Y = decltype(y);
	public:
		inline Offset2D() = delete;
		inline Offset2D(const X& x_, const Y& y_);
		inline Offset2D(const Offset2D&) = delete;
		inline ~Offset2D() = default;
	public:
		inline Offset2D& operator = (const Offset2D&) = delete;
	};
	class Extent2D:
		public VkExtent2D
	{
	public:
		using Width = decltype(width);
		using Height = decltype(height);
	public:
		inline Extent2D() = delete;
		inline Extent2D(const Width& width_, const Height& height_);
		inline Extent2D(const Extent2D&) = delete;
		inline ~Extent2D() = default;
	public:
		inline Extent2D& operator = (const Extent2D&) = delete;
	};
	class Rect2D:
		public VkRect2D
	{
	public:
		using Offset = decltype(offset);
		using Extent = decltype(extent);
	public:
		inline Rect2D() = delete;
		inline Rect2D(const Offset& offset_, const Extent& extent_);
		inline Rect2D(const Rect2D&) = delete;
		inline ~Rect2D() = default;
	public:
		inline Rect2D& operator = (const Rect2D&) = delete;
	};
	class Viewport:
		public VkViewport
	{
	public:
		inline Viewport() = delete;
		inline Viewport(
			const float&	x_,
			const float&	y_,
			const float&	width_,
			const float&	height_,
			const float&	minDepth_,
			const float&	maxDepth_
		);
		inline Viewport(const Viewport&) = delete;
		inline ~Viewport() = default;
	public:
		inline Viewport& operator = (const Viewport&) = delete;
	};
	class ClearValue
	{
	public:
		inline static VkClearValue Color(const float& r_, const float& g_, const float& b_, const float& a_);
	public:
		inline ClearValue() = delete;
		inline ClearValue(const ClearValue&) = delete;
		inline ~ClearValue() = default;
	public:
		inline ClearValue& operator = (const ClearValue&) = delete;
	};
	class StencilOpState:
		public VkStencilOpState
	{
	public:
		inline StencilOpState() = delete;
		inline StencilOpState(
			const VkStencilOp&	failOp_,
			const VkStencilOp&	passOp_,
			const VkStencilOp&	depthFailOp_,
			const VkCompareOp&	compareOp_,
			const uint32_t&		compareMask_,
			const uint32_t&		writeMask_,
			const uint32_t&		reference_
		);
		inline StencilOpState(const StencilOpState&) = delete;
		inline ~StencilOpState() = default;
	public:
		inline StencilOpState& operator = (const StencilOpState&) = delete;
	};
	class PipelineColorBlendAttachmentState:
		public VkPipelineColorBlendAttachmentState
	{
	public:
		inline PipelineColorBlendAttachmentState() = delete;
		inline PipelineColorBlendAttachmentState(
			const VkBool32&					blendEnable_,
			const VkBlendFactor&			srcColorBlendFactor_,
			const VkBlendFactor&			dstColorBlendFactor_,
			const VkBlendOp&				colorBlendOp_,
			const VkBlendFactor&			srcAlphaBlendFactor_,
			const VkBlendFactor&			dstAlphaBlendFactor_,
			const VkBlendOp&				alphaBlendOp_,
			const VkColorComponentFlags&	colorWriteMask_
		);
		inline PipelineColorBlendAttachmentState(const PipelineColorBlendAttachmentState&) = delete;
		inline ~PipelineColorBlendAttachmentState() = default;
	public:
		inline PipelineColorBlendAttachmentState& operator = (const PipelineColorBlendAttachmentState&) = delete;
	};
	class ComponentMapping:
		public VkComponentMapping
	{
	public:
		inline ComponentMapping() = delete;
		inline ComponentMapping(const VkComponentSwizzle& r_, const VkComponentSwizzle& g_, const VkComponentSwizzle& b_, const VkComponentSwizzle& a_);
		inline ComponentMapping(const ComponentMapping&) = delete;
		inline ~ComponentMapping() = default;
	public:
		inline ComponentMapping& operator = (const ComponentMapping&) = delete;
	};
	class ImageSubresourceRange:
		public VkImageSubresourceRange
	{
	public:
		inline ImageSubresourceRange() = delete;
		inline ImageSubresourceRange(
			VkImageAspectFlags aspectMask_,
			uint32_t baseMipLevel_,
			uint32_t levelCount_,
			uint32_t baseArrayLayer_,
			uint32_t layerCount_
		);
		inline ImageSubresourceRange(const ImageSubresourceRange&) = delete;
		inline ~ImageSubresourceRange() = default;
	public:
		inline ImageSubresourceRange& operator = (const ImageSubresourceRange&) = delete;
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
	class VertexInputBindingDescription:
		public VkVertexInputBindingDescription
	{
	public:
		inline VertexInputBindingDescription() = delete;
		inline VertexInputBindingDescription(
			const uint32_t& binding_,
			const uint32_t& stride_,
			const VkVertexInputRate& inputRate_
		);
		inline VertexInputBindingDescription(const VertexInputBindingDescription&) = delete;
		inline ~VertexInputBindingDescription() = default;
	public:
		inline VertexInputBindingDescription& operator = (const VertexInputBindingDescription&) = delete;
	};
	class VertexInputAttributeDescription:
		public VkVertexInputAttributeDescription
	{
	public:
		inline VertexInputAttributeDescription() = delete;
		inline VertexInputAttributeDescription(
			const uint32_t& location_,
			const uint32_t& binding_,
			const VkFormat& format_,
			const uint32_t& offset_
		);
		inline VertexInputAttributeDescription(const VertexInputAttributeDescription&) = delete;
		inline ~VertexInputAttributeDescription() = default;
	public:
		inline VertexInputAttributeDescription& operator = (const VertexInputAttributeDescription&) = delete;
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

#if VK_USE_PLATFORM_WIN32_KHR
	class SwapchainCreateInfoKHR:
		public VkSwapchainCreateInfoKHR
	{
	public:
		inline SwapchainCreateInfoKHR() = delete;
		inline SwapchainCreateInfoKHR(
			VkSwapchainCreateFlagsKHR		flags_,
			VkSurfaceKHR					vk_surface_,
			uint32_t						minImageCount_,
			VkFormat						imageFormat_,
			VkColorSpaceKHR					imageColorSpace_,
			VkExtent2D						imageExtent_,
			uint32_t						imageArrayLayers_,
			VkImageUsageFlags				imageUsage_,
			VkSharingMode					imageSharingMode_,
			const Vector<uint32_t>&			queueFamilyIndices_,
			VkSurfaceTransformFlagBitsKHR	preTransform_,
			VkCompositeAlphaFlagBitsKHR		compositeAlpha_,
			VkPresentModeKHR				presentMode_,
			VkBool32						clipped_,
			VkSwapchainKHR					oldSwapchain_
		);
		inline SwapchainCreateInfoKHR(const SwapchainCreateInfoKHR&) = delete;
		inline ~SwapchainCreateInfoKHR() = default;
	public:
		inline SwapchainCreateInfoKHR& operator = (const SwapchainCreateInfoKHR&) = delete;
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

	// VK_KHR_surface
#if VK_USE_PLATFORM_WIN32_KHR
	inline VkBool32 GetPhysicalDeviceWin32PresentationSupportKHR(const VkPhysicalDevice& vk_physicalDevice_, const Size& queueFamilyIndex_);
	inline VkSurfaceKHR CreateWin32SurfaceKHR(const VkInstance& vk_instance_, const VkWin32SurfaceCreateInfoKHR& vk_win32SurfaceCreateInfo_);
	inline VkBool32 GetPhysicalDeviceSurfaceSupportKHR(const VkPhysicalDevice& vk_physicalDevice_, const decltype(VkDeviceQueueCreateInfo::queueFamilyIndex)& vk_queueFamilyIndex_, const VkSurfaceKHR& vk_surface_);
	inline VkSurfaceCapabilitiesKHR GetPhysicalDeviceSurfaceCapabilitiesKHR(const VkPhysicalDevice& vk_physicalDevice_, const VkSurfaceKHR& vk_surface_);
	inline Vector<VkSurfaceFormatKHR> GetPhysicalDeviceSurfaceFormatsKHR(const VkPhysicalDevice& vk_physicalDevice_, const VkSurfaceKHR& vk_surface_);
	inline void DestroySurfaceKHR(const VkInstance& vk_instance_, const VkSurfaceKHR& vk_surface_);
#endif

	// VK_KHR_swapchain
#if VK_USE_PLATFORM_WIN32_KHR
	inline VkSwapchainKHR CreateSwapchainKHR(const VkDevice& vk_device_, const VkSwapchainCreateInfoKHR& vk_swapchainCreateInfo_);
	inline Vector<VkImage> GetSwapchainImagesKHR(const VkDevice& vk_device_, const VkSwapchainKHR& vk_swapchain_);
	inline uint32_t AcquireNextImageKHR(const VkDevice& vk_device_, const VkSwapchainKHR& vk_swapchain_, const uint64_t& timeout_, const VkSemaphore& vk_semaphore_, const VkFence& vk_fence_);
	inline void DestroySwapchainKHR(const VkDevice& vk_device_, const VkSwapchainKHR& vk_swapchain_);
#endif

	// Queue
	inline VkQueue GetDeviceQueue(const VkDevice& vk_device_, const uint32_t& vk_queueFamilyIndex_, const uint32_t& vk_queueIndex_);
	inline void QueueSubmit(const VkQueue& vk_queue_, const Vector<VkSubmitInfo>& vk_submitInfos_, const VkFence& vk_fence_ = VK_NULL_HANDLE);
	inline void QueueWaitIdle(const VkQueue& vk_queue_);
	inline void QueuePresentKHR(const VkQueue& vk_queue_, const VkPresentInfoKHR& vk_presentInfoKHR_);

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

	// Image View
	inline VkImageView CreateImageView(const VkDevice& vk_device_, const VkImageViewCreateInfo& vk_imageViewCreateInfo_);
	inline void DestroyImageView(const VkDevice& vk_device_, const VkImageView& vk_imageView_);

	// Framebuffer
	inline VkFramebuffer CreateFramebuffer(const VkDevice& vk_device_, const VkFramebufferCreateInfo& vk_framebufferCreateInfo_);
	inline void DestroyFramebuffer(const VkDevice& vk_device_, const VkFramebuffer& vk_framebuffer_);

	// Commands
	inline void CmdBeginRenderPass(const VkCommandBuffer& vk_commandBuffer_, const VkRenderPassBeginInfo& vk_renderPassBeginInfo_, const VkSubpassContents& vk_subpassContents_);
	inline void CmdEndRenderPass(const VkCommandBuffer& vk_commandBuffer_);
	inline void CmdBindPipeline(const VkCommandBuffer& vk_commandBuffer_, const VkPipelineBindPoint& vk_pipelineBindPoint_, const VkPipeline& vk_pipeline_);
	inline void CmdBindVertexBuffers(const VkCommandBuffer& vk_commandBuffer_, const uint32_t& firstBinding_, const uint32_t& bindingsCount_, const Vector<VkBuffer>& vk_buffers_, const Vector<VkDeviceSize>& offsets_);
	inline void CmdDraw(const VkCommandBuffer& vk_commandBuffer_, const uint32_t& verticesCount_, const uint32_t& indicesCount_, const uint32_t& firstVertex_, const uint32_t& firstIndex_);

	// Fence
	inline VkFence CreateFence(const VkDevice& vk_device_, const VkFenceCreateInfo& vk_fenceCreateInfo_);
	inline void ResetFences(const VkDevice& vk_device_, const Vector<VkFence>& vk_fences_);
	inline bool WaitForFences(const VkDevice& vk_device_, const Vector<VkFence>& vk_fences_, const VkBool32& waitAll_, const uint64_t timeout_);
	inline void DestroyFence(const VkDevice& vk_device_, const VkFence& vk_fence_);

	// Buffer
	inline VkBuffer CreateBuffer(const VkDevice& vk_device_, const VkBufferCreateInfo& vk_bufferCreateInfo_);
	inline VkMemoryRequirements GetBufferMemoryRequirements(const VkDevice& vk_device_, const VkBuffer& vk_buffer_);
	inline void BindBufferMemory(const VkDevice& vk_device_, const VkBuffer& vk_buffer_, const VkDeviceMemory& vk_deviceMemory_);
	inline void DestroyBuffer(const VkDevice& vk_device_, const VkBuffer& vk_buffer_);

	// Device Memory
	inline VkDeviceMemory AllocateMemory(const VkDevice& vk_device_, const VkMemoryAllocateInfo& vk_memoryAllocateInfo_);
	inline void* MapMemory(const VkDevice& vk_device_, const VkDeviceMemory& vk_deviceMemory_, const VkDeviceSize& offset_, const VkDeviceSize& size_, const VkMemoryMapFlags& flags_);
	inline void UnmapMemory(const VkDevice& vk_device_, const VkDeviceMemory& vk_deviceMemory_);
	inline void FreeMemory(const VkDevice& vk_device_, const VkDeviceMemory& vk_deviceMemory_);

	// Shader Module
	inline VkShaderModule CreateShaderModule(const VkDevice& vk_device_, const VkShaderModuleCreateInfo& vk_shaderModuleCreateInfo_);
	inline void DestroyShaderModule(const VkDevice& vk_device_, const VkShaderModule& vk_shaderModule_);

	// Pipeline Layout
	inline VkPipelineLayout CreatePipelineLayout(const VkDevice& vk_device_, const VkPipelineLayoutCreateInfo& vk_pipelineLayoutCreateInfo_);
	inline void DestroyPipelineLayout(const VkDevice& vk_device_, const VkPipelineLayout& vk_pipelineLayout_);
	
	// Pipeline
	inline Vector<VkPipeline> CreateGraphicsPipelines(const VkDevice& vk_device_, const VkPipelineCache& vk_pipelineCache_, const Vector<VkGraphicsPipelineCreateInfo>& vk_createInfos_);
	inline void DestroyPipeline(const VkDevice& vk_device_, const VkPipeline& vk_pipeline_);
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

#pragma region CommandPoolCreateInfo

inline GreatVulkan::CommandPoolCreateInfo::CommandPoolCreateInfo(const VkCommandPoolCreateFlags& flags_, const decltype(queueFamilyIndex)& queueFamilyIndex_)
{
	sType				= VkStructureType::VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
	pNext				= nullptr;
	flags				= flags_;
	queueFamilyIndex	= queueFamilyIndex_;
}

#pragma endregion

#pragma region ImageViewCreateInfo

inline GreatVulkan::ImageViewCreateInfo::ImageViewCreateInfo(
	const VkImageViewCreateFlags& flags_,
	const VkImage& vk_image_,
	const VkImageViewType& vk_imageViewType_,
	const VkFormat& format_,
	const VkComponentMapping& vk_componentMapping_,
	const VkImageSubresourceRange& vk_imageSubresourceRange_
) {
	sType				= VkStructureType::VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
	pNext				= nullptr;
	flags				= flags_;
	image				= vk_image_;
	viewType			= vk_imageViewType_;
	format				= format_;
	components			= vk_componentMapping_;
	subresourceRange	= vk_imageSubresourceRange_;
}

#pragma endregion

#pragma region FramebufferCreateInfo

inline GreatVulkan::FramebufferCreateInfo::FramebufferCreateInfo(
	const VkRenderPass& vk_renderPass_,
	const Vector<VkImageView>& attachments_,
	const decltype(width)& width_,
	const decltype(height)& height_,
	const decltype(layers)& layers_
) {
	sType				= VkStructureType::VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
	pNext				= nullptr;
	flags				= 0;
	renderPass			= vk_renderPass_;
	attachmentCount		= attachments_.size();
	pAttachments		= attachments_.data();
	width				= width_;
	height				= height_;
	layers				= layers_;
}

#pragma endregion

#pragma region FenceCreateInfo

inline GreatVulkan::FenceCreateInfo::FenceCreateInfo(const VkFenceCreateFlags& flags_)
{
	sType	= VkStructureType::VK_STRUCTURE_TYPE_FENCE_CREATE_INFO;
	pNext	= nullptr;
	flags	= flags_;
}

#pragma endregion

#pragma region BufferCreateInfo

inline GreatVulkan::BufferCreateInfo::BufferCreateInfo(
	const VkBufferCreateFlags& flags_,
	const VkDeviceSize size_,
	const VkBufferUsageFlags& usage_,
	const VkSharingMode& sharingMode_,
	const Vector<uint32_t>& queueFamilyIndices_
) {
	sType					= VkStructureType::VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
	pNext					= nullptr;
	flags					= flags_;
	size					= size_;
	usage					= usage_;
	sharingMode				= sharingMode_;
	queueFamilyIndexCount	= queueFamilyIndices_.size();
	pQueueFamilyIndices		= queueFamilyIndices_.data();
}

#pragma endregion

#pragma region ShaderModuleCreateInfo

inline GreatVulkan::ShaderModuleCreateInfo::ShaderModuleCreateInfo(const Vector<uint32_t>& sourceCode_)
{
	sType		= VkStructureType::VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
	pNext		= nullptr;
	flags		= 0;
	codeSize	= sourceCode_.size();
	pCode		= sourceCode_.data();
}

#pragma endregion

#pragma region PipelineLayoutCreateInfo

inline GreatVulkan::PipelineLayoutCreateInfo::PipelineLayoutCreateInfo(const Vector<VkDescriptorSetLayout>& vk_descriptorSetLayouts_, const Vector<VkPushConstantRange>& vk_pushConstantRanges_)
{
	sType					= VkStructureType::VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
	pNext					= nullptr;
	flags					= 0;
	setLayoutCount			= vk_descriptorSetLayouts_.size();
	pSetLayouts				= vk_descriptorSetLayouts_.data();
	pushConstantRangeCount	= vk_pushConstantRanges_.size();
	pPushConstantRanges		= vk_pushConstantRanges_.data();
}

#pragma endregion

#pragma region GraphicsPipelineCreateInfo

inline GreatVulkan::GraphicsPipelineCreateInfo::GraphicsPipelineCreateInfo(
	const VkPipelineCreateFlags& flags_,
	const Vector<VkPipelineShaderStageCreateInfo>& vk_stages_,
	const VkPipelineVertexInputStateCreateInfo& vk_vertexInputState_,
	const VkPipelineInputAssemblyStateCreateInfo& vk_inputAssemblyState_,
	const VkPipelineViewportStateCreateInfo& vk_viewportState_,
	const VkPipelineRasterizationStateCreateInfo& vk_rasterizationState_,
	const VkPipelineMultisampleStateCreateInfo& vk_multisampleState_,
	const VkPipelineDepthStencilStateCreateInfo& vk_depthStencilState_,
	const VkPipelineColorBlendStateCreateInfo& vk_colorBlendState_,
	const VkPipelineLayout& vk_pipelineLayout_,
	const VkRenderPass& vk_renderPass_,
	const uint32_t subpass_
): GraphicsPipelineCreateInfo(
	flags_,
	vk_stages_,
	&vk_vertexInputState_,
	&vk_inputAssemblyState_,
	nullptr,
	&vk_viewportState_,
	&vk_rasterizationState_,
	&vk_multisampleState_,
	&vk_depthStencilState_,
	&vk_colorBlendState_,
	nullptr,
	vk_pipelineLayout_,
	vk_renderPass_,
	subpass_,
	VK_NULL_HANDLE,
	-1
)
{
}
inline GreatVulkan::GraphicsPipelineCreateInfo::GraphicsPipelineCreateInfo(
	const VkPipelineCreateFlags& flags_,
	const Vector<VkPipelineShaderStageCreateInfo>& vk_stages_,
	const VkPipelineVertexInputStateCreateInfo* vk_vertexInputState_,
	const VkPipelineInputAssemblyStateCreateInfo* vk_inputAssemblyState_,
	const VkPipelineTessellationStateCreateInfo* vk_tessellationState_,
	const VkPipelineViewportStateCreateInfo* vk_viewportState_,
	const VkPipelineRasterizationStateCreateInfo* vk_rasterizationState_,
	const VkPipelineMultisampleStateCreateInfo* vk_multisampleState_,
	const VkPipelineDepthStencilStateCreateInfo* vk_depthStencilState_,
	const VkPipelineColorBlendStateCreateInfo* vk_colorBlendState_,
	const VkPipelineDynamicStateCreateInfo* vk_dynamicState_,
	const VkPipelineLayout& vk_pipelineLayout_,
	const VkRenderPass& vk_renderPass_,
	const uint32_t subpass_,
	const VkPipeline vk_basePipeline_,
	const int32_t basePipelineIndex_
) {
	sType				= VkStructureType::VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO;
	pNext				= nullptr;
	flags				= flags_;
	stageCount			= vk_stages_.size();
	pStages				= vk_stages_.data();
	pVertexInputState	= vk_vertexInputState_;
	pInputAssemblyState	= vk_inputAssemblyState_;
	pTessellationState	= vk_tessellationState_;
	pViewportState		= vk_viewportState_;
	pRasterizationState	= vk_rasterizationState_;
	pMultisampleState	= vk_multisampleState_;
	pDepthStencilState	= vk_depthStencilState_;
	pColorBlendState	= vk_colorBlendState_;
	pDynamicState		= vk_dynamicState_;
	layout				= vk_pipelineLayout_;
	renderPass			= vk_renderPass_;
	subpass				= subpass_;
	basePipelineHandle	= vk_basePipeline_;
	basePipelineIndex	= basePipelineIndex_;
}

#pragma endregion

#pragma region PipelineShaderStageCreateInfo

inline GreatVulkan::PipelineShaderStageCreateInfo::PipelineShaderStageCreateInfo(
	const VkShaderStageFlagBits& stage_,
	const VkShaderModule& vk_shaderModule_,
	const String& name_,
	const VkSpecializationInfo* vk_specializationInfo_
):
	nameContainer(name_)
{
	sType					= VkStructureType::VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
	pNext					= nullptr;
	flags					= 0;
	stage					= stage_;
	module					= vk_shaderModule_;
	pName					= nameContainer.c_str();
	pSpecializationInfo		= vk_specializationInfo_;
}

#pragma endregion

#pragma region PipelineVertexInputStateCreateInfo

inline GreatVulkan::PipelineVertexInputStateCreateInfo::PipelineVertexInputStateCreateInfo(const Vector<VkVertexInputBindingDescription>& bindings_, const Vector<VkVertexInputAttributeDescription>& attributes_)
{
	sType								= VkStructureType::VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO;
	pNext								= nullptr;
	flags								= 0;
	vertexBindingDescriptionCount		= bindings_.size();
	pVertexBindingDescriptions			= bindings_.data();
	vertexAttributeDescriptionCount		= attributes_.size();
	pVertexAttributeDescriptions		= attributes_.data();
}

#pragma endregion

#pragma region PipelineInputAssemblyStateCreateInfo

inline GreatVulkan::PipelineInputAssemblyStateCreateInfo::PipelineInputAssemblyStateCreateInfo(const VkPrimitiveTopology& topology_, const VkBool32& isPrimitiveRestartEnabled_)
{
	sType					= VkStructureType::VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO;
	pNext					= nullptr;
	flags					= 0;
	topology				= topology_;
	primitiveRestartEnable	= isPrimitiveRestartEnabled_;
}

#pragma endregion

#pragma region PipelineViewportStateCreateInfo

inline GreatVulkan::PipelineViewportStateCreateInfo::PipelineViewportStateCreateInfo(const Vector<VkViewport>& vk_viewports_, const Vector<VkRect2D>& vk_scissors_)
{
	sType			= VkStructureType::VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO;
	pNext			= nullptr;
	flags			= 0;
	viewportCount	= vk_viewports_.size();
	pViewports		= vk_viewports_.data();
	scissorCount	= vk_scissors_.size();
	pScissors		= vk_scissors_.data();
}

#pragma endregion

#pragma region PipelineViewportStateCreateInfo

inline GreatVulkan::PipelineRasterizationStateCreateInfo::PipelineRasterizationStateCreateInfo(
	const VkBool32& isDepthClampEnabled_,
	const VkBool32& isRasterizationDiscardEnabled_,
	const VkPolygonMode& polygonMode_,
	const VkCullModeFlags& cullMode_,
	const VkFrontFace& frontFace_,
	const VkBool32& isDepthBiasEnabled_,
	const float& depthBiasConstantFactor_,
	const float& depthBiasClamp_,
	const float& depthBiasSlopeFactor_,
	const float& lineWidth_
) {
	sType					= VkStructureType::VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO;
	pNext					= nullptr;
	flags					= 0;
	depthClampEnable		= isDepthClampEnabled_;
	rasterizerDiscardEnable	= isRasterizationDiscardEnabled_;
	polygonMode				= polygonMode_;
	cullMode				= cullMode_;
	frontFace				= frontFace_;
	depthBiasEnable			= isDepthBiasEnabled_;
	depthBiasConstantFactor	= depthBiasConstantFactor_;
	depthBiasClamp			= depthBiasClamp_;
	depthBiasSlopeFactor	= depthBiasSlopeFactor_;
	lineWidth				= lineWidth_;
}

#pragma endregion

#pragma region PipelineMultisampleStateCreateInfo

inline GreatVulkan::PipelineMultisampleStateCreateInfo::PipelineMultisampleStateCreateInfo(
	const VkSampleCountFlagBits& rasterizationSamples_,
	const VkBool32& isSampleShadingEnable_,
	const decltype(minSampleShading)& minSampleShading_,
	const VkSampleMask* vk_sampleMask_,
	const VkBool32& isAlphaToCoverageEnable_,
	const VkBool32& isAlphaToOneEnable_
) {
	sType					= VkStructureType::VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO;
	pNext					= nullptr;
	flags					= 0;
	rasterizationSamples	= rasterizationSamples_;
	sampleShadingEnable		= isSampleShadingEnable_;
	minSampleShading		= minSampleShading_;
	pSampleMask				= vk_sampleMask_;
	alphaToCoverageEnable	= isAlphaToCoverageEnable_;
	alphaToOneEnable		= isAlphaToOneEnable_;
}

#pragma endregion

#pragma region PipelineDepthStencilStateCreateInfo

inline GreatVulkan::PipelineDepthStencilStateCreateInfo::PipelineDepthStencilStateCreateInfo(
	const VkBool32& isDepthTestEnabled_,
	const VkBool32& isDepthWriteEnabled_,
	const VkCompareOp& depthCompareOp_,
	const VkBool32& isDepthBoundTestEnabled_,
	const VkBool32& isStencilTestEnabled_,
	const VkStencilOpState& front_,
	const VkStencilOpState& back_,
	const float& minDepthBounds_,
	const float& maxDepthBounds_
) {
	sType					= VkStructureType::VK_STRUCTURE_TYPE_PIPELINE_DEPTH_STENCIL_STATE_CREATE_INFO;
	pNext					= nullptr;
	flags					= 0;
	depthTestEnable			= isDepthTestEnabled_;
	depthWriteEnable		= isDepthWriteEnabled_;
	depthCompareOp			= depthCompareOp_;
	depthBoundsTestEnable	= isDepthBoundTestEnabled_;
	stencilTestEnable		= isStencilTestEnabled_;
	front					= front_;
	back					= back_;
	minDepthBounds			= minDepthBounds_;
	maxDepthBounds			= maxDepthBounds_;
}

#pragma endregion

#pragma region PipelineColorBlendStateCreateInfo

inline GreatVulkan::PipelineColorBlendStateCreateInfo::PipelineColorBlendStateCreateInfo(
	const VkBool32& isLogicOpEnabled_,
	const VkLogicOp& logicOp_,
	const Vector<VkPipelineColorBlendAttachmentState>& vk_colorBlendAttachmentStates_,
	const Initializer<float>& blendConstants_
) {
	sType					= VkStructureType::VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO;
	pNext					= nullptr;
	flags					= 0;
	logicOpEnable			= isLogicOpEnabled_;
	logicOp					= logicOp_;
	attachmentCount			= vk_colorBlendAttachmentStates_.size();
	pAttachments			= vk_colorBlendAttachmentStates_.data();

	if (blendConstants_.size() != 4)
	{
		throw Exception(); // TODO
	}

	auto it = blendConstants_.begin();

	for (Size i = 0; i < 4; ++i)
	{
		blendConstants[i] = *it;
		++it;
	}
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

#pragma region SwapchainCreateInfoKHR

#if VK_USE_PLATFORM_WIN32_KHR

inline GreatVulkan::SwapchainCreateInfoKHR::SwapchainCreateInfoKHR(
    VkSwapchainCreateFlagsKHR		flags_,
	VkSurfaceKHR					vk_surface_,
    uint32_t						minImageCount_,
    VkFormat						imageFormat_,
    VkColorSpaceKHR					imageColorSpace_,
    VkExtent2D						imageExtent_,
    uint32_t						imageArrayLayers_,
    VkImageUsageFlags				imageUsage_,
    VkSharingMode					imageSharingMode_,
	const Vector<uint32_t>&			queueFamilyIndices_,
    VkSurfaceTransformFlagBitsKHR	preTransform_,
    VkCompositeAlphaFlagBitsKHR		compositeAlpha_,
    VkPresentModeKHR				presentMode_,
    VkBool32						clipped_,
    VkSwapchainKHR					oldSwapchain_
) {
	sType					= VkStructureType::VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR;
	pNext					= nullptr;
	flags					= flags_;
	surface					= vk_surface_;
	minImageCount			= minImageCount_;
	imageFormat				= imageFormat_;
	imageColorSpace			= imageColorSpace_;
	imageExtent				= imageExtent_;
	imageArrayLayers		= imageArrayLayers_;
	imageUsage				= imageUsage_;
	imageSharingMode		= imageSharingMode_;
	queueFamilyIndexCount	= queueFamilyIndices_.size();
	pQueueFamilyIndices		= queueFamilyIndices_.data();
	preTransform			= preTransform_;
	compositeAlpha			= compositeAlpha_;
	presentMode				= presentMode_;
	clipped					= clipped_;
	oldSwapchain			= oldSwapchain_;
}

#endif

#pragma endregion

#pragma region CommandBufferAllocateInfo

inline GreatVulkan::CommandBufferAllocateInfo::CommandBufferAllocateInfo(const VkCommandPool& vk_commandPool_, const VkCommandBufferLevel& level_, const decltype(commandBufferCount)& commandBufferCount_)
{
	sType				= VkStructureType::VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
	pNext				= nullptr;
	commandPool			= vk_commandPool_;
	level				= level_;
	commandBufferCount	= commandBufferCount_;
}

#pragma endregion

#pragma region MemoryAllocateInfo

inline GreatVulkan::MemoryAllocateInfo::MemoryAllocateInfo(const decltype(allocationSize)& allocationSize_, const decltype(memoryTypeIndex)& memoryTypeIndex_)
{
	sType			= VkStructureType::VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
	pNext			= nullptr;
	allocationSize	= allocationSize_;
	memoryTypeIndex = memoryTypeIndex_;
}

#pragma endregion

#pragma region CommandBufferBeginInfo

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

#pragma region RenderPassBeginInfo

inline GreatVulkan::RenderPassBeginInfo::RenderPassBeginInfo(const VkRenderPass& vk_renderPass_, const VkFramebuffer& vk_framebuffer_, const VkRect2D& vk_renderArea_, const Vector<VkClearValue>& vk_clearValues_)
{
	sType			= VkStructureType::VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
	pNext			= nullptr;
	renderPass		= vk_renderPass_;
	framebuffer		= vk_framebuffer_;
	renderArea		= vk_renderArea_;
	clearValueCount	= vk_clearValues_.size();
	pClearValues	= vk_clearValues_.data();
}

#pragma endregion

#pragma region SubmitInfo

inline GreatVulkan::SubmitInfo::SubmitInfo(const Vector<VkCommandBuffer>& vk_commandBuffers_, const Vector<VkSemaphore>& vk_signalSemaphores_)
{
	sType					= VkStructureType::VK_STRUCTURE_TYPE_SUBMIT_INFO;
	pNext					= nullptr;
	waitSemaphoreCount		= 0; // TODO: vk_waitSemaphores_.size();
	pWaitSemaphores			= nullptr; // TODO: vk_waitSemaphores_.data();
	pWaitDstStageMask		= nullptr; // TODO
	commandBufferCount		= vk_commandBuffers_.size();
	pCommandBuffers			= vk_commandBuffers_.data();
	signalSemaphoreCount	= vk_signalSemaphores_.size();
	pSignalSemaphores		= vk_signalSemaphores_.data();
}

#pragma endregion

#pragma region PresentInfoKHR

inline GreatVulkan::PresentInfoKHR::PresentInfoKHR(const Vector<VkSemaphore>& vk_waitSemaphores_, const Vector<VkSwapchainKHR>& vk_swapchains_, const Vector<uint32_t>& vk_nextImageIndices_)
{
	sType				= VkStructureType::VK_STRUCTURE_TYPE_PRESENT_INFO_KHR;
	pNext				= nullptr;
	waitSemaphoreCount	= vk_waitSemaphores_.size();
	pWaitSemaphores		= vk_waitSemaphores_.data();
	swapchainCount		= vk_swapchains_.size();
	pSwapchains			= vk_swapchains_.data();
	pImageIndices		= vk_nextImageIndices_.data();
	pResults			= nullptr; // may be NULL, TODO: implement
}

#pragma endregion

#pragma region Offset2D

inline GreatVulkan::Offset2D::Offset2D(const X& x_, const Y& y_)
{
	x = x_;
	y = y_;
}

#pragma endregion

#pragma region Offset2D

inline GreatVulkan::Extent2D::Extent2D(const Width& width_, const Height& height_)
{
	width = width_;
	height = height_;
}

#pragma endregion

#pragma region Rect2D

inline GreatVulkan::Rect2D::Rect2D(const Offset& offset_, const Extent& extent_)
{
	offset = offset_;
	extent = extent_;
}

#pragma endregion

#pragma region Viewport

inline GreatVulkan::Viewport::Viewport(
	const float&	x_,
	const float&	y_,
	const float&	width_,
	const float&	height_,
	const float&	minDepth_,
	const float&	maxDepth_
) {
	x			= x_;
	y			= y_;
	width		= width_;
	height		= height_;
	minDepth	= minDepth_;
	maxDepth	= maxDepth_;
}

#pragma endregion

#pragma region ClearValue

inline VkClearValue GreatVulkan::ClearValue::Color(const float& r_, const float& g_, const float& b_, const float& a_)
{
	VkClearValue vk_clearValue;

	vk_clearValue.color.float32[0] = r_;
	vk_clearValue.color.float32[1] = g_;
	vk_clearValue.color.float32[2] = b_;
	vk_clearValue.color.float32[3] = a_;

	return vk_clearValue;
}

#pragma endregion

#pragma region StencilOpState

inline GreatVulkan::StencilOpState::StencilOpState(
	const VkStencilOp&	failOp_,
	const VkStencilOp&	passOp_,
	const VkStencilOp&	depthFailOp_,
	const VkCompareOp&	compareOp_,
	const uint32_t&		compareMask_,
	const uint32_t&		writeMask_,
	const uint32_t&		reference_
) {
	failOp		= failOp_;
	passOp		= passOp_;
	depthFailOp	= depthFailOp_;
	compareOp	= compareOp_;
	compareMask	= compareMask_;
	writeMask	= writeMask_;
	reference	= reference_;
}

#pragma endregion

#pragma region PipelineColorBlendAttachmentState

inline GreatVulkan::PipelineColorBlendAttachmentState::PipelineColorBlendAttachmentState(
	const VkBool32&					blendEnable_,
	const VkBlendFactor&			srcColorBlendFactor_,
	const VkBlendFactor&			dstColorBlendFactor_,
	const VkBlendOp&				colorBlendOp_,
	const VkBlendFactor&			srcAlphaBlendFactor_,
	const VkBlendFactor&			dstAlphaBlendFactor_,
	const VkBlendOp&				alphaBlendOp_,
	const VkColorComponentFlags&	colorWriteMask_
) {
	blendEnable = blendEnable_;
	srcColorBlendFactor = srcColorBlendFactor_;
	dstColorBlendFactor = dstColorBlendFactor_;
	colorBlendOp = colorBlendOp_;
	srcAlphaBlendFactor = srcAlphaBlendFactor_;
	dstAlphaBlendFactor = dstAlphaBlendFactor_;
	alphaBlendOp = alphaBlendOp_;
	colorWriteMask = colorWriteMask_;
}

#pragma endregion

#pragma region ComponentMapping

inline GreatVulkan::ComponentMapping::ComponentMapping(const VkComponentSwizzle& r_, const VkComponentSwizzle& g_, const VkComponentSwizzle& b_, const VkComponentSwizzle& a_)
{
	r	= r_;
	g	= g_;
	b	= b_;
	a	= a_;
}

#pragma endregion

#pragma region ImageSubresourceRange

inline GreatVulkan::ImageSubresourceRange::ImageSubresourceRange(
	VkImageAspectFlags aspectMask_,
	uint32_t baseMipLevel_,
	uint32_t levelCount_,
	uint32_t baseArrayLayer_,
	uint32_t layerCount_
) {
	aspectMask = aspectMask_;
	baseMipLevel = baseMipLevel_;
	levelCount = levelCount_;
	baseArrayLayer = baseArrayLayer_;
	layerCount = layerCount_;
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

#pragma region AttachmentReference

inline GreatVulkan::AttachmentReference::AttachmentReference(const decltype(attachment)& attachment_, const VkImageLayout& layout_)
{
	attachment	= attachment_;
	layout		= layout_;
}

#pragma endregion

#pragma region SubpassDescription

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

#pragma region VertexInputBindingDescription

inline GreatVulkan::VertexInputBindingDescription::VertexInputBindingDescription(
	const uint32_t& binding_,
	const uint32_t& stride_,
	const VkVertexInputRate& inputRate_
) {
	binding		= binding_;
	stride		= stride_;
	inputRate	= inputRate_;
}

#pragma endregion

#pragma region VertexInputAttributeDescription

inline GreatVulkan::VertexInputAttributeDescription::VertexInputAttributeDescription(
	const uint32_t& location_,
	const uint32_t& binding_,
	const VkFormat& format_,
	const uint32_t& offset_
)
{
	location	= location_;
	binding		= binding_;
	format		= format_;
	offset		= offset_;
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

// VK_KHR_swapchain
#if VK_USE_PLATFORM_WIN32_KHR

inline VkSwapchainKHR GreatVulkan::CreateSwapchainKHR(const VkDevice& vk_device_, const VkSwapchainCreateInfoKHR& vk_swapchainCreateInfo_)
{
	VkSwapchainKHR vk_swapchain;

	if (auto result = Result(vkCreateSwapchainKHR(vk_device_, &vk_swapchainCreateInfo_, nullptr, &vk_swapchain)))
	{
		return vk_swapchain;
	}
	else
	{
		throw Exception(); // TODO
	}
}
inline GreatVulkan::Vector<VkImage> GreatVulkan::GetSwapchainImagesKHR(const VkDevice& vk_device_, const VkSwapchainKHR& vk_swapchain_)
{
	uint32_t vk_swapchainImagesCount;

	if (auto result = Result(vkGetSwapchainImagesKHR(vk_device_, vk_swapchain_, &vk_swapchainImagesCount, nullptr)))
	{
		Vector<VkImage> vk_swapchainImages(vk_swapchainImagesCount);

		if (auto result = Result(vkGetSwapchainImagesKHR(vk_device_, vk_swapchain_, &vk_swapchainImagesCount, vk_swapchainImages.data())))
		{
			return Move(vk_swapchainImages);
		}
		else
		{
			throw Exception();
		}
	}
	else
	{
		throw Exception();
	}
}
inline uint32_t GreatVulkan::AcquireNextImageKHR(const VkDevice& vk_device_, const VkSwapchainKHR& vk_swapchain_, const uint64_t& timeout_, const VkSemaphore& vk_semaphore_, const VkFence& vk_fence_)
{
	uint32_t vk_nextImageIndex;

	if (auto result = Result(vkAcquireNextImageKHR(vk_device_, vk_swapchain_, timeout_, vk_semaphore_, vk_fence_, &vk_nextImageIndex)))
	{
		return vk_nextImageIndex;
	}
	else
	{
		throw Exception(); // TODO
	}
}
inline void GreatVulkan::DestroySwapchainKHR(const VkDevice& vk_device_, const VkSwapchainKHR& vk_swapchain_)
{
	vkDestroySwapchainKHR(vk_device_, vk_swapchain_, nullptr);
}

#endif

// Queue
inline VkQueue GreatVulkan::GetDeviceQueue(const VkDevice& vk_device_, const uint32_t& vk_queueFamilyIndex_, const uint32_t& vk_queueIndex_)
{
	VkQueue vk_queue = VK_NULL_HANDLE; // TODO: should we really need to initialize this value?

	vkGetDeviceQueue(vk_device_, vk_queueFamilyIndex_, vk_queueIndex_, &vk_queue);

	return vk_queue;
}
inline void GreatVulkan::QueueSubmit(const VkQueue& vk_queue_, const Vector<VkSubmitInfo>& vk_submitInfos_, const VkFence& vk_fence_)
{
	if (auto result = Result(vkQueueSubmit(vk_queue_, vk_submitInfos_.size(), vk_submitInfos_.data(), vk_fence_))); else
	{
		throw Exception(); // TODO
	}
}
inline void GreatVulkan::QueueWaitIdle(const VkQueue& vk_queue_)
{
	if (auto result = Result(vkQueueWaitIdle(vk_queue_))); else
	{
		throw Exception(); // TODO
	}
}
inline void GreatVulkan::QueuePresentKHR(const VkQueue& vk_queue_, const VkPresentInfoKHR& vk_presentInfoKHR_)
{
	if (auto result = Result(vkQueuePresentKHR(vk_queue_, &vk_presentInfoKHR_))); else
	{
		throw Exception(); // TODO
	}
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

// Image View
inline VkImageView GreatVulkan::CreateImageView(const VkDevice& vk_device_, const VkImageViewCreateInfo& vk_imageViewCreateInfo_)
{
	VkImageView vk_imageView;

	if (auto result = Result(vkCreateImageView(vk_device_, &vk_imageViewCreateInfo_, nullptr, &vk_imageView)))
	{
		return vk_imageView;
	}
	else
	{
		throw Exception(); // TODO
	}
}
inline void GreatVulkan::DestroyImageView(const VkDevice& vk_device_, const VkImageView& vk_imageView_)
{
	vkDestroyImageView(vk_device_, vk_imageView_, nullptr);
}

// Framebuffer
inline VkFramebuffer GreatVulkan::CreateFramebuffer(const VkDevice& vk_device_, const VkFramebufferCreateInfo& vk_framebufferCreateInfo_)
{
	VkFramebuffer vk_framebuffer;

	if (auto result = Result(vkCreateFramebuffer(vk_device_, &vk_framebufferCreateInfo_, nullptr, &vk_framebuffer)))
	{
		return vk_framebuffer;
	}
	else
	{
		throw Exception(); // TODO
	}
}
inline void GreatVulkan::DestroyFramebuffer(const VkDevice& vk_device_, const VkFramebuffer& vk_framebuffer_)
{
	vkDestroyFramebuffer(vk_device_, vk_framebuffer_, nullptr);
}

// Commands
inline void GreatVulkan::CmdBeginRenderPass(const VkCommandBuffer& vk_commandBuffer_, const VkRenderPassBeginInfo& vk_renderPassBeginInfo_, const VkSubpassContents& vk_subpassContents_)
{
	vkCmdBeginRenderPass(vk_commandBuffer_, &vk_renderPassBeginInfo_, vk_subpassContents_);
}
inline void GreatVulkan::CmdEndRenderPass(const VkCommandBuffer& vk_commandBuffer_)
{
	vkCmdEndRenderPass(vk_commandBuffer_);
}
inline void GreatVulkan::CmdBindPipeline(const VkCommandBuffer& vk_commandBuffer_, const VkPipelineBindPoint& vk_pipelineBindPoint_, const VkPipeline& vk_pipeline_)
{
	vkCmdBindPipeline(vk_commandBuffer_, vk_pipelineBindPoint_, vk_pipeline_);
}
inline void GreatVulkan::CmdBindVertexBuffers(const VkCommandBuffer& vk_commandBuffer_, const uint32_t& firstBinding_, const uint32_t& bindingsCount_, const Vector<VkBuffer>& vk_buffers_, const Vector<VkDeviceSize>& offsets_)
{
	vkCmdBindVertexBuffers(vk_commandBuffer_, firstBinding_, bindingsCount_, vk_buffers_.data(), offsets_.data());
}
inline void GreatVulkan::CmdDraw(const VkCommandBuffer& vk_commandBuffer_, const uint32_t& verticesCount_, const uint32_t& indicesCount_, const uint32_t& firstVertex_, const uint32_t& firstIndex_)
{
	vkCmdDraw(vk_commandBuffer_, verticesCount_, indicesCount_, firstVertex_, firstIndex_);
}

// Fence
inline VkFence GreatVulkan::CreateFence(const VkDevice& vk_device_, const VkFenceCreateInfo& vk_fenceCreateInfo_)
{
	VkFence vk_fence;

	if (auto result = Result(vkCreateFence(vk_device_, &vk_fenceCreateInfo_, nullptr, &vk_fence)))
	{
		return vk_fence;
	}
	else
	{
		throw Exception(); // TODO
	}
}
inline void GreatVulkan::ResetFences(const VkDevice& vk_device_, const Vector<VkFence>& vk_fences_)
{
	if (auto result = Result(vkResetFences(vk_device_, vk_fences_.size(), vk_fences_.data()))); else
	{
		throw Exception(); // TODO
	}
}
inline bool GreatVulkan::WaitForFences(const VkDevice& vk_device_, const Vector<VkFence>& vk_fences_, const VkBool32& waitAll_, const uint64_t timeout_)
{
	if (auto result = Result(vkWaitForFences(vk_device_, vk_fences_.size(), vk_fences_.data(), waitAll_, timeout_)))
	{
		return true;
	}
	else
	{
		if (static_cast<const Result::Value>(result) == Result::Value::Timeout)
		{
			return false;
		}
		else
		{
			throw Exception(); // TODO
		}
	}
}
inline void GreatVulkan::DestroyFence(const VkDevice& vk_device_, const VkFence& vk_fence_)
{
	vkDestroyFence(vk_device_, vk_fence_, nullptr);
}

// Buffer
inline VkBuffer GreatVulkan::CreateBuffer(const VkDevice& vk_device_, const VkBufferCreateInfo& vk_bufferCreateInfo_)
{
	VkBuffer vk_buffer;

	if (auto result = Result(vkCreateBuffer(vk_device_, &vk_bufferCreateInfo_, nullptr, &vk_buffer)))
	{
		return vk_buffer;
	}
	else
	{
		throw Exception(); // TODO
	}
}
inline VkMemoryRequirements GreatVulkan::GetBufferMemoryRequirements(const VkDevice& vk_device_, const VkBuffer& vk_buffer_)
{
	VkMemoryRequirements vk_memoryRequirements;

	vkGetBufferMemoryRequirements(vk_device_, vk_buffer_, &vk_memoryRequirements);

	return vk_memoryRequirements;
}
inline void GreatVulkan::BindBufferMemory(const VkDevice& vk_device_, const VkBuffer& vk_buffer_, const VkDeviceMemory& vk_deviceMemory_)
{
	if (auto result = Result(vkBindBufferMemory(vk_device_, vk_buffer_, vk_deviceMemory_, 0))); else
	{
		throw Exception(); // TODO
	}
}
inline void GreatVulkan::DestroyBuffer(const VkDevice& vk_device_, const VkBuffer& vk_buffer_)
{
	vkDestroyBuffer(vk_device_, vk_buffer_, nullptr);
}

// Device Memory
inline VkDeviceMemory GreatVulkan::AllocateMemory(const VkDevice& vk_device_, const VkMemoryAllocateInfo& vk_memoryAllocateInfo_)
{
	VkDeviceMemory vk_deviceMemory;

	if (auto result = Result(vkAllocateMemory(vk_device_, &vk_memoryAllocateInfo_, nullptr, &vk_deviceMemory)))
	{
		return vk_deviceMemory;
	}
	else
	{
		throw Exception(); // TODO
	}
}
inline void* GreatVulkan::MapMemory(const VkDevice& vk_device_, const VkDeviceMemory& vk_deviceMemory_, const VkDeviceSize& offset_, const VkDeviceSize& size_, const VkMemoryMapFlags& flags_)
{
	void* data;
	if (auto result = Result(vkMapMemory(vk_device_, vk_deviceMemory_, offset_, size_, flags_, &data)))
	{
		return data;
	}
	else
	{
		throw Exception(); // TODO
	}
}
inline void GreatVulkan::UnmapMemory(const VkDevice& vk_device_, const VkDeviceMemory& vk_deviceMemory_)
{
	vkUnmapMemory(vk_device_, vk_deviceMemory_);
}
inline void GreatVulkan::FreeMemory(const VkDevice& vk_device_, const VkDeviceMemory& vk_deviceMemory_)
{
	vkFreeMemory(vk_device_, vk_deviceMemory_, nullptr);
}

// Shader Module
inline VkShaderModule GreatVulkan::CreateShaderModule(const VkDevice& vk_device_, const VkShaderModuleCreateInfo& vk_shaderModuleCreateInfo_)
{
	VkShaderModule vk_shaderModule;

	if (auto result = Result(vkCreateShaderModule(vk_device_, &vk_shaderModuleCreateInfo_, nullptr, &vk_shaderModule)))
	{
		return vk_shaderModule;
	}
	else
	{
		throw Exception();
	}
}
inline void GreatVulkan::DestroyShaderModule(const VkDevice& vk_device_, const VkShaderModule& vk_shaderModule_)
{
	vkDestroyShaderModule(vk_device_, vk_shaderModule_, nullptr);
}

// Pipeline Layout
inline VkPipelineLayout GreatVulkan::CreatePipelineLayout(const VkDevice& vk_device_, const VkPipelineLayoutCreateInfo& vk_pipelineLayoutCreateInfo_)
{
	VkPipelineLayout vk_pipelineLayout;

	if (auto result = Result(vkCreatePipelineLayout(vk_device_, &vk_pipelineLayoutCreateInfo_, nullptr, &vk_pipelineLayout)))
	{
		return vk_pipelineLayout;
	}
	else
	{
		throw Exception(); // TODO
	}
}
inline void GreatVulkan::DestroyPipelineLayout(const VkDevice& vk_device_, const VkPipelineLayout& vk_pipelineLayout_)
{
	vkDestroyPipelineLayout(vk_device_, vk_pipelineLayout_, nullptr);
}

// Pipeline
inline GreatVulkan::Vector<VkPipeline> GreatVulkan::CreateGraphicsPipelines(const VkDevice& vk_device_, const VkPipelineCache& vk_pipelineCache_, const Vector<VkGraphicsPipelineCreateInfo>& vk_createInfos_)
{
	Vector<VkPipeline> vk_pipelines(vk_createInfos_.size());

	if (auto result = Result(vkCreateGraphicsPipelines(vk_device_, vk_pipelineCache_, vk_createInfos_.size(), vk_createInfos_.data(), nullptr, vk_pipelines.data())))
	{
		return Move(vk_pipelines);
	}
	else
	{
		throw Exception(); // TODO
	}
}
inline void GreatVulkan::DestroyPipeline(const VkDevice& vk_device_, const VkPipeline& vk_pipeline_)
{
	vkDestroyPipeline(vk_device_, vk_pipeline_, nullptr);
}

#pragma endregion


#pragma region
#pragma endregion


