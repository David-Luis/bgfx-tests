#pragma once

struct GLFWwindow;

namespace bx
{
	struct FileReaderI;
	struct AllocatorI;
}

class CGLFWBGFXApplication
{
public:
	CGLFWBGFXApplication();
	virtual ~CGLFWBGFXApplication();
	CGLFWBGFXApplication(const CGLFWBGFXApplication&) = delete;
	CGLFWBGFXApplication& operator=(const CGLFWBGFXApplication&) = delete;

	void Initialize();
	void RenderLoop();

	bx::FileReaderI* GetFileReader();

protected:
	virtual void OnInitialize() = 0;
	virtual void OnRender() = 0;

private:
	void InitGraphics();
	void InitSystems();

	bx::AllocatorI* m_allocator;
	bx::FileReaderI* m_fileReader;

	GLFWwindow * m_window;
	int m_width;
	int m_height;
};