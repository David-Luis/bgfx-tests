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
	void InitializeScene();
	void InitializeRenderToTexture();
	void InitializeUI();

	void RenderSceneToTexture();
	void RenderToBackbuffer();
	void RenderPostProcessScene(bgfx::ViewId viewId);
	void RenderSimulatedUI(bgfx::ViewId viewId);

	std::unique_ptr<SGeometry> m_geometry;
	bgfx::TextureHandle m_textureColorTextureHandle;
	bgfx::TextureHandle m_textureNormalTextureHandle;

	bgfx::FrameBufferHandle m_frameBufferHandle;
	bgfx::TextureHandle m_frameBufferTextureHandle;

	bgfx::ProgramHandle m_programScene;
	bgfx::UniformHandle m_timeUniform;
	bgfx::UniformHandle m_texColorUniform;
	bgfx::UniformHandle m_texNormalUniform;
	bgfx::UniformHandle m_lightPosRadiusUniform;
	bgfx::UniformHandle m_lightRgbInnerRUniform;

	uint16_t m_numLights;


	//render to texture
	bgfx::UniformHandle m_dataUniform;
	std::unique_ptr<SGeometry> m_viewGeometry;
	bgfx::ProgramHandle m_programPostProcess;
	bgfx::UniformHandle m_texColorUniformPostProccess;


	//"UI"
	bgfx::ProgramHandle m_programUI;
};