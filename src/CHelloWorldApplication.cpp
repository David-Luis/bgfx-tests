#include "../include/CHelloWorldApplication.h"

#include "../include/BGFXUtils.h"

#include <bx/math.h>

CHelloWorldApplication::CHelloWorldApplication(int width, int height)
	: CGLFWBGFXApplication(width, height)
{
}

CHelloWorldApplication::~CHelloWorldApplication()
{
	geometry.reset();
	bgfx::destroy(m_program);
	bgfx::destroy(m_timeUniform);
}

void CHelloWorldApplication::OnInitialize()
{
	SVertexLayout vertexLayout;
	geometry = std::make_unique<SCubeGeometry>(vertexLayout);

	m_timeUniform = bgfx::createUniform("u_time", bgfx::UniformType::Vec4);
	m_program = BGFXUtils::LoadProgram(GetFileReader(), "time_color_vs", "time_color_fs");
}

void CHelloWorldApplication::OnRender(float /*dt*/)
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
		| BGFX_STATE_WRITE_R
		| BGFX_STATE_WRITE_G
		| BGFX_STATE_WRITE_B
		| BGFX_STATE_WRITE_A
		| BGFX_STATE_WRITE_Z
		| BGFX_STATE_DEPTH_TEST_LESS
		| BGFX_STATE_CULL_CW
		| BGFX_STATE_MSAA
		;

	// Set vertex and index buffer.
	bgfx::setVertexBuffer(0, geometry->m_vbh);
	bgfx::setIndexBuffer(geometry->m_ibh);

	// Set render states.
	bgfx::setState(state);

	float time[4] = { GetTotalTime(), 0.f, 0.f, 0.f};
	bgfx::setUniform(m_timeUniform, time);

	// Submit primitive for rendering to view 0.
	bgfx::submit(0, m_program);
}
