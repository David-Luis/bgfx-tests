#pragma once

#include <bgfx/bgfx.h>
#include <bimg/bimg.h>

namespace bx
{
	struct FileReaderI;
}

namespace BGFXUtils
{
	bgfx::ProgramHandle LoadProgram(bx::FileReaderI* _reader, const char* _vsName, const char* _fsName);
	uint32_t EncodeNormalRgba8(float _x, float _y = 0.0f, float _z = 0.0f, float _w = 0.0f);
	bgfx::TextureHandle LoadTexture(bx::AllocatorI* allocator, bx::FileReaderI* _reader, const char* _name, uint64_t _flags = BGFX_TEXTURE_NONE | BGFX_SAMPLER_NONE, uint8_t _skip = 0, bgfx::TextureInfo* _info = NULL, bimg::Orientation::Enum* _orientation = NULL);

}