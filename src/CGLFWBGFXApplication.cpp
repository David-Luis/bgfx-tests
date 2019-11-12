#include "../include/CGLFWBGFXApplication.h"

#include <stdio.h>
#include <bx/bx.h>
#include <bx/file.h>
#include <bx/timer.h>
#include <bgfx/bgfx.h>
#include <bgfx/platform.h>
#include <GLFW/glfw3.h>
#if BX_PLATFORM_LINUX
#define GLFW_EXPOSE_NATIVE_X11
#elif BX_PLATFORM_WINDOWS
#define GLFW_EXPOSE_NATIVE_WIN32
#endif
#include <GLFW/glfw3native.h>

namespace GLFWBGFXApplicationInternal
{
	bool s_showStats = false;
	constexpr bgfx::ViewId kClearView = 0;

	void glfw_keyCallback(GLFWwindow *window, int key, int scancode, int action, int mods)
	{
		if (key == GLFW_KEY_F1 && action == GLFW_RELEASE)
			s_showStats = !s_showStats;
	}

	void glfw_errorCallback(int error, const char *description)
	{
		fprintf(stderr, "GLFW error %d: %s\n", error, description);
	}
}

bx::DefaultAllocator CGLFWBGFXApplication::s_allocator;

CGLFWBGFXApplication::CGLFWBGFXApplication(int width, int height)
	: m_width(width)
	, m_height(height)
{
}

CGLFWBGFXApplication::~CGLFWBGFXApplication()
{
	bgfx::shutdown();
}

void CGLFWBGFXApplication::Initialize()
{
	InitGraphics();
	InitSystems();

	m_startTime = bx::getHPCounter();
	m_currentTime = m_startTime;

	OnInitialize();
}

void CGLFWBGFXApplication::RenderLoop()
{
	while (!glfwWindowShouldClose(m_window)) {
		glfwPollEvents();
		// Handle window resize.
		int oldWidth = m_width, oldHeight = m_height;
		glfwGetWindowSize(m_window, &m_width, &m_height);
		if (m_width != oldWidth || m_height != oldHeight) {
			bgfx::reset((uint32_t)m_width, (uint32_t)m_height, BGFX_RESET_VSYNC);
			bgfx::setViewRect(GLFWBGFXApplicationInternal::kClearView, 0, 0, bgfx::BackbufferRatio::Equal);
		}
		// This dummy draw call is here to make sure that view 0 is cleared if no other draw calls are submitted to view 0.
		bgfx::touch(GLFWBGFXApplicationInternal::kClearView);

		bgfx::dbgTextClear();
		bgfx::dbgTextPrintf(0, 0, 0x0f, "Press F1 to toggle stats.");
		const bgfx::Stats* stats = bgfx::getStats();
		bgfx::dbgTextPrintf(0, 1, 0x0f, "Backbuffer %dW x %dH in pixels, debug text %dW x %dH in characters.", stats->width, stats->height, stats->textWidth, stats->textHeight);

		float time = (float)((bx::getHPCounter() - m_currentTime) / double(bx::getHPFrequency()));
		m_currentTime = bx::getHPCounter();
		OnRender(time);

		// Enable stats or debug text.
		bgfx::setDebug(GLFWBGFXApplicationInternal::s_showStats ? BGFX_DEBUG_STATS : BGFX_DEBUG_TEXT);
		// Advance to next frame. Process submitted rendering primitives.
		bgfx::frame();
	}
}

void CGLFWBGFXApplication::InitGraphics()
{
	// Create a GLFW window without an OpenGL context.
	glfwSetErrorCallback(GLFWBGFXApplicationInternal::glfw_errorCallback);
	if (!glfwInit())
		return;
	glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
	m_window = glfwCreateWindow(m_width, m_height, "helloworld", nullptr, nullptr);
	if (!m_window)
		return;

	glfwSetWindowUserPointer(m_window, this);

	glfwSetKeyCallback(m_window, GLFWBGFXApplicationInternal::glfw_keyCallback);

	// Call bgfx::renderFrame before bgfx::init to signal to bgfx not to create a render thread.
	// Most graphics APIs must be used on the same thread that created the window.
	bgfx::renderFrame();
	// Initialize bgfx using the native window handle and window resolution.
	bgfx::Init init;
#if BX_PLATFORM_LINUX || BX_PLATFORM_BSD
	init.platformData.ndt = glfwGetX11Display();
	init.platformData.nwh = (void*)(uintptr_t)glfwGetX11Window(window);
#elif BX_PLATFORM_OSX
	init.platformData.nwh = glfwGetCocoaWindow(m_window);
#elif BX_PLATFORM_WINDOWS
	init.platformData.nwh = glfwGetWin32Window(m_window);
#endif

	glfwGetWindowSize(m_window, &m_width, &m_height);
	init.resolution.width = (uint32_t)m_width;
	init.resolution.height = (uint32_t)m_height;
	init.resolution.reset = BGFX_RESET_VSYNC;
	if (!bgfx::init(init))
		return;
	// Set view 0 to the same dimensions as the window and to clear the color buffer.
	bgfx::setViewClear(GLFWBGFXApplicationInternal::kClearView, BGFX_CLEAR_COLOR | BGFX_CLEAR_DEPTH, 0x443355FF, 1.0f, 0);
	bgfx::setViewRect(GLFWBGFXApplicationInternal::kClearView, 0, 0, bgfx::BackbufferRatio::Equal);
}

void CGLFWBGFXApplication::InitSystems()
{
	m_fileReader = BX_NEW(GetAllocator(), bx::FileReader);
}

bx::FileReaderI* CGLFWBGFXApplication::GetFileReader()
{
	return m_fileReader;
}

float CGLFWBGFXApplication::GetTotalTime()
{
	return (float)((bx::getHPCounter() - m_startTime) / double(bx::getHPFrequency()));
}