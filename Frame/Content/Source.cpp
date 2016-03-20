#include <iostream>
#include <string>
#include <GL\glew.h>
#include <glut.h>

#include "Light.h"
#include "HAN_UTIL.h"
#include "ViewControl.h"
#include "ShaderLoading.h"
#include "math_3d.h"
#include "Pipeline.h"
#include "camera.h"
#include "texture.h"
#include "ObjLoader.h"
#include "LightControl.h"

using namespace std;

#define WINDOW_WIDTH  1024
#define WINDOW_HEIGHT  768 

#pragma comment(lib, "glew32.lib")

PersProjInfo persprojection;
ObjLoader* pLoader = NULL;
Camera* pCamera = NULL;
Texture* pTexture = NULL;
Shader shader_light("Shader/light.vs", "Shader/light.fs");
//-------Light--------------------------------
Light* pLight = NULL;
DirectionLight* pDirLight = NULL;
LightControl* pLC = NULL;
//-------Buffer setting----------------
GLuint VBO, IBO, uvBuffer;
GLfloat rotate_val = 0.0f;
//-------Callback Func ----------------
static void RenderSceneCB()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	pCamera->OnRender();

	static float Scale = 0.0f;

	Scale += 0.1f;
	Pipeline p;
	p.Rotate(0.0f, Scale, 0.0f);
	p.Translate(0.0f, 0.0f, 3.0f);
	p.SetCamera(*pCamera);
	p.SetPersjection(persprojection);

	//set the world ,view, projection matrix into shader
	
	shader_light.SetWVP(p.GetWVPTrans());
	shader_light.SetgWorld(p.GetWorldTrans());

	shader_light.SetAmbientLight(*pLight);
	shader_light.SetDirectiontLight(*pDirLight);
	//bind the texture
	

	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
	glEnableVertexAttribArray(2);

	glBindBuffer(GL_ARRAY_BUFFER, pLoader->loader_vertexbuffer);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
	//glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBO);
	glBindBuffer(GL_ARRAY_BUFFER, pLoader->loader_uvbuffer);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, 0);

	glBindBuffer(GL_ARRAY_BUFFER, pLoader->loader_normalbuffer);
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 0, 0);

	pTexture->Bind(GL_TEXTURE0);

	glDrawArrays(GL_TRIANGLES, 0, pLoader->Mesh_num);
	glDisableVertexAttribArray(0);
	glDisableVertexAttribArray(1);
	glDisableVertexAttribArray(2);
	
	glFlush();
	glutSwapBuffers();
}

static void SpecialKeyboardCB(int Key, int x, int y)
{
	
}


static void MouseCB(int button, int state, int x, int y)
{
	pLC->clickMouse(button, state, x, y);
}

static void  MouseMotion(int x, int y)
{
	pCamera->OnMouse(x, y);
}

static void MouseMotionwithClick(int x, int y)
{
	pLC->onMouse(x, y);
}

static void NormalKeyboardCB(unsigned char Key, int x, int y)
{
	pCamera->OnNormalKey(Key);
}

static void InitializeGlutCallBacks()
{
	glutDisplayFunc(RenderSceneCB);
	glutIdleFunc(RenderSceneCB);
	//glutReshapeFunc(ReRenderScene);

	glutSpecialFunc(SpecialKeyboardCB);
	glutKeyboardFunc(NormalKeyboardCB);

	//click the mouse to adjust the light
	glutMotionFunc(MouseMotionwithClick); //Êó±êÒÆ¶¯¡£
	glutMouseFunc(MouseCB);

	//unclicked to control the camera
	glutPassiveMotionFunc(MouseMotion);
}

