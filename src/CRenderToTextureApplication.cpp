#include "../include/CRenderToTextureApplication.h"

#include "../include/BGFXUtils.h"

#include <bx/math.h>

#include <bx/file.h>

CRenderToTextureApplication::CRenderToTextureApplication(int width, int height)
	: CGLFWBGFXApplication(width, height)
{
}

CRenderToTextureApplication::~CRenderToTextureApplication()
{
	m_geometry.reset();
	bgfx::destroy(m_programScene);
	bgfx::destroy(m_timeUniform);
	bgfx::destroy(m_textureColorTextureHandle);
	bgfx::destroy(m_textureNormalTextureHandle);
	bgfx::destroy(m_texColorUniform);
	bgfx::destroy(m_texNormalUniform);
	bgfx::destroy(m_lightPosRadiusUniform);
	bgfx::destroy(m_lightRgbInnerRUniform);
}

void CRenderToTextureApplication::OnInitialize()
{
	InitializeScene();

	InitializeRenderToTexture();

	InitializeUI();
}

void CRenderToTextureApplication::OnRender(float /*dt*/)
{
	RenderSceneToTexture();
	RenderToBackbuffer();
}

void CRenderToTextureApplication::InitializeScene()
{
	SVertexTexturedLayout vertexLayout;
	m_geometry = std::make_unique<STexturedPlaneGeometry>(vertexLayout);

	m_timeUniform = bgfx::createUniform("u_time", bgfx::UniformType::Vec4);
	m_texColorUniform = bgfx::createUniform("s_texColor", bgfx::UniformType::Sampler);
	m_texNormalUniform = bgfx::createUniform("s_texNormal", bgfx::UniformType::Sampler);

	m_numLights = 4;
	m_lightPosRadiusUniform = bgfx::createUniform("u_lightPosRadius", bgfx::UniformType::Vec4, m_numLights);
	m_lightRgbInnerRUniform = bgfx::createUniform("u_lightRgbInnerR", bgfx::UniformType::Vec4, m_numLights);

	m_programScene = BGFXUtils::LoadProgram(GetFileReader(), "textured_vs", "textured_fs");

	// Load diffuse texture.
	m_textureColorTextureHandle = BGFXUtils::LoadTexture(GetAllocator(), GetFileReader(), "textures/fieldstone-rgba.dds");

	// Load normal texture.
	m_textureNormalTextureHandle = BGFXUtils::LoadTexture(GetAllocator(), GetFileReader(), "textures/fieldstone-n.dds");
}

void CRenderToTextureApplication::InitializeRenderToTexture()
{
	bgfx::setViewClear(1, BGFX_CLEAR_COLOR | BGFX_CLEAR_DEPTH, 0x443355FF, 1.0f, 0);
	bgfx::setViewRect(1, 0, 0, bgfx::BackbufferRatio::Equal);

	constexpr bool hasMips = false;
	constexpr uint16_t numLayers = 1;

	m_frameBufferTextureHandle = bgfx::createTexture2D(GetWidth(), GetHeight(),
		hasMips, numLayers,
		bgfx::TextureFormat::BGRA8,
		BGFX_TEXTURE_RT);  // | BGFX_TEXTURE_U_CLAMP | BGFX_TEXTURE_V_CLAMP);

	m_frameBufferHandle = bgfx::createFrameBuffer(1, &m_frameBufferTextureHandle);

	SVertexTexturedLayout vertexLayout;
	m_viewGeometry = std::make_unique<STexturedPlaneGeometry>(vertexLayout);

	m_dataUniform = bgfx::createUniform("u_data", bgfx::UniformType::Vec4);
	m_texColorUniformPostProccess = bgfx::createUniform("s_texColor", bgfx::UniformType::Sampler);

	m_programPostProcess = BGFXUtils::LoadProgram(GetFileReader(), "gray_vs", "gray_fs");
}

void CRenderToTextureApplication::InitializeUI()
{
	bgfx::setViewClear(2, BGFX_CLEAR_DEPTH, 0x44335500, 1.0f, 0);
	bgfx::setViewRect(2, 0, 0, bgfx::BackbufferRatio::Equal);

	m_programUI = BGFXUtils::LoadProgram(GetFileReader(), "texture_no_light_vs", "texture_no_light_fs");
}

