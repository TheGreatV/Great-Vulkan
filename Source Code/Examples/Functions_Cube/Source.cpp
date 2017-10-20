#include <iostream>
#include <fstream>

#define VK_USE_PLATFORM_WIN32_KHR 1

#include <Common/Common.hpp>
#include <Vulkan/Vulkan.hpp>

#include <Windows.h>
#include <glm/glm.hpp>

#include <il/il.h>
#include <il/ilu.h>
#include <il/ilut.h>
using namespace GreatVulkan;


inline glm::mat4 rotateX(const glm::float32& angle_)
{
	auto a = glm::radians(angle_);
	auto s = glm::sin(a);
	auto c = glm::cos(a);

	return glm::transpose(glm::mat4(
		1.0f,	0.0f,	0.0f,	0.0f,
		0.0f,	c,		-s,		0.0f,
		0.0f,	s,		c,		0.0f,
		0.0f,	0.0f,	0.0f,	1.0f));
}
inline glm::mat4 rotateY(const glm::float32& angle_)
{
	auto a = glm::radians(angle_);
	auto s = glm::sin(a);
	auto c = glm::cos(a);

	return glm::transpose(glm::mat4(
		c,		0.0f,	s,		0.0f,
		0.0f,	1.0f,	0.0f,	0.0f,
		-s,		0.0f,	c,		0.0f,
		0.0f,	0.0f,	0.0f,	1.0f));
}
inline glm::mat4 rotateZ(const glm::float32& angle_)
{
	auto a = glm::radians(angle_);
	auto s = glm::sin(a);
	auto c = glm::cos(a);

	return glm::transpose(glm::mat4(
		c,		s,		0.0f,	0.0f,
		-s,		c,		0.0f,	0.0f,
		0.0f,	0.0f,	1.0f,	0.0f,
		0.0f,	0.0f,	0.0f,	1.0f));
}
inline glm::mat4 rotateZXY(const glm::vec3& angle_)
{
	return rotateY(angle_.y) * rotateX(angle_.x) * rotateZ(angle_.z);
}
inline glm::mat4 rotateYXZ(const glm::vec3& angle_)
{
	return rotateZ(angle_.z) * rotateX(angle_.x) * rotateY(angle_.y);
}
inline glm::mat4 move(const glm::vec3& position_)
{
	return glm::transpose(glm::mat4(
		1.0f, 0.0f, 0.0f, position_.x,
		0.0f, 1.0f, 0.0f, position_.y,
		0.0f, 0.0f, 1.0f, position_.z,
		0.0f, 0.0f, 0.0f, 1.0f));
}
inline glm::mat4 scale(const glm::vec3& scale_)
{
	return glm::transpose(glm::mat4(
		scale_.x,	0.0f,		0.0f,		0.0f,
		0.0f,		scale_.y,	0.0f,		0.0f,
		0.0f,		0.0f,		scale_.z,	0.0f,
		0.0f,		0.0f,		0.0f,		1.0f));
}
inline glm::mat4 perspective(const glm::float32& fov_, const glm::float32& aspect_, const glm::float32& near_, const glm::float32& far_)
{
	auto g = glm::radians(fov_);
	glm::float32 f = 1.0f / tanf(g / 2.0f);
	glm::float32 a = (far_ + near_) / (near_ - far_);
	glm::float32 b = (2.0f * far_ * near_) / (near_ - far_);

	return glm::transpose(glm::mat4( // corrected by *(1,1,-1)
		f / aspect_, 0.0f, 0.0f, 0.0f,
		0.0f, f, 0.0f, 0.0f,
		0.0f, 0.0f, -a, b,
		0.0f, 0.0f, 1.0f, 0.0f));
}


