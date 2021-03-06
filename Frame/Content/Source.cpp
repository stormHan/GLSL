#include <iostream>
#include <string>
#include <GL\glew.h>
#include <glut.h>

#include "Light.h"
#include "HAN_UTIL.h"
#include "ViewControl.h"
#include "ShadowMapShader.h"
#include "LightShader.h"
#include "math_3d.h"
#include "Pipeline.h"
#include "camera.h"
#include "texture.h"
#include "ObjLoader.h"
#include "LightControl.h"
#include "ShadowMapFBO.h"

using namespace std;

#define WINDOW_WIDTH  1024
#define WINDOW_HEIGHT  768 

#pragma comment(lib, "glew32.lib")

PersProjInfo persprojection;
ObjLoader* pLoader = NULL;
Camera* pCamera = NULL;
Texture* pTexture = NULL;

LightShader* pLightShader = NULL;
ShadowMapShader* pShadowShader = NULL;

//-------Light--------------------------------
DirectionLight* pDirLight = NULL;
PointLight* pPoiLight = NULL;
LightControl* pLC = NULL;
SpotLight m_SpotLight;

//-------Shadow-------------------------------
ShadowMapFBO m_shadowMapFBO;

struct Vertex
{
	Vector3f m_pos;
	Vector2f m_tex;
	Vector3f m_normal;

	Vertex() {}

	Vertex(const Vector3f& pos, const Vector2f& tex, const Vector3f& normal)
	{
		m_pos = pos;
		m_tex = tex;
		m_normal = normal;
	}
};
//-------Buffer setting----------------
GLuint m_VBO, IBO, uvBuffer;

static const float FieldDepth = 20.0f;
static const float FieldWidth = 10.0f;
//-------Callback Func ----------------

void ShadowMapPass()
{
	m_shadowMapFBO.BindForWriting();

	glClear(GL_DEPTH_BUFFER);

	pShadowShader->Enable();

	Pipeline p;
	p.SetCamera(m_SpotLight.getLightPosition(), m_SpotLight.GetSpotLightDir(), Vector3f(0.0f, 1.0f, 0.0f));
	p.SetPersjection(persprojection);
	pShadowShader->SetWVP(p.GetWVPTrans());

	//render first
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

	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void RenderPass()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	//启用 light shader
	pLightShader->Enable();
	
	//pass the uniform variebles
	pLightShader->SetEye2World(pCamera->GetPos());

	m_shadowMapFBO.BindForReading(GL_TEXTURE1);

	Pipeline p;
	p.SetPersjection(persprojection);

	p.SetCamera(pCamera->GetPos(), pCamera->GetTarget(), pCamera->GetUp());
	pLightShader->SetWVP(p.GetWVPTrans());

}


static void RenderSceneCB()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	pCamera->OnRender();

	static float Scale = 0.0f;

	Scale += 0.01f;

	ShadowMapPass();
	RenderPass();
	Pipeline p;
	//p.Rotate(0.0f, Scale, 0.0f);
	//p.Translate(10.0f, 0.0f, 5.0f);
	p.SetCamera(*pCamera);
	p.SetPersjection(persprojection);

	pl[0].setDiffuseIntensity(0.5f);
	pl[0].setLightColor(1.0f, 0.5f, 0.0f);
	pl[0].setLightPosition(3.0f, 1.0f, FieldDepth * (cosf(Scale) + 1.0f) / 2.0f);
	pl[0].Attenuation.Linear = 0.1f;
	pl[1].setDiffuseIntensity(0.5f);
	pl[1].setLightColor(1.0f, 1.0f, 1.0f);
	pl[1].setLightPosition(2.0f * cos(Scale), 5.0f, 2.0f * sin(Scale));
	pl[1].Attenuation.Linear = 0.2f;
	//shader_light.SetPointLights(2, pl);
	////set the world ,view, projection matrix into shader
	//
	//shader_light.SetWVP(p.GetWVPTrans());
	//shader_light.SetgWorld(p.GetWorldTrans());
	//shader_light.SetEye2World(pCamera->GetPos());;

	//shader_light.SetMatSpecularIntensity(0.25f);
	//shader_light.SetMatSpecularPower(1);

	//shader_light.SetDirectionalLight(*pDirLight);
	//bind the texture
	
	//----------------------------------when read obj model using the pLoader-------------
	
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
	
	//-------------------------------------- Set the VBO yourself-------------------------
	/*
	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
	glEnableVertexAttribArray(2);
	glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), 0);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (const GLvoid*)12);
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (const GLvoid*)20);
	pTexture->Bind(GL_TEXTURE0);
	glDrawArrays(GL_TRIANGLES, 0, 6);

	glDisableVertexAttribArray(0);
	glDisableVertexAttribArray(1);
	glDisableVertexAttribArray(2);
	*/
	//-----------------------------------Show the Point Lights--------------------------
	glPushMatrix();
	glColor3f(1.0f, 1.0f, 0.0f);
	glPointSize(10.0f);
	glBegin(GL_POINTS);
		glVertex3f(pl[1].getLightPosition().x, pl[1].getLightPosition().y, pl[1].getLightPosition().z);
		glVertex3f(pl[0].getLightPosition().x, pl[0].getLightPosition().y, pl[0].getLightPosition().z);
	glEnd();
	glPopMatrix();

	glFlush();
	glutSwapBuffers();
}