void CRenderToTextureApplication::RenderSceneToTexture()
{
	const bx::Vec3 at = { 0.0f, 0.0f,   0.0f };
	const bx::Vec3 eye = { 0.0f, 0.0f, -10.0f };

	// Set view and projection matrix for view 0.
	{
		float view[16];
		bx::mtxLookAt(view, eye, at);

		float proj[16];
		bx::mtxProj(proj, 60.0f, float(GetWidth()) / float(GetHeight()), 0.1f, 100.0f, bgfx::getCaps()->homogeneousDepth);
		//bx::mtxOrtho(proj, -1.f, 1.f, -1.f, 1.f, 0.1f, 100.f, 0, bgfx::getCaps()->homogeneousDepth);
		bgfx::setViewTransform(0, view, proj);

		// Set view 0 default viewport.
		bgfx::setViewRect(0, 0, 0, uint16_t(GetWidth()), uint16_t(GetHeight()));
	}

	uint64_t state = 0
		| BGFX_STATE_WRITE_RGB
		| BGFX_STATE_WRITE_A
		| BGFX_STATE_WRITE_Z
		| BGFX_STATE_MSAA
		;

	bgfx::setViewFrameBuffer(0, m_frameBufferHandle);
	
	// Set vertex and index buffer.

	for (uint32_t yy = 0; yy < 11; ++yy)
	{
		for (uint32_t xx = 0; xx < 11; ++xx)
		{
			float mtx[16];
			bx::mtxRotateXY(mtx, GetTotalTime() + xx * 0.21f, GetTotalTime() + yy * 0.37f);
			mtx[12] = -15.0f + float(xx)*3.0f;
			mtx[13] = -15.0f + float(yy)*3.0f;
			mtx[14] = 0.0f;

			// Set model matrix for rendering.
			bgfx::setTransform(mtx);

			bgfx::setVertexBuffer(0, m_geometry->m_vbh);
			bgfx::setIndexBuffer(m_geometry->m_ibh);

			// Bind textures.
			bgfx::setTexture(0, m_texColorUniform, m_textureColorTextureHandle);
			bgfx::setTexture(1, m_texNormalUniform, m_textureNormalTextureHandle);

			// Set render states.
			bgfx::setState(state);

			float time[4] = { GetTotalTime(), 0.f, 0.f, 0.f };
			bgfx::setUniform(m_timeUniform, time);


			// Submit primitive for rendering to view 0.
			bgfx::submit(0, m_programScene);
		}
	}
}

void CRenderToTextureApplication::RenderToBackbuffer()
{
	bgfx::ViewId viewIdPostProcess = 1;
	bgfx::setViewRect(viewIdPostProcess, 0, 0, bgfx::BackbufferRatio::Equal);
	bgfx::touch(viewIdPostProcess);
	RenderPostProcessScene(viewIdPostProcess);

	bgfx::ViewId viewIdUI = 2;
	RenderSimulatedUI(viewIdUI);
}

void CRenderToTextureApplication::RenderPostProcessScene(bgfx::ViewId viewId)
{
	const bx::Vec3 at = { 0.0f, 0.0f,   0.0f };
	const bx::Vec3 eye = { 0.0f, 0.0f, -10.0f };

	// Set view and projection matrix for view 0.
	{
		float view[16];
		bx::mtxLookAt(view, eye, at);

		float proj[16];
		bx::mtxOrtho(proj, -1.f, 1.f, -1.f, 1.f, 0.1f, 100.f, 0, bgfx::getCaps()->homogeneousDepth);
		bgfx::setViewTransform(viewId, view, proj);

		// Set view 0 default viewport.
		bgfx::setViewRect(viewId, 0, 0, uint16_t(GetWidth()), uint16_t(GetHeight()));
	}

	uint64_t state = 0
		| BGFX_STATE_WRITE_RGB
		| BGFX_STATE_WRITE_A
		| BGFX_STATE_WRITE_Z
		| BGFX_STATE_DEPTH_TEST_LESS
		| BGFX_STATE_MSAA
		;

	// Set vertex and index buffer.
	bgfx::setVertexBuffer(0, m_viewGeometry->m_vbh);
	bgfx::setIndexBuffer(m_viewGeometry->m_ibh);

	bgfx::setTexture(0, m_texColorUniformPostProccess, m_frameBufferTextureHandle);

	// Set render states.
	bgfx::setState(state);

	float data[4] = { GetTotalTime(), static_cast<float>(GetWidth()), static_cast<float>(GetHeight()), 0.f };
	bgfx::setUniform(m_dataUniform, data);

	// Submit primitive for rendering to view 0.
	bgfx::submit(viewId, m_programPostProcess);
}

void CRenderToTextureApplication::RenderSimulatedUI(bgfx::ViewId viewId)
{
	const bx::Vec3 at = { 0.0f, 0.0f,   0.0f };
	const bx::Vec3 eye = { 0.0f, 0.0f, -10.0f };

	// Set view and projection matrix for view 0.
	{
		float view[16];
		bx::mtxLookAt(view, eye, at);

		float proj[16];
		bx::mtxProj(proj, 60.0f, float(GetWidth()) / float(GetHeight()), 0.1f, 100.0f, bgfx::getCaps()->homogeneousDepth);
		bgfx::setViewTransform(viewId, view, proj);

		// Set view 0 default viewport.
		bgfx::setViewRect(viewId, 0, 0, uint16_t(GetWidth()), uint16_t(GetHeight()));
	}

	uint64_t state = 0
		| BGFX_STATE_WRITE_RGB
		| BGFX_STATE_WRITE_A
		| BGFX_STATE_WRITE_Z
		| BGFX_STATE_DEPTH_TEST_LESS
		| BGFX_STATE_MSAA
		;

	// Set vertex and index buffer.
	bgfx::setVertexBuffer(0, m_geometry->m_vbh);
	bgfx::setIndexBuffer(m_geometry->m_ibh);

	// Bind textures.
	bgfx::setTexture(0, m_texColorUniform, m_textureColorTextureHandle);

	// Set render states.
	bgfx::setState(state);

	float time[4] = { GetTotalTime(), 0.f, 0.f, 0.f };
	bgfx::setUniform(m_timeUniform, time);

	bgfx::submit(viewId, m_programUI);

}