static void CreateVertexBuffer()
{
	static const GLfloat Vertices[] = {
		-1.0f, -1.0f, -1.0f,
		-1.0f, -1.0f, 1.0f,
		-1.0f, 1.0f, 1.0f,
		1.0f, 1.0f, -1.0f,
		-1.0f, -1.0f, -1.0f,
		-1.0f, 1.0f, -1.0f,
		1.0f, -1.0f, 1.0f,
		-1.0f, -1.0f, -1.0f,
		1.0f, -1.0f, -1.0f,
		1.0f, 1.0f, -1.0f,
		1.0f, -1.0f, -1.0f,
		-1.0f, -1.0f, -1.0f,
		-1.0f, -1.0f, -1.0f,
		-1.0f, 1.0f, 1.0f,
		-1.0f, 1.0f, -1.0f,
		1.0f, -1.0f, 1.0f,
		-1.0f, -1.0f, 1.0f,
		-1.0f, -1.0f, -1.0f,
		-1.0f, 1.0f, 1.0f,
		-1.0f, -1.0f, 1.0f,
		1.0f, -1.0f, 1.0f,
		1.0f, 1.0f, 1.0f,
		1.0f, -1.0f, -1.0f,
		1.0f, 1.0f, -1.0f,
		1.0f, -1.0f, -1.0f,
		1.0f, 1.0f, 1.0f,
		1.0f, -1.0f, 1.0f,
		1.0f, 1.0f, 1.0f,
		1.0f, 1.0f, -1.0f,
		-1.0f, 1.0f, -1.0f,
		1.0f, 1.0f, 1.0f,
		-1.0f, 1.0f, -1.0f,
		-1.0f, 1.0f, 1.0f,
		1.0f, 1.0f, 1.0f,
		-1.0f, 1.0f, 1.0f,
		1.0f, -1.0f, 1.0f
	};
	static const GLfloat g_uv_buffer_data[] = {
		0.000059f, 1.0f - 0.000004f,
		0.000103f, 1.0f - 0.336048f,
		0.335973f, 1.0f - 0.335903f,
		1.000023f, 1.0f - 0.000013f,
		0.667979f, 1.0f - 0.335851f,
		0.999958f, 1.0f - 0.336064f,
		0.667979f, 1.0f - 0.335851f,
		0.336024f, 1.0f - 0.671877f,
		0.667969f, 1.0f - 0.671889f,
		1.000023f, 1.0f - 0.000013f,
		0.668104f, 1.0f - 0.000013f,
		0.667979f, 1.0f - 0.335851f,
		0.000059f, 1.0f - 0.000004f,
		0.335973f, 1.0f - 0.335903f,
		0.336098f, 1.0f - 0.000071f,
		0.667979f, 1.0f - 0.335851f,
		0.335973f, 1.0f - 0.335903f,
		0.336024f, 1.0f - 0.671877f,
		1.000004f, 1.0f - 0.671847f,
		0.999958f, 1.0f - 0.336064f,
		0.667979f, 1.0f - 0.335851f,
		0.668104f, 1.0f - 0.000013f,
		0.335973f, 1.0f - 0.335903f,
		0.667979f, 1.0f - 0.335851f,
		0.335973f, 1.0f - 0.335903f,
		0.668104f, 1.0f - 0.000013f,
		0.336098f, 1.0f - 0.000071f,
		0.000103f, 1.0f - 0.336048f,
		0.000004f, 1.0f - 0.671870f,
		0.336024f, 1.0f - 0.671877f,
		0.000103f, 1.0f - 0.336048f,
		0.336024f, 1.0f - 0.671877f,
		0.335973f, 1.0f - 0.335903f,
		0.667969f, 1.0f - 0.671889f,
		1.000004f, 1.0f - 0.671847f,
		0.667979f, 1.0f - 0.335851f
	};

	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(Vertices), Vertices, GL_STATIC_DRAW);

	glGenBuffers(1, &uvBuffer);
	glBindBuffer(GL_ARRAY_BUFFER, uvBuffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(g_uv_buffer_data), g_uv_buffer_data, GL_STATIC_DRAW);
}


static void CreateIndexBuffer()
{
	unsigned int Indices[] = { 0, 3, 1,
		1, 3, 2,
		2, 3, 0,
		0, 1, 2 };

	glGenBuffers(1, &IBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(Indices), Indices, GL_STATIC_DRAW);
}

int main(int argc, char** argv)
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA);
	glutInitWindowSize(WINDOW_WIDTH, WINDOW_HEIGHT);
	glutInitWindowPosition(100, 100);
	glutCreateWindow("...");
	
	InitializeGlutCallBacks();

	//Must be done after glut is initialized!
	GLenum res = glewInit();
	if (res != GLEW_OK){
		fprintf(stderr, "Error: '%s'\n ", glewGetErrorString(res));
		return 1;
	}

	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);

	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);

	glFrontFace(GL_CW);
	glCullFace(GL_BACK);
	glEnable(GL_CULL_FACE);

	//read the texture
	pTexture = new Texture(GL_TEXTURE_2D);
	pTexture->loadDDS("../Resource/uvmap2.DDS");

	//road the Obj model
	pLoader = new ObjLoader("../Resource/suzanne.obj");
	pLoader->Load();

	//set the camera parameter
	pCamera = new Camera(WINDOW_WIDTH, WINDOW_HEIGHT);

	//initialize the light
	pLight = new Light();
	pDirLight = new DirectionLight();

	//start the light control
	pLC = new LightControl();
	pLC->Bind(pLight);
	pLC->Bind(pDirLight);

	//CreateVertexBuffer();
	//CreateIndexBuffer();

	shader_light.CompileShaders();
	shader_light.init();

	persprojection.FOV = 60.0f;
	persprojection.Height = WINDOW_HEIGHT;
	persprojection.Width = WINDOW_WIDTH;
	persprojection.zFar = 100.0f;
	persprojection.zNear = 1.0f;

	glutMainLoop();

	return 0;
}