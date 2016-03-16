#include <iostream>
#include <string>
#include <glew.h>
#include <glut.h>

#include "Light.h"
#include "HAN_UTIL.h"
#include "ViewControl.h"
#include "ShaderLoading.h"
#include "math_3d.h"
#include "Pipeline.h"

using namespace std;

#define WINDOW_WIDTH  1024
#define WINDOW_HEIGHT  768 

#pragma comment(lib, "glew32.lib")

/*struct Vertex
{
	Vector3f m_pos;
	Vector2f m_tex;

	Vertex() {}

	Vertex(Vector3f pos, Vector2f tex)
	{
		m_pos = pos;
		m_tex = tex;
	}
};*/

PersProjInfo persprojection;

Shader shader_light("Shader/light.vs", "Shader/light.fs");

//-------Buffer setting----------------
GLuint VBO, IBO;
GLfloat rotate_val = 0.0f;
//-------Callback Func ----------------
static void RenderSceneCB()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	static float Scale = 0.0f;

	Scale += 0.1f;
	Pipeline p;
	p.Rotate(0.0f, Scale, 0.0f);
	p.Translate(0.0f, 0.0f, 3.0f);
	Vector3f e =  Vector3f(0.0f, 0.0f, 0.0f);
	Vector3f t = Vector3f(0.0f, 0.0f, .0f);
	Vector3f u = Vector3f(0.0f, 1.0f, 0.0f);
	p.SetCamera(e, t, u);
	p.SetPersjection(persprojection);
	//Matrix4f World;
	//World.m[0][0] = cosf(Scale); World.m[0][1] = 0.0f; World.m[0][2] = -sinf(Scale); World.m[0][3] = 0.0f;
	//World.m[1][0] = 0.0;         World.m[1][1] = 1.0f; World.m[1][2] = 0.0f; World.m[1][3] = 0.0f;
	//World.m[2][0] = sinf(Scale); World.m[2][1] = 0.0f; World.m[2][2] = cosf(Scale); World.m[2][3] = 0.0f;
	//World.m[3][0] = 0.0f;        World.m[3][1] = 0.0f; World.m[3][2] = 0.0f; World.m[3][3] = 1.0f;

	glUniformMatrix4fv(shader_light.gWorldLocation, 1, GL_TRUE, p.GetWVPTrans());

	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBO);
	glDrawElements(GL_TRIANGLES, 12, GL_UNSIGNED_INT, 0);
	glDisableVertexAttribArray(0);
	
	//glPushMatrix();
	//glMatrixMode(GL_MODELVIEW);
	//glLoadIdentity();
	//CalEyePosition();
	//gluLookAt(Eye[0], Eye[1], Eye[2], Center[0], Center[1], Center[2], Up[0], Up[1], Up[2]);
	//glPopMatrix();

	glFlush();
	glutSwapBuffers();
}

//static void ReRenderScene(int width, int height)
//{
//	//get the viewport of GLUI and then set the viewport back to that after the resize
//	glViewport(0, 0, width, height);//viewport函数用于在窗口中设定一个视角的大小，可以用来将一个窗口划分为多个视角
//	glMatrixMode(GL_PROJECTION);
//
//	glLoadIdentity();
//	gluPerspective(60.0, (float)width / height, 0.1, 20.0);//重新设定视角
//	glGetDoublev(GL_PROJECTION_MATRIX, projection);
//	glMatrixMode(GL_MODELVIEW);
//	glLoadIdentity();
//	gluLookAt(Eye[0], Eye[1], Eye[2], Center[0], Center[1], Center[2], Up[0], Up[1], Up[2]);
//	glGetDoublev(GL_MODELVIEW_MATRIX, modelview);
//	glClearColor((GLclampf)1 / 255, (GLclampf)1 / 255, (GLclampf)1 / 255, 0.0);//将背景刷成灰色
//	glEnable(GL_DEPTH_TEST);
//	glEnable(GL_LIGHTING);
//	glEnable(GL_LIGHT0);
//}

static void SpecialKeyboardCB(int Key, int x, int y)
{

}

static void NormalKeyboardCB(unsigned char)
{

}

static void MouseCB(int button, int state, int x, int y)
{
	if (state == GLUT_DOWN){
		switch (button)
		{
		case GLUT_RIGHT_BUTTON:	Mouse_Rightdown = GL_TRUE; break;
		case GLUT_LEFT_BUTTON:  Mouse_Leftdown = GL_TRUE; break;
		case GLUT_MIDDLE_BUTTON:Mouse_Middledown = GL_TRUE; break;

		default:
			break;
		}
	}
	else
	{
		Mouse_Leftdown = GL_FALSE;
		Mouse_Rightdown = GL_FALSE;
		Mouse_Middledown = GL_FALSE;
	}

	MousePosition_x = x;
	MousePosition_y = y;
}

static void  MouseMotion(int x, int y)
{
	if (Mouse_Leftdown == GL_TRUE)
	{
		X_rotate += (x - MousePosition_x) / 80.0f;
		Y_rotate += (y - MousePosition_y) / 120.0f;
	}

	if (Mouse_Rightdown == GL_TRUE)
	{
		C2Elength += (y - MousePosition_y) / 25.0f;
	}

	MousePosition_x = x;
	MousePosition_y = y;
	CalEyePosition();
	glutPostRedisplay();
}

static void NormalKeyboardCB(unsigned char Key, int x, int y)
{

}

static void InitializeGlutCallBacks()
{
	glutDisplayFunc(RenderSceneCB);
	glutIdleFunc(RenderSceneCB);
	//glutReshapeFunc(ReRenderScene);

	glutSpecialFunc(SpecialKeyboardCB);
	glutKeyboardFunc(NormalKeyboardCB);
	glutMotionFunc(MouseMotion); //鼠标移动。
	glutMouseFunc(MouseCB);

}

static void CreateVertexBuffer()
{
	/*Vertex Vertices[4] = { Vertex(Vector3f(-1.0f, -1.0f, 0.5773f), Vector2f(0.0f, 0.0f)),
		Vertex(Vector3f(0.0f, -1.0f, -1.15475f), Vector2f(0.5f, 0.0f)),
		Vertex(Vector3f(1.0f, -1.0f, 0.5773f), Vector2f(1.0f, 0.0f)),
		Vertex(Vector3f(0.0f, 1.0f, 0.0f), Vector2f(0.5f, 1.0f)) };*/

	Vector3f Vertices[4] = { Vector3f(-1.0f, -1.0f, 0.0f), Vector3f(0.0f, -1.0f, 1.0f),
		Vector3f(1.0f, -1.0f, 0.0f), Vector3f(0.0f, 1.0f, 0.0f) };

	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
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

	CreateVertexBuffer();
	CreateIndexBuffer();

	shader_light.CompileShaders();

	persprojection.FOV = 60.0f;
	persprojection.Height = WINDOW_HEIGHT;
	persprojection.Width = WINDOW_WIDTH;
	persprojection.zFar = 100.0f;
	persprojection.zNear = 1.0f;

	glutMainLoop();

	return 0;
}