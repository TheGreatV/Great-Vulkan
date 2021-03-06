#include <iostream>
#include <fstream>

#define VK_USE_PLATFORM_WIN32_KHR 1

#include <Common/Common.hpp>
#include <Vulkan/Vulkan.hpp>

#include <Windows.h>
using namespace GreatVulkan;


void func()
{
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
	DestroyRenderPass(vk_device, vk_renderPass);
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