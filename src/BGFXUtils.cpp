#pragma once

#include "../include/BGFXUtils.h"
#include <bx/readerwriter.h>

#include <bx/file.h>
#include <bx/pixelformat.h>

#include <bimg/decode.h>

#include <iostream>
#include <memory>

namespace BGFXUtils
{
	const bgfx::Memory* LoadMem(bx::FileReaderI* _reader, const char* _filePath)
	{
		if (bx::open(_reader, _filePath))
		{
			uint32_t size = (uint32_t)bx::getSize(_reader);
			const bgfx::Memory* mem = bgfx::alloc(size + 1);
			bx::read(_reader, mem->data, size);
			bx::close(_reader);
			mem->data[mem->size - 1] = '\0';
			return mem;
		}

		std::cerr << "Failed to load " << _filePath << std::endl;
		return NULL;
	}

	bgfx::ShaderHandle LoadShader(bx::FileReaderI* _reader, const char* _name)
	{
		char filePath[512];

		const char* shaderPath = "???";

		switch (bgfx::getRendererType())
		{
		case bgfx::RendererType::Noop:
		case bgfx::RendererType::Direct3D9:  shaderPath = "shaders/dx9/";   break;
		case bgfx::RendererType::Direct3D11:
		case bgfx::RendererType::Direct3D12: shaderPath = "shaders/dx11/";  break;
		case bgfx::RendererType::Gnm:        shaderPath = "shaders/pssl/";  break;
		case bgfx::RendererType::Metal:      shaderPath = "shaders/metal/"; break;
		case bgfx::RendererType::Nvn:        shaderPath = "shaders/nvn/";   break;
		case bgfx::RendererType::OpenGL:     shaderPath = "shaders/glsl/";  break;
		case bgfx::RendererType::OpenGLES:   shaderPath = "shaders/essl/";  break;
		case bgfx::RendererType::Vulkan:     shaderPath = "shaders/spirv/"; break;

		case bgfx::RendererType::Count:
			BX_CHECK(false, "You should not be here!");
			break;
		}

		bx::strCopy(filePath, BX_COUNTOF(filePath), shaderPath);
		bx::strCat(filePath, BX_COUNTOF(filePath), _name);
		bx::strCat(filePath, BX_COUNTOF(filePath), ".bin");

		bgfx::ShaderHandle handle = bgfx::createShader(LoadMem(_reader, filePath));
		bgfx::setName(handle, _name);

		return handle;
	}

	bgfx::ProgramHandle LoadProgram(bx::FileReaderI* _reader, const char* _vsName, const char* _fsName)
	{
		bgfx::ShaderHandle vsh = LoadShader(_reader, _vsName);
		bgfx::ShaderHandle fsh = BGFX_INVALID_HANDLE;
		if (NULL != _fsName)
		{
			fsh = LoadShader(_reader, _fsName);
		}

		return bgfx::createProgram(vsh, fsh, true /* destroy shaders when program is destroyed */);
	}

	void* load(bx::FileReaderI* _reader, bx::AllocatorI* _allocator, const char* _filePath, uint32_t* _size)
	{
		if (bx::open(_reader, _filePath))
		{
			uint32_t size = (uint32_t)bx::getSize(_reader);
			void* data = BX_ALLOC(_allocator, size);
			bx::read(_reader, data, size);
			bx::close(_reader);
			if (NULL != _size)
			{
				*_size = size;
			}
			return data;
		}
		else
		{
			std::cerr << "Failed to open: " << _filePath;
		}

		if (NULL != _size)
		{
			*_size = 0;
		}

		return NULL;
	}

	void unload(bx::AllocatorI* allocator, void* _ptr)
	{
		BX_FREE(allocator, _ptr);
	}

	void imageReleaseCb(void* _ptr, void* _userData)
	{
		BX_UNUSED(_ptr);
		bimg::ImageContainer* imageContainer = (bimg::ImageContainer*)_userData;
		bimg::imageFree(imageContainer);
	}

	uint32_t EncodeNormalRgba8(float _x, float _y, float _z, float _w)
	{
		const float src[] =
		{
			_x * 0.5f + 0.5f,
			_y * 0.5f + 0.5f,
			_z * 0.5f + 0.5f,
			_w * 0.5f + 0.5f,
		};
		uint32_t dst;
		bx::packRgba8(&dst, src);
		return dst;
	}


	bgfx::TextureHandle LoadTexture(bx::AllocatorI* allocator, bx::FileReaderI* _reader, const char* _filePath, uint64_t _flags, uint8_t _skip, bgfx::TextureInfo* _info, bimg::Orientation::Enum* _orientation)
	{
		BX_UNUSED(_skip);
		bgfx::TextureHandle handle = BGFX_INVALID_HANDLE;

		uint32_t size;
		void* data = load(_reader, allocator, _filePath, &size);
		if (NULL != data)
		{
			bimg::ImageContainer* imageContainer = bimg::imageParse(allocator, data, size);

			if (NULL != imageContainer)
			{
				if (NULL != _orientation)
				{
					*_orientation = imageContainer->m_orientation;
				}

				const bgfx::Memory* mem = bgfx::makeRef(
					imageContainer->m_data
					, imageContainer->m_size
					, imageReleaseCb
					, imageContainer
				);
				unload(allocator, data);

				if (imageContainer->m_cubeMap)
				{
					handle = bgfx::createTextureCube(
						uint16_t(imageContainer->m_width)
						, 1 < imageContainer->m_numMips
						, imageContainer->m_numLayers
						, bgfx::TextureFormat::Enum(imageContainer->m_format)
						, _flags
						, mem
					);
				}
				else if (1 < imageContainer->m_depth)
				{
					handle = bgfx::createTexture3D(
						uint16_t(imageContainer->m_width)
						, uint16_t(imageContainer->m_height)
						, uint16_t(imageContainer->m_depth)
						, 1 < imageContainer->m_numMips
						, bgfx::TextureFormat::Enum(imageContainer->m_format)
						, _flags
						, mem
					);
				}
				else if (bgfx::isTextureValid(0, false, imageContainer->m_numLayers, bgfx::TextureFormat::Enum(imageContainer->m_format), _flags))
				{
					handle = bgfx::createTexture2D(
						uint16_t(imageContainer->m_width)
						, uint16_t(imageContainer->m_height)
						, 1 < imageContainer->m_numMips
						, imageContainer->m_numLayers
						, bgfx::TextureFormat::Enum(imageContainer->m_format)
						, _flags
						, mem
					);
				}

				if (bgfx::isValid(handle))
				{
					bgfx::setName(handle, _filePath);
				}

				if (NULL != _info)
				{
					bgfx::calcTextureSize(
						*_info
						, uint16_t(imageContainer->m_width)
						, uint16_t(imageContainer->m_height)
						, uint16_t(imageContainer->m_depth)
						, imageContainer->m_cubeMap
						, 1 < imageContainer->m_numMips
						, imageContainer->m_numLayers
						, bgfx::TextureFormat::Enum(imageContainer->m_format)
					);
				}
			}
		}

		return handle;
	}
}
