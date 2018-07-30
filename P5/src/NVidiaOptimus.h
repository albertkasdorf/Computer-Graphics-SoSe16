#pragma once

/*
Global Variable NvOptimusEnablement( new in Driver Release 302 )

Starting with the Release 302 drivers, application developers can direct the
Optimus driver at runtime to use the High Performance Graphics to render any
application even those applications for which there is no existing application
profile.They can do this by exporting a global variable named
NvOptimusEnablement.The Optimus driver looks for the existence and value of the
export.Only the LSB of the DWORD matters at this time.A value of 0x00000001
indicates that rendering should be performed using High Performance Graphics.
A value of 0x00000000 indicates that this method should be ignored.

http://developer.download.nvidia.com/devzone/devcenter/gamegraphics/files/OptimusRenderingPolicies.pdf
http://stackoverflow.com/questions/16154921/how-to-create-an-opengl-context-with-a-specific-graphics-driver

*/

extern "C" {
	_declspec(dllexport) unsigned long NvOptimusEnablement = 0x00000001;
}
