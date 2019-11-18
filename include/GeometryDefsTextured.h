#pragma once

#include "BGFXUtils.h"
#include "GeometryDefs.h"

#include <bx/bx.h>
#include <bgfx/bgfx.h>

#include <vector>
#include <array>

struct SVertexTexturedLayout
{
	bgfx::VertexLayout ms_layout;

	SVertexTexturedLayout()
	{
		ms_layout
			.begin()
			.add(bgfx::Attrib::Position, 3, bgfx::AttribType::Float)
			.add(bgfx::Attrib::Normal, 4, bgfx::AttribType::Uint8, true, true)
			.add(bgfx::Attrib::Tangent, 4, bgfx::AttribType::Uint8, true, true)
			.add(bgfx::Attrib::TexCoord0, 2, bgfx::AttribType::Int16, true, true)
			.end();
	}
};

struct PosNormalTangentTexcoordVertex
{
	float m_x;
	float m_y;
	float m_z;
	uint32_t m_normal;
	uint32_t m_tangent;
	int16_t m_u;
	int16_t m_v;
};

struct STexturedCubeGeometry : public SGeometry
{
	std::array<PosNormalTangentTexcoordVertex, 24> m_vertices;
	std::array<uint16_t, 36> m_triList;

	STexturedCubeGeometry(SVertexTexturedLayout& layout)
	{
		m_vertices =
		{
			{{ -1.0f,  1.0f,  1.0f, BGFXUtils::EncodeNormalRgba8(0.0f,  0.0f,  1.0f), 0,      0,      0 },
		{ 1.0f,  1.0f,  1.0f, BGFXUtils::EncodeNormalRgba8(0.0f,  0.0f,  1.0f), 0, 0x7fff,      0 },
		{ -1.0f, -1.0f,  1.0f, BGFXUtils::EncodeNormalRgba8(0.0f,  0.0f,  1.0f), 0,      0, 0x7fff },
		{ 1.0f, -1.0f,  1.0f, BGFXUtils::EncodeNormalRgba8(0.0f,  0.0f,  1.0f), 0, 0x7fff, 0x7fff },
		{ -1.0f,  1.0f, -1.0f, BGFXUtils::EncodeNormalRgba8(0.0f,  0.0f, -1.0f), 0,      0,      0 },
		{ 1.0f,  1.0f, -1.0f, BGFXUtils::EncodeNormalRgba8(0.0f,  0.0f, -1.0f), 0, 0x7fff,      0 },
		{ -1.0f, -1.0f, -1.0f, BGFXUtils::EncodeNormalRgba8(0.0f,  0.0f, -1.0f), 0,      0, 0x7fff },
		{ 1.0f, -1.0f, -1.0f, BGFXUtils::EncodeNormalRgba8(0.0f,  0.0f, -1.0f), 0, 0x7fff, 0x7fff },
		{ -1.0f,  1.0f,  1.0f, BGFXUtils::EncodeNormalRgba8(0.0f,  1.0f,  0.0f), 0,      0,      0 },
		{ 1.0f,  1.0f,  1.0f, BGFXUtils::EncodeNormalRgba8(0.0f,  1.0f,  0.0f), 0, 0x7fff,      0 },
		{ -1.0f,  1.0f, -1.0f, BGFXUtils::EncodeNormalRgba8(0.0f,  1.0f,  0.0f), 0,      0, 0x7fff },
		{ 1.0f,  1.0f, -1.0f, BGFXUtils::EncodeNormalRgba8(0.0f,  1.0f,  0.0f), 0, 0x7fff, 0x7fff },
		{ -1.0f, -1.0f,  1.0f, BGFXUtils::EncodeNormalRgba8(0.0f, -1.0f,  0.0f), 0,      0,      0 },
		{ 1.0f, -1.0f,  1.0f, BGFXUtils::EncodeNormalRgba8(0.0f, -1.0f,  0.0f), 0, 0x7fff,      0 },
		{ -1.0f, -1.0f, -1.0f, BGFXUtils::EncodeNormalRgba8(0.0f, -1.0f,  0.0f), 0,      0, 0x7fff },
		{ 1.0f, -1.0f, -1.0f, BGFXUtils::EncodeNormalRgba8(0.0f, -1.0f,  0.0f), 0, 0x7fff, 0x7fff },
		{ 1.0f, -1.0f,  1.0f, BGFXUtils::EncodeNormalRgba8(1.0f,  0.0f,  0.0f), 0,      0,      0 },
		{ 1.0f,  1.0f,  1.0f, BGFXUtils::EncodeNormalRgba8(1.0f,  0.0f,  0.0f), 0, 0x7fff,      0 },
		{ 1.0f, -1.0f, -1.0f, BGFXUtils::EncodeNormalRgba8(1.0f,  0.0f,  0.0f), 0,      0, 0x7fff },
		{ 1.0f,  1.0f, -1.0f, BGFXUtils::EncodeNormalRgba8(1.0f,  0.0f,  0.0f), 0, 0x7fff, 0x7fff },
		{ -1.0f, -1.0f,  1.0f, BGFXUtils::EncodeNormalRgba8(-1.0f,  0.0f,  0.0f), 0,      0,      0 },
		{ -1.0f,  1.0f,  1.0f, BGFXUtils::EncodeNormalRgba8(-1.0f,  0.0f,  0.0f), 0, 0x7fff,      0 },
		{ -1.0f, -1.0f, -1.0f, BGFXUtils::EncodeNormalRgba8(-1.0f,  0.0f,  0.0f), 0,      0, 0x7fff },
		{ -1.0f,  1.0f, -1.0f, BGFXUtils::EncodeNormalRgba8(-1.0f,  0.0f,  0.0f), 0, 0x7fff, 0x7fff }}
		};

		m_triList =
		{
			0,  2,  1,
			1,  2,  3,
			4,  5,  6,
			5,  7,  6,

			8, 10,  9,
			9, 10, 11,
			12, 13, 14,
			13, 15, 14,

			16, 18, 17,
			17, 18, 19,
			20, 21, 22,
			21, 23, 22,
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

	~STexturedCubeGeometry()
	{
		bgfx::destroy(m_ibh);
		bgfx::destroy(m_vbh);
	}
};

struct STexturedPlaneGeometry : public SGeometry
{
	std::array<PosNormalTangentTexcoordVertex, 4> m_vertices;
	std::array<uint16_t, 6> m_triList;

	STexturedPlaneGeometry(SVertexTexturedLayout& layout)
	{
		m_vertices =
		{
			{ { -1.0f,  1.0f,  0.0f, BGFXUtils::EncodeNormalRgba8(0.0f,  0.0f,  1.0f), 0,      0,      0 },
		{ 1.0f,  1.0f,  0.0f, BGFXUtils::EncodeNormalRgba8(0.0f,  0.0f,  1.0f), 0, 0x7fff,      0 },
		{ -1.0f, -1.0f,  0.0f, BGFXUtils::EncodeNormalRgba8(0.0f,  0.0f,  1.0f), 0,      0, 0x7fff },
		{ 1.0f, -1.0f,  0.0f, BGFXUtils::EncodeNormalRgba8(0.0f,  0.0f,  1.0f), 0, 0x7fff, 0x7fff }
		} };

		m_triList =
		{
			0,  2,  1,
			1,  2,  3,
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

	~STexturedPlaneGeometry()
	{
		bgfx::destroy(m_ibh);
		bgfx::destroy(m_vbh);
	}
};
