#include "../include/CRenderToTextureApplication.h"

#include "../include/BGFXUtils.h"

#include <bx/math.h>

CRenderToTextureApplication::CRenderToTextureApplication(int width, int height)
	: CGLFWBGFXApplication(width, height)
{
}

CRenderToTextureApplication::~CRenderToTextureApplication()
{
	geometry.reset();
	bgfx::destroy(m_program);
	bgfx::destroy(m_timeUniform);
	bgfx::destroy(m_textureColor);
	bgfx::destroy(m_textureNormal);
	bgfx::destroy(m_texColorUniform);
	bgfx::destroy(m_texNormalUniform);
	bgfx::destroy(m_lightPosRadiusUniform);
	bgfx::destroy(m_lightRgbInnerRUniform);
}

void CRenderToTextureApplication::OnInitialize()
{
	SVertexTexturedLayout vertexLayout;
	geometry = std::make_unique<STexturedCubeGeometry>(vertexLayout);

	m_timeUniform = bgfx::createUniform("u_time", bgfx::UniformType::Vec4);
	m_texColorUniform = bgfx::createUniform("s_texColor", bgfx::UniformType::Sampler);
	m_texNormalUniform = bgfx::createUniform("s_texNormal", bgfx::UniformType::Sampler);

	m_numLights = 4;
	m_lightPosRadiusUniform = bgfx::createUniform("u_lightPosRadius", bgfx::UniformType::Vec4, m_numLights);
	m_lightRgbInnerRUniform = bgfx::createUniform("u_lightRgbInnerR", bgfx::UniformType::Vec4, m_numLights);

	m_program = BGFXUtils::LoadProgram(GetFileReader(), "textured_vs", "textured_fs");

	// Load diffuse texture.
	m_textureColor = BGFXUtils::LoadTexture(GetAllocator(), GetFileReader(), "textures/fieldstone-rgba.dds");

	// Load normal texture.
	m_textureNormal = BGFXUtils::LoadTexture(GetAllocator(), GetFileReader(), "textures/fieldstone-n.dds");
}

void CRenderToTextureApplication::OnRender(float /*dt*/)
{
	const bx::Vec3 at = { 0.0f, 0.0f,   0.0f };
	const bx::Vec3 eye = { 0.0f, 0.0f, -10.0f };

	// Set view and projection matrix for view 0.
	{
		float view[16];
		bx::mtxLookAt(view, eye, at);

		float proj[16];
		bx::mtxProj(proj, 60.0f, float(GetWidth()) / float(GetHeight()), 0.1f, 100.0f, bgfx::getCaps()->homogeneousDepth);
		bgfx::setViewTransform(0, view, proj);

		// Set view 0 default viewport.
		bgfx::setViewRect(0, 0, 0, uint16_t(GetWidth()), uint16_t(GetHeight()));
	}

	uint64_t state = 0
		| BGFX_STATE_WRITE_RGB
		| BGFX_STATE_WRITE_A
		| BGFX_STATE_WRITE_Z
		| BGFX_STATE_DEPTH_TEST_LESS
		| BGFX_STATE_MSAA
		;

	// Set vertex and index buffer.
	bgfx::setVertexBuffer(0, geometry->m_vbh);
	bgfx::setIndexBuffer(geometry->m_ibh);

	// Bind textures.
	bgfx::setTexture(0, m_texColorUniform, m_textureColor);
	bgfx::setTexture(1, m_texNormalUniform, m_textureNormal);

	// Set render states.
	bgfx::setState(state);

	float time[4] = { GetTotalTime(), 0.f, 0.f, 0.f};
	bgfx::setUniform(m_timeUniform, time);

	// Submit primitive for rendering to view 0.
	bgfx::submit(0, m_program);
}
