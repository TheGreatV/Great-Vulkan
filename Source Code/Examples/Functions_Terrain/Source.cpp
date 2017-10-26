#include <iostream>
#include <fstream>

#define VK_USE_PLATFORM_WIN32_KHR 1

#include <Windows.h>

#ifdef CreateSemaphore
#undef CreateSemaphore
#endif

#include <Common/Common.hpp>
#include <Vulkan/Vulkan.hpp>

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
inline glm::mat3 rotateX3(const glm::float32& angle_)
{
	auto a = glm::radians(angle_);
	auto s = glm::sin(a);
	auto c = glm::cos(a);

	return glm::transpose(glm::mat3(
		1.0f,	0.0f,	0.0f,
		0.0f,	c,		-s,	
		0.0f,	s,		c));
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
inline glm::mat3 rotateY3(const glm::float32& angle_)
{
	auto a = glm::radians(angle_);
	auto s = glm::sin(a);
	auto c = glm::cos(a);

	return glm::transpose(glm::mat3(
		c,		0.0f,	s,		
		0.0f,	1.0f,	0.0f,	
		-s,		0.0f,	c));
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
inline glm::mat3 rotateZ3(const glm::float32& angle_)
{
	auto a = glm::radians(angle_);
	auto s = glm::sin(a);
	auto c = glm::cos(a);

	return glm::transpose(glm::mat3(
		c,		s,		0.0f,	
		-s,		c,		0.0f,	
		0.0f,	0.0f,	1.0f));
}
inline glm::mat4 rotateZXY(const glm::vec3& angle_)
{
	return rotateY(angle_.y) * rotateX(angle_.x) * rotateZ(angle_.z);
}
inline glm::mat3 rotateZXY3(const glm::vec3& angle_)
{
	return rotateY3(angle_.y) * rotateX3(angle_.x) * rotateZ3(angle_.z);
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
inline glm::mat4 perspectiveInverse(const glm::float32& fov_, const glm::float32& aspect_, const glm::float32& near_, const glm::float32& far_)
{
	auto g = glm::radians(fov_);
	glm::float32 f = 1.0f / tanf(g / 2.0f);
	glm::float32 a = (far_ + near_) / (near_ - far_);
	glm::float32 b = (2.0f * far_ * near_) / (near_ - far_);

	return glm::transpose(glm::mat4( // corrected by *(1,1,-1)
		aspect_ / f,	0.0f,		0.0f,		0.0f,
		0.0f,			1.0f / f,	0.0f,		0.0f,
		0.0f,			0.0f,		0.0f,		1.0f,
		0.0f,			0.0f,		-1.0f / b,	a / b));
}
inline glm::vec3 xyz(const glm::vec4& xyzw_)
{
	return glm::vec3(xyzw_.x, xyzw_.y, xyzw_.z);
}


struct Vertex
{
	glm::vec3 pos;
	glm::vec2 tex;
	glm::vec3 tan;
	glm::vec3 bin;
	glm::vec3 nor;
};
struct Mesh
{
	Vector<Vertex> vertices;
	Vector<uint32_t> indices;
	inline Mesh() = default;
	inline Mesh(Mesh&& m): vertices(Move(m.vertices)), indices(Move(m.indices))
	{
	}
};
inline Mesh CreateSphere(const float& radius_, const glm::vec2& tex_, const glm::uvec2& seg_)
{
	using namespace glm;

	if(seg_.x < 3 || seg_.y < 3)
	{
		throw Exception("Invalid segments count");
	}

	Mesh mesh;
	{
		mesh.vertices.resize((seg_.x + 1)*(seg_.y + 1));
		mesh.indices.resize(6 * seg_.x*seg_.y);
	}

	for(Size x = 0; x <= seg_.x; ++x)
	for(Size y = 0; y <= seg_.y; ++y)
	{
		float dx = float(x) / float(seg_.x);
		float dy = float(y) / float(seg_.y);
		Size id = y*(seg_.x + 1) + x;
		auto rMat = rotateZXY3(vec3(90.0f - dy*180.0f, 180.0f - dx*360.0f, 0.0f));
		mesh.vertices[id].pos = rMat * vec3(0.0f, 0.0f, radius_);
		mesh.vertices[id].tex = vec2(dx, dy)*tex_;
		mesh.vertices[id].tan = rMat * vec3(-1.0f, 0.0f, 0.0f);
		mesh.vertices[id].bin = rMat * vec3(0.0f, 1.0f, 0.0f);
		mesh.vertices[id].nor = rMat * vec3(0.0f, 0.0f, 1.0f);
	}

	for(Size x = 0; x < seg_.x; ++x)
	for(Size y = 0; y < seg_.y; ++y)
	{
		Size id = 6 * (y*seg_.x + x);
		mesh.indices[id + 0] = (y + 0)*(seg_.x + 1) + (x + 0);
		mesh.indices[id + 1] = (y + 0)*(seg_.x + 1) + (x + 1);
		mesh.indices[id + 2] = (y + 1)*(seg_.x + 1) + (x + 0);
		mesh.indices[id + 3] = mesh.indices[id + 1];
		mesh.indices[id + 4] = (y + 1)*(seg_.x + 1) + (x + 1);
		mesh.indices[id + 5] = mesh.indices[id + 2];
	}

	return Move(mesh);
}
inline Mesh CreateTorus(const float radius_, const float width_, const glm::vec2& tex_, const glm::uvec2& seg_)
{
	if(seg_.x < 3 || seg_.y < 3)
	{
		throw Exception("Invalid segment count");
	}

	Mesh mesh;
	{
		mesh.vertices.resize((seg_.x + 1)*(seg_.y + 1));
		mesh.indices.resize(seg_.x*seg_.y * 6);
	}

	Size segX = seg_.x + 1;
	Size segY = seg_.y + 1;
	float step_tx = tex_.x / float(seg_.x);
	float step_ty = tex_.y / float(seg_.y);
	float step_ay = 360.0f / float(seg_.y);
	float step_ax = 360.0f / float(seg_.x);

	glm::vec3 tpos;
	glm::mat3 tmat;
	Size id;

	for(Size x = 0; x < Size(seg_.x) + 1; ++x)
	{
		for(Size y = 0; y < Size(seg_.y) + 1; ++y)
		{
			id = (seg_.y + 1)*x + y;
			tpos = xyz((
				rotateZXY(glm::vec3(0, step_ax*x, 0))
				*move(glm::vec3(0, 0, radius_))
				*rotateZXY(glm::vec3(step_ay*y, 0, 0))
				) * glm::vec4(glm::vec3(0, 0, width_), 1.0f));
			mesh.vertices[id].pos = tpos;
			mesh.vertices[id].tex = glm::vec2(step_tx*x, step_ty*y);
			tmat = rotateZXY3(glm::vec3(step_ay*y, step_ax*x, 0));
			mesh.vertices[id].tan = tmat * glm::vec3(1, 0, 0);
			mesh.vertices[id].bin = tmat * glm::vec3(0, -1, 0);
			mesh.vertices[id].nor = tmat * glm::vec3(0, 0, 1);
		}
	}

	for(Size x = 0; x < Size(seg_.x); ++x)
	{
		for(Size y = 0; y < Size(seg_.y); ++y)
		{
			id = 6 * (seg_.y*x + y);
			mesh.indices[id + 0] = (seg_.y + 1)*(x + 0) + (y + 0);
			mesh.indices[id + 1] = (seg_.y + 1)*(x + 1) + (y + 0);
			mesh.indices[id + 2] = (seg_.y + 1)*(x + 0) + (y + 1);
			mesh.indices[id + 3] = mesh.indices[id + 1];
			mesh.indices[id + 4] = (seg_.y + 1)*(x + 1) + (y + 1);
			mesh.indices[id + 5] = mesh.indices[id + 2];
		}
	}

	return Move(mesh);
}
struct Vertex2
{
	glm::vec3 pos;
	glm::vec2 tex;
};
struct Mesh2
{
	Vector<Vertex2> vertices;
	Vector<uint32_t> indices;
	inline Mesh2() = default;
	inline Mesh2(Mesh2&& m): vertices(Move(m.vertices)), indices(Move(m.indices))
	{
	}
};
inline Mesh2 CreatePatch4PlainZ(const glm::vec2& size_, const glm::uvec2& seg_)
{
	Mesh2 mesh;
	{
		mesh.vertices.resize((seg_.x + 1) * (seg_.y + 1));
		mesh.indices.resize(seg_.x*seg_.y * 4);
	}

	auto seg = seg_ + glm::uvec2(1);
	auto h = size_ / 2.0f;

	for (Size x = 0; x < seg.x; ++x)
	{
		for (Size y = 0; y < seg.y; ++y)
		{
			Size i = x * seg.y + y;
			auto k = glm::uvec2(x, y);
			auto t = static_cast<glm::vec2>(k) / static_cast<glm::vec2>(seg_);
			auto j = t * 2.0f - 1.0f;

			mesh.vertices[i].pos = glm::vec3(j.x * h.x, 0.0f, j.y * h.y);
			mesh.vertices[i].tex = t;
		}
	}

	for (Size x = 0; x < seg_.x; ++x)
	{
		for (Size y = 0; y < seg_.y; ++y)
		{
			Size i = (x + y * seg_.x) * 4;

			mesh.indices[i + 0] = (x + 0) + (y + 0) * seg.x;
			mesh.indices[i + 1] = (x + 1) + (y + 0) * seg.x;
			mesh.indices[i + 2] = (x + 0) + (y + 1) * seg.x;
			mesh.indices[i + 3] = (x + 1) + (y + 1) * seg.x;
		}
	}

	return Move(mesh);
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
	auto loadCubemap = [](const String filename_)
	{
		auto handle = ilGenImage();
		ilBindImage(handle);

		if (ilLoadImage(filename_.c_str()))
		{
			auto ilWidth		= ilGetInteger(IL_IMAGE_WIDTH); 
			auto ilHeight		= ilGetInteger(IL_IMAGE_HEIGHT); 
			auto ilDepth		= ilGetInteger(IL_IMAGE_DEPTH); 
			auto ilFormat		= ilGetInteger(IL_IMAGE_FORMAT);  // GL_RGBA
			auto ilType			= ilGetInteger(IL_IMAGE_TYPE); 
			auto ilBytes		= ilGetInteger(IL_IMAGE_BYTES_PER_PIXEL); 
			auto ilBits			= ilGetInteger(IL_IMAGE_BITS_PER_PIXEL); 
			auto ilMipmaps		= ilGetInteger(IL_NUM_MIPMAPS); 

			Map<Size, Map<Size, Vector<uint8_t>>> data;

			for (Size faceIndex = 0; faceIndex < 6; ++faceIndex)
			{
				auto &face = data[faceIndex];

				for (Size mipmapIndex = 0; mipmapIndex < static_cast<Size>(ilMipmaps) + 1; ++mipmapIndex)
				{
					ilBindImage(handle);
					ilActiveFace(faceIndex);
					ilActiveMipmap(mipmapIndex);

					auto ilOriginMode	= ilGetInteger(IL_IMAGE_ORIGIN); 	// IL_ORIGIN_LOWER_LEFT, IL_ORIGIN_UPPER_LEFT

					// if (ilOriginMode != IL_ORIGIN_LOWER_LEFT)
					// {
					// 	iluFlipImage();
					// }

					auto &mipmap = face[mipmapIndex];

					auto ilWidth	= ilGetInteger(IL_IMAGE_WIDTH);
					auto ilHeight	= ilGetInteger(IL_IMAGE_HEIGHT);
					auto ilDepth	= ilGetInteger(IL_IMAGE_DEPTH);
					auto ilDataSize	= ilGetInteger(IL_IMAGE_SIZE_OF_DATA);
					auto ilData		= ilGetData();

					mipmap.resize(ilDataSize);
					std::memcpy(mipmap.data(), ilData, mipmap.size());
				}
			}

			return Move(data);
		}
		else
		{
			throw Exception();
		}
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
					Size si0 = (x * 2 + 0) + (y * 2 + 0) * sourceSize; // index in source mip-level
					Size si1 = (x * 2 + 1) + (y * 2 + 0) * sourceSize; // index in source mip-level
					Size si2 = (x * 2 + 0) + (y * 2 + 1) * sourceSize; // index in source mip-level
					Size si3 = (x * 2 + 1) + (y * 2 + 1) * sourceSize; // index in source mip-level

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
		windowSize.width = 1280;
		windowSize.height = 620;
	}
	float windowAspect = static_cast<float>(windowSize.width) / static_cast<float>(windowSize.height);

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
		
		// addLayer("VK_LAYER_LUNARG_api_dump");
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
			PhysicalDeviceFeatures::Feature::GeometryShader,
			PhysicalDeviceFeatures::Feature::TessellationShader,
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
		vk_surfaceCapabilities.currentExtent,
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
	auto vk_depthFormat = VkFormat::VK_FORMAT_D32_SFLOAT;
	auto vk_swapchainDepthImage = CreateImage(vk_device, ImageCreateInfo(0,
		VkImageType::VK_IMAGE_TYPE_2D, vk_depthFormat, Extent3D(vk_surfaceCapabilities.currentExtent, 1), 1, 1, VkSampleCountFlagBits::VK_SAMPLE_COUNT_1_BIT,
		VkImageTiling::VK_IMAGE_TILING_OPTIMAL, VkImageUsageFlagBits::VK_IMAGE_USAGE_DEPTH_STENCIL_ATTACHMENT_BIT, VkImageLayout::VK_IMAGE_LAYOUT_UNDEFINED
	));
	auto vk_swapchainDepthImageDeviceMemory = [&]()
	{
		auto vk_memoryRequirements = GetImageMemoryRequirements(vk_device, vk_swapchainDepthImage);
		auto vk_deviceMemory = AllocateMemory(vk_device, MemoryAllocateInfo(
			vk_memoryRequirements.size,
			getDeviceMemoryIndex2(0, vk_memoryRequirements.memoryTypeBits)
		));

		BindImageMemory(vk_device, vk_swapchainDepthImage, vk_deviceMemory);

		return vk_deviceMemory;
	}();
	auto vk_swapchainDepthImageView = CreateImageView(vk_device, ImageViewCreateInfo(
		0, vk_swapchainDepthImage, VkImageViewType::VK_IMAGE_VIEW_TYPE_2D, vk_depthFormat,
		ComponentMapping(
			VkComponentSwizzle::VK_COMPONENT_SWIZZLE_IDENTITY,
			VkComponentSwizzle::VK_COMPONENT_SWIZZLE_IDENTITY,
			VkComponentSwizzle::VK_COMPONENT_SWIZZLE_IDENTITY,
			VkComponentSwizzle::VK_COMPONENT_SWIZZLE_IDENTITY
		),
		ImageSubresourceRange(VkImageAspectFlagBits::VK_IMAGE_ASPECT_DEPTH_BIT, 0, 1, 0, 1)
	));

	// Data
	// auto mesh = Move(CreateSphere(0.8f, glm::vec2(4.0f, 2.0f), glm::uvec2(64, 32)));
	auto mesh = Move(CreateTorus(0.6f, 0.2f, glm::vec2(8.0f, 2.0f), glm::uvec2(128, 32)));
	auto &vertices = mesh.vertices;
	Size vertexStride = sizeof(Vertex);
	Size verticesTotalSize = sizeof(Vertex) * vertices.size();
	auto &indices = mesh.indices;
	Size indicesTotalSize = sizeof(uint32_t) * indices.size();

	auto terrainSize = glm::vec2(1000.0f);
	auto meshTerrain = Move(CreatePatch4PlainZ(terrainSize, glm::uvec2(50)));
	auto &terrainVertices = meshTerrain.vertices;
	Size terrainVertexStride = sizeof(Vertex2);
	Size terrainVerticesTotalSize = sizeof(Vertex2)* terrainVertices.size();
	auto &terrainIndices = meshTerrain.indices;
	Size terrainIndicesTotalSize = sizeof(uint32_t)* terrainIndices.size();

	Vector<float> uniforms(16 * 4);
	Size uniformsTotalSize = sizeof(float)* uniforms.size();

	Vector<float> terrainUniforms(16 + 4);
	Size terrainUniformsTotalSize = sizeof(float)* terrainUniforms.size();

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
	auto vk_indexBuffer = CreateBuffer(vk_device, BufferCreateInfo(0, indicesTotalSize, VkBufferUsageFlagBits::VK_BUFFER_USAGE_INDEX_BUFFER_BIT, VkSharingMode::VK_SHARING_MODE_EXCLUSIVE));
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

	auto vk_terrainVertexBuffer = CreateBuffer(vk_device, BufferCreateInfo(0, terrainVerticesTotalSize, VkBufferUsageFlagBits::VK_BUFFER_USAGE_VERTEX_BUFFER_BIT, VkSharingMode::VK_SHARING_MODE_EXCLUSIVE));
	auto vk_terrainVertexBufferDeviceMemory = [&]()
	{
		auto vk_memoryRequirements = GetBufferMemoryRequirements(vk_device, vk_terrainVertexBuffer);
		auto vk_deviceMemory = AllocateMemory(vk_device, MemoryAllocateInfo(
			vk_memoryRequirements.size,
			getDeviceMemoryIndex(VkMemoryPropertyFlagBits::VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT)
			));

		auto data = MapMemory(vk_device, vk_deviceMemory, 0, VK_WHOLE_SIZE, 0);

		std::memcpy(data, terrainVertices.data(), terrainVerticesTotalSize);

		UnmapMemory(vk_device, vk_deviceMemory);

		BindBufferMemory(vk_device, vk_terrainVertexBuffer, vk_deviceMemory);

		return vk_deviceMemory;
	}();
	auto vk_terrainIndexBuffer = CreateBuffer(vk_device, BufferCreateInfo(0, terrainIndicesTotalSize, VkBufferUsageFlagBits::VK_BUFFER_USAGE_INDEX_BUFFER_BIT, VkSharingMode::VK_SHARING_MODE_EXCLUSIVE));
	auto vk_terrainIndexBufferDeviceMemory = [&]()
	{
		auto vk_memoryRequirements = GetBufferMemoryRequirements(vk_device, vk_terrainIndexBuffer);
		auto vk_deviceMemory = AllocateMemory(vk_device, MemoryAllocateInfo(
			vk_memoryRequirements.size,
			getDeviceMemoryIndex(VkMemoryPropertyFlagBits::VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT)
		));

		auto data = MapMemory(vk_device, vk_deviceMemory, 0, VK_WHOLE_SIZE, 0);

		std::memcpy(data, terrainIndices.data(), terrainIndicesTotalSize);

		UnmapMemory(vk_device, vk_deviceMemory);

		BindBufferMemory(vk_device, vk_terrainIndexBuffer, vk_deviceMemory);

		return vk_deviceMemory;
	}();

	auto vk_terrainUniformBuffer = CreateBuffer(vk_device, BufferCreateInfo(0, terrainUniformsTotalSize, VkBufferUsageFlagBits::VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT, VkSharingMode::VK_SHARING_MODE_EXCLUSIVE));
	auto vk_terrainUniformBufferDeviceMemory = [&]()
	{
		auto vk_memoryRequirements = GetBufferMemoryRequirements(vk_device, vk_terrainUniformBuffer);
		auto vk_deviceMemory = AllocateMemory(vk_device, MemoryAllocateInfo(
			vk_memoryRequirements.size,
			getDeviceMemoryIndex(VkMemoryPropertyFlagBits::VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT)
		));
	
		auto data = MapMemory(vk_device, vk_deviceMemory, 0, VK_WHOLE_SIZE, 0);
	
		std::memcpy(data, terrainUniforms.data(), terrainUniformsTotalSize);
	
		UnmapMemory(vk_device, vk_deviceMemory);
	
		BindBufferMemory(vk_device, vk_terrainUniformBuffer, vk_deviceMemory);
	
		return vk_deviceMemory;
	}();

	// Shaders
	auto vk_objectVertexShaderModule = CreateShaderModule(vk_device, ShaderModuleCreateInfo(Move(loadShader("../../../../../Media/Shaders/Functions_Cube/model.spir-v.vs"))));
	auto vk_objectFragmentShaderModule = CreateShaderModule(vk_device, ShaderModuleCreateInfo(Move(loadShader("../../../../../Media/Shaders/Functions_Cube/model.spir-v.fs"))));

	auto vk_backgroundVertexShaderModule = CreateShaderModule(vk_device, ShaderModuleCreateInfo(Move(loadShader("../../../../../Media/Shaders/Functions_Cube/background.spir-v.vs"))));
	auto vk_backgroundGeometryShaderModule = CreateShaderModule(vk_device, ShaderModuleCreateInfo(Move(loadShader("../../../../../Media/Shaders/Functions_Cube/background.spir-v.gs"))));
	auto vk_backgroundFragmentShaderModule = CreateShaderModule(vk_device, ShaderModuleCreateInfo(Move(loadShader("../../../../../Media/Shaders/Functions_Cube/background.spir-v.fs"))));

	auto vk_terrainVertexShaderModule = CreateShaderModule(vk_device, ShaderModuleCreateInfo(Move(loadShader("../../../../../Media/Shaders/Functions_Terrain/terrain.spir-v.vertex-shader"))));
	auto vk_terrainTessellationControlShaderModule = CreateShaderModule(vk_device, ShaderModuleCreateInfo(Move(loadShader("../../../../../Media/Shaders/Functions_Terrain/terrain.spir-v.tessellation-control-shader"))));
	auto vk_terrainTessellationEvaluationShaderModule = CreateShaderModule(vk_device, ShaderModuleCreateInfo(Move(loadShader("../../../../../Media/Shaders/Functions_Terrain/terrain.spir-v.tessellation-evaluation-shader"))));
	auto vk_terrainGeometryShaderModule = CreateShaderModule(vk_device, ShaderModuleCreateInfo(Move(loadShader("../../../../../Media/Shaders/Functions_Terrain/terrain.spir-v.geometry-shader"))));
	auto vk_terrainFragmentShaderModule = CreateShaderModule(vk_device, ShaderModuleCreateInfo(Move(loadShader("../../../../../Media/Shaders/Functions_Terrain/terrain.spir-v.fragment-shader"))));

	// Queue
	auto vk_queue = GetDeviceQueue(vk_device, 0, 0);

	// Command Pool
	auto vk_commandPool = CreateCommandPool(vk_device, CommandPoolCreateInfo(VkCommandPoolCreateFlagBits::VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT, 0));
	{
		ResetCommandPool(vk_device, vk_commandPool, VkCommandPoolResetFlagBits::VK_COMMAND_POOL_RESET_RELEASE_RESOURCES_BIT);
	}

	// mipmap builder
	auto buildMipmaps = [&](const VkImage& sourceImage, Map<Size, Vector<uint8_t>>& sourceData_, const Size& sourceSize_, const Size& components_)
	{
		auto powOf = glm::log2(static_cast<glm::float32_t>(sourceSize_));
		auto levels = (powOf - glm::floor(powOf)) == 0
			? (static_cast<Size>(powOf)+1)
			: throw Exception();

		for (Size level = 0; level < levels; ++level)
		{
			Size powOf = levels - 1 - level;
			Size size = static_cast<Size>(glm::pow(2, powOf));

			auto mipImage = CreateImage(vk_device, ImageCreateInfo(
				0, VkImageType::VK_IMAGE_TYPE_2D, VkFormat::VK_FORMAT_R8G8B8A8_UNORM, Extent3D(size, size, 1), 1, 1,
				VkSampleCountFlagBits::VK_SAMPLE_COUNT_1_BIT, VkImageTiling::VK_IMAGE_TILING_LINEAR,
				VkImageUsageFlagBits::VK_IMAGE_USAGE_SAMPLED_BIT | VkImageUsageFlagBits::VK_IMAGE_USAGE_TRANSFER_SRC_BIT, VkImageLayout::VK_IMAGE_LAYOUT_UNDEFINED
			));
			auto mipImageDeviceMemory = [&]()
			{
				auto vk_memoryRequirements = GetImageMemoryRequirements(vk_device, mipImage);
				auto vk_deviceMemory = AllocateMemory(vk_device, MemoryAllocateInfo(
					vk_memoryRequirements.size,
					getDeviceMemoryIndex2(VkMemoryPropertyFlagBits::VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT, vk_memoryRequirements.memoryTypeBits)
				));

				auto vk_imageSubresourceLayout = GetImageSubresourceLayout(vk_device, mipImage, ImageSubresource(VkImageAspectFlagBits::VK_IMAGE_ASPECT_COLOR_BIT, 0, 0));

				auto data = static_cast<uint8_t*>(MapMemory(vk_device, vk_deviceMemory, 0, VK_WHOLE_SIZE, 0));
				auto &sourceData = sourceData_[level];

				for (Size x = 0; x < size; ++x)
				{
					for (Size y = 0; y < size; ++y)
					{
						Size i = static_cast<Size>(vk_imageSubresourceLayout.offset) + y * static_cast<Size>(vk_imageSubresourceLayout.rowPitch) + x * 4;
						Size j = (x + y * size) * components_;

						if (components_ == 3)
						{
							data[i + 0] = sourceData[j + 0];
							data[i + 1] = sourceData[j + 1];
							data[i + 2] = sourceData[j + 2];
							data[i + 3] = 255;
						}
						else if (components_ == 4)
						{
							data[i + 0] = sourceData[j + 0];
							data[i + 1] = sourceData[j + 1];
							data[i + 2] = sourceData[j + 2];
							data[i + 3] = sourceData[j + 3];
						}
						else
						{
							data[i + 0] = sourceData[j];
							data[i + 1] = sourceData[j];
							data[i + 2] = sourceData[j];
							data[i + 3] = 255;
						}
					}
				}

				UnmapMemory(vk_device, vk_deviceMemory);

				BindImageMemory(vk_device, mipImage, vk_deviceMemory);

				return vk_deviceMemory;
			}();

			auto mipCommandBuffers = Move(AllocateCommandBuffers(vk_device, CommandBufferAllocateInfo(vk_commandPool, VkCommandBufferLevel::VK_COMMAND_BUFFER_LEVEL_PRIMARY, 1)));
			auto &mipCommandBuffer = mipCommandBuffers[0];

			ResetCommandBuffer(mipCommandBuffer, VkCommandBufferResetFlagBits::VK_COMMAND_BUFFER_RESET_RELEASE_RESOURCES_BIT);
			BeginCommandBuffer(mipCommandBuffer, CommandBufferBeginInfo(0));
			
			CmdPipelineBarrier(mipCommandBuffer, VkPipelineStageFlagBits::VK_PIPELINE_STAGE_TRANSFER_BIT, VkPipelineStageFlagBits::VK_PIPELINE_STAGE_TRANSFER_BIT, 0, {}, {}, {
				ImageMemoryBarrier(
					0,
					VkAccessFlagBits::VK_ACCESS_TRANSFER_READ_BIT,
					VkImageLayout::VK_IMAGE_LAYOUT_UNDEFINED,
					VkImageLayout::VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL,
					0,
					0,
					mipImage,
					ImageSubresourceRange(VkImageAspectFlagBits::VK_IMAGE_ASPECT_COLOR_BIT, 0, 1, 0, 1)
				),
			});
			CmdPipelineBarrier(mipCommandBuffer, VkPipelineStageFlagBits::VK_PIPELINE_STAGE_TRANSFER_BIT, VkPipelineStageFlagBits::VK_PIPELINE_STAGE_TRANSFER_BIT, 0, {}, {}, {
				ImageMemoryBarrier(
					0,
					VkAccessFlagBits::VK_ACCESS_TRANSFER_WRITE_BIT,
					VkImageLayout::VK_IMAGE_LAYOUT_UNDEFINED,
					VkImageLayout::VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL,
					0,
					0,
					sourceImage,
					ImageSubresourceRange(VkImageAspectFlagBits::VK_IMAGE_ASPECT_COLOR_BIT, level, 1, 0, 1)
				),
			});

			CmdCopyImage(mipCommandBuffer,
				mipImage, VkImageLayout::VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL,
				sourceImage, VkImageLayout::VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL,
				{
					ImageCopy(
						ImageSubresourceLayers(VkImageAspectFlagBits::VK_IMAGE_ASPECT_COLOR_BIT, 0, 0, 1),
						Offset3D(0, 0, 0),
						ImageSubresourceLayers(VkImageAspectFlagBits::VK_IMAGE_ASPECT_COLOR_BIT, level, 0, 1),
						Offset3D(0, 0, 0),
						Extent3D(size, size, 1)
					),
				}
			);

			EndCommandBuffer(mipCommandBuffer);

			QueueWaitIdle(vk_queue);
			QueueSubmit(vk_queue, {SubmitInfo({mipCommandBuffer})});
			QueueWaitIdle(vk_queue);

			FreeCommandBuffers(vk_device, vk_commandPool, mipCommandBuffers);

			FreeMemory(vk_device, mipImageDeviceMemory);
			DestroyImage(vk_device, mipImage);
		}

		auto finalCommandBuffers = Move(AllocateCommandBuffers(vk_device, CommandBufferAllocateInfo(vk_commandPool, VkCommandBufferLevel::VK_COMMAND_BUFFER_LEVEL_PRIMARY, 1)));
		auto &finalCommandBuffer = finalCommandBuffers[0];

		ResetCommandBuffer(finalCommandBuffer, VkCommandBufferResetFlagBits::VK_COMMAND_BUFFER_RESET_RELEASE_RESOURCES_BIT);
		BeginCommandBuffer(finalCommandBuffer, CommandBufferBeginInfo(0));
			
		CmdPipelineBarrier(finalCommandBuffer, VkPipelineStageFlagBits::VK_PIPELINE_STAGE_TRANSFER_BIT, VkPipelineStageFlagBits::VK_PIPELINE_STAGE_ALL_GRAPHICS_BIT, 0, {}, {}, {
			ImageMemoryBarrier(
				0,
				VkAccessFlagBits::VK_ACCESS_SHADER_READ_BIT,
				VkImageLayout::VK_IMAGE_LAYOUT_UNDEFINED,
				VkImageLayout::VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL,
				0,
				0,
				sourceImage,
				ImageSubresourceRange(VkImageAspectFlagBits::VK_IMAGE_ASPECT_COLOR_BIT, 0, levels, 0, 1)
			),
		});

		EndCommandBuffer(finalCommandBuffer);

		QueueWaitIdle(vk_queue);
		QueueSubmit(vk_queue, {SubmitInfo({finalCommandBuffer})});
		QueueWaitIdle(vk_queue);

		FreeCommandBuffers(vk_device, vk_commandPool, finalCommandBuffers);
	};

	// TEST
	// GetPhysicalDeviceFormatProperties(vk_physicalDevice, VkFormat::VK_FORMAT_R16_UNORM);
	// GetPhysicalDeviceFormatProperties(vk_physicalDevice, VkFormat::VK_FORMAT_R8G8B8A8_UNORM);
	// GetPhysicalDeviceImageFormatProperties(vk_physicalDevice, VkFormat::VK_FORMAT_R8G8B8A8_UNORM, VkImageType::VK_IMAGE_TYPE_2D, VkImageTiling::VK_IMAGE_TILING_LINEAR, VkImageUsageFlagBits::VK_IMAGE_USAGE_SAMPLED_BIT | VkImageUsageFlagBits::VK_IMAGE_USAGE_TRANSFER_SRC_BIT, 0);
	// GetPhysicalDeviceImageFormatProperties(vk_physicalDevice, VkFormat::VK_FORMAT_R8G8B8A8_UNORM, VkImageType::VK_IMAGE_TYPE_2D, VkImageTiling::VK_IMAGE_TILING_OPTIMAL, VkImageUsageFlagBits::VK_IMAGE_USAGE_SAMPLED_BIT | VkImageUsageFlagBits::VK_IMAGE_USAGE_TRANSFER_DST_BIT, 0);

	// Images
	auto imageAlbedoData = Move(loadImage("../../../../../Media/Images/Functions_Cube/Albedo.png"));
	auto imageAlbedoMipmaps = generateMipmaps(imageAlbedoData, 2048, 3);
	auto vk_imageAlbedo = CreateImage(vk_device, ImageCreateInfo(0,
		VkImageType::VK_IMAGE_TYPE_2D, VkFormat::VK_FORMAT_R8G8B8A8_UNORM, Extent3D(2048, 2048, 1), 12, 1, VkSampleCountFlagBits::VK_SAMPLE_COUNT_1_BIT,
		VkImageTiling::VK_IMAGE_TILING_OPTIMAL, VkImageUsageFlagBits::VK_IMAGE_USAGE_SAMPLED_BIT | VkImageUsageFlagBits::VK_IMAGE_USAGE_TRANSFER_DST_BIT, VkImageLayout::VK_IMAGE_LAYOUT_UNDEFINED
	));
	auto vk_imageAlbedoDeviceMemory = [&]()
	{
		auto vk_memoryRequirements = GetImageMemoryRequirements(vk_device, vk_imageAlbedo);
		auto vk_deviceMemory = AllocateMemory(vk_device, MemoryAllocateInfo(
			vk_memoryRequirements.size,
			getDeviceMemoryIndex2(0, vk_memoryRequirements.memoryTypeBits)
		));

		BindImageMemory(vk_device, vk_imageAlbedo, vk_deviceMemory);

		buildMipmaps(vk_imageAlbedo, imageAlbedoMipmaps, 2048, 3);

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
		ImageSubresourceRange(VkImageAspectFlagBits::VK_IMAGE_ASPECT_COLOR_BIT, 0, 12, 0, 1)
	));
	auto imageNormalsData = Move(loadImage("../../../../../Media/Images/Functions_Cube/Normals.png"));
	auto imageNormalsMipmaps = generateMipmaps(imageNormalsData, 2048, 4);
	auto vk_imageNormals = CreateImage(vk_device, ImageCreateInfo(0,
		VkImageType::VK_IMAGE_TYPE_2D, VkFormat::VK_FORMAT_R8G8B8A8_UNORM, Extent3D(2048, 2048, 1), 12, 1, VkSampleCountFlagBits::VK_SAMPLE_COUNT_1_BIT,
		VkImageTiling::VK_IMAGE_TILING_OPTIMAL, VkImageUsageFlagBits::VK_IMAGE_USAGE_SAMPLED_BIT | VkImageUsageFlagBits::VK_IMAGE_USAGE_TRANSFER_DST_BIT, VkImageLayout::VK_IMAGE_LAYOUT_UNDEFINED
	));
	auto vk_imageNormalsDeviceMemory = [&]()
	{
		auto vk_memoryRequirements = GetImageMemoryRequirements(vk_device, vk_imageNormals);
		auto vk_deviceMemory = AllocateMemory(vk_device, MemoryAllocateInfo(
			vk_memoryRequirements.size,
			getDeviceMemoryIndex2(0, vk_memoryRequirements.memoryTypeBits)
		));

		BindImageMemory(vk_device, vk_imageNormals, vk_deviceMemory);

		buildMipmaps(vk_imageNormals, imageNormalsMipmaps, 2048, 4);

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
		ImageSubresourceRange(VkImageAspectFlagBits::VK_IMAGE_ASPECT_COLOR_BIT, 0, 12, 0, 1)
	));
	auto imageRoughnessData = Move(loadImage("../../../../../Media/Images/Functions_Cube/Roughness.png"));
	auto imageRoughnessMipmaps = generateMipmaps(imageRoughnessData, 2048, 1);
	auto vk_imageRoughness = CreateImage(vk_device, ImageCreateInfo(0,
		VkImageType::VK_IMAGE_TYPE_2D, VkFormat::VK_FORMAT_R8G8B8A8_UNORM, Extent3D(2048, 2048, 1), 12, 1, VkSampleCountFlagBits::VK_SAMPLE_COUNT_1_BIT,
		VkImageTiling::VK_IMAGE_TILING_OPTIMAL, VkImageUsageFlagBits::VK_IMAGE_USAGE_SAMPLED_BIT | VkImageUsageFlagBits::VK_IMAGE_USAGE_TRANSFER_DST_BIT, VkImageLayout::VK_IMAGE_LAYOUT_UNDEFINED
	));
	auto vk_imageRoughnessDeviceMemory = [&]()
	{
		auto vk_memoryRequirements = GetImageMemoryRequirements(vk_device, vk_imageRoughness);
		auto vk_deviceMemory = AllocateMemory(vk_device, MemoryAllocateInfo(
			vk_memoryRequirements.size,
			getDeviceMemoryIndex2(0, vk_memoryRequirements.memoryTypeBits)
		));

		BindImageMemory(vk_device, vk_imageRoughness, vk_deviceMemory);

		buildMipmaps(vk_imageRoughness, imageRoughnessMipmaps, 2048, 1);

		return vk_deviceMemory;
	}();
	auto vk_imageViewRoughness = CreateImageView(vk_device, ImageViewCreateInfo(
		0, vk_imageRoughness, VkImageViewType::VK_IMAGE_VIEW_TYPE_2D, VkFormat::VK_FORMAT_R8G8B8A8_UNORM,
		ComponentMapping(
			VkComponentSwizzle::VK_COMPONENT_SWIZZLE_R,
			VkComponentSwizzle::VK_COMPONENT_SWIZZLE_R,
			VkComponentSwizzle::VK_COMPONENT_SWIZZLE_R,
			VkComponentSwizzle::VK_COMPONENT_SWIZZLE_R
		),
		ImageSubresourceRange(VkImageAspectFlagBits::VK_IMAGE_ASPECT_COLOR_BIT, 0, 12, 0, 1)
	));
	auto imageMetalnessData = Move(loadImage("../../../../../Media/Images/Functions_Cube/Metalness.png"));
	auto imageMetalnessMipmaps = generateMipmaps(imageMetalnessData, 2048, 1);
	auto vk_imageMetalness = CreateImage(vk_device, ImageCreateInfo(0,
		VkImageType::VK_IMAGE_TYPE_2D, VkFormat::VK_FORMAT_R8G8B8A8_UNORM, Extent3D(2048, 2048, 1), 12, 1, VkSampleCountFlagBits::VK_SAMPLE_COUNT_1_BIT,
		VkImageTiling::VK_IMAGE_TILING_OPTIMAL, VkImageUsageFlagBits::VK_IMAGE_USAGE_SAMPLED_BIT | VkImageUsageFlagBits::VK_IMAGE_USAGE_TRANSFER_DST_BIT, VkImageLayout::VK_IMAGE_LAYOUT_UNDEFINED
	));
	auto vk_imageMetalnessDeviceMemory = [&]()
	{
		auto vk_memoryRequirements = GetImageMemoryRequirements(vk_device, vk_imageMetalness);
		auto vk_deviceMemory = AllocateMemory(vk_device, MemoryAllocateInfo(
			vk_memoryRequirements.size,
			getDeviceMemoryIndex2(0, vk_memoryRequirements.memoryTypeBits)
		));

		BindImageMemory(vk_device, vk_imageMetalness, vk_deviceMemory);

		buildMipmaps(vk_imageMetalness, imageMetalnessMipmaps, 2048, 1);

		return vk_deviceMemory;
	}();
	auto vk_imageViewMetalness = CreateImageView(vk_device, ImageViewCreateInfo(
		0, vk_imageMetalness, VkImageViewType::VK_IMAGE_VIEW_TYPE_2D, VkFormat::VK_FORMAT_R8G8B8A8_UNORM,
		ComponentMapping(
			VkComponentSwizzle::VK_COMPONENT_SWIZZLE_R,
			VkComponentSwizzle::VK_COMPONENT_SWIZZLE_R,
			VkComponentSwizzle::VK_COMPONENT_SWIZZLE_R,
			VkComponentSwizzle::VK_COMPONENT_SWIZZLE_R
		),
		ImageSubresourceRange(VkImageAspectFlagBits::VK_IMAGE_ASPECT_COLOR_BIT, 0, 12, 0, 1)
	));
	auto imageOcclusionData = Move(loadImage("../../../../../Media/Images/Functions_Cube/Occlusion.png"));
	auto imageOcclusionMipmaps = generateMipmaps(imageOcclusionData, 2048, 1);
	auto vk_imageOcclusion = CreateImage(vk_device, ImageCreateInfo(0,
		VkImageType::VK_IMAGE_TYPE_2D, VkFormat::VK_FORMAT_R8G8B8A8_UNORM, Extent3D(2048, 2048, 1), 12, 1, VkSampleCountFlagBits::VK_SAMPLE_COUNT_1_BIT,
		VkImageTiling::VK_IMAGE_TILING_OPTIMAL, VkImageUsageFlagBits::VK_IMAGE_USAGE_SAMPLED_BIT | VkImageUsageFlagBits::VK_IMAGE_USAGE_TRANSFER_DST_BIT, VkImageLayout::VK_IMAGE_LAYOUT_UNDEFINED
	));
	auto vk_imageOcclusionDeviceMemory = [&]()
	{
		auto vk_memoryRequirements = GetImageMemoryRequirements(vk_device, vk_imageOcclusion);
		auto vk_deviceMemory = AllocateMemory(vk_device, MemoryAllocateInfo(
			vk_memoryRequirements.size,
			getDeviceMemoryIndex2(0, vk_memoryRequirements.memoryTypeBits)
		));

		BindImageMemory(vk_device, vk_imageOcclusion, vk_deviceMemory);

		buildMipmaps(vk_imageOcclusion, imageOcclusionMipmaps, 2048, 1);

		return vk_deviceMemory;
	}();
	auto vk_imageViewOcclusion = CreateImageView(vk_device, ImageViewCreateInfo(
		0, vk_imageOcclusion, VkImageViewType::VK_IMAGE_VIEW_TYPE_2D, VkFormat::VK_FORMAT_R8G8B8A8_UNORM,
		ComponentMapping(
			VkComponentSwizzle::VK_COMPONENT_SWIZZLE_R,
			VkComponentSwizzle::VK_COMPONENT_SWIZZLE_R,
			VkComponentSwizzle::VK_COMPONENT_SWIZZLE_R,
			VkComponentSwizzle::VK_COMPONENT_SWIZZLE_R
		),
		ImageSubresourceRange(VkImageAspectFlagBits::VK_IMAGE_ASPECT_COLOR_BIT, 0, 12, 0, 1)
	));

	auto imageEnvironmentData = Move(loadCubemap("../../../../../Media/Images/Functions_Cube/Environment.dds"));
	auto vk_imageEnvironment = CreateImage(vk_device, ImageCreateInfo(VkImageCreateFlagBits::VK_IMAGE_CREATE_CUBE_COMPATIBLE_BIT,
		VkImageType::VK_IMAGE_TYPE_2D, VkFormat::VK_FORMAT_R8G8B8A8_UNORM, Extent3D(1024, 1024, 1), 11, 6, VkSampleCountFlagBits::VK_SAMPLE_COUNT_1_BIT,
		VkImageTiling::VK_IMAGE_TILING_OPTIMAL, VkImageUsageFlagBits::VK_IMAGE_USAGE_SAMPLED_BIT | VkImageUsageFlagBits::VK_IMAGE_USAGE_TRANSFER_DST_BIT, VkImageLayout::VK_IMAGE_LAYOUT_UNDEFINED
	));
	auto vk_imageEnvironmentDeviceMemory = [&]()
	{
		auto vk_memoryRequirements = GetImageMemoryRequirements(vk_device, vk_imageEnvironment);
		auto vk_deviceMemory = AllocateMemory(vk_device, MemoryAllocateInfo(
			vk_memoryRequirements.size,
			getDeviceMemoryIndex2(0, vk_memoryRequirements.memoryTypeBits)
		));

		BindImageMemory(vk_device, vk_imageEnvironment, vk_deviceMemory);

		Size mipmapLevels = 11;

		for (Size faceIndex = 0; faceIndex < 6; ++faceIndex)
		{
			for (Size mipmapIndex = 0; mipmapIndex < mipmapLevels; ++mipmapIndex)
			{
				Size powOf = mipmapLevels - 1 - mipmapIndex;
				Size size = static_cast<Size>(glm::pow(2, powOf));

				auto mipImage = CreateImage(vk_device, ImageCreateInfo(0,
					VkImageType::VK_IMAGE_TYPE_2D, VkFormat::VK_FORMAT_R8G8B8A8_UNORM, Extent3D(size, size, 1), 1, 1,
					VkSampleCountFlagBits::VK_SAMPLE_COUNT_1_BIT, VkImageTiling::VK_IMAGE_TILING_LINEAR,
					VkImageUsageFlagBits::VK_IMAGE_USAGE_SAMPLED_BIT | VkImageUsageFlagBits::VK_IMAGE_USAGE_TRANSFER_SRC_BIT, VkImageLayout::VK_IMAGE_LAYOUT_UNDEFINED
				));
				auto mipImageDeviceMemory = [&]()
				{
					auto vk_memoryRequirements = GetImageMemoryRequirements(vk_device, mipImage);
					auto vk_deviceMemory = AllocateMemory(vk_device, MemoryAllocateInfo(
						vk_memoryRequirements.size,
						getDeviceMemoryIndex2(VkMemoryPropertyFlagBits::VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT, vk_memoryRequirements.memoryTypeBits)
					));

					auto vk_imageSubresourceLayout = GetImageSubresourceLayout(vk_device, mipImage, ImageSubresource(VkImageAspectFlagBits::VK_IMAGE_ASPECT_COLOR_BIT, 0, 0));

					auto data = static_cast<uint8_t*>(MapMemory(vk_device, vk_deviceMemory, 0, VK_WHOLE_SIZE, 0));
					auto &sourceData = imageEnvironmentData[faceIndex][mipmapIndex];

					for (Size x = 0; x < size; ++x)
					{
						for (Size y = 0; y < size; ++y)
						{
							Size i = static_cast<Size>(vk_imageSubresourceLayout.offset) + y * static_cast<Size>(vk_imageSubresourceLayout.rowPitch) + x * 4;
							Size j = (x + y * size) * 4;

							data[i + 0] = sourceData[j + 0];
							data[i + 1] = sourceData[j + 1];
							data[i + 2] = sourceData[j + 2];
							data[i + 3] = sourceData[j + 3];
						}
					}

					UnmapMemory(vk_device, vk_deviceMemory);

					BindImageMemory(vk_device, mipImage, vk_deviceMemory);

					return vk_deviceMemory;
				}();

				auto mipCommandBuffers = Move(AllocateCommandBuffers(vk_device, CommandBufferAllocateInfo(vk_commandPool, VkCommandBufferLevel::VK_COMMAND_BUFFER_LEVEL_PRIMARY, 1)));
				auto &mipCommandBuffer = mipCommandBuffers[0];

				ResetCommandBuffer(mipCommandBuffer, VkCommandBufferResetFlagBits::VK_COMMAND_BUFFER_RESET_RELEASE_RESOURCES_BIT);
				BeginCommandBuffer(mipCommandBuffer, CommandBufferBeginInfo(0));
			
				CmdPipelineBarrier(mipCommandBuffer, VkPipelineStageFlagBits::VK_PIPELINE_STAGE_TRANSFER_BIT, VkPipelineStageFlagBits::VK_PIPELINE_STAGE_TRANSFER_BIT, 0, {}, {}, {
					ImageMemoryBarrier(
						0,
						VkAccessFlagBits::VK_ACCESS_TRANSFER_READ_BIT,
						VkImageLayout::VK_IMAGE_LAYOUT_UNDEFINED,
						VkImageLayout::VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL,
						0,
						0,
						mipImage,
						ImageSubresourceRange(VkImageAspectFlagBits::VK_IMAGE_ASPECT_COLOR_BIT, 0, 1, 0, 1)
					),
				});
				CmdPipelineBarrier(mipCommandBuffer, VkPipelineStageFlagBits::VK_PIPELINE_STAGE_TRANSFER_BIT, VkPipelineStageFlagBits::VK_PIPELINE_STAGE_TRANSFER_BIT, 0, {}, {}, {
					ImageMemoryBarrier(
						0,
						VkAccessFlagBits::VK_ACCESS_TRANSFER_WRITE_BIT,
						VkImageLayout::VK_IMAGE_LAYOUT_UNDEFINED,
						VkImageLayout::VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL,
						0,
						0,
						vk_imageEnvironment,
						ImageSubresourceRange(VkImageAspectFlagBits::VK_IMAGE_ASPECT_COLOR_BIT, mipmapIndex, 1, faceIndex, 1)
					),
				});

				CmdCopyImage(mipCommandBuffer,
					mipImage, VkImageLayout::VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL,
					vk_imageEnvironment, VkImageLayout::VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL,
					{
						ImageCopy(
							ImageSubresourceLayers(VkImageAspectFlagBits::VK_IMAGE_ASPECT_COLOR_BIT, 0, 0, 1),
							Offset3D(0, 0, 0),
							ImageSubresourceLayers(VkImageAspectFlagBits::VK_IMAGE_ASPECT_COLOR_BIT, mipmapIndex, faceIndex, 1),
							Offset3D(0, 0, 0),
							Extent3D(size, size, 1)
						),
					}
				);

				EndCommandBuffer(mipCommandBuffer);

				QueueWaitIdle(vk_queue);
				QueueSubmit(vk_queue, {SubmitInfo({mipCommandBuffer})});
				QueueWaitIdle(vk_queue);

				FreeCommandBuffers(vk_device, vk_commandPool, mipCommandBuffers);

				FreeMemory(vk_device, mipImageDeviceMemory);
				DestroyImage(vk_device, mipImage);
			}
		}

		auto finalCommandBuffers = Move(AllocateCommandBuffers(vk_device, CommandBufferAllocateInfo(vk_commandPool, VkCommandBufferLevel::VK_COMMAND_BUFFER_LEVEL_PRIMARY, 1)));
		auto &finalCommandBuffer = finalCommandBuffers[0];

		ResetCommandBuffer(finalCommandBuffer, VkCommandBufferResetFlagBits::VK_COMMAND_BUFFER_RESET_RELEASE_RESOURCES_BIT);
		BeginCommandBuffer(finalCommandBuffer, CommandBufferBeginInfo(0));
			
		CmdPipelineBarrier(finalCommandBuffer, VkPipelineStageFlagBits::VK_PIPELINE_STAGE_TRANSFER_BIT, VkPipelineStageFlagBits::VK_PIPELINE_STAGE_ALL_GRAPHICS_BIT, 0, {}, {}, {
			ImageMemoryBarrier(
				0,
				VkAccessFlagBits::VK_ACCESS_SHADER_READ_BIT,
				VkImageLayout::VK_IMAGE_LAYOUT_UNDEFINED,
				VkImageLayout::VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL,
				0,
				0,
				vk_imageEnvironment,
				ImageSubresourceRange(VkImageAspectFlagBits::VK_IMAGE_ASPECT_COLOR_BIT, 0, mipmapLevels, 0, 6)
			),
		});

		EndCommandBuffer(finalCommandBuffer);

		QueueWaitIdle(vk_queue);
		QueueSubmit(vk_queue, {SubmitInfo({finalCommandBuffer})});
		QueueWaitIdle(vk_queue);

		FreeCommandBuffers(vk_device, vk_commandPool, finalCommandBuffers);

		return vk_deviceMemory;
	}();
	auto vk_imageViewEnvironment = CreateImageView(vk_device, ImageViewCreateInfo(
		0, vk_imageEnvironment, VkImageViewType::VK_IMAGE_VIEW_TYPE_CUBE, VkFormat::VK_FORMAT_R8G8B8A8_UNORM,
		ComponentMapping(
			VkComponentSwizzle::VK_COMPONENT_SWIZZLE_R,
			VkComponentSwizzle::VK_COMPONENT_SWIZZLE_G,
			VkComponentSwizzle::VK_COMPONENT_SWIZZLE_B,
			VkComponentSwizzle::VK_COMPONENT_SWIZZLE_A
		),
		ImageSubresourceRange(VkImageAspectFlagBits::VK_IMAGE_ASPECT_COLOR_BIT, 0, 11, 0, 6)
	));

	auto imageTerrainSize = glm::uvec2(4096);
	auto imageTerrainHeightData = Move(loadImage("../../../../../Media/Images/Functions_Terrain/Height.png"));
	auto vk_imageTerrainHeight = CreateImage(vk_device, ImageCreateInfo(0,
		VkImageType::VK_IMAGE_TYPE_2D, VkFormat::VK_FORMAT_R16_UNORM, Extent3D(4096, 4096, 1), 1, 1, VkSampleCountFlagBits::VK_SAMPLE_COUNT_1_BIT,
		VkImageTiling::VK_IMAGE_TILING_LINEAR, VkImageUsageFlagBits::VK_IMAGE_USAGE_SAMPLED_BIT | VkImageUsageFlagBits::VK_IMAGE_USAGE_TRANSFER_DST_BIT, VkImageLayout::VK_IMAGE_LAYOUT_UNDEFINED
	));
	auto vk_imageTerrainHeightDeviceMemory = [&]()
	{
		auto vk_memoryRequirements = GetImageMemoryRequirements(vk_device, vk_imageTerrainHeight);
		auto vk_deviceMemory = AllocateMemory(vk_device, MemoryAllocateInfo(
			vk_memoryRequirements.size,
			getDeviceMemoryIndex2(VkMemoryPropertyFlagBits::VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT, vk_memoryRequirements.memoryTypeBits)
		));

		BindImageMemory(vk_device, vk_imageTerrainHeight, vk_deviceMemory);

		auto data = static_cast<uint8_t*>(MapMemory(vk_device, vk_deviceMemory, 0, VK_WHOLE_SIZE, 0));
		
		std::memcpy(data, imageTerrainHeightData.data(), imageTerrainHeightData.size());

		UnmapMemory(vk_device, vk_deviceMemory);

		return vk_deviceMemory;
	}();
	auto vk_imageViewTerrainHeight = CreateImageView(vk_device, ImageViewCreateInfo(
		0, vk_imageTerrainHeight, VkImageViewType::VK_IMAGE_VIEW_TYPE_2D, VkFormat::VK_FORMAT_R16_UNORM,
		ComponentMapping(
			VkComponentSwizzle::VK_COMPONENT_SWIZZLE_R,
			VkComponentSwizzle::VK_COMPONENT_SWIZZLE_R,
			VkComponentSwizzle::VK_COMPONENT_SWIZZLE_R,
			VkComponentSwizzle::VK_COMPONENT_SWIZZLE_R
		),
		ImageSubresourceRange(VkImageAspectFlagBits::VK_IMAGE_ASPECT_COLOR_BIT, 0, 1, 0, 1)
	));

	// Sampler
	auto vk_sampler = CreateSampler(vk_device, SamplerCreateInfo(
		VkFilter::VK_FILTER_LINEAR, VkFilter::VK_FILTER_LINEAR, VkSamplerMipmapMode::VK_SAMPLER_MIPMAP_MODE_LINEAR,
		VkSamplerAddressMode::VK_SAMPLER_ADDRESS_MODE_REPEAT, VkSamplerAddressMode::VK_SAMPLER_ADDRESS_MODE_REPEAT, VkSamplerAddressMode::VK_SAMPLER_ADDRESS_MODE_REPEAT,
		0.0f, VK_FALSE, 1.0f, VK_FALSE, VkCompareOp::VK_COMPARE_OP_ALWAYS, 0.0f, 12.0f, VkBorderColor::VK_BORDER_COLOR_INT_OPAQUE_BLACK, VK_FALSE
	));
	auto vk_samplerTerrain = CreateSampler(vk_device, SamplerCreateInfo(
		VkFilter::VK_FILTER_LINEAR, VkFilter::VK_FILTER_LINEAR, VkSamplerMipmapMode::VK_SAMPLER_MIPMAP_MODE_NEAREST,
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
			AttachmentDescription(
				vk_depthFormat,
				VkSampleCountFlagBits::VK_SAMPLE_COUNT_1_BIT,
				VkAttachmentLoadOp::VK_ATTACHMENT_LOAD_OP_CLEAR,
				VkAttachmentStoreOp::VK_ATTACHMENT_STORE_OP_STORE,
				VkAttachmentLoadOp::VK_ATTACHMENT_LOAD_OP_DONT_CARE,
				VkAttachmentStoreOp::VK_ATTACHMENT_STORE_OP_DONT_CARE,
				VkImageLayout::VK_IMAGE_LAYOUT_UNDEFINED,
				VkImageLayout::VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL
			),
		},
		{
			SubpassDescription(VkPipelineBindPoint::VK_PIPELINE_BIND_POINT_GRAPHICS,
				{AttachmentReference(0, VkImageLayout::VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL)},
				AttachmentReference(1, VkImageLayout::VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL)
			)
		}
	));

	// Descriptor Pool
	auto vk_descriptorPool = CreateDescriptorPool(vk_device, DescriptorPoolCreateInfo(VkDescriptorPoolCreateFlagBits::VK_DESCRIPTOR_POOL_CREATE_FREE_DESCRIPTOR_SET_BIT, 3, {
		DescriptorPoolSize(VkDescriptorType::VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, 1),
		DescriptorPoolSize(VkDescriptorType::VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, 6),
	}));

	// Descriptor Set Layout
	auto vk_objectDescriptorSetLayout = CreateDescriptorSetLayout(vk_device, DescriptorSetLayoutCreateInfo(0, {
		DescriptorSetLayoutBinding(0, VkDescriptorType::VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, 1, VkShaderStageFlagBits::VK_SHADER_STAGE_VERTEX_BIT),
		DescriptorSetLayoutBinding(1, VkDescriptorType::VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, 1, VkShaderStageFlagBits::VK_SHADER_STAGE_FRAGMENT_BIT),
		DescriptorSetLayoutBinding(2, VkDescriptorType::VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, 1, VkShaderStageFlagBits::VK_SHADER_STAGE_FRAGMENT_BIT),
		DescriptorSetLayoutBinding(3, VkDescriptorType::VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, 1, VkShaderStageFlagBits::VK_SHADER_STAGE_FRAGMENT_BIT),
		DescriptorSetLayoutBinding(4, VkDescriptorType::VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, 1, VkShaderStageFlagBits::VK_SHADER_STAGE_FRAGMENT_BIT),
		DescriptorSetLayoutBinding(5, VkDescriptorType::VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, 1, VkShaderStageFlagBits::VK_SHADER_STAGE_FRAGMENT_BIT),
		DescriptorSetLayoutBinding(6, VkDescriptorType::VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, 1, VkShaderStageFlagBits::VK_SHADER_STAGE_FRAGMENT_BIT),
	}));
	auto vk_backgroundDescriptorSetLayout = CreateDescriptorSetLayout(vk_device, DescriptorSetLayoutCreateInfo(0, {
		DescriptorSetLayoutBinding(0, VkDescriptorType::VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, 1, VkShaderStageFlagBits::VK_SHADER_STAGE_FRAGMENT_BIT),
	}));
	auto vk_terrainDescriptorSetLayout = CreateDescriptorSetLayout(vk_device, DescriptorSetLayoutCreateInfo(0, {
		DescriptorSetLayoutBinding(0, VkDescriptorType::VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, 1, VkShaderStageFlagBits::VK_SHADER_STAGE_TESSELLATION_CONTROL_BIT | VkShaderStageFlagBits::VK_SHADER_STAGE_TESSELLATION_EVALUATION_BIT),
		DescriptorSetLayoutBinding(1, VkDescriptorType::VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, 1, VkShaderStageFlagBits::VK_SHADER_STAGE_TESSELLATION_EVALUATION_BIT),
	}));

	// Descriptor Set
	auto vk_descriptorSets = Move(AllocateDescriptorSets(vk_device, DescriptorSetAllocateInfo(vk_descriptorPool, {
		vk_objectDescriptorSetLayout,
		vk_backgroundDescriptorSetLayout,
		vk_terrainDescriptorSetLayout
	})));
	auto &vk_objectDescriptorSet = vk_descriptorSets[0];
	{
		UpdateDescriptorSets(vk_device, {
			WriteDescriptorSet(vk_objectDescriptorSet, 0, 0, VkDescriptorType::VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, {DescriptorBufferInfo(vk_uniformBuffer)}),
			WriteDescriptorSet(vk_objectDescriptorSet, 1, 0, VkDescriptorType::VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, {DescriptorImageInfo(vk_sampler, vk_imageViewAlbedo, VkImageLayout::VK_IMAGE_LAYOUT_PREINITIALIZED)}),
			WriteDescriptorSet(vk_objectDescriptorSet, 2, 0, VkDescriptorType::VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, {DescriptorImageInfo(vk_sampler, vk_imageViewNormals, VkImageLayout::VK_IMAGE_LAYOUT_PREINITIALIZED)}),
			WriteDescriptorSet(vk_objectDescriptorSet, 3, 0, VkDescriptorType::VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, {DescriptorImageInfo(vk_sampler, vk_imageViewRoughness, VkImageLayout::VK_IMAGE_LAYOUT_PREINITIALIZED)}),
			WriteDescriptorSet(vk_objectDescriptorSet, 4, 0, VkDescriptorType::VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, {DescriptorImageInfo(vk_sampler, vk_imageViewMetalness, VkImageLayout::VK_IMAGE_LAYOUT_PREINITIALIZED)}),
			WriteDescriptorSet(vk_objectDescriptorSet, 5, 0, VkDescriptorType::VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, {DescriptorImageInfo(vk_sampler, vk_imageViewOcclusion, VkImageLayout::VK_IMAGE_LAYOUT_PREINITIALIZED)}),
			WriteDescriptorSet(vk_objectDescriptorSet, 6, 0, VkDescriptorType::VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, {DescriptorImageInfo(vk_sampler, vk_imageViewEnvironment, VkImageLayout::VK_IMAGE_LAYOUT_PREINITIALIZED)}),
		}, {});
	}
	auto &vk_backgroundDescriptorSet = vk_descriptorSets[1];
	{
		UpdateDescriptorSets(vk_device, {
			WriteDescriptorSet(vk_backgroundDescriptorSet, 0, 0, VkDescriptorType::VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, {DescriptorImageInfo(vk_sampler, vk_imageViewEnvironment, VkImageLayout::VK_IMAGE_LAYOUT_PREINITIALIZED)}),
		}, {});
	}
	auto &vk_terrainDescriptorSet = vk_descriptorSets[2];
	{
		UpdateDescriptorSets(vk_device, {
			WriteDescriptorSet(vk_terrainDescriptorSet, 0, 0, VkDescriptorType::VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, {DescriptorBufferInfo(vk_terrainUniformBuffer)}),
			WriteDescriptorSet(vk_terrainDescriptorSet, 1, 0, VkDescriptorType::VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, {DescriptorImageInfo(vk_samplerTerrain, vk_imageViewTerrainHeight , VkImageLayout::VK_IMAGE_LAYOUT_PREINITIALIZED)}),
		}, {});
	}

	// Pipeline Layout
	auto vk_objectPipelineLayout = CreatePipelineLayout(vk_device, PipelineLayoutCreateInfo({vk_objectDescriptorSetLayout}, {}));
	auto vk_backgroundPipelineLayout = CreatePipelineLayout(vk_device, PipelineLayoutCreateInfo({vk_backgroundDescriptorSetLayout}, {PushConstantRange(VkShaderStageFlagBits::VK_SHADER_STAGE_GEOMETRY_BIT, 0, sizeof(float)* 16)}));
	auto vk_terrainPipelineLayout = CreatePipelineLayout(vk_device, PipelineLayoutCreateInfo({vk_terrainDescriptorSetLayout}, {}));

	// Pipeline
	auto vk_pipelines = Move(CreateGraphicsPipelines(vk_device, VK_NULL_HANDLE, {
		GraphicsPipelineCreateInfo(
			0,
			{
				PipelineShaderStageCreateInfo(VkShaderStageFlagBits::VK_SHADER_STAGE_VERTEX_BIT, vk_objectVertexShaderModule, "main"),
				PipelineShaderStageCreateInfo(VkShaderStageFlagBits::VK_SHADER_STAGE_FRAGMENT_BIT, vk_objectFragmentShaderModule, "main"),
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
				{Viewport(0, 0, static_cast<float>(vk_surfaceCapabilities.currentExtent.width), static_cast<float>(vk_surfaceCapabilities.currentExtent.height), 0.0f, 1.0f)},
				{Rect2D(Offset2D(0, 0), Extent2D(vk_surfaceCapabilities.currentExtent.width, vk_surfaceCapabilities.currentExtent.height))}
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
				VK_TRUE, VK_TRUE, VkCompareOp::VK_COMPARE_OP_LESS, VK_FALSE, VK_FALSE,
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
			vk_objectPipelineLayout,
			vk_renderPass,
			0
		),
		GraphicsPipelineCreateInfo(
			0,
			{
				PipelineShaderStageCreateInfo(VkShaderStageFlagBits::VK_SHADER_STAGE_VERTEX_BIT, vk_backgroundVertexShaderModule, "main"),
				PipelineShaderStageCreateInfo(VkShaderStageFlagBits::VK_SHADER_STAGE_GEOMETRY_BIT, vk_backgroundGeometryShaderModule, "main"),
				PipelineShaderStageCreateInfo(VkShaderStageFlagBits::VK_SHADER_STAGE_FRAGMENT_BIT, vk_backgroundFragmentShaderModule, "main"),
			},
			PipelineVertexInputStateCreateInfo({}, {}),
			PipelineInputAssemblyStateCreateInfo(VkPrimitiveTopology::VK_PRIMITIVE_TOPOLOGY_POINT_LIST, VK_FALSE),
			PipelineViewportStateCreateInfo(
				{Viewport(0, 0, static_cast<float>(vk_surfaceCapabilities.currentExtent.width), static_cast<float>(vk_surfaceCapabilities.currentExtent.height), 0.0f, 1.0f)},
				{Rect2D(Offset2D(0, 0), Extent2D(vk_surfaceCapabilities.currentExtent.width, vk_surfaceCapabilities.currentExtent.height))}
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
			vk_backgroundPipelineLayout,
			vk_renderPass,
			0
		),
		GraphicsPipelineCreateInfo(
			0,
			{
				PipelineShaderStageCreateInfo(VkShaderStageFlagBits::VK_SHADER_STAGE_VERTEX_BIT, vk_terrainVertexShaderModule, "main"),
				PipelineShaderStageCreateInfo(VkShaderStageFlagBits::VK_SHADER_STAGE_TESSELLATION_CONTROL_BIT, vk_terrainTessellationControlShaderModule, "main"),
				PipelineShaderStageCreateInfo(VkShaderStageFlagBits::VK_SHADER_STAGE_TESSELLATION_EVALUATION_BIT, vk_terrainTessellationEvaluationShaderModule, "main"),
				PipelineShaderStageCreateInfo(VkShaderStageFlagBits::VK_SHADER_STAGE_GEOMETRY_BIT, vk_terrainGeometryShaderModule, "main"),
				PipelineShaderStageCreateInfo(VkShaderStageFlagBits::VK_SHADER_STAGE_FRAGMENT_BIT, vk_terrainFragmentShaderModule, "main"),
			},
			PipelineVertexInputStateCreateInfo(
				{VertexInputBindingDescription(0, sizeof(Vertex2), VkVertexInputRate::VK_VERTEX_INPUT_RATE_VERTEX)},
				{
					VertexInputAttributeDescription(0, 0, VkFormat::VK_FORMAT_R32G32B32_SFLOAT, 0),
					VertexInputAttributeDescription(1, 0, VkFormat::VK_FORMAT_R32G32_SFLOAT, sizeof(glm::vec3)),
				}
			),
			PipelineInputAssemblyStateCreateInfo(VkPrimitiveTopology::VK_PRIMITIVE_TOPOLOGY_PATCH_LIST, VK_FALSE),
			PipelineTessellationStateCreateInfo(4),
			PipelineViewportStateCreateInfo(
				{Viewport(0, 0, static_cast<float>(vk_surfaceCapabilities.currentExtent.width), static_cast<float>(vk_surfaceCapabilities.currentExtent.height), 0.0f, 1.0f)},
				{Rect2D(Offset2D(0, 0), Extent2D(vk_surfaceCapabilities.currentExtent.width, vk_surfaceCapabilities.currentExtent.height))}
			),
			PipelineRasterizationStateCreateInfo(
				VK_FALSE, VK_FALSE,
				VkPolygonMode::VK_POLYGON_MODE_LINE, VkCullModeFlagBits::VK_CULL_MODE_NONE, VkFrontFace::VK_FRONT_FACE_COUNTER_CLOCKWISE,
				VK_FALSE, 0.0f, 0.0f, 0.0f, 1.0f
			),
			PipelineMultisampleStateCreateInfo(
				VkSampleCountFlagBits::VK_SAMPLE_COUNT_1_BIT,
				VK_FALSE, 0.0f, nullptr, VK_FALSE, VK_FALSE
			),
			PipelineDepthStencilStateCreateInfo(
				VK_TRUE, VK_TRUE, VkCompareOp::VK_COMPARE_OP_LESS, VK_FALSE,
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
			vk_terrainPipelineLayout,
			vk_renderPass,
			0
		),
	}));
	auto &vk_objectPipeline = vk_pipelines[0];
	auto &vk_backgroundPipeline = vk_pipelines[1];
	auto &vk_terrainPipeline = vk_pipelines[2];

	DestroyShaderModule(vk_device, vk_objectVertexShaderModule);
	DestroyShaderModule(vk_device, vk_objectFragmentShaderModule);

	DestroyShaderModule(vk_device, vk_backgroundVertexShaderModule);
	DestroyShaderModule(vk_device, vk_backgroundGeometryShaderModule);
	DestroyShaderModule(vk_device, vk_backgroundFragmentShaderModule);

	DestroyShaderModule(vk_device, vk_terrainVertexShaderModule);
	DestroyShaderModule(vk_device, vk_terrainTessellationControlShaderModule);
	DestroyShaderModule(vk_device, vk_terrainTessellationEvaluationShaderModule);
	DestroyShaderModule(vk_device, vk_terrainGeometryShaderModule);
	DestroyShaderModule(vk_device, vk_terrainFragmentShaderModule);

	// Framebuffers
	Vector<VkFramebuffer> vk_framebuffers(vk_swapchainImageViews.size());
	{
		for (Size i = 0; i < vk_framebuffers.size(); ++i)
		{
			auto &vk_framebuffer = vk_framebuffers[i];
			auto &vk_swapchainImageView = vk_swapchainImageViews[i];

			vk_framebuffer = CreateFramebuffer(vk_device, FramebufferCreateInfo(vk_renderPass, { vk_swapchainImageView, vk_swapchainDepthImageView }, windowSize.width, windowSize.height, 1));
		}
	}

	// Command Buffers
	auto vk_constantsCommandBuffers = Move(AllocateCommandBuffers(vk_device, CommandBufferAllocateInfo(vk_commandPool, VkCommandBufferLevel::VK_COMMAND_BUFFER_LEVEL_PRIMARY, 1)));
	auto &vk_constantsCommandBuffer = vk_constantsCommandBuffers[0];

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
					ClearValue::DepthStencil(1.0f, 0),
				}),
				VkSubpassContents::VK_SUBPASS_CONTENTS_INLINE
			);

			CmdBindPipeline(vk_commandBuffer, VkPipelineBindPoint::VK_PIPELINE_BIND_POINT_GRAPHICS, vk_backgroundPipeline);
			CmdBindDescriptorSets(vk_commandBuffer, VkPipelineBindPoint::VK_PIPELINE_BIND_POINT_GRAPHICS, vk_backgroundPipelineLayout, 0, {vk_backgroundDescriptorSet}, {});
			CmdDraw(vk_commandBuffer, 1, 1, 0, 0);

			CmdBindPipeline(vk_commandBuffer, VkPipelineBindPoint::VK_PIPELINE_BIND_POINT_GRAPHICS, vk_terrainPipeline);
			CmdBindVertexBuffers(vk_commandBuffer, 0, 1, {vk_terrainVertexBuffer}, {0});
			CmdBindIndexBuffer(vk_commandBuffer, vk_terrainIndexBuffer, 0, VkIndexType::VK_INDEX_TYPE_UINT32);
			CmdBindDescriptorSets(vk_commandBuffer, VkPipelineBindPoint::VK_PIPELINE_BIND_POINT_GRAPHICS, vk_terrainPipelineLayout, 0, {vk_terrainDescriptorSet}, {});
			CmdDrawIndexed(vk_commandBuffer, terrainIndices.size(), 1, 0, 0, 0);

			CmdBindPipeline(vk_commandBuffer, VkPipelineBindPoint::VK_PIPELINE_BIND_POINT_GRAPHICS, vk_objectPipeline);
			CmdBindVertexBuffers(vk_commandBuffer, 0, 1, {vk_vertexBuffer}, {0});
			CmdBindIndexBuffer(vk_commandBuffer, vk_indexBuffer, 0, VkIndexType::VK_INDEX_TYPE_UINT32);
			CmdBindDescriptorSets(vk_commandBuffer, VkPipelineBindPoint::VK_PIPELINE_BIND_POINT_GRAPHICS, vk_objectPipelineLayout, 0, {vk_objectDescriptorSet}, {});
			CmdDrawIndexed(vk_commandBuffer, indices.size(), 1, 0, 0, 0);

			CmdEndRenderPass(vk_commandBuffer);

			EndCommandBuffer(vk_commandBuffer);
		}
	}

	// Semaphore
	auto vk_semaphore = CreateSemaphore(vk_device, SemaphoreCreateInfo());

	// Fence
	auto vk_fence = CreateFence(vk_device, FenceCreateInfo(0));

	// other stuff
	bool isRotatingEnabled = true; bool isRotatingChanged = false;
	float distance = 2.0f;;
	auto angle = glm::vec3(0.0f);
	auto viewPosition = glm::vec3(0.0f, 25.0f, -5.0f);
	auto viewAngle = glm::vec3(32.0f, 0.0f, 0.0f);
	auto terrainAngle = glm::vec3(0.0f);
	auto terrainPosition = glm::vec3(0.0f, -0.5f, 0.0f);


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

		if (GetAsyncKeyState(VK_UP))
		{
			viewAngle.x += 1.0f;
		}
		if (GetAsyncKeyState(VK_DOWN))
		{
			viewAngle.x -= 1.0f;
		}
		if (GetAsyncKeyState(VK_RIGHT))
		{
			viewAngle.y += 1.0f;
		}
		if (GetAsyncKeyState(VK_LEFT))
		{
			viewAngle.y -= 1.0f;
		}

		float speed = 0.1f;
		if (GetAsyncKeyState(VK_SHIFT))
		{
			speed *= 10.0f;
		}
		if (GetAsyncKeyState('W'))
		{
			viewPosition += rotateZXY3(viewAngle) * glm::vec3(0.0f, 0.0f, +speed);
		}
		if (GetAsyncKeyState('S'))
		{
			viewPosition += rotateZXY3(viewAngle) * glm::vec3(0.0f, 0.0f, -speed);
		}
		if (GetAsyncKeyState('D'))
		{
			viewPosition += rotateZXY3(viewAngle) * glm::vec3(+speed, 0.0f, 0.0f);
		}
		if (GetAsyncKeyState('A'))
		{
			viewPosition += rotateZXY3(viewAngle) * glm::vec3(-speed, 0.0f, 0.0f);
		}
		if (GetAsyncKeyState(VK_SPACE))
		{
			viewPosition += rotateZXY3(viewAngle) * glm::vec3(0.0f, +speed, 0.0f);
		}
		if (GetAsyncKeyState(VK_CONTROL))
		{
			viewPosition += rotateZXY3(viewAngle) * glm::vec3(0.0f, -speed, 0.0f);
		}

		auto tm = rotateYXZ(-terrainAngle) * move(-terrainPosition);
		{
			auto t = xyz(tm * glm::vec4(viewPosition, 1.0f));
			auto k = (glm::vec2(t.x, t.z) / (terrainSize / 2.0f)) * 0.5f + 0.5f;
			auto u = glm::vec2(imageTerrainSize) * k;
			auto miG = glm::floor(u);
			auto maG = glm::ceil(u);
			
			if (miG.x >= 0 && maG.x < imageTerrainSize.x && miG.y >= 0 && maG.y < imageTerrainSize.y)
			{
				auto getHeight = [&](glm::ivec2 g)
				{
					float maxHeight = 250.0f;
					auto p = reinterpret_cast<unsigned short*>(imageTerrainHeightData.data());
					auto l = p[g.x + g.y * imageTerrainSize.x];
					auto h = static_cast<float>(static_cast<double>(l) / static_cast<double>(UINT16_MAX) * static_cast<double>(maxHeight));
					auto m = terrainPosition.y + h;

					return m;
				};

				auto q = (u - static_cast<glm::vec2>(miG)) / (static_cast<glm::vec2>(maG) - static_cast<glm::vec2>(miG));
				auto h00 = getHeight(glm::ivec2(miG.x, miG.y));
				auto h10 = getHeight(glm::ivec2(maG.x, miG.y));
				auto h01 = getHeight(glm::ivec2(miG.x, maG.y));
				auto h11 = getHeight(glm::ivec2(maG.x, maG.y));
				auto h = glm::mix(
					glm::mix(h00, h10, q.x),
					glm::mix(h01, h11, q.x),
					q.y
				);

				if (viewPosition.y < h + 1.0f)
				{
					viewPosition.y = h + 1.0f;
				}
			}


			/*auto g = glm::ivec2(glm::vec2(imageTerrainSize) * k);

			if (g.x >= 0 && g.x < imageTerrainSize.x && g.y >= 0 && g.y < imageTerrainSize.y)
			{
				float maxHeight = 20.0f;
				auto p = reinterpret_cast<unsigned short*>(imageTerrainHeightData.data());
				auto l = p[g.x + g.y * imageTerrainSize.x];
				auto h = static_cast<float>(l) / static_cast<float>(UINT16_MAX) * maxHeight;
				auto m = terrainPosition.y + h + 1.0f;

				if (viewPosition.y < m)
				{
					viewPosition.y = m;
				}
			}*/
		}

		if (GetAsyncKeyState('R'))
		{
			if (!isRotatingChanged)
			{
				isRotatingEnabled = !isRotatingEnabled;

				isRotatingChanged = true;
			}
		}
		else
		{
			isRotatingChanged = false;
		}

		// update uniforms
		QueueWaitIdle(vk_queue);

		// angle += glm::vec3(0.2f, 0.5f, 0.1f);
		if (isRotatingEnabled)
		{
			angle += glm::vec3(0.0f, 0.25f, 0.0f);
		}

		// auto viewPosition = xyz(rotateZXY(viewAngle) * glm::vec4(0.0f, 0.0f, -distance, 1.0f));
		auto viewMatrix = rotateYXZ(-viewAngle) * move(-viewPosition);
		auto projectionMatrix = perspective(60.0f, windowAspect, 0.1f, 1000.0f) * scale(glm::vec3(1.0f, -1.0f, 1.0f));
		auto viewProjectionMatrix = projectionMatrix * viewMatrix;

		auto modelAngle = angle;
		auto modelPosition = glm::vec3(0.0f, 25.0f, 0.0f);

		auto rotateMatrix = rotateZXY(angle);
		auto modelMatrix = move(modelPosition) * rotateMatrix;
		auto modelMatrix2 = move(-viewPosition) * move(modelPosition) * rotateMatrix;
		auto viewProjectionInverseMatrix = rotateZXY(viewAngle) * scale(glm::vec3(1.0f, -1.0f, 1.0f)) * perspectiveInverse(60.0f, 1.0f, 0.1f, 1000.0f);

		auto modelViewProjectionMatrix = viewProjectionMatrix * modelMatrix;

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

		auto terrainRotateMatrix = rotateZXY(terrainAngle);
		auto terrainModelMatrix = move(terrainPosition) * terrainRotateMatrix;
		auto terrainModelInverseMatrix = rotateYXZ(-terrainAngle) * move(-terrainPosition);
		auto terrainModelViewProjectionMatrix = viewProjectionMatrix * terrainModelMatrix;
		auto terrainViewPosition = xyz(terrainModelInverseMatrix * glm::vec4(viewPosition, 1.0f));

		auto terrainData = MapMemory(vk_device, vk_terrainUniformBufferDeviceMemory, 0, VK_WHOLE_SIZE, 0);
		{
			for (Size i = 0; i < 16; ++i)
			{
				terrainUniforms[i] = terrainModelViewProjectionMatrix[i / 4][i % 4];
			}

			terrainUniforms[16 + 0] = terrainViewPosition[0];
			terrainUniforms[16 + 1] = terrainViewPosition[1];
			terrainUniforms[16 + 2] = terrainViewPosition[2];
			terrainUniforms[16 + 3] = 1.0f;

			std::memcpy(terrainData, terrainUniforms.data(), terrainUniformsTotalSize);
		}
		UnmapMemory(vk_device, vk_terrainUniformBufferDeviceMemory);

		auto vk_nextImageIndex = AcquireNextImageKHR(vk_device, vk_swapchain, UINT64_MAX, VK_NULL_HANDLE, vk_fence);
		
		WaitForFences(vk_device, {vk_fence}, VK_FALSE, UINT64_MAX);
		ResetFences(vk_device, {vk_fence});

		//
		ResetCommandBuffer(vk_constantsCommandBuffer, VkCommandBufferResetFlagBits::VK_COMMAND_BUFFER_RESET_RELEASE_RESOURCES_BIT);
		{
			BeginCommandBuffer(vk_constantsCommandBuffer, CommandBufferBeginInfo(0));
			
			Vector<float> data(16);
			{
				for (Size i = 0; i < 16; ++i) data[i] = viewProjectionInverseMatrix[i / 4][i % 4];
			}
		
			CmdPushConstants<float>(vk_constantsCommandBuffer, vk_backgroundPipelineLayout, VkShaderStageFlagBits::VK_SHADER_STAGE_GEOMETRY_BIT, 0, data);
		
			EndCommandBuffer(vk_constantsCommandBuffer);
		}
		//

		auto &vk_commandBuffer = vk_commandBuffers[vk_nextImageIndex];

		QueueSubmit(vk_queue, {
			SubmitInfo({vk_constantsCommandBuffer}, {}, {}, {vk_semaphore}),
			SubmitInfo({vk_commandBuffer}, {vk_semaphore}, {VkPipelineStageFlagBits::VK_PIPELINE_STAGE_GEOMETRY_SHADER_BIT}),
		});
		QueueWaitIdle(vk_queue);

		QueuePresentKHR(vk_queue, PresentInfoKHR({}, {vk_swapchain}, {vk_nextImageIndex}));

		// Sleep(1000 / 60);
	}

	DestroySemaphore(vk_device, vk_semaphore);
	DestroyFence(vk_device, vk_fence);
	FreeCommandBuffers(vk_device, vk_commandPool, vk_commandBuffers);
	FreeCommandBuffers(vk_device, vk_commandPool, vk_constantsCommandBuffers);
	DestroyCommandPool(vk_device, vk_commandPool); // TODO: crash when attempting to destroy pool without any buffers allocated
	
	DestroyPipeline(vk_device, vk_objectPipeline);
	DestroyPipeline(vk_device, vk_backgroundPipeline);
	DestroyPipeline(vk_device, vk_terrainPipeline);
	DestroyPipelineLayout(vk_device, vk_objectPipelineLayout);
	DestroyPipelineLayout(vk_device, vk_backgroundPipelineLayout);
	DestroyPipelineLayout(vk_device, vk_terrainPipelineLayout);
	
	DestroyDescriptorSetLayout(vk_device, vk_objectDescriptorSetLayout);
	DestroyDescriptorSetLayout(vk_device, vk_backgroundDescriptorSetLayout);
	DestroyDescriptorSetLayout(vk_device, vk_terrainDescriptorSetLayout);
	FreeDescriptorSets(vk_device, vk_descriptorPool, vk_descriptorSets);
	DestroyDescriptorPool(vk_device, vk_descriptorPool);
	
	DestroyRenderPass(vk_device, vk_renderPass);
	DestroySampler(vk_device, vk_sampler);
	DestroySampler(vk_device, vk_samplerTerrain);

	DestroyImageView(vk_device, vk_imageViewAlbedo); FreeMemory(vk_device, vk_imageAlbedoDeviceMemory); DestroyImage(vk_device, vk_imageAlbedo);
	DestroyImageView(vk_device, vk_imageViewNormals); FreeMemory(vk_device, vk_imageNormalsDeviceMemory); DestroyImage(vk_device, vk_imageNormals);
	DestroyImageView(vk_device, vk_imageViewRoughness); FreeMemory(vk_device, vk_imageRoughnessDeviceMemory); DestroyImage(vk_device, vk_imageRoughness);
	DestroyImageView(vk_device, vk_imageViewMetalness); FreeMemory(vk_device, vk_imageMetalnessDeviceMemory); DestroyImage(vk_device, vk_imageMetalness);
	DestroyImageView(vk_device, vk_imageViewOcclusion); FreeMemory(vk_device, vk_imageOcclusionDeviceMemory); DestroyImage(vk_device, vk_imageOcclusion);
	DestroyImageView(vk_device, vk_imageViewEnvironment); FreeMemory(vk_device, vk_imageEnvironmentDeviceMemory); DestroyImage(vk_device, vk_imageEnvironment);
	DestroyImageView(vk_device, vk_imageViewTerrainHeight); FreeMemory(vk_device, vk_imageTerrainHeightDeviceMemory); DestroyImage(vk_device, vk_imageTerrainHeight);

	FreeMemory(vk_device, vk_terrainUniformBufferDeviceMemory); DestroyBuffer(vk_device, vk_terrainUniformBuffer);
	FreeMemory(vk_device, vk_uniformBufferDeviceMemory); DestroyBuffer(vk_device, vk_uniformBuffer);
	FreeMemory(vk_device, vk_indexBufferDeviceMemory); DestroyBuffer(vk_device, vk_indexBuffer);
	FreeMemory(vk_device, vk_terrainVertexBufferDeviceMemory); DestroyBuffer(vk_device, vk_terrainVertexBuffer);
	FreeMemory(vk_device, vk_terrainIndexBufferDeviceMemory); DestroyBuffer(vk_device, vk_terrainIndexBuffer);
	FreeMemory(vk_device, vk_vertexBufferDeviceMemory); DestroyBuffer(vk_device, vk_vertexBuffer);

	for (auto &vk_framebuffer : vk_framebuffers)
	{
		DestroyFramebuffer(vk_device, vk_framebuffer);
	}
	for (auto &vk_swapchainImageView : vk_swapchainImageViews)
	{
		DestroyImageView(vk_device, vk_swapchainImageView);
	}
	DestroyImageView(vk_device, vk_swapchainDepthImageView); FreeMemory(vk_device, vk_swapchainDepthImageDeviceMemory); DestroyImage(vk_device, vk_swapchainDepthImage);
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