static void SpecialKeyboardCB(int Key, int x, int y)
{
	pLC->specialKeyMoved(Key, x, y);
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
	glutMotionFunc(MouseMotionwithClick); //鼠标移动。
	glutMouseFunc(MouseCB);

	//unclicked to control the camera
	glutPassiveMotionFunc(MouseMotion);
}

static void CreateVertexBuffer()
{
	const Vector3f Normal = Vector3f(0.0, 1.0f, 0.0f);

	Vertex Vertices[6] = {
		Vertex(Vector3f(0.0f, 0.0f, 0.0f),			Vector2f(0.0f, 0.0f), Normal),
		Vertex(Vector3f(0.0f, 0.0f, FieldDepth),	 Vector2f(0.0f, 1.0f), Normal),
		Vertex(Vector3f(FieldWidth, 0.0f, 0.0f),	 Vector2f(1.0f, 0.0f), Normal),

		Vertex(Vector3f(FieldWidth, 0.0f, 0.0f),			Vector2f(1.0f, 0.0f), Normal),
		Vertex(Vector3f(0.0f, 0.0f, FieldDepth),			Vector2f(0.0f, 1.0f), Normal),
		Vertex(Vector3f(FieldWidth, 0.0f, FieldDepth),		 Vector2f(1.0f, 1.0f), Normal)
	};

	glGenBuffers(1, &m_VBO);
	glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(Vertices), Vertices, GL_STATIC_DRAW);
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
	pTexture->loadDDS("../Resource/uvmap.DDS");

	//road the Obj model
	pLoader = new ObjLoader("../Resource/room_thickwalls.obj");
	pLoader->Load();

	//set the camera parameter
	Vector3f Pos(5.0f, 1.0f, -3.0f);
	Vector3f Target(0.0f, 0.0f, 1.0f);
	Vector3f Up(0.0, 1.0f, 0.0f);

	pCamera = new Camera(WINDOW_WIDTH, WINDOW_HEIGHT, Pos, Target, Up);

	//initialize the light
	pDirLight = new DirectionLight();
	pPoiLight = new PointLight();

	//start the light control
	pLC = new LightControl();
	pLC->Bind(&pl[0]);
	//pLC->Bind(pPoiLight);

	//CreateVertexBuffer();
	//CreateIndexBuffer();

	if (!m_shadowMapFBO.Init(WINDOW_WIDTH, WINDOW_HEIGHT)) {
		printf("fail to Initialize the ShadowMap Frame buffer!");
		exit(0);
	}

	pLightShader = new LightShader();
	if (!pLightShader->Init())
	{
		printf("Error initializing the lighting technique\n");
		exit(0);
	}
	//shader之间转换时，会保留之前的设定值，固一些不变的值可以在初始化时先设定好
	pLightShader->Enable();
	pLightShader->SetPointLights(1, &pl[0]);
	pLightShader->SetTextureUnit(0);
	pLightShader->SetShadowMapTextureUnit(1);

	pShadowShader = new ShadowMapShader();
	if (pShadowShader->Init())
	{
		printf("Error initializing the shadowmap technique\n");
		exit(0);
	}


	persprojection.FOV = 60.0f;
	persprojection.Height = WINDOW_HEIGHT;
	persprojection.Width = WINDOW_WIDTH;
	persprojection.zFar = 50.0f;
	persprojection.zNear = 1.0f;

	glutMainLoop();

	return 0;
}