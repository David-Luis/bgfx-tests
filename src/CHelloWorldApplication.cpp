#include "../include/CHelloWorldApplication.h"

#include "../include/BGFXUtils.h"

CHelloWorldApplication::~CHelloWorldApplication()
{
	geometry.reset();
	bgfx::destroy(m_program);
}

void CHelloWorldApplication::OnInitialize()
{
	SVertexLayout vertexLayout;
	geometry = std::make_unique<SPlaneGeometry>(vertexLayout);

	m_program = BGFXUtils::LoadProgram(GetFileReader(), "vs_cubes", "fs_cubes");
}

void CHelloWorldApplication::OnRender()
{
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

	// Submit primitive for rendering to view 0.
	bgfx::submit(0, m_program);
}
