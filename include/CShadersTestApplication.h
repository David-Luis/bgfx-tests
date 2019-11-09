#pragma once

#include "CGLFWBGFXApplication.h"
#include "GeometryDefs.h"

#include <memory>

class CShadersTestApplication : public CGLFWBGFXApplication
{
public:
	CShadersTestApplication(int width, int height);
	~CShadersTestApplication() override;

protected:
	void OnInitialize() override;
	void OnRender(float dt) override;

private:
	std::unique_ptr<SGeometry> geometry;
	bgfx::ProgramHandle m_program;
	bgfx::UniformHandle m_dataUniform;
};