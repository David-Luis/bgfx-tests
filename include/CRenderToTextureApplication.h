#pragma once

#include "CGLFWBGFXApplication.h"
#include "GeometryDefsTextured.h"

#include <memory>

class CRenderToTextureApplication : public CGLFWBGFXApplication
{
public:
	CRenderToTextureApplication(int width, int height);
	~CRenderToTextureApplication() override;

protected:
	void OnInitialize() override;
	void OnRender(float dt) override;

private:
	std::unique_ptr<SGeometry> geometry;
	bgfx::TextureHandle m_textureColor;
	bgfx::TextureHandle m_textureNormal;

	bgfx::ProgramHandle m_program;
	bgfx::UniformHandle m_timeUniform;
	bgfx::UniformHandle m_texColorUniform;
	bgfx::UniformHandle m_texNormalUniform;
	bgfx::UniformHandle m_lightPosRadiusUniform;
	bgfx::UniformHandle m_lightRgbInnerRUniform;

	uint16_t m_numLights;
};