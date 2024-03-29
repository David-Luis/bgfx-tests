#pragma once

#include "CGLFWBGFXApplication.h"
#include "GeometryDefs.h"

#include <memory>

class CHelloWorldApplication : public CGLFWBGFXApplication
{
public:
	CHelloWorldApplication(int width, int height);
	~CHelloWorldApplication() override;

protected:
	void OnInitialize() override;
	void OnRender(float dt) override;

private:
	std::unique_ptr<SGeometry> geometry;
	bgfx::ProgramHandle m_program;
	bgfx::UniformHandle m_timeUniform;
};