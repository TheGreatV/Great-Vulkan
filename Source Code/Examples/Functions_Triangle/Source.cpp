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

	// Instance
	auto vk_instanceLayersProperties = EnumerateInstanceLayerProperties();
	auto vk_instanceLayersNames = ExtractInstanceLayersNames(vk_instanceLayersProperties);
	{
		while (true)
		{
			auto i = std::find_if(vk_instanceLayersNames.begin(), vk_instanceLayersNames.end(), [](const String& a)
			{
				auto r =
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
		vk_instanceLayersNames,
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
			DeviceQueueCreateInfo(0, {0.0f}),
		},
		vk_deviceLayersNames,
		vk_deviceLayersExtensionsNames,
		PhysicalDeviceFeatures(Initializer<PhysicalDeviceFeatures::Feature>({
		}))
	));
	
	// Win32SurfaceKHR
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

	// TODO

	DestroyDevice(vk_device);
	DestroyInstance(vk_instance);

	DestroyWindow(handleWindow);
}

void main()
{
	func();

	std::system("pause");
}