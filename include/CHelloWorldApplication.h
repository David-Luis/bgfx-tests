#pragma once

#include "CGLFWBGFXApplication.h"
#include "GeometryDefs.h"

#include <memory>

class CHelloWorldApplication : public CGLFWBGFXApplication
{
public:
	~CHelloWorldApplication() override;

protected:
	void OnInitialize() override;
	void OnRender() override;

private:
	std::unique_ptr<SGeometry> geometry;
	bgfx::ProgramHandle m_program;
};