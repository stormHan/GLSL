

#ifndef WIN32
#include <unistd.h>
#endif
#include <stdio.h>
#include <GL/glew.h>
#include <GL/freeglut.h>

#include "Util_han.h"
#include "Glut_backend.h"

// Points to the object implementing the ICallbacks interface which was delivered to
// GLUTBackendRun(). All events are forwarded to this object.
static ICallbacks* s_pCallbacks = NULL;

static bool sWithDepth = false;
static bool sWithStencil = false;

HAN_KEY GLUTKeyToHANKey(uint Key)
{
	switch (Key) {
	case GLUT_KEY_F1:
		return HAN_KEY_F1;
	case GLUT_KEY_F2:
		return HAN_KEY_F2;
	case GLUT_KEY_F3:
		return HAN_KEY_F3;
	case GLUT_KEY_F4:
		return HAN_KEY_F4;
	case GLUT_KEY_F5:
		return HAN_KEY_F5;
	case GLUT_KEY_F6:
		return HAN_KEY_F6;
	case GLUT_KEY_F7:
		return HAN_KEY_F7;
	case GLUT_KEY_F8:
		return HAN_KEY_F8;
	case GLUT_KEY_F9:
		return HAN_KEY_F9;
	case GLUT_KEY_F10:
		return HAN_KEY_F10;
	case GLUT_KEY_F11:
		return HAN_KEY_F11;
	case GLUT_KEY_F12:
		return HAN_KEY_F12;
	case GLUT_KEY_LEFT:
		return HAN_KEY_LEFT;
	case GLUT_KEY_UP:
		return HAN_KEY_UP;
	case GLUT_KEY_RIGHT:
		return HAN_KEY_RIGHT;
	case GLUT_KEY_DOWN:
		return HAN_KEY_DOWN;
	case GLUT_KEY_PAGE_UP:
		return HAN_KEY_PAGE_UP;
	case GLUT_KEY_PAGE_DOWN:
		return HAN_KEY_PAGE_DOWN;
	case GLUT_KEY_HOME:
		return HAN_KEY_HOME;
	case GLUT_KEY_END:
		return HAN_KEY_END;
	case GLUT_KEY_INSERT:
		return HAN_KEY_INSERT;
	case GLUT_KEY_DELETE:
		return HAN_KEY_DELETE;
	default:
		OGLDEV_ERROR("Unimplemented GLUT key");
		exit(1);
	}

	return HAN_KEY_UNDEFINED;
}


static HAN_MOUSE GLUTMouseToHANMouse(uint Button)
{
	switch (Button) {
	case GLUT_LEFT_BUTTON:
		return HAN_MOUSE_BUTTON_LEFT;
	case GLUT_RIGHT_BUTTON:
		return HAN_MOUSE_BUTTON_RIGHT;
	case GLUT_MIDDLE_BUTTON:
		return HAN_MOUSE_BUTTON_MIDDLE;
	default:
		OGLDEV_ERROR("Unimplemented GLUT mouse button");
	}

	return HAN_MOUSE_UNDEFINED;
}


static void SpecialKeyboardCB(int Key, int x, int y)
{
	HAN_KEY HANKey = GLUTKeyToHANKey(Key);
	s_pCallbacks->KeyboardCB(HANKey);
}


static void KeyboardCB(unsigned char Key, int x, int y)
{

	if (
		((Key >= '+') && (Key <= '9')) ||
		((Key >= 'A') && (Key <= 'Z')) ||
		((Key >= 'a') && (Key <= 'z'))
		) {
		HAN_KEY HANKey = (HAN_KEY)Key;
		s_pCallbacks->KeyboardCB(HANKey);
	}
	else {
		printf("%d\n", Key);
		OGLDEV_ERROR("Unimplemented GLUT key");
	}

}


static void PassiveMouseCB(int x, int y)
{
	s_pCallbacks->PassiveMouseCB(x, y);
}


static void RenderSceneCB()
{
	s_pCallbacks->RenderSceneCB();
}


static void IdleCB()
{
	s_pCallbacks->RenderSceneCB();
}


static void MouseCB(int Button, int State, int x, int y)
{
	HAN_MOUSE HANMouse = GLUTMouseToHANMouse(Button);
	HAN_KEY_STATE HANKeyState = (State == GLUT_DOWN) ? HAN_KEY_STATE_PRESS : HAN_KEY_STATE_RELEASE;

	s_pCallbacks->MouseCB(HANMouse, HANKeyState, x, y);
}


static void InitCallbacks()
{
	glutDisplayFunc(RenderSceneCB);
	glutIdleFunc(IdleCB);
	glutSpecialFunc(SpecialKeyboardCB);
	glutPassiveMotionFunc(PassiveMouseCB);
	glutKeyboardFunc(KeyboardCB);
	glutMouseFunc(MouseCB);
}


void GLUTBackendInit(int argc, char** argv, bool WithDepth, bool WithStencil)
{
	sWithDepth = WithDepth;
	sWithStencil = WithStencil;

	glutInit(&argc, argv);

	uint DisplayMode = GLUT_DOUBLE | GLUT_RGBA;

	if (WithDepth) {
		DisplayMode |= GLUT_DEPTH;
	}

	if (WithStencil) {
		DisplayMode |= GLUT_STENCIL;
	}

	glutInitDisplayMode(DisplayMode);

	glutSetOption(GLUT_ACTION_ON_WINDOW_CLOSE, GLUT_ACTION_GLUTMAINLOOP_RETURNS);
}


bool GLUTBackendCreateWindow(unsigned int Width, unsigned int Height, bool isFullScreen, const char* pTitle)
{
	if (isFullScreen) {
		char ModeString[64] = { 0 };
		int bpp = 32;
		SNPRINTF(ModeString, sizeof(ModeString), "%dx%d:%d@60", Width, Height, bpp);
		glutGameModeString(ModeString);
		glutEnterGameMode();
	}
	else {
		glutInitWindowSize(Width, Height);
		glutCreateWindow(pTitle);
	}

	// Must be done after glut is initialized!
	GLenum res = glewInit();
	if (res != GLEW_OK) {
		fprintf(stderr, "Error: '%s'\n", glewGetErrorString(res));
		return false;
	}

	return true;
}

void GLUTBackendRun(ICallbacks* pCallbacks)
{
	if (!pCallbacks) {
		fprintf(stderr, "%s : callbacks not specified!\n", __FUNCTION__);
		return;
	}

	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
	glFrontFace(GL_CW);
	glCullFace(GL_BACK);
	glEnable(GL_CULL_FACE);

	if (sWithDepth) {
		glEnable(GL_DEPTH_TEST);
	}

	s_pCallbacks = pCallbacks;
	InitCallbacks();
	glutMainLoop();
}


void GLUTBackendSwapBuffers()
{
	glutSwapBuffers();
}

void GLUTBackendLeaveMainLoop()
{
	glutLeaveMainLoop();
}