void func()
{
	ilInit();
	iluInit();
	ilutInit();

	// texture loader
	auto loadImage = [](const String filename_)
	{
		auto handle = ilGenImage();
		ilBindImage(handle);

		if (!ilLoadImage(filename_.c_str()))
		{
			throw Exception();
		}

		auto ilWidth		= ilGetInteger(IL_IMAGE_WIDTH); 
		auto ilHeight		= ilGetInteger(IL_IMAGE_HEIGHT); 
		auto ilDepth		= ilGetInteger(IL_IMAGE_DEPTH); 
		auto ilFormat		= ilGetInteger(IL_IMAGE_FORMAT);  // GL_RGBA
		auto ilType			= ilGetInteger(IL_IMAGE_TYPE); 
		auto ilBytes		= ilGetInteger(IL_IMAGE_BYTES_PER_PIXEL); 
		auto ilBits			= ilGetInteger(IL_IMAGE_BITS_PER_PIXEL); 
		auto ilMipmap		= ilGetInteger(IL_NUM_MIPMAPS); 
		auto ilOriginMode	= ilGetInteger(IL_IMAGE_ORIGIN); 	// IL_ORIGIN_LOWER_LEFT, IL_ORIGIN_UPPER_LEFT
		auto ilDataSize		= ilGetInteger(IL_IMAGE_SIZE_OF_DATA); 

		if (ilOriginMode != IL_ORIGIN_LOWER_LEFT)
		{
			iluFlipImage();
		}

		auto ilData			= ilGetData();

		Vector<uint8_t> data(ilDataSize);

		std::memcpy(data.data(), ilData, data.size());

		ilDeleteImage(handle);

		return Move(data);
	};
	auto generateMipmaps = [](const Vector<uint8_t>& data_, const Size& size_, const Size& components_)
	{
		auto powOf = glm::log2(static_cast<glm::float32_t>(size_));
		auto levels = (powOf - glm::floor(powOf)) == 0
			? (static_cast<Size>(powOf) + 1)
			: throw Exception();

		auto mipmaps = Map<Size, Vector<uint8_t>>();

		mipmaps[0] = data_;

		for (Size level = 1; level < levels; ++level)
		{
			Size powOf = levels - 1 - level;
			Size size = static_cast<Size>(glm::pow(2, powOf));
			auto &mipmap = mipmaps[level];
			mipmap = Vector<uint8_t>(size * size * components_);

			Size sourceLevel = level - 1;
			Size sourcePowOf = levels - 1 - sourceLevel;
			Size sourceSize = static_cast<Size>(glm::pow(2, sourcePowOf));
			auto &sourceMipmap = mipmaps[sourceLevel];

			for (Size x = 0; x < size; ++x)
			{
				for (Size y = 0; y < size; ++y)
				{
					Size i = x + y * size; // index in destination mip-level
					Size si0 = (x + 0) + (y + 0) * sourceSize; // index in source mip-level
					Size si1 = (x + 1) + (y + 0) * sourceSize; // index in source mip-level
					Size si2 = (x + 0) + (y + 1) * sourceSize; // index in source mip-level
					Size si3 = (x + 1) + (y + 1) * sourceSize; // index in source mip-level

					for (Size c = 0; c < components_; ++c)
					{
						Size id = i*components_ + c;
						Size sid0 = si0*components_ + c;
						Size sid1 = si1*components_ + c;
						Size sid2 = si2*components_ + c;
						Size sid3 = si3*components_ + c;

						auto &v = mipmap[id];
						auto &sv0 = sourceMipmap[sid0];
						auto &sv1 = sourceMipmap[sid1];
						auto &sv2 = sourceMipmap[sid2];
						auto &sv3 = sourceMipmap[sid3];

						v = static_cast<uint8_t>((sv0 + sv1 + sv2 + sv3) / 4.0f);
					}
				}
			}
		}

		return Move(mipmaps);
	};

	// angle of cube
	glm::vec3 angle(0.0f);

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
	auto vk_instanceLayersNames = Vector<const char*>();
	{
		auto addLayer = [&](const String& layer_)
		{
			for (auto &i : vk_instanceLayersProperties)
			{
				if (i.layerName == layer_)
				{
					vk_instanceLayersNames.push_back(i.layerName);

					return;
				}
			}
		};
		
		addLayer("VK_LAYER_LUNARG_api_dump");
		addLayer("VK_LAYER_LUNARG_core_validation");
		addLayer("VK_LAYER_LUNARG_monitor");
		addLayer("VK_LAYER_LUNARG_object_tracker");
		addLayer("VK_LAYER_LUNARG_parameter_validation");
		addLayer("VK_LAYER_LUNARG_screenshot");
	}
	auto vk_instanceLayersExtensionsProperties = EnumerateInstanceExtensionsProperties(vk_instanceLayersNames + static_cast<const char*>(""));
	auto vk_instanceLayersExtensionsNames = Vector<const char*>();
	{
		auto addExtension = [&](const String& extension_)
		{
			for (auto &i : vk_instanceLayersExtensionsProperties)
			{
				for (auto &j : i.second)
				{
					if (j.extensionName == extension_)
					{
						vk_instanceLayersExtensionsNames.push_back(j.extensionName);

						return;
					}
				}
			}
		};

		addExtension(VK_KHR_WIN32_SURFACE_EXTENSION_NAME);
		addExtension(VK_KHR_SURFACE_EXTENSION_NAME);
		addExtension(VK_EXT_DEBUG_REPORT_EXTENSION_NAME);
	}
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
	auto getDeviceMemoryIndex2 = [&](const VkMemoryPropertyFlags& properties, const uint32_t& memoryTypeBits) -> uint32_t
	{
		for (uint32_t i = 0; i < vk_physicalDeviceMemoryProperties.memoryTypeCount; ++i)
		{
			if (((1 << i) & memoryTypeBits) != 0)
			{
				auto &memoryType = vk_physicalDeviceMemoryProperties.memoryTypes[i];

				if ((memoryType.propertyFlags & properties) == properties)
				{
					return i;
				}
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
		glm::vec3 pos;
		glm::vec2 tex;
		glm::vec3 tan;
		glm::vec3 bin;
		glm::vec3 nor;
	};
	Vector<Vertex> vertices(24);
	{
		vertices[0]		= Vertex{glm::vec3(-0.5f, -0.5f, -0.5f), glm::vec2(0.0f, 0.0f), glm::vec3(+1.0f, +0.0f, +0.0f), glm::vec3(+0.0f, +1.0f, +0.0f), glm::vec3(+0.0f, +0.0f, -1.0f)};
		vertices[1]		= Vertex{glm::vec3(+0.5f, -0.5f, -0.5f), glm::vec2(1.0f, 0.0f), glm::vec3(+1.0f, +0.0f, +0.0f), glm::vec3(+0.0f, +1.0f, +0.0f), glm::vec3(+0.0f, +0.0f, -1.0f)};
		vertices[2]		= Vertex{glm::vec3(-0.5f, +0.5f, -0.5f), glm::vec2(0.0f, 1.0f), glm::vec3(+1.0f, +0.0f, +0.0f), glm::vec3(+0.0f, +1.0f, +0.0f), glm::vec3(+0.0f, +0.0f, -1.0f)};
		vertices[3]		= Vertex{glm::vec3(+0.5f, +0.5f, -0.5f), glm::vec2(1.0f, 1.0f), glm::vec3(+1.0f, +0.0f, +0.0f), glm::vec3(+0.0f, +1.0f, +0.0f), glm::vec3(+0.0f, +0.0f, -1.0f)};

		vertices[4]		= Vertex{glm::vec3(+0.5f, -0.5f, -0.5f), glm::vec2(0.0f, 0.0f), glm::vec3(+0.0f, +0.0f, +1.0f), glm::vec3(+0.0f, +1.0f, +0.0f), glm::vec3(+1.0f, +0.0f, +0.0f)};
		vertices[5]		= Vertex{glm::vec3(+0.5f, -0.5f, +0.5f), glm::vec2(1.0f, 0.0f), glm::vec3(+0.0f, +0.0f, +1.0f), glm::vec3(+0.0f, +1.0f, +0.0f), glm::vec3(+1.0f, +0.0f, +0.0f)};
		vertices[6]		= Vertex{glm::vec3(+0.5f, +0.5f, -0.5f), glm::vec2(0.0f, 1.0f), glm::vec3(+0.0f, +0.0f, +1.0f), glm::vec3(+0.0f, +1.0f, +0.0f), glm::vec3(+1.0f, +0.0f, +0.0f)};
		vertices[7]		= Vertex{glm::vec3(+0.5f, +0.5f, +0.5f), glm::vec2(1.0f, 1.0f), glm::vec3(+0.0f, +0.0f, +1.0f), glm::vec3(+0.0f, +1.0f, +0.0f), glm::vec3(+1.0f, +0.0f, +0.0f)};
		
		vertices[8]		= Vertex{glm::vec3(+0.5f, -0.5f, +0.5f), glm::vec2(0.0f, 0.0f), glm::vec3(-1.0f, +0.0f, +0.0f), glm::vec3(+0.0f, +1.0f, +0.0f), glm::vec3(+0.0f, +0.0f, +1.0f)};
		vertices[9]		= Vertex{glm::vec3(-0.5f, -0.5f, +0.5f), glm::vec2(1.0f, 0.0f), glm::vec3(-1.0f, +0.0f, +0.0f), glm::vec3(+0.0f, +1.0f, +0.0f), glm::vec3(+0.0f, +0.0f, +1.0f)};
		vertices[10]	= Vertex{glm::vec3(+0.5f, +0.5f, +0.5f), glm::vec2(0.0f, 1.0f), glm::vec3(-1.0f, +0.0f, +0.0f), glm::vec3(+0.0f, +1.0f, +0.0f), glm::vec3(+0.0f, +0.0f, +1.0f)};
		vertices[11]	= Vertex{glm::vec3(-0.5f, +0.5f, +0.5f), glm::vec2(1.0f, 1.0f), glm::vec3(-1.0f, +0.0f, +0.0f), glm::vec3(+0.0f, +1.0f, +0.0f), glm::vec3(+0.0f, +0.0f, +1.0f)};

		vertices[12]	= Vertex{glm::vec3(-0.5f, -0.5f, +0.5f), glm::vec2(0.0f, 0.0f), glm::vec3(+0.0f, +0.0f, -1.0f), glm::vec3(+0.0f, +1.0f, +0.0f), glm::vec3(-1.0f, +0.0f, +0.0f)};
		vertices[13]	= Vertex{glm::vec3(-0.5f, -0.5f, -0.5f), glm::vec2(1.0f, 0.0f), glm::vec3(+0.0f, +0.0f, -1.0f), glm::vec3(+0.0f, +1.0f, +0.0f), glm::vec3(-1.0f, +0.0f, +0.0f)};
		vertices[14]	= Vertex{glm::vec3(-0.5f, +0.5f, +0.5f), glm::vec2(0.0f, 1.0f), glm::vec3(+0.0f, +0.0f, -1.0f), glm::vec3(+0.0f, +1.0f, +0.0f), glm::vec3(-1.0f, +0.0f, +0.0f)};
		vertices[15]	= Vertex{glm::vec3(-0.5f, +0.5f, -0.5f), glm::vec2(1.0f, 1.0f), glm::vec3(+0.0f, +0.0f, -1.0f), glm::vec3(+0.0f, +1.0f, +0.0f), glm::vec3(-1.0f, +0.0f, +0.0f)};

		vertices[16]	= Vertex{glm::vec3(+0.5f, -0.5f, -0.5f), glm::vec2(0.0f, 0.0f), glm::vec3(-1.0f, +0.0f, -1.0f), glm::vec3(+0.0f, +0.0f, +1.0f), glm::vec3(+0.0f, -1.0f, +0.0f)};
		vertices[17]	= Vertex{glm::vec3(-0.5f, -0.5f, -0.5f), glm::vec2(1.0f, 0.0f), glm::vec3(-1.0f, +0.0f, -1.0f), glm::vec3(+0.0f, +0.0f, +1.0f), glm::vec3(+0.0f, -1.0f, +0.0f)};
		vertices[18]	= Vertex{glm::vec3(+0.5f, -0.5f, +0.5f), glm::vec2(0.0f, 1.0f), glm::vec3(-1.0f, +0.0f, -1.0f), glm::vec3(+0.0f, +0.0f, +1.0f), glm::vec3(+0.0f, -1.0f, +0.0f)};
		vertices[19]	= Vertex{glm::vec3(-0.5f, -0.5f, +0.5f), glm::vec2(1.0f, 1.0f), glm::vec3(-1.0f, +0.0f, -1.0f), glm::vec3(+0.0f, +0.0f, +1.0f), glm::vec3(+0.0f, -1.0f, +0.0f)};

		vertices[20]	= Vertex{glm::vec3(-0.5f, +0.5f, -0.5f), glm::vec2(0.0f, 0.0f), glm::vec3(+1.0f, +0.0f, -1.0f), glm::vec3(+0.0f, +0.0f, +1.0f), glm::vec3(+0.0f, +1.0f, +0.0f)};
		vertices[21]	= Vertex{glm::vec3(+0.5f, +0.5f, -0.5f), glm::vec2(1.0f, 0.0f), glm::vec3(+1.0f, +0.0f, -1.0f), glm::vec3(+0.0f, +0.0f, +1.0f), glm::vec3(+0.0f, +1.0f, +0.0f)};
		vertices[22]	= Vertex{glm::vec3(-0.5f, +0.5f, +0.5f), glm::vec2(0.0f, 1.0f), glm::vec3(+1.0f, +0.0f, -1.0f), glm::vec3(+0.0f, +0.0f, +1.0f), glm::vec3(+0.0f, +1.0f, +0.0f)};
		vertices[23]	= Vertex{glm::vec3(+0.5f, +0.5f, +0.5f), glm::vec2(1.0f, 1.0f), glm::vec3(+1.0f, +0.0f, -1.0f), glm::vec3(+0.0f, +0.0f, +1.0f), glm::vec3(+0.0f, +1.0f, +0.0f)};
	}
	Size vertexStride = sizeof(Vertex);
	Size verticesTotalSize = sizeof(Vertex) * vertices.size();
	Vector<uint32_t> indices(36);
	{
		indices[0] = 0; indices[1] = 1; indices[2] = 2;
		indices[3] = 1; indices[4] = 3; indices[5] = 2;

		indices[6] = 4; indices[7] = 5; indices[8] = 6;
		indices[9] = 5; indices[10] = 7; indices[11] = 6;

		indices[12] = 8; indices[13] = 9; indices[14] = 10;
		indices[15] = 9; indices[16] = 11; indices[17] = 10;

		indices[18] = 12; indices[19] = 13; indices[20] = 14;
		indices[21] = 13; indices[22] = 15; indices[23] = 14;
		
		indices[24] = 16; indices[25] = 17; indices[26] = 18;
		indices[27] = 17; indices[28] = 19; indices[29] = 18;

		indices[30] = 20; indices[31] = 21; indices[32] = 22;
		indices[33] = 21; indices[34] = 23; indices[35] = 22;
	}
	Size indicesTotalSize = sizeof(uint32_t) * indices.size();
	Vector<float> uniforms(16 * 4);
	Size uniformsTotalSize = sizeof(float)* uniforms.size();

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
	auto vk_vertexShaderModule = CreateShaderModule(vk_device, ShaderModuleCreateInfo(Move(loadShader("../../../../../Media/Shaders/Functions_Cube/1.spir-v.vs"))));
	auto vk_fragmentShaderModule = CreateShaderModule(vk_device, ShaderModuleCreateInfo(Move(loadShader("../../../../../Media/Shaders/Functions_Cube/1.spir-v.fs"))));

	// Images
	auto imageAlbedoData = Move(loadImage("../../../../../Media/Images/Functions_Cube/Albedo.png"));
	// auto imageAlbedoMipmaps = generateMipmaps(imageAlbedoData, 2048, 3);
	auto vk_imageAlbedo = CreateImage(vk_device, ImageCreateInfo(
		0,
		VkImageType::VK_IMAGE_TYPE_2D,
		VkFormat::VK_FORMAT_R8G8B8A8_UNORM,
		Extent3D(2048, 2048, 1),
		1,
		1,
		VkSampleCountFlagBits::VK_SAMPLE_COUNT_1_BIT,
		VkImageTiling::VK_IMAGE_TILING_LINEAR,
		VkImageUsageFlagBits::VK_IMAGE_USAGE_SAMPLED_BIT,
		VkSharingMode::VK_SHARING_MODE_EXCLUSIVE,
		{},
		VkImageLayout::VK_IMAGE_LAYOUT_UNDEFINED
	));
	auto vk_imageAlbedoDeviceMemory = [&]()
	{
		auto vk_memoryRequirements = GetImageMemoryRequirements(vk_device, vk_imageAlbedo);
		auto vk_deviceMemory = AllocateMemory(vk_device, MemoryAllocateInfo(
			vk_memoryRequirements.size,
			getDeviceMemoryIndex2(VkMemoryPropertyFlagBits::VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT, vk_memoryRequirements.memoryTypeBits)
		));

		auto data = static_cast<uint8_t*>(MapMemory(vk_device, vk_deviceMemory, 0, VK_WHOLE_SIZE, 0));

		for (Size x = 0; x < 2048; ++x)
		{
			for (Size y = 0; y < 2048; ++y)
			{
				Size i = x + y * 2048;

				data[i * 4 + 0] = imageAlbedoData[i * 3 + 0];
				data[i * 4 + 1] = imageAlbedoData[i * 3 + 1];
				data[i * 4 + 2] = imageAlbedoData[i * 3 + 2];
				data[i * 4 + 3] = 255;
			}
		}

		UnmapMemory(vk_device, vk_deviceMemory);

		BindImageMemory(vk_device, vk_imageAlbedo, vk_deviceMemory);

		return vk_deviceMemory;
	}();
	auto vk_imageViewAlbedo = CreateImageView(vk_device, ImageViewCreateInfo(
		0, vk_imageAlbedo, VkImageViewType::VK_IMAGE_VIEW_TYPE_2D, VkFormat::VK_FORMAT_R8G8B8A8_UNORM,
		ComponentMapping(
			VkComponentSwizzle::VK_COMPONENT_SWIZZLE_R,
			VkComponentSwizzle::VK_COMPONENT_SWIZZLE_G,
			VkComponentSwizzle::VK_COMPONENT_SWIZZLE_B,
			VkComponentSwizzle::VK_COMPONENT_SWIZZLE_A
		),
		ImageSubresourceRange(VkImageAspectFlagBits::VK_IMAGE_ASPECT_COLOR_BIT, 0, 1, 0, 1)
	));
	
	/*auto imageNormalsData = Move(loadImage("../../../../../Media/Images/Functions_Cube/Normals.png"));
	auto vk_imageNormals = CreateImage(vk_device, ImageCreateInfo(
		0,
		VkImageType::VK_IMAGE_TYPE_2D,
		VkFormat::VK_FORMAT_R8G8B8A8_UNORM,
		Extent3D(2048, 2048, 1),
		1,
		1,
		VkSampleCountFlagBits::VK_SAMPLE_COUNT_1_BIT,
		VkImageTiling::VK_IMAGE_TILING_LINEAR,
		VkImageUsageFlagBits::VK_IMAGE_USAGE_SAMPLED_BIT,
		VkSharingMode::VK_SHARING_MODE_EXCLUSIVE,
		{},
		VkImageLayout::VK_IMAGE_LAYOUT_UNDEFINED
	));
	auto vk_imageNormalsDeviceMemory = [&]()
	{
		auto vk_memoryRequirements = GetImageMemoryRequirements(vk_device, vk_imageNormals);
		auto vk_deviceMemory = AllocateMemory(vk_device, MemoryAllocateInfo(
			vk_memoryRequirements.size,
			getDeviceMemoryIndex2(VkMemoryPropertyFlagBits::VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT, vk_memoryRequirements.memoryTypeBits)
		));

		auto data = static_cast<uint8_t*>(MapMemory(vk_device, vk_deviceMemory, 0, VK_WHOLE_SIZE, 0));

		for (Size x = 0; x < 2048; ++x)
		{
			for (Size y = 0; y < 2048; ++y)
			{
				Size i = x + y * 2048;

				data[i * 4 + 0] = imageNormalsData[i * 4 + 0];
				data[i * 4 + 1] = imageNormalsData[i * 4 + 1];
				data[i * 4 + 2] = imageNormalsData[i * 4 + 2];
				data[i * 4 + 3] = imageNormalsData[i * 4 + 3];
			}
		}

		UnmapMemory(vk_device, vk_deviceMemory);

		BindImageMemory(vk_device, vk_imageNormals, vk_deviceMemory);

		return vk_deviceMemory;
	}();
	auto vk_imageViewNormals = CreateImageView(vk_device, ImageViewCreateInfo(
		0, vk_imageNormals, VkImageViewType::VK_IMAGE_VIEW_TYPE_2D, VkFormat::VK_FORMAT_R8G8B8A8_UNORM,
		ComponentMapping(
			VkComponentSwizzle::VK_COMPONENT_SWIZZLE_R,
			VkComponentSwizzle::VK_COMPONENT_SWIZZLE_G,
			VkComponentSwizzle::VK_COMPONENT_SWIZZLE_B,
			VkComponentSwizzle::VK_COMPONENT_SWIZZLE_A
		),
		ImageSubresourceRange(VkImageAspectFlagBits::VK_IMAGE_ASPECT_COLOR_BIT, 0, 1, 0, 1)
	));*/

	/*auto imageRoughnessData = Move(loadImage("../../../../../Media/Images/Functions_Cube/Roughness.png"));
	auto vk_imageRoughness = CreateImage(vk_device, ImageCreateInfo(
		0,
		VkImageType::VK_IMAGE_TYPE_2D,
		VkFormat::VK_FORMAT_R8_UNORM,
		Extent3D(2048, 2048, 1),
		1,
		1,
		VkSampleCountFlagBits::VK_SAMPLE_COUNT_1_BIT,
		VkImageTiling::VK_IMAGE_TILING_LINEAR,
		VkImageUsageFlagBits::VK_IMAGE_USAGE_SAMPLED_BIT,
		VkSharingMode::VK_SHARING_MODE_EXCLUSIVE,
		{},
		VkImageLayout::VK_IMAGE_LAYOUT_UNDEFINED
	));
	auto vk_imageRoughnessDeviceMemory = [&]()
	{
		auto vk_memoryRequirements = GetImageMemoryRequirements(vk_device, vk_imageRoughness);
		auto vk_deviceMemory = AllocateMemory(vk_device, MemoryAllocateInfo(
			vk_memoryRequirements.size,
			getDeviceMemoryIndex2(VkMemoryPropertyFlagBits::VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT, vk_memoryRequirements.memoryTypeBits)
		));

		auto data = static_cast<uint8_t*>(MapMemory(vk_device, vk_deviceMemory, 0, VK_WHOLE_SIZE, 0));

		for (Size x = 0; x < 2048; ++x)
		{
			for (Size y = 0; y < 2048; ++y)
			{
				Size i = x + y * 2048;

				data[i * 1 + 0] = imageRoughnessData[i * 3 + 0];
			}
		}

		UnmapMemory(vk_device, vk_deviceMemory);

		BindImageMemory(vk_device, vk_imageRoughness, vk_deviceMemory);

		return vk_deviceMemory;
	}();
	auto vk_imageViewRoughness = CreateImageView(vk_device, ImageViewCreateInfo(
		0, vk_imageRoughness, VkImageViewType::VK_IMAGE_VIEW_TYPE_2D, VkFormat::VK_FORMAT_R8_UNORM,
		ComponentMapping(
			VkComponentSwizzle::VK_COMPONENT_SWIZZLE_R,
			VkComponentSwizzle::VK_COMPONENT_SWIZZLE_R,
			VkComponentSwizzle::VK_COMPONENT_SWIZZLE_R,
			VkComponentSwizzle::VK_COMPONENT_SWIZZLE_R
		),
		ImageSubresourceRange(VkImageAspectFlagBits::VK_IMAGE_ASPECT_COLOR_BIT, 0, 1, 0, 1)
	));*/

	/*auto imageOcclusionData = Move(loadImage("../../../../../Media/Images/Functions_Cube/Occlusion.png"));
	auto vk_imageOcclusion = CreateImage(vk_device, ImageCreateInfo(
		0,
		VkImageType::VK_IMAGE_TYPE_2D,
		VkFormat::VK_FORMAT_R8_UNORM,
		Extent3D(2048, 2048, 1),
		1,
		1,
		VkSampleCountFlagBits::VK_SAMPLE_COUNT_1_BIT,
		VkImageTiling::VK_IMAGE_TILING_LINEAR,
		VkImageUsageFlagBits::VK_IMAGE_USAGE_SAMPLED_BIT,
		VkSharingMode::VK_SHARING_MODE_EXCLUSIVE,
		{},
		VkImageLayout::VK_IMAGE_LAYOUT_UNDEFINED
	));
	auto vk_imageOcclusionDeviceMemory = [&]()
	{
		auto vk_memoryRequirements = GetImageMemoryRequirements(vk_device, vk_imageOcclusion);
		auto vk_deviceMemory = AllocateMemory(vk_device, MemoryAllocateInfo(
			vk_memoryRequirements.size,
			getDeviceMemoryIndex2(VkMemoryPropertyFlagBits::VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT, vk_memoryRequirements.memoryTypeBits)
		));

		auto data = static_cast<uint8_t*>(MapMemory(vk_device, vk_deviceMemory, 0, VK_WHOLE_SIZE, 0));

		for (Size x = 0; x < 2048; ++x)
		{
			for (Size y = 0; y < 2048; ++y)
			{
				Size i = x + y * 2048;

				data[i * 1 + 0] = imageOcclusionData[i * 3 + 0];
			}
		}

		UnmapMemory(vk_device, vk_deviceMemory);

		BindImageMemory(vk_device, vk_imageOcclusion, vk_deviceMemory);

		return vk_deviceMemory;
	}();
	auto vk_imageViewOcclusion = CreateImageView(vk_device, ImageViewCreateInfo(
		0, vk_imageOcclusion, VkImageViewType::VK_IMAGE_VIEW_TYPE_2D, VkFormat::VK_FORMAT_R8_UNORM,
		ComponentMapping(
			VkComponentSwizzle::VK_COMPONENT_SWIZZLE_R,
			VkComponentSwizzle::VK_COMPONENT_SWIZZLE_R,
			VkComponentSwizzle::VK_COMPONENT_SWIZZLE_R,
			VkComponentSwizzle::VK_COMPONENT_SWIZZLE_R
		),
		ImageSubresourceRange(VkImageAspectFlagBits::VK_IMAGE_ASPECT_COLOR_BIT, 0, 1, 0, 1)
	));*/

	// Queue
	auto vk_queue = GetDeviceQueue(vk_device, 0, 0);

	// Command Pool
	auto vk_commandPool = CreateCommandPool(vk_device, CommandPoolCreateInfo(VkCommandPoolCreateFlagBits::VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT, 0));
	{
		ResetCommandPool(vk_device, vk_commandPool, VkCommandPoolResetFlagBits::VK_COMMAND_POOL_RESET_RELEASE_RESOURCES_BIT);
	}

	// TEST
	GetPhysicalDeviceFormatProperties(vk_physicalDevice, VkFormat::VK_FORMAT_R8G8B8A8_UNORM);
	GetPhysicalDeviceImageFormatProperties(vk_physicalDevice, VkFormat::VK_FORMAT_R8G8B8A8_UNORM, VkImageType::VK_IMAGE_TYPE_2D, VkImageTiling::VK_IMAGE_TILING_LINEAR, VkImageUsageFlagBits::VK_IMAGE_USAGE_SAMPLED_BIT | VkImageUsageFlagBits::VK_IMAGE_USAGE_TRANSFER_SRC_BIT, 0);
	GetPhysicalDeviceImageFormatProperties(vk_physicalDevice, VkFormat::VK_FORMAT_R8G8B8A8_UNORM, VkImageType::VK_IMAGE_TYPE_2D, VkImageTiling::VK_IMAGE_TILING_OPTIMAL, VkImageUsageFlagBits::VK_IMAGE_USAGE_SAMPLED_BIT | VkImageUsageFlagBits::VK_IMAGE_USAGE_TRANSFER_DST_BIT, 0);

	auto testSourceImage = CreateImage(vk_device, ImageCreateInfo(
		0, VkImageType::VK_IMAGE_TYPE_2D, VkFormat::VK_FORMAT_R8G8B8A8_UNORM, Extent3D(4, 4, 1), 1, 1,
		VkSampleCountFlagBits::VK_SAMPLE_COUNT_1_BIT, VkImageTiling::VK_IMAGE_TILING_LINEAR,
		VkImageUsageFlagBits::VK_IMAGE_USAGE_SAMPLED_BIT | VkImageUsageFlagBits::VK_IMAGE_USAGE_TRANSFER_SRC_BIT, VkImageLayout::VK_IMAGE_LAYOUT_UNDEFINED
	));
	auto testSourceImageDeviceMemory = [&]()
	{
		auto vk_memoryRequirements = GetImageMemoryRequirements(vk_device, testSourceImage);
		auto vk_deviceMemory = AllocateMemory(vk_device, MemoryAllocateInfo(
			vk_memoryRequirements.size,
			getDeviceMemoryIndex2(VkMemoryPropertyFlagBits::VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT, vk_memoryRequirements.memoryTypeBits)
		));

		auto vk_imageSubresourceLayout = GetImageSubresourceLayout(vk_device, testSourceImage, ImageSubresource(VkImageAspectFlagBits::VK_IMAGE_ASPECT_COLOR_BIT, 0, 0));

		auto data = static_cast<uint8_t*>(MapMemory(vk_device, vk_deviceMemory, 0, VK_WHOLE_SIZE, 0));

		// std::memset(data, 0xFF, static_cast<size_t>(vk_memoryRequirements.size));
		for (Size x = 0; x < 4; ++x)
		{
			for (Size y = 0; y < 4; ++y)
			{
				Size i = vk_imageSubresourceLayout.offset + y * vk_imageSubresourceLayout.rowPitch + x * 4;
				
				data[i + 0] = 255;
				data[i + 1] = 0;
				data[i + 2] = 0;
				data[i + 3] = 255;
			}
		}

		UnmapMemory(vk_device, vk_deviceMemory);

		BindImageMemory(vk_device, testSourceImage, vk_deviceMemory);

		return vk_deviceMemory;
	}();
	auto testSourceImage2 = CreateImage(vk_device, ImageCreateInfo(
		0, VkImageType::VK_IMAGE_TYPE_2D, VkFormat::VK_FORMAT_R8G8B8A8_UNORM, Extent3D(2, 2, 1), 1, 1,
		VkSampleCountFlagBits::VK_SAMPLE_COUNT_1_BIT, VkImageTiling::VK_IMAGE_TILING_LINEAR,
		VkImageUsageFlagBits::VK_IMAGE_USAGE_SAMPLED_BIT | VkImageUsageFlagBits::VK_IMAGE_USAGE_TRANSFER_SRC_BIT, VkImageLayout::VK_IMAGE_LAYOUT_UNDEFINED
	));
	auto testSourceImageDeviceMemory2 = [&]()
	{
		auto vk_memoryRequirements = GetImageMemoryRequirements(vk_device, testSourceImage2);
		auto vk_deviceMemory = AllocateMemory(vk_device, MemoryAllocateInfo(
			vk_memoryRequirements.size,
			getDeviceMemoryIndex2(VkMemoryPropertyFlagBits::VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT, vk_memoryRequirements.memoryTypeBits)
		));

		auto vk_imageSubresourceLayout = GetImageSubresourceLayout(vk_device, testSourceImage2, ImageSubresource(VkImageAspectFlagBits::VK_IMAGE_ASPECT_COLOR_BIT, 0, 0));

		auto data = static_cast<uint8_t*>(MapMemory(vk_device, vk_deviceMemory, 0, VK_WHOLE_SIZE, 0));

		// std::memset(data, 0xFF, static_cast<size_t>(vk_memoryRequirements.size));
		for (Size x = 0; x < 2; ++x)
		{
			for (Size y = 0; y < 2; ++y)
			{
				Size i = vk_imageSubresourceLayout.offset + y * vk_imageSubresourceLayout.rowPitch + x * 4;

				data[i + 0] = 0;
				data[i + 1] = 255;
				data[i + 2] = 0;
				data[i + 3] = 255;
			}
		}

		UnmapMemory(vk_device, vk_deviceMemory);

		BindImageMemory(vk_device, testSourceImage2, vk_deviceMemory);

		return vk_deviceMemory;
	}();
	auto testSourceImage3 = CreateImage(vk_device, ImageCreateInfo(
		0, VkImageType::VK_IMAGE_TYPE_2D, VkFormat::VK_FORMAT_R8G8B8A8_UNORM, Extent3D(1, 1, 1), 1, 1,
		VkSampleCountFlagBits::VK_SAMPLE_COUNT_1_BIT, VkImageTiling::VK_IMAGE_TILING_LINEAR,
		VkImageUsageFlagBits::VK_IMAGE_USAGE_SAMPLED_BIT | VkImageUsageFlagBits::VK_IMAGE_USAGE_TRANSFER_SRC_BIT, VkImageLayout::VK_IMAGE_LAYOUT_UNDEFINED
	));
	auto testSourceImageDeviceMemory3 = [&]()
	{
		auto vk_memoryRequirements = GetImageMemoryRequirements(vk_device, testSourceImage3);
		auto vk_deviceMemory = AllocateMemory(vk_device, MemoryAllocateInfo(
			vk_memoryRequirements.size,
			getDeviceMemoryIndex2(VkMemoryPropertyFlagBits::VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT, vk_memoryRequirements.memoryTypeBits)
		));

		auto vk_imageSubresourceLayout = GetImageSubresourceLayout(vk_device, testSourceImage3, ImageSubresource(VkImageAspectFlagBits::VK_IMAGE_ASPECT_COLOR_BIT, 0, 0));

		auto data = static_cast<uint8_t*>(MapMemory(vk_device, vk_deviceMemory, 0, VK_WHOLE_SIZE, 0));

		for (Size x = 0; x < 1; ++x)
		{
			for (Size y = 0; y < 1; ++y)
			{
				Size i = vk_imageSubresourceLayout.offset + y * vk_imageSubresourceLayout.rowPitch + x * 4;

				data[i + 0] = 0;
				data[i + 1] = 0;
				data[i + 2] = 255;
				data[i + 3] = 255;
			}
		}

		UnmapMemory(vk_device, vk_deviceMemory);

		BindImageMemory(vk_device, testSourceImage3, vk_deviceMemory);

		return vk_deviceMemory;
	}();

	/*auto testSourceImage = CreateImage(vk_device, ImageCreateInfo(
		0, VkImageType::VK_IMAGE_TYPE_2D, VkFormat::VK_FORMAT_R8G8B8A8_UNORM, Extent3D(4, 4, 1), 1, 1,
		VkSampleCountFlagBits::VK_SAMPLE_COUNT_1_BIT, VkImageTiling::VK_IMAGE_TILING_LINEAR,
		VkImageUsageFlagBits::VK_IMAGE_USAGE_SAMPLED_BIT | VkImageUsageFlagBits::VK_IMAGE_USAGE_TRANSFER_SRC_BIT, VkImageLayout::VK_IMAGE_LAYOUT_UNDEFINED
	));
	auto testSourceImageDeviceMemory = [&]()
	{
		auto vk_memoryRequirements = GetImageMemoryRequirements(vk_device, testSourceImage);
		auto vk_deviceMemory = AllocateMemory(vk_device, MemoryAllocateInfo(
			vk_memoryRequirements.size,
			getDeviceMemoryIndex2(VkMemoryPropertyFlagBits::VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT, vk_memoryRequirements.memoryTypeBits)
		));

		auto vk_imageSubresourceLayout = GetImageSubresourceLayout(vk_device, testSourceImage, ImageSubresource(VkImageAspectFlagBits::VK_IMAGE_ASPECT_COLOR_BIT, 0, 0));

		auto data = static_cast<uint8_t*>(MapMemory(vk_device, vk_deviceMemory, 0, VK_WHOLE_SIZE, 0));

		for (Size x = 0; x < 4; ++x)
		{
			for (Size y = 0; y < 4; ++y)
			{
				Size i = vk_imageSubresourceLayout.offset + y * vk_imageSubresourceLayout.rowPitch + x * 4;
				
				data[i + 0] = 255;
				data[i + 1] = 0;
				data[i + 2] = 0;
				data[i + 3] = 255;
			}
		}

		UnmapMemory(vk_device, vk_deviceMemory);

		BindImageMemory(vk_device, testSourceImage, vk_deviceMemory);

		return vk_deviceMemory;
	}();*/
	
	/*auto testDestinationImage = CreateImage(vk_device, ImageCreateInfo(
		0, VkImageType::VK_IMAGE_TYPE_2D, VkFormat::VK_FORMAT_R8G8B8A8_UNORM, Extent3D(4, 4, 1), 3, 1,
		VkSampleCountFlagBits::VK_SAMPLE_COUNT_1_BIT, VkImageTiling::VK_IMAGE_TILING_OPTIMAL,
		VkImageUsageFlagBits::VK_IMAGE_USAGE_SAMPLED_BIT | VkImageUsageFlagBits::VK_IMAGE_USAGE_TRANSFER_DST_BIT, VkImageLayout::VK_IMAGE_LAYOUT_UNDEFINED
	));
	auto testDestinationImageDeviceMemory = [&]()
	{
		auto vk_memoryRequirements = GetImageMemoryRequirements(vk_device, testDestinationImage);
		auto vk_deviceMemory = AllocateMemory(vk_device, MemoryAllocateInfo(
			vk_memoryRequirements.size,
			getDeviceMemoryIndex2(0, vk_memoryRequirements.memoryTypeBits)
		));

		BindImageMemory(vk_device, testDestinationImage, vk_deviceMemory);

		return vk_deviceMemory;
	}();*/
	/*auto testDestinationImage = CreateImage(vk_device, ImageCreateInfo(
		0, VkImageType::VK_IMAGE_TYPE_2D, VkFormat::VK_FORMAT_R8G8B8A8_UNORM, Extent3D(4, 4, 1), 1, 1,
		VkSampleCountFlagBits::VK_SAMPLE_COUNT_1_BIT, VkImageTiling::VK_IMAGE_TILING_LINEAR,
		VkImageUsageFlagBits::VK_IMAGE_USAGE_SAMPLED_BIT, VkImageLayout::VK_IMAGE_LAYOUT_UNDEFINED
	));
	auto testDestinationImageDeviceMemory = [&]()
	{
		auto vk_memoryRequirements = GetImageMemoryRequirements(vk_device, testDestinationImage);
		auto vk_deviceMemory = AllocateMemory(vk_device, MemoryAllocateInfo(
			vk_memoryRequirements.size,
			getDeviceMemoryIndex2(VkMemoryPropertyFlagBits::VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT, vk_memoryRequirements.memoryTypeBits)
		));

		auto vk_imageSubresourceLayout = GetImageSubresourceLayout(vk_device, testDestinationImage, ImageSubresource(VkImageAspectFlagBits::VK_IMAGE_ASPECT_COLOR_BIT, 0, 0));

		auto data = static_cast<uint8_t*>(MapMemory(vk_device, vk_deviceMemory, 0, VK_WHOLE_SIZE, 0));

		// std::memset(data, 0xFF, static_cast<size_t>(vk_memoryRequirements.size));
		for (Size x = 0; x < 4; ++x)
		{
			for (Size y = 0; y < 4; ++y)
			{
				Size i = vk_imageSubresourceLayout.offset + y * vk_imageSubresourceLayout.rowPitch + x * 4;
				
				data[i + 0] = rand() % 255;
				data[i + 1] = rand() % 255;
				data[i + 2] = rand() % 255;
				data[i + 3] = 255;
			}
		}

		UnmapMemory(vk_device, vk_deviceMemory);

		BindImageMemory(vk_device, testDestinationImage, vk_deviceMemory);

		return vk_deviceMemory;
	}();*/
	/*auto testDestinationImage = CreateImage(vk_device, ImageCreateInfo(
		0, VkImageType::VK_IMAGE_TYPE_2D, VkFormat::VK_FORMAT_R8G8B8A8_UNORM, Extent3D(4, 4, 1), 1, 1,
		VkSampleCountFlagBits::VK_SAMPLE_COUNT_1_BIT, VkImageTiling::VK_IMAGE_TILING_OPTIMAL,
		VkImageUsageFlagBits::VK_IMAGE_USAGE_SAMPLED_BIT | VkImageUsageFlagBits::VK_IMAGE_USAGE_TRANSFER_DST_BIT, VkImageLayout::VK_IMAGE_LAYOUT_UNDEFINED
	));
	auto testDestinationImageDeviceMemory = [&]()
	{
		auto vk_memoryRequirements = GetImageMemoryRequirements(vk_device, testDestinationImage);
		auto vk_deviceMemory = AllocateMemory(vk_device, MemoryAllocateInfo(
			vk_memoryRequirements.size,
			getDeviceMemoryIndex2(0, vk_memoryRequirements.memoryTypeBits)
		));

		// auto vk_imageSubresourceLayout = GetImageSubresourceLayout(vk_device, testSourceImage, ImageSubresource(VkImageAspectFlagBits::VK_IMAGE_ASPECT_COLOR_BIT, 0, 0));
		// 
		// auto data = static_cast<uint8_t*>(MapMemory(vk_device, vk_deviceMemory, 0, VK_WHOLE_SIZE, 0));
		// 
		// for (Size x = 0; x < 4; ++x)
		// {
		// 	for (Size y = 0; y < 4; ++y)
		// 	{
		// 		Size i = vk_imageSubresourceLayout.offset + y * vk_imageSubresourceLayout.rowPitch + x * 4;
		// 		
		// 		data[i + 0] = 0;
		// 		data[i + 1] = 255;
		// 		data[i + 2] = 0;
		// 		data[i + 3] = 255;
		// 	}
		// }
		// 
		// UnmapMemory(vk_device, vk_deviceMemory);

		BindImageMemory(vk_device, testDestinationImage, vk_deviceMemory);

		return vk_deviceMemory;
	}();*/
	auto testDestinationImage = CreateImage(vk_device, ImageCreateInfo(
		0, VkImageType::VK_IMAGE_TYPE_2D, VkFormat::VK_FORMAT_R8G8B8A8_UNORM, Extent3D(4, 4, 1), 3, 1,
		VkSampleCountFlagBits::VK_SAMPLE_COUNT_1_BIT, VkImageTiling::VK_IMAGE_TILING_OPTIMAL,
		VkImageUsageFlagBits::VK_IMAGE_USAGE_SAMPLED_BIT | VkImageUsageFlagBits::VK_IMAGE_USAGE_TRANSFER_DST_BIT, VkImageLayout::VK_IMAGE_LAYOUT_UNDEFINED
	));
	auto testDestinationImageDeviceMemory = [&]()
	{
		auto vk_memoryRequirements = GetImageMemoryRequirements(vk_device, testDestinationImage);
		auto vk_deviceMemory = AllocateMemory(vk_device, MemoryAllocateInfo(
			vk_memoryRequirements.size,
			getDeviceMemoryIndex2(0, vk_memoryRequirements.memoryTypeBits)
		));

		BindImageMemory(vk_device, testDestinationImage, vk_deviceMemory);

		return vk_deviceMemory;
	}();
	
	auto testCommandBuffers = Move(AllocateCommandBuffers(vk_device, CommandBufferAllocateInfo(vk_commandPool, VkCommandBufferLevel::VK_COMMAND_BUFFER_LEVEL_PRIMARY, 1)));
	auto &testCommandBuffer = testCommandBuffers[0];
	{
		ResetCommandBuffer(testCommandBuffer, VkCommandBufferResetFlagBits::VK_COMMAND_BUFFER_RESET_RELEASE_RESOURCES_BIT);
		BeginCommandBuffer(testCommandBuffer, CommandBufferBeginInfo(0));
			
		// CmdCopyImage(testCommandBuffer,
		// 	testSourceImage, VkImageLayout::VK_IMAGE_LAYOUT_GENERAL,
		// 	testDestinationImage, VkImageLayout::VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL,
		// 	{
		// 		ImageCopy(
		// 			ImageSubresourceLayers(VkImageAspectFlagBits::VK_IMAGE_ASPECT_COLOR_BIT, 0, 0, 0),
		// 			Offset3D(0, 0, 0),
		// 			ImageSubresourceLayers(VkImageAspectFlagBits::VK_IMAGE_ASPECT_COLOR_BIT, 0, 0, 0),
		// 			Offset3D(0, 0, 0),
		// 			Extent3D(4, 4, 1)
		// 		),
		// 	}
		// );
		// CmdCopyImage(testCommandBuffer,
		// 	testSourceImage2, VkImageLayout::VK_IMAGE_LAYOUT_GENERAL,
		// 	testDestinationImage, VkImageLayout::VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL,
		// 	{
		// 		ImageCopy(
		// 			ImageSubresourceLayers(VkImageAspectFlagBits::VK_IMAGE_ASPECT_COLOR_BIT, 0, 0, 0),
		// 			Offset3D(0, 0, 0),
		// 			ImageSubresourceLayers(VkImageAspectFlagBits::VK_IMAGE_ASPECT_COLOR_BIT, 1, 0, 0),
		// 			Offset3D(0, 0, 0),
		// 			Extent3D(2, 2, 1)
		// 		),
		// 	}
		// );
		// CmdCopyImage(testCommandBuffer,
		// 	testSourceImage3, VkImageLayout::VK_IMAGE_LAYOUT_GENERAL,
		// 	testDestinationImage, VkImageLayout::VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL,
		// 	{
		// 		ImageCopy(
		// 			ImageSubresourceLayers(VkImageAspectFlagBits::VK_IMAGE_ASPECT_COLOR_BIT, 0, 0, 0),
		// 			Offset3D(0, 0, 0),
		// 			ImageSubresourceLayers(VkImageAspectFlagBits::VK_IMAGE_ASPECT_COLOR_BIT, 2, 0, 0),
		// 			Offset3D(0, 0, 0),
		// 			Extent3D(1, 1, 1)
		// 		),
		// 	}
		// );
		
		CmdPipelineBarrier(testCommandBuffer, VkPipelineStageFlagBits::VK_PIPELINE_STAGE_TRANSFER_BIT, VkPipelineStageFlagBits::VK_PIPELINE_STAGE_TRANSFER_BIT,
			0, {}, {},
			{
				ImageMemoryBarrier(
					0,
					VkAccessFlagBits::VK_ACCESS_TRANSFER_READ_BIT,
					VkImageLayout::VK_IMAGE_LAYOUT_UNDEFINED,
					VkImageLayout::VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL,
					0,
					0,
					testSourceImage,
					ImageSubresourceRange(VkImageAspectFlagBits::VK_IMAGE_ASPECT_COLOR_BIT, 0, 1, 0, 1)
				),
			}
		);
		CmdPipelineBarrier(testCommandBuffer, VkPipelineStageFlagBits::VK_PIPELINE_STAGE_TRANSFER_BIT, VkPipelineStageFlagBits::VK_PIPELINE_STAGE_TRANSFER_BIT,
			0, {}, {},
			{
				ImageMemoryBarrier(
					0,
					VkAccessFlagBits::VK_ACCESS_TRANSFER_READ_BIT,
					VkImageLayout::VK_IMAGE_LAYOUT_UNDEFINED,
					VkImageLayout::VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL,
					0,
					0,
					testSourceImage2,
					ImageSubresourceRange(VkImageAspectFlagBits::VK_IMAGE_ASPECT_COLOR_BIT, 0, 1, 0, 1)
				),
			}
		);
		CmdPipelineBarrier(testCommandBuffer, VkPipelineStageFlagBits::VK_PIPELINE_STAGE_TRANSFER_BIT, VkPipelineStageFlagBits::VK_PIPELINE_STAGE_TRANSFER_BIT,
			0, {}, {},
			{
				ImageMemoryBarrier(
					0,
					VkAccessFlagBits::VK_ACCESS_TRANSFER_READ_BIT,
					VkImageLayout::VK_IMAGE_LAYOUT_UNDEFINED,
					VkImageLayout::VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL,
					0,
					0,
					testSourceImage3,
					ImageSubresourceRange(VkImageAspectFlagBits::VK_IMAGE_ASPECT_COLOR_BIT, 0, 1, 0, 1)
				),
			}
		);
		CmdPipelineBarrier(testCommandBuffer, VkPipelineStageFlagBits::VK_PIPELINE_STAGE_TRANSFER_BIT, VkPipelineStageFlagBits::VK_PIPELINE_STAGE_TRANSFER_BIT,
			0, {}, {},
			{
				ImageMemoryBarrier(
					0,
					VkAccessFlagBits::VK_ACCESS_TRANSFER_WRITE_BIT,
					VkImageLayout::VK_IMAGE_LAYOUT_UNDEFINED,
					VkImageLayout::VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL,
					0,
					0,
					testDestinationImage,
					ImageSubresourceRange(VkImageAspectFlagBits::VK_IMAGE_ASPECT_COLOR_BIT, 0, 3, 0, 1)
				),
			}
		);

		CmdCopyImage(testCommandBuffer,
			testSourceImage3, VkImageLayout::VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL,
			testDestinationImage, VkImageLayout::VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL,
			{
				ImageCopy(
					ImageSubresourceLayers(VkImageAspectFlagBits::VK_IMAGE_ASPECT_COLOR_BIT, 0, 0, 1),
					Offset3D(0, 0, 0),
					ImageSubresourceLayers(VkImageAspectFlagBits::VK_IMAGE_ASPECT_COLOR_BIT, 2, 0, 1),
					Offset3D(0, 0, 0),
					Extent3D(1, 1, 1)
				),
			}
		);
		CmdCopyImage(testCommandBuffer,
			testSourceImage2, VkImageLayout::VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL,
			testDestinationImage, VkImageLayout::VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL,
			{
				ImageCopy(
					ImageSubresourceLayers(VkImageAspectFlagBits::VK_IMAGE_ASPECT_COLOR_BIT, 0, 0, 1),
					Offset3D(0, 0, 0),
					ImageSubresourceLayers(VkImageAspectFlagBits::VK_IMAGE_ASPECT_COLOR_BIT, 1, 0, 1),
					Offset3D(0, 0, 0),
					Extent3D(2, 2, 1)
				),
			}
		);
		CmdCopyImage(testCommandBuffer,
			testSourceImage, VkImageLayout::VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL,
			testDestinationImage, VkImageLayout::VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL,
			{
				ImageCopy(
					ImageSubresourceLayers(VkImageAspectFlagBits::VK_IMAGE_ASPECT_COLOR_BIT, 0, 0, 1),
					Offset3D(0, 0, 0),
					ImageSubresourceLayers(VkImageAspectFlagBits::VK_IMAGE_ASPECT_COLOR_BIT, 0, 0, 1),
					Offset3D(0, 0, 0),
					Extent3D(4, 4, 1)
				),
			}
		);

		CmdPipelineBarrier(testCommandBuffer,
			VkPipelineStageFlagBits::VK_PIPELINE_STAGE_TRANSFER_BIT,
			VkPipelineStageFlagBits::VK_PIPELINE_STAGE_TRANSFER_BIT,
			0,
			{},
			{},
			{
				ImageMemoryBarrier(
					0,
					0,
					VkImageLayout::VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL,
					VkImageLayout::VK_IMAGE_LAYOUT_GENERAL,
					0,
					0,
					testDestinationImage,
					ImageSubresourceRange(VkImageAspectFlagBits::VK_IMAGE_ASPECT_COLOR_BIT, 0, 3, 0, 1)
				),
			}
		);

		EndCommandBuffer(testCommandBuffer);

		QueueWaitIdle(vk_queue);
		QueueSubmit(vk_queue, {SubmitInfo({testCommandBuffer})});
		QueueWaitIdle(vk_queue);
	}

	/*auto testDestinationImage = CreateImage(vk_device, ImageCreateInfo(
		0, VkImageType::VK_IMAGE_TYPE_2D, VkFormat::VK_FORMAT_R8G8B8A8_UNORM, Extent3D(4, 4, 1), 1, 1,
		VkSampleCountFlagBits::VK_SAMPLE_COUNT_1_BIT, VkImageTiling::VK_IMAGE_TILING_LINEAR,
		VkImageUsageFlagBits::VK_IMAGE_USAGE_SAMPLED_BIT, VkImageLayout::VK_IMAGE_LAYOUT_UNDEFINED
	));
	auto testDestinationImageDeviceMemory = [&]()
	{
		auto vk_memoryRequirements = GetImageMemoryRequirements(vk_device, testDestinationImage);
		auto vk_deviceMemory = AllocateMemory(vk_device, MemoryAllocateInfo(
			vk_memoryRequirements.size,
			getDeviceMemoryIndex2(VkMemoryPropertyFlagBits::VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT, vk_memoryRequirements.memoryTypeBits)
		));

		auto data = static_cast<uint8_t*>(MapMemory(vk_device, vk_deviceMemory, 0, VK_WHOLE_SIZE, 0));

		auto vk_imageSubresourceLayout = GetImageSubresourceLayout(vk_device, testDestinationImage, ImageSubresource(VkImageAspectFlagBits::VK_IMAGE_ASPECT_COLOR_BIT, 0, 0));

		for (Size x = 0; x < 4; ++x)
		{
			for (Size y = 0; y < 4; ++y)
			{
				Size i = vk_imageSubresourceLayout.offset + y * vk_imageSubresourceLayout.rowPitch + x * 4;

				data[i + 0] = rand() % 255;
				data[i + 1] = rand() % 255;
				data[i + 2] = rand() % 255;
				data[i + 3] = 255;
			}
		}

		UnmapMemory(vk_device, vk_deviceMemory);

		BindImageMemory(vk_device, testDestinationImage, vk_deviceMemory);

		return vk_deviceMemory;
	}();*/
	auto testDestinationImageView = CreateImageView(vk_device, ImageViewCreateInfo(
		0, testDestinationImage, VkImageViewType::VK_IMAGE_VIEW_TYPE_2D, VkFormat::VK_FORMAT_R8G8B8A8_UNORM,
		ComponentMapping(
			VkComponentSwizzle::VK_COMPONENT_SWIZZLE_R,
			VkComponentSwizzle::VK_COMPONENT_SWIZZLE_G,
			VkComponentSwizzle::VK_COMPONENT_SWIZZLE_B,
			VkComponentSwizzle::VK_COMPONENT_SWIZZLE_A
		),
		ImageSubresourceRange(VkImageAspectFlagBits::VK_IMAGE_ASPECT_COLOR_BIT, 0, 1, 0, 1)
	));



	// Sampler
	auto vk_sampler = CreateSampler(vk_device, SamplerCreateInfo(
		VkFilter::VK_FILTER_NEAREST, VkFilter::VK_FILTER_NEAREST, VkSamplerMipmapMode::VK_SAMPLER_MIPMAP_MODE_LINEAR,
		VkSamplerAddressMode::VK_SAMPLER_ADDRESS_MODE_REPEAT, VkSamplerAddressMode::VK_SAMPLER_ADDRESS_MODE_REPEAT, VkSamplerAddressMode::VK_SAMPLER_ADDRESS_MODE_REPEAT,
		0.0f, VK_FALSE, 1.0f, VK_FALSE, VkCompareOp::VK_COMPARE_OP_ALWAYS, 0.0f, 0.0f, VkBorderColor::VK_BORDER_COLOR_INT_OPAQUE_BLACK, VK_FALSE
	));

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
		DescriptorPoolSize(VkDescriptorType::VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, 4),
	}));

	// Descriptor Set Layout
	auto vk_descriptorSetLayout = CreateDescriptorSetLayout(vk_device, DescriptorSetLayoutCreateInfo(0, {
		DescriptorSetLayoutBinding(0, VkDescriptorType::VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, 1, VkShaderStageFlagBits::VK_SHADER_STAGE_VERTEX_BIT),
		DescriptorSetLayoutBinding(1, VkDescriptorType::VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, 1, VkShaderStageFlagBits::VK_SHADER_STAGE_FRAGMENT_BIT),
		// DescriptorSetLayoutBinding(2, VkDescriptorType::VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, 1, VkShaderStageFlagBits::VK_SHADER_STAGE_FRAGMENT_BIT),
		// DescriptorSetLayoutBinding(3, VkDescriptorType::VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, 1, VkShaderStageFlagBits::VK_SHADER_STAGE_FRAGMENT_BIT),
		// DescriptorSetLayoutBinding(4, VkDescriptorType::VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, 1, VkShaderStageFlagBits::VK_SHADER_STAGE_FRAGMENT_BIT),
	}));

	// Descriptor Set
	auto vk_descriptorSets = Move(AllocateDescriptorSets(vk_device, DescriptorSetAllocateInfo(vk_descriptorPool, {vk_descriptorSetLayout})));
	auto &vk_descriptorSet = vk_descriptorSets[0];
	{
		UpdateDescriptorSets(vk_device, {
			WriteDescriptorSet(vk_descriptorSet, 0, 0, VkDescriptorType::VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, {DescriptorBufferInfo(vk_uniformBuffer)}),

			WriteDescriptorSet(vk_descriptorSet, 1, 0, VkDescriptorType::VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, {DescriptorImageInfo(vk_sampler, testDestinationImageView, VkImageLayout::VK_IMAGE_LAYOUT_PREINITIALIZED)}),

			// WriteDescriptorSet(vk_descriptorSet, 1, 0, VkDescriptorType::VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, {DescriptorImageInfo(vk_sampler, vk_imageViewAlbedo, VkImageLayout::VK_IMAGE_LAYOUT_PREINITIALIZED)}),
			// WriteDescriptorSet(vk_descriptorSet, 2, 0, VkDescriptorType::VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, {DescriptorImageInfo(vk_sampler, vk_imageViewNormals, VkImageLayout::VK_IMAGE_LAYOUT_PREINITIALIZED)}),
			// WriteDescriptorSet(vk_descriptorSet, 3, 0, VkDescriptorType::VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, {DescriptorImageInfo(vk_sampler, vk_imageViewRoughness, VkImageLayout::VK_IMAGE_LAYOUT_PREINITIALIZED)}),
			// WriteDescriptorSet(vk_descriptorSet, 4, 0, VkDescriptorType::VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, {DescriptorImageInfo(vk_sampler, vk_imageViewOcclusion, VkImageLayout::VK_IMAGE_LAYOUT_PREINITIALIZED)}),
		}, {});
	}

	// Pipeline Layout
	auto vk_pipelineLayout = CreatePipelineLayout(vk_device, PipelineLayoutCreateInfo(
		{vk_descriptorSetLayout},
		{}
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
				},
				{
					VertexInputAttributeDescription(0, 0, VkFormat::VK_FORMAT_R32G32B32_SFLOAT, 0),
					VertexInputAttributeDescription(1, 0, VkFormat::VK_FORMAT_R32G32_SFLOAT, sizeof(glm::vec3)),
					VertexInputAttributeDescription(2, 0, VkFormat::VK_FORMAT_R32G32B32_SFLOAT, sizeof(glm::vec3) + sizeof(glm::vec2)),
					VertexInputAttributeDescription(3, 0, VkFormat::VK_FORMAT_R32G32B32_SFLOAT, sizeof(glm::vec3) * 2 + sizeof(glm::vec2)),
					VertexInputAttributeDescription(4, 0, VkFormat::VK_FORMAT_R32G32B32_SFLOAT, sizeof(glm::vec3) * 3 + sizeof(glm::vec2)),
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
				VkPolygonMode::VK_POLYGON_MODE_FILL, VkCullModeFlagBits::VK_CULL_MODE_BACK_BIT, VkFrontFace::VK_FRONT_FACE_COUNTER_CLOCKWISE,
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
					ClearValue::Color(0.16f, 0.16f, 0.16f, 1.0f),
				}),
				VkSubpassContents::VK_SUBPASS_CONTENTS_INLINE
			);

			CmdBindPipeline(vk_commandBuffer, VkPipelineBindPoint::VK_PIPELINE_BIND_POINT_GRAPHICS, vk_pipeline);
			CmdBindVertexBuffers(vk_commandBuffer, 0, 1, {vk_vertexBuffer}, {0});
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

		// update uniforms
		QueueWaitIdle(vk_queue);

		angle += glm::vec3(0.2f, 0.5f, 0.1f);

		auto modelAngle = angle;
		auto modelPosition = glm::vec3(0.0f);
		auto viewAngle = glm::vec3(32.0f, 0.0f, 0.0f);
		auto viewPosition = glm::vec3(0.0f, 0.8f, -1.5f);

		auto rotateMatrix = rotateZXY(angle);
		auto modelMatrix = move(modelPosition) * rotateMatrix;
		auto modelMatrix2 = move(-viewPosition) * move(modelPosition) * rotateMatrix;
		auto viewMatrix = rotateYXZ(-viewAngle) * move(-viewPosition);
		auto projectionMatrix = perspective(60.0f, 1.0f, 0.1f, 1000.0f) * scale(glm::vec3(1.0f, -1.0f, 1.0f));

		auto modelViewProjectionMatrix = projectionMatrix * viewMatrix * modelMatrix;

		for (Size i = 0; i < 16; ++i)
		{
			uniforms[00 + i] = rotateMatrix[i / 4][i % 4];
			uniforms[16 + i] = modelMatrix2[i / 4][i % 4];
			uniforms[32 + i] = viewMatrix[i / 4][i % 4];
			uniforms[48 + i] = modelViewProjectionMatrix[i / 4][i % 4];
		}

		auto data = MapMemory(vk_device, vk_uniformBufferDeviceMemory, 0, VK_WHOLE_SIZE, 0);

		std::memcpy(data, uniforms.data(), uniformsTotalSize);

		UnmapMemory(vk_device, vk_uniformBufferDeviceMemory);

		auto vk_nextImageIndex = AcquireNextImageKHR(vk_device, vk_swapchain, UINT64_MAX, VK_NULL_HANDLE, vk_fence);
		
		WaitForFences(vk_device, {vk_fence}, VK_FALSE, UINT64_MAX);
		ResetFences(vk_device, {vk_fence});

		auto &vk_commandBuffer = vk_commandBuffers[vk_nextImageIndex];

		QueueSubmit(vk_queue, {SubmitInfo({vk_commandBuffer})});
		QueueWaitIdle(vk_queue);

		QueuePresentKHR(vk_queue, PresentInfoKHR({}, {vk_swapchain}, {vk_nextImageIndex}));

		Sleep(1000 / 60);
	}


	FreeCommandBuffers(vk_device, vk_commandPool, testCommandBuffers);


	DestroyFence(vk_device, vk_fence);
	FreeCommandBuffers(vk_device, vk_commandPool, vk_commandBuffers);
	DestroyCommandPool(vk_device, vk_commandPool); // TODO: crash when attempting to destroy pool without any buffers allocated
	DestroyPipeline(vk_device, vk_pipeline);
	DestroyPipelineLayout(vk_device, vk_pipelineLayout);
	FreeDescriptorSets(vk_device, vk_descriptorPool, vk_descriptorSets);
	DestroyDescriptorSetLayout(vk_device, vk_descriptorSetLayout);
	DestroyDescriptorPool(vk_device, vk_descriptorPool);
	DestroyRenderPass(vk_device, vk_renderPass);
	DestroySampler(vk_device, vk_sampler);
	DestroyImageView(vk_device, vk_imageViewAlbedo); FreeMemory(vk_device, vk_imageAlbedoDeviceMemory); DestroyImage(vk_device, vk_imageAlbedo);
	// DestroyImageView(vk_device, vk_imageViewNormals); FreeMemory(vk_device, vk_imageNormalsDeviceMemory); DestroyImage(vk_device, vk_imageNormals);
	// DestroyImageView(vk_device, vk_imageViewRoughness); FreeMemory(vk_device, vk_imageRoughnessDeviceMemory); DestroyImage(vk_device, vk_imageRoughness);
	// DestroyImageView(vk_device, vk_imageViewOcclusion); FreeMemory(vk_device, vk_imageOcclusionDeviceMemory); DestroyImage(vk_device, vk_imageOcclusion);


	FreeMemory(vk_device, testSourceImageDeviceMemory); DestroyImage(vk_device, testSourceImage);
	FreeMemory(vk_device, testSourceImageDeviceMemory2); DestroyImage(vk_device, testSourceImage2);
	FreeMemory(vk_device, testSourceImageDeviceMemory3); DestroyImage(vk_device, testSourceImage3);
	FreeMemory(vk_device, testDestinationImageDeviceMemory); DestroyImage(vk_device, testDestinationImage);
	DestroyImageView(vk_device, testDestinationImageView);


	FreeMemory(vk_device, vk_uniformBufferDeviceMemory); DestroyBuffer(vk_device, vk_uniformBuffer);
	FreeMemory(vk_device, vk_indexBufferDeviceMemory); DestroyBuffer(vk_device, vk_indexBuffer);
	FreeMemory(vk_device, vk_vertexBufferDeviceMemory); DestroyBuffer(vk_device, vk_vertexBuffer);
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