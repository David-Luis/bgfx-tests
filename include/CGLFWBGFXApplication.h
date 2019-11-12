#pragma once

#include <bx/allocator.h>

#include <stdint.h>

struct GLFWwindow;

namespace bx
{
	struct FileReaderI;
	struct AllocatorI;
}

class CGLFWBGFXApplication
{
public:
	CGLFWBGFXApplication(int width, int height);
	virtual ~CGLFWBGFXApplication();
	CGLFWBGFXApplication(const CGLFWBGFXApplication&) = delete;
	CGLFWBGFXApplication& operator=(const CGLFWBGFXApplication&) = delete;

	void Initialize();
	void RenderLoop();

	bx::FileReaderI* GetFileReader();
	static bx::AllocatorI* GetAllocator()
	{
		return &s_allocator;
	}

	int GetWidth() { return m_width; }
	int GetHeight() { return m_height; }

	float GetTotalTime();

protected:
	virtual void OnInitialize() = 0;
	virtual void OnRender(float dt) = 0;

private:
	void InitGraphics();
	void InitSystems();

	bx::FileReaderI* m_fileReader;
	static bx::DefaultAllocator s_allocator;

	GLFWwindow * m_window;
	int m_width;
	int m_height;

	int64_t m_currentTime;
	int64_t m_startTime;
};