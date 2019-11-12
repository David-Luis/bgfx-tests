#include "include/CHelloWorldApplication.h"
#include "include/CShadersTestApplication.h"
#include "include/CRenderToTextureApplication.h"

#include <memory>

int main(int argc, char **argv)
{
	//std::unique_ptr<CGLFWBGFXApplication> application = std::make_unique<CHelloWorldApplication>(1024, 768);
	//std::unique_ptr<CGLFWBGFXApplication> application = std::make_unique<CShadersTestApplication>(1024, 768);
	std::unique_ptr<CRenderToTextureApplication> application = std::make_unique<CRenderToTextureApplication>(1024, 768);
	application->Initialize();
	application->RenderLoop();
	return 0;
}
