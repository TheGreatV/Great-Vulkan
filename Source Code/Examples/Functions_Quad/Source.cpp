#include <iostream>
#include <fstream>

#define VK_USE_PLATFORM_WIN32_KHR 1

#include <Common/Common.hpp>
#include <Vulkan/Vulkan.hpp>

#include <Windows.h>
using namespace GreatVulkan;


void func()
{
	// shaders loader
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

	// common stuff
	VkExtent2D windowSize;
	{
		windowSize.width = 512;
		windowSize.height = 512;
	}

	// WinAPI Instance
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

	// Hack for VS shit
	Vector<const char*> empty;

	// Instance
	auto vk_instanceLayersProperties = EnumerateInstanceLayerProperties();
	auto vk_instanceLayersNames = ExtractInstanceLayersNames(vk_instanceLayersProperties);
	{
		while (true)
		{
			auto i = std::find_if(vk_instanceLayersNames.begin(), vk_instanceLayersNames.end(), [](const String& a)
			{
				auto r =
					// a == "VK_LAYER_LUNARG_api_dump" || // TODO: bug on DestroyCommandPool

					a == "VK_LAYER_LUNARG_device_simulation" ||
					a == "VK_LAYER_LUNARG_standard_validation" ||
					// a == "VK_LAYER_LUNARG_api_dump" ||
					a == "VK_LAYER_LUNARG_vktrace";

				return r;
			});

			if (i == vk_instanceLayersNames.end())
			{
				break;
			}

			vk_instanceLayersNames.erase(i);
		}
	}
	auto vk_instanceLayersExtensionsProperties = EnumerateInstanceExtensionsProperties(vk_instanceLayersNames + static_cast<const char*>(""));
	auto vk_instanceLayersExtensionsNames = ExtractInstanceExtensionsNames(vk_instanceLayersExtensionsProperties);
	auto vk_instance = CreateInstance(InstanceCreateInfo(
		ApplicationInfo("Application Name", 0, "Engine Name", 0, VK_MAKE_VERSION(1, 0, 61)),
#if _DEBUG
		vk_instanceLayersNames,
#else
		empty,
#endif
		vk_instanceLayersExtensionsNames
	));
	
	// Physical Device
	auto vk_physicalDevices = Move(EnumeratePhysicalDevices(vk_instance));
	auto vk_physicalDevice = vk_physicalDevices[0];
	auto vk_physicalDeviceProperties = GetPhysicalDeviceProperties(vk_physicalDevice);
	auto vk_physicalDeviceQueueFamilyProperties = GetPhysicalDeviceQueueFamilyProperties(vk_physicalDevice);
	auto vk_physicalDeviceMemoryProperties = GetPhysicalDeviceMemoryProperties(vk_physicalDevice);

	// Device
	auto vk_deviceLayersProperties = EnumerateDeviceLayerProperties(vk_physicalDevice);
	auto vk_deviceLayersNames = ExtractDeviceLayersNames(vk_deviceLayersProperties);
	auto vk_deviceLayersExtensionsProperties = EnumerateDeviceExtensionsProperties(vk_physicalDevice, vk_deviceLayersNames + static_cast<const char*>(""));
	auto vk_deviceLayersExtensionsNames = ExtractDeviceExtensionsNames(vk_deviceLayersExtensionsProperties);
	auto vk_device = CreateDevice(vk_physicalDevice, DeviceCreateInfo(
		{
			DeviceQueueCreateInfo(0, {1.0f}),
		},
#if _DEBUG
		vk_deviceLayersNames,
#else
		empty,
#endif
		vk_deviceLayersExtensionsNames,
		PhysicalDeviceFeatures(Initializer<PhysicalDeviceFeatures::Feature>({
		}))
	));

	auto getDeviceMemoryIndex = [&](const VkMemoryPropertyFlags& properties) -> uint32_t
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
	};

	// Surface
	if (auto isPresentationSupported = Boolean(GetPhysicalDeviceWin32PresentationSupportKHR(vk_physicalDevice, 0))); else
	{
		throw Exception(); // TODO
	}
	auto vk_surface = CreateWin32SurfaceKHR(vk_instance, Win32SurfaceCreateInfoKHR(handleInstance, handleWindow));
	{
		if (auto isSufraceSupported = Boolean(GetPhysicalDeviceSurfaceSupportKHR(vk_physicalDevice, 0, vk_surface))); else
		{
			throw Exception(); // TODO
		}
	}
	auto vk_surfaceCapabilities = GetPhysicalDeviceSurfaceCapabilitiesKHR(vk_physicalDevice, vk_surface);
	auto vk_surfaceFormats = Move(GetPhysicalDeviceSurfaceFormatsKHR(vk_physicalDevice, vk_surface));
	auto vk_surfaceFormat = vk_surfaceFormats[0];

	// Swapchain
	auto vk_swapchain = CreateSwapchainKHR(vk_device, SwapchainCreateInfoKHR(
		0,
		vk_surface,
		std::max<uint32_t>(vk_surfaceCapabilities.minImageCount, 2),
		vk_surfaceFormat.format,
		vk_surfaceFormat.colorSpace,
		windowSize,
		1,
		VkImageUsageFlagBits::VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT,
		VkSharingMode::VK_SHARING_MODE_EXCLUSIVE,
		{},
		VkSurfaceTransformFlagBitsKHR::VK_SURFACE_TRANSFORM_IDENTITY_BIT_KHR,
		VkCompositeAlphaFlagBitsKHR::VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR,
		VkPresentModeKHR::VK_PRESENT_MODE_FIFO_KHR,
		VK_FALSE,
		VK_NULL_HANDLE
	));
	auto vk_swapchainImages = Move(GetSwapchainImagesKHR(vk_device, vk_swapchain));
	auto vk_swapchainImageViews = Vector<VkImageView>(vk_swapchainImages.size());
	{
		for (Size i = 0; i < vk_swapchainImages.size(); ++i)
		{
			auto &vk_swapchainImage = vk_swapchainImages[i];
			auto &vk_swapchainImageView = vk_swapchainImageViews[i];

			// vk_swapchainImageView = CreateImageView(vk_device, )
			vk_swapchainImageView = CreateImageView(vk_device, ImageViewCreateInfo(
				0, vk_swapchainImage, VkImageViewType::VK_IMAGE_VIEW_TYPE_2D, vk_surfaceFormat.format,
				ComponentMapping(
					VkComponentSwizzle::VK_COMPONENT_SWIZZLE_R,
					VkComponentSwizzle::VK_COMPONENT_SWIZZLE_G,
					VkComponentSwizzle::VK_COMPONENT_SWIZZLE_B,
					VkComponentSwizzle::VK_COMPONENT_SWIZZLE_A
				),
				ImageSubresourceRange(VkImageAspectFlagBits::VK_IMAGE_ASPECT_COLOR_BIT, 0, 1, 0, 1)
			));
		}
	}

	// Data
	struct Vertex
	{
		float x;
		float y;
		uint8_t r;
	};
	struct VertexColor
	{
		uint8_t g;
		uint8_t b;
	};
	Vector<Vertex> vertices(4);
	{
		vertices[0] = Vertex{-0.5f, +0.5f, 0xFF};
		vertices[1] = Vertex{+0.5f, +0.5f, 0x00};
		vertices[2] = Vertex{-0.5f, -0.5f, 0x00};
		vertices[3] = Vertex{+0.5f, -0.5f, 0xFF};
	}
	Size vertexStride = sizeof(Vertex);
	Size verticesTotalSize = sizeof(Vertex) * vertices.size();
	Vector<VertexColor> verticesColor(4);
	{
		verticesColor[0] = VertexColor{0x00, 0x00};
		verticesColor[1] = VertexColor{0xFF, 0x00};
		verticesColor[2] = VertexColor{0x00, 0xFF};
		verticesColor[3] = VertexColor{0xFF, 0xFF};
	}
	Size vertexColorStride = sizeof(VertexColor);
	Size verticesColorTotalSize = sizeof(VertexColor)* verticesColor.size();
	Vector<uint32_t> indices(6);
	{
		indices[0] = 0;
		indices[1] = 1;
		indices[2] = 2;
		indices[3] = 1;
		indices[4] = 3;
		indices[5] = 2;
	}
	Size indicesTotalSize = sizeof(uint32_t) * indices.size();
	Vector<float> uniforms(1);
	{
		uniforms[0] = 0.5f;
	}
	Size uniformsTotalSize = sizeof(float) * uniforms.size();

	// Buffers
	auto vk_vertexBuffer = CreateBuffer(vk_device, BufferCreateInfo(0, verticesTotalSize, VkBufferUsageFlagBits::VK_BUFFER_USAGE_VERTEX_BUFFER_BIT, VkSharingMode::VK_SHARING_MODE_EXCLUSIVE));
	auto vk_vertexBufferDeviceMemory = [&]()
	{
		auto vk_memoryRequirements = GetBufferMemoryRequirements(vk_device, vk_vertexBuffer);
		auto vk_deviceMemory = AllocateMemory(vk_device, MemoryAllocateInfo(
			vk_memoryRequirements.size,
			getDeviceMemoryIndex(VkMemoryPropertyFlagBits::VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT)
		));

		auto data = MapMemory(vk_device, vk_deviceMemory, 0, VK_WHOLE_SIZE, 0);

		std::memcpy(data, vertices.data(), verticesTotalSize);

		UnmapMemory(vk_device, vk_deviceMemory);

		BindBufferMemory(vk_device, vk_vertexBuffer, vk_deviceMemory);

		return vk_deviceMemory;
	}();
	auto vk_vertexColorBuffer = CreateBuffer(vk_device, BufferCreateInfo(0, verticesTotalSize, VkBufferUsageFlagBits::VK_BUFFER_USAGE_VERTEX_BUFFER_BIT, VkSharingMode::VK_SHARING_MODE_EXCLUSIVE));
	auto vk_vertexColorBufferDeviceMemory = [&]()
	{
		auto vk_memoryRequirements = GetBufferMemoryRequirements(vk_device, vk_vertexColorBuffer);
		auto vk_deviceMemory = AllocateMemory(vk_device, MemoryAllocateInfo(
			vk_memoryRequirements.size,
			getDeviceMemoryIndex(VkMemoryPropertyFlagBits::VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT)
		));

		auto data = MapMemory(vk_device, vk_deviceMemory, 0, VK_WHOLE_SIZE, 0);

		std::memcpy(data, verticesColor.data(), verticesColorTotalSize);

		UnmapMemory(vk_device, vk_deviceMemory);

		BindBufferMemory(vk_device, vk_vertexColorBuffer, vk_deviceMemory);

		return vk_deviceMemory;
	}();
	auto vk_indexBuffer = CreateBuffer(vk_device, BufferCreateInfo(0, verticesTotalSize, VkBufferUsageFlagBits::VK_BUFFER_USAGE_INDEX_BUFFER_BIT, VkSharingMode::VK_SHARING_MODE_EXCLUSIVE));
	auto vk_indexBufferDeviceMemory = [&]()
	{
		auto vk_memoryRequirements = GetBufferMemoryRequirements(vk_device, vk_indexBuffer);
		auto vk_deviceMemory = AllocateMemory(vk_device, MemoryAllocateInfo(
			vk_memoryRequirements.size,
			getDeviceMemoryIndex(VkMemoryPropertyFlagBits::VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT)
		));

		auto data = MapMemory(vk_device, vk_deviceMemory, 0, VK_WHOLE_SIZE, 0);

		std::memcpy(data, indices.data(), indicesTotalSize);

		UnmapMemory(vk_device, vk_deviceMemory);

		BindBufferMemory(vk_device, vk_indexBuffer, vk_deviceMemory);

		return vk_deviceMemory;
	}();
	auto vk_uniformBuffer = CreateBuffer(vk_device, BufferCreateInfo(0, uniformsTotalSize, VkBufferUsageFlagBits::VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT, VkSharingMode::VK_SHARING_MODE_EXCLUSIVE));
	auto vk_uniformBufferDeviceMemory = [&]()
	{
		auto vk_memoryRequirements = GetBufferMemoryRequirements(vk_device, vk_uniformBuffer);
		auto vk_deviceMemory = AllocateMemory(vk_device, MemoryAllocateInfo(
			vk_memoryRequirements.size,
			getDeviceMemoryIndex(VkMemoryPropertyFlagBits::VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT)
		));

		auto data = MapMemory(vk_device, vk_deviceMemory, 0, VK_WHOLE_SIZE, 0);

		std::memcpy(data, uniforms.data(), uniformsTotalSize);

		UnmapMemory(vk_device, vk_deviceMemory);

		BindBufferMemory(vk_device, vk_uniformBuffer, vk_deviceMemory);

		return vk_deviceMemory;
	}();

	// Shaders
	auto vk_vertexShaderModule = CreateShaderModule(vk_device, ShaderModuleCreateInfo(Move(loadShader("../../../../../Media/Shaders/Functions_Quad/1.spir-v.vs"))));
	auto vk_fragmentShaderModule = CreateShaderModule(vk_device, ShaderModuleCreateInfo(Move(loadShader("../../../../../Media/Shaders/Functions_Quad/1.spir-v.fs"))));

	// RenderPass
	auto vk_renderPass = CreateRenderPass(vk_device, RenderPassCreateInfo(
		{
			AttachmentDescription(
				vk_surfaceFormat.format,
				VkSampleCountFlagBits::VK_SAMPLE_COUNT_1_BIT,
				VkAttachmentLoadOp::VK_ATTACHMENT_LOAD_OP_CLEAR,
				VkAttachmentStoreOp::VK_ATTACHMENT_STORE_OP_STORE,
				VkAttachmentLoadOp::VK_ATTACHMENT_LOAD_OP_DONT_CARE,
				VkAttachmentStoreOp::VK_ATTACHMENT_STORE_OP_DONT_CARE,
				VkImageLayout::VK_IMAGE_LAYOUT_UNDEFINED,
				VkImageLayout::VK_IMAGE_LAYOUT_PRESENT_SRC_KHR
			),
		},
		{
			SubpassDescription(VkPipelineBindPoint::VK_PIPELINE_BIND_POINT_GRAPHICS, {
				AttachmentReference(0, VkImageLayout::VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL),
			})
		}
	));

	// Descriptor Pool
	auto vk_descriptorPool = CreateDescriptorPool(vk_device, DescriptorPoolCreateInfo(VkDescriptorPoolCreateFlagBits::VK_DESCRIPTOR_POOL_CREATE_FREE_DESCRIPTOR_SET_BIT, 1, {
		DescriptorPoolSize(VkDescriptorType::VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, 1),
	}));

	// Descriptor Set Layout
	auto vk_descriptorSetLayout = CreateDescriptorSetLayout(vk_device, DescriptorSetLayoutCreateInfo(0, {
		DescriptorSetLayoutBinding(0, VkDescriptorType::VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, 1, VkShaderStageFlagBits::VK_SHADER_STAGE_VERTEX_BIT),
	}));

	// Descriptor Set
	auto vk_descriptorSets = Move(AllocateDescriptorSets(vk_device, DescriptorSetAllocateInfo(vk_descriptorPool, {vk_descriptorSetLayout})));
	auto &vk_descriptorSet = vk_descriptorSets[0];
	{
		UpdateDescriptorSets(vk_device, {
			WriteDescriptorSet(vk_descriptorSet, 0, 0, VkDescriptorType::VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, {DescriptorBufferInfo(vk_uniformBuffer)}),
		}, {});
	}

	// Pipeline Layout
	auto vk_pipelineLayout = CreatePipelineLayout(vk_device, PipelineLayoutCreateInfo(
		{vk_descriptorSetLayout},
		{PushConstantRange(VkShaderStageFlagBits::VK_SHADER_STAGE_VERTEX_BIT, 0, 4)}
	));

	// Pipeline
	auto vk_pipelines = Move(CreateGraphicsPipelines(vk_device, VK_NULL_HANDLE, {
		GraphicsPipelineCreateInfo(
			0,
			{
				PipelineShaderStageCreateInfo(VkShaderStageFlagBits::VK_SHADER_STAGE_VERTEX_BIT, vk_vertexShaderModule, "main"),
				PipelineShaderStageCreateInfo(VkShaderStageFlagBits::VK_SHADER_STAGE_FRAGMENT_BIT, vk_fragmentShaderModule, "main"),
			},
			PipelineVertexInputStateCreateInfo(
				{
					VertexInputBindingDescription(0, vertexStride, VkVertexInputRate::VK_VERTEX_INPUT_RATE_VERTEX),
					VertexInputBindingDescription(1, vertexColorStride, VkVertexInputRate::VK_VERTEX_INPUT_RATE_VERTEX),
				},
				{
					VertexInputAttributeDescription(0, 0, VkFormat::VK_FORMAT_R32G32_SFLOAT, 0),
					VertexInputAttributeDescription(1, 0, VkFormat::VK_FORMAT_R8_UNORM, sizeof(float) * 2),
					VertexInputAttributeDescription(2, 1, VkFormat::VK_FORMAT_R8G8_UNORM, 0),
				}
			),
			PipelineInputAssemblyStateCreateInfo(VkPrimitiveTopology::VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST, VK_FALSE),
			PipelineViewportStateCreateInfo(
				{
					Viewport(0, 0, static_cast<float>(vk_surfaceCapabilities.currentExtent.width), static_cast<float>(vk_surfaceCapabilities.currentExtent.height), 0.0f, 1.0f),
				},
				{
					Rect2D(Offset2D(0, 0), Extent2D(vk_surfaceCapabilities.currentExtent.width, vk_surfaceCapabilities.currentExtent.height)),
				}
			),
			PipelineRasterizationStateCreateInfo(
				VK_FALSE, VK_FALSE,
				VkPolygonMode::VK_POLYGON_MODE_FILL, VkCullModeFlagBits::VK_CULL_MODE_NONE, VkFrontFace::VK_FRONT_FACE_COUNTER_CLOCKWISE,
				VK_FALSE, 0.0f, 0.0f, 0.0f, 1.0f
			),
			PipelineMultisampleStateCreateInfo(
				VkSampleCountFlagBits::VK_SAMPLE_COUNT_1_BIT,
				VK_FALSE, 0.0f, nullptr, VK_FALSE, VK_FALSE
			),
			PipelineDepthStencilStateCreateInfo(
				VK_FALSE, VK_FALSE, VkCompareOp::VK_COMPARE_OP_ALWAYS, VK_FALSE,
				VK_FALSE,
				StencilOpState(VkStencilOp::VK_STENCIL_OP_KEEP, VkStencilOp::VK_STENCIL_OP_KEEP, VkStencilOp::VK_STENCIL_OP_KEEP, VkCompareOp::VK_COMPARE_OP_ALWAYS, 0, 0, 0),
				StencilOpState(VkStencilOp::VK_STENCIL_OP_KEEP, VkStencilOp::VK_STENCIL_OP_KEEP, VkStencilOp::VK_STENCIL_OP_KEEP, VkCompareOp::VK_COMPARE_OP_ALWAYS, 0, 0, 0),
				0.0f,
				0.0f
			),
			PipelineColorBlendStateCreateInfo(
				VK_FALSE, VkLogicOp::VK_LOGIC_OP_CLEAR,
				{
					PipelineColorBlendAttachmentState(
						VK_FALSE,
						VkBlendFactor::VK_BLEND_FACTOR_ONE,
						VkBlendFactor::VK_BLEND_FACTOR_ONE,
						VkBlendOp::VK_BLEND_OP_ADD,
						VkBlendFactor::VK_BLEND_FACTOR_ONE,
						VkBlendFactor::VK_BLEND_FACTOR_ONE,
						VkBlendOp::VK_BLEND_OP_ADD,
						VkColorComponentFlagBits::VK_COLOR_COMPONENT_R_BIT | VkColorComponentFlagBits::VK_COLOR_COMPONENT_G_BIT | VkColorComponentFlagBits::VK_COLOR_COMPONENT_B_BIT
					),
				},
				{0.0f, 0.0f, 0.0f, 0.0f}
			),
			vk_pipelineLayout,
			vk_renderPass,
			0
		),
	}));
	auto &vk_pipeline = vk_pipelines[0];

	DestroyShaderModule(vk_device, vk_vertexShaderModule);
	DestroyShaderModule(vk_device, vk_fragmentShaderModule);

	// Framebuffers
	Vector<VkFramebuffer> vk_framebuffers(vk_swapchainImageViews.size());
	{
		for (Size i = 0; i < vk_framebuffers.size(); ++i)
		{
			auto &vk_framebuffer = vk_framebuffers[i];
			auto &vk_swapchainImageView = vk_swapchainImageViews[i];

			vk_framebuffer = CreateFramebuffer(vk_device, FramebufferCreateInfo(vk_renderPass, { vk_swapchainImageView }, windowSize.width, windowSize.height, 1));
		}
	}

	// Queue
	auto vk_queue = GetDeviceQueue(vk_device, 0, 0);

	// Command Pool
	auto vk_commandPool = CreateCommandPool(vk_device, CommandPoolCreateInfo(VkCommandPoolCreateFlagBits::VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT, 0));
	{
		ResetCommandPool(vk_device, vk_commandPool, VkCommandPoolResetFlagBits::VK_COMMAND_POOL_RESET_RELEASE_RESOURCES_BIT);
	}

	// Command Buffers
	auto vk_commandBuffers = Move(AllocateCommandBuffers(vk_device, CommandBufferAllocateInfo(vk_commandPool, VkCommandBufferLevel::VK_COMMAND_BUFFER_LEVEL_PRIMARY, vk_framebuffers.size())));
	{
		for (Size i = 0; i < vk_commandBuffers.size(); ++i)
		{
			auto &vk_commandBuffer = vk_commandBuffers[i];
			auto &vk_framebuffer = vk_framebuffers[i];

			ResetCommandBuffer(vk_commandBuffer, VkCommandBufferResetFlagBits::VK_COMMAND_BUFFER_RESET_RELEASE_RESOURCES_BIT);

			BeginCommandBuffer(vk_commandBuffer, CommandBufferBeginInfo(0));
			
			CmdBeginRenderPass(vk_commandBuffer, RenderPassBeginInfo(
				vk_renderPass,
				vk_framebuffer,
				Rect2D(Offset2D(0, 0), vk_surfaceCapabilities.currentExtent), {
					ClearValue::Color(1.0f, 0.5f, 0.0f, 1.0f),
				}),
				VkSubpassContents::VK_SUBPASS_CONTENTS_INLINE
			);

			CmdBindPipeline(vk_commandBuffer, VkPipelineBindPoint::VK_PIPELINE_BIND_POINT_GRAPHICS, vk_pipeline);
			CmdPushConstants(vk_commandBuffer, vk_pipelineLayout, VkShaderStageFlagBits::VK_SHADER_STAGE_VERTEX_BIT, 0, Vector<float>({2.0f}));
			CmdBindVertexBuffers(vk_commandBuffer, 0, 1, {vk_vertexBuffer}, {0});
			CmdBindVertexBuffers(vk_commandBuffer, 1, 1, {vk_vertexColorBuffer}, {0});
			CmdBindIndexBuffer(vk_commandBuffer, vk_indexBuffer, 0, VkIndexType::VK_INDEX_TYPE_UINT32);
			CmdBindDescriptorSets(vk_commandBuffer, VkPipelineBindPoint::VK_PIPELINE_BIND_POINT_GRAPHICS, vk_pipelineLayout, 0, {vk_descriptorSet}, {});
			CmdDrawIndexed(vk_commandBuffer, indices.size(), 1, 0, 0, 0);

			CmdEndRenderPass(vk_commandBuffer);

			EndCommandBuffer(vk_commandBuffer);
		}
	}

	// Fence
	auto vk_fence = CreateFence(vk_device, FenceCreateInfo(0));

	// Main loop
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

		//
		auto vk_nextImageIndex = AcquireNextImageKHR(vk_device, vk_swapchain, UINT64_MAX, VK_NULL_HANDLE, vk_fence);
		
		WaitForFences(vk_device, {vk_fence}, VK_FALSE, UINT64_MAX);
		ResetFences(vk_device, {vk_fence});

		auto &vk_commandBuffer = vk_commandBuffers[vk_nextImageIndex];

		QueueSubmit(vk_queue, {SubmitInfo({vk_commandBuffer})});
		QueueWaitIdle(vk_queue);

		QueuePresentKHR(vk_queue, PresentInfoKHR({}, {vk_swapchain}, {vk_nextImageIndex}));
	}

	DestroyFence(vk_device, vk_fence);
	FreeCommandBuffers(vk_device, vk_commandPool, vk_commandBuffers);
	DestroyCommandPool(vk_device, vk_commandPool); // TODO: crash when attempting to destroy pool without any buffers allocated
	DestroyPipeline(vk_device, vk_pipeline);
	DestroyPipelineLayout(vk_device, vk_pipelineLayout);
	FreeDescriptorSets(vk_device, vk_descriptorPool, vk_descriptorSets);
	DestroyDescriptorSetLayout(vk_device, vk_descriptorSetLayout);
	DestroyDescriptorPool(vk_device, vk_descriptorPool);
	DestroyRenderPass(vk_device, vk_renderPass);
	FreeMemory(vk_device, vk_uniformBufferDeviceMemory);
	DestroyBuffer(vk_device, vk_uniformBuffer);
	FreeMemory(vk_device, vk_indexBufferDeviceMemory);
	DestroyBuffer(vk_device, vk_indexBuffer);
	FreeMemory(vk_device, vk_vertexColorBufferDeviceMemory);
	DestroyBuffer(vk_device, vk_vertexColorBuffer);
	FreeMemory(vk_device, vk_vertexBufferDeviceMemory);
	DestroyBuffer(vk_device, vk_vertexBuffer);
	for (auto &vk_framebuffer : vk_framebuffers)
	{
		DestroyFramebuffer(vk_device, vk_framebuffer);
	}
	for (auto &vk_swapchainImageView : vk_swapchainImageViews)
	{
		DestroyImageView(vk_device, vk_swapchainImageView);
	}
	DestroySwapchainKHR(vk_device, vk_swapchain);
	DestroySurfaceKHR(vk_instance, vk_surface);
	DestroyDevice(vk_device);
	DestroyInstance(vk_instance);

	DestroyWindow(handleWindow);
}

void main()
{
	func();

	std::system("pause");
}