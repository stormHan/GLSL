
#ifndef GLUT_BACKEND_H
#define	GLUT_BACKEND_H

#include "Types_han.h"
#include "Callbacks_han.h"

void GLUTBackendInit(int argc, char** argv, bool WithDepth, bool WithStencil);


bool GLUTBackendCreateWindow(uint Width, uint Height, bool isFullScreen, const char* pTitle);

void GLUTBackendRun(ICallbacks* pCallbacks);

void GLUTBackendSwapBuffers();

void GLUTBackendLeaveMainLoop();

HAN_KEY GLUTKeyToHANKey(uint Key);

#endif	/* GLUT_BACKEND_H */

