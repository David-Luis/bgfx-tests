#pragma once

#include <bgfx/bgfx.h>

namespace bx
{
	struct FileReaderI;
}

namespace BGFXUtils
{
	bgfx::ProgramHandle LoadProgram(bx::FileReaderI* _reader, const char* _vsName, const char* _fsName);
}