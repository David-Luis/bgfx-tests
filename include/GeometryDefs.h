#pragma once

#include <vector>
#include <array>

#include <bx/bx.h>
#include <bgfx/bgfx.h>

struct SVertexLayout
{
	bgfx::VertexLayout ms_layout;

	SVertexLayout()
	{
		ms_layout
			.begin()
			.add(bgfx::Attrib::Position, 3, bgfx::AttribType::Float)
			.add(bgfx::Attrib::Color0, 4, bgfx::AttribType::Uint8, true)
			.add(bgfx::Attrib::TexCoord0, 2, bgfx::AttribType::Float)
			.end();
	}
};

struct PosColorVertex
{
	float m_x;
	float m_y;
	float m_z;
	uint32_t m_abgr;
	float uv_x;
	float uv_y;
};

struct SGeometry
{
	bgfx::VertexBufferHandle m_vbh;
	bgfx::IndexBufferHandle m_ibh;
};

struct SCubeGeometry : public SGeometry
{
	std::array<PosColorVertex, 8> m_vertices;
	std::array<uint16_t, 36> m_triList;

	SCubeGeometry(SVertexLayout& layout)
	{
		m_vertices =
		{
			{{ -1.0f,  1.0f,  1.0f, 0xff000000, 0.0f, 0.0f },
		{ 1.0f,  1.0f,  1.0f, 0xff0000ff, 1.0f, 0.0f },
		{ -1.0f, -1.0f,  1.0f, 0xff00ff00, 0.0f, 1.0f },
		{ 1.0f, -1.0f,  1.0f, 0xff00ffff , 1.0f, 1.0f},
		{ -1.0f,  1.0f, -1.0f, 0xffff0000, 1.0f, 0.0f },
		{ 1.0f,  1.0f, -1.0f, 0xffff00ff, 0.0f, 0.0f },
		{ -1.0f, -1.0f, -1.0f, 0xffffff00, 1.0f, 1.0f },
		{ 1.0f, -1.0f, -1.0f, 0xffffffff, 0.0f, 1.0f },
		} };

		m_triList =
		{
			0, 1, 2, // 0
			1, 3, 2,
			4, 6, 5, // 2
			5, 6, 7,
			0, 2, 4, // 4
			4, 2, 6,
			1, 5, 3, // 6
			5, 7, 3,
			0, 4, 1, // 8
			4, 5, 1,
			2, 3, 6, // 10
			6, 3, 7,
		};

		m_vbh = bgfx::createVertexBuffer(
			// Static data can be passed with bgfx::makeRef
			bgfx::makeRef(m_vertices.data(), sizeof(m_vertices))
			, layout.ms_layout
		);

		// Create static index buffer for triangle list rendering.
		m_ibh = bgfx::createIndexBuffer(
			// Static data can be passed with bgfx::makeRef
			bgfx::makeRef(m_triList.data(), sizeof(m_triList))
		);
	}

	~SCubeGeometry()
	{
		bgfx::destroy(m_ibh);
		bgfx::destroy(m_vbh);
	}
};

struct SPlaneGeometry : public SGeometry
{
	std::array<PosColorVertex, 4> m_vertices;
	std::array<uint16_t, 6> m_triList;

	SPlaneGeometry(SVertexLayout& layout)
	{
		m_vertices =
		{
			{ { -1.0f,  1.0f,  1.0f, 0xffffffff, 0.0f, 1.0f },
			  { 1.0f,  1.0f,  1.0f, 0xffffffff, 1.0f, 1.0f },
			  { -1.0f, -1.0f,  1.0f, 0xffffffff, 0.0f, 0.0f },
			  { 1.0f, -1.0f,  1.0f, 0xffffffff, 1.0f, 0.0f },
			} };

		m_triList =
		{
			2, 1, 0, // 0
			2, 3, 1
		};

		m_vbh = bgfx::createVertexBuffer(
			// Static data can be passed with bgfx::makeRef
			bgfx::makeRef(m_vertices.data(), sizeof(m_vertices))
			, layout.ms_layout
		);

		// Create static index buffer for triangle list rendering.
		m_ibh = bgfx::createIndexBuffer(
			// Static data can be passed with bgfx::makeRef
			bgfx::makeRef(m_triList.data(), sizeof(m_triList))
		);
	}

	~SPlaneGeometry()
	{
		bgfx::destroy(m_ibh);
		bgfx::destroy(m_vbh);
	}
};

struct SSphereGeometry : public SGeometry
{
	static constexpr uint32_t segments = 100;
	std::array<PosColorVertex, (segments + 1)*(segments + 1)> m_vertices;
	std::array<uint16_t, segments*segments * 6> m_triList;

	SSphereGeometry(SVertexLayout& layout)
	{
		constexpr float PI = 3.1415926f;

		size_t index = 0;
		for (int y = 0; y <= segments; y++) {
			for (int x = 0; x <= segments; x++) {
				float xSegment = (float)x / (float)segments;
				float ySegment = (float)y / (float)segments;
				float xPos = cos(xSegment * 2.0f * PI) * sin(ySegment * PI);
				float yPos = cos(ySegment * PI);
				float zPos = sin(xSegment * 2.0f * PI) * sin(ySegment * PI);
				m_vertices[index++] = { xPos, yPos, zPos, 0xff0000ff };
			}
		}

		index = 0;
		for (int i = 0; i < segments; i++) {
			for (int j = 0; j < segments; j++) {
				m_triList[index++] = (i * (segments + 1) + j);
				m_triList[index++] = ((i + 1) * (segments + 1) + j);
				m_triList[index++] = ((i + 1) * (segments + 1) + j + 1);
				m_triList[index++] = (i * (segments + 1) + j);
				m_triList[index++] = ((i + 1) * (segments + 1) + j + 1);
				m_triList[index++] = (i * (segments + 1) + j + 1);
			}
		}

		m_vbh = bgfx::createVertexBuffer(
			// Static data can be passed with bgfx::makeRef
			bgfx::makeRef(m_vertices.data(), sizeof(m_vertices))
			, layout.ms_layout
		);

		// Create static index buffer for triangle list rendering.
		m_ibh = bgfx::createIndexBuffer(
			// Static data can be passed with bgfx::makeRef
			bgfx::makeRef(m_triList.data(), sizeof(m_triList))
		);
	}
};