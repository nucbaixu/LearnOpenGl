#include "TestAllGLDemon.h"
#include "CWindow.h"


int GetStart()
{
	std::string windname("GetStartGL");
	CWindowGL *glwind = new CWindowGL(600, 800, windname);
	glwind->RunGL();
	delete glwind;
	return 0;
}