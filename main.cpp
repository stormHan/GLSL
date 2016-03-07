#include <iostream>
#include <string>

#include <math.h>
#include <GL\glew.h>
#include <glut.h>
#include "glm.h"

#include "math_3d.h"
#include "HAN_UTIL.h"
#include "HAN_texture.h"

#pragma comment (lib, "glew32.lib")
#pragma comment (lib, "CORE_DB_Magick++_.lib")
#pragma comment (lib, "CORE_RL_Magick++_.lib")

using namespace std;

#define WINDOW_WIDTH  1024
#define WINDOW_HEIGHT  768

GLfloat light_position0[] = { 0.8, 0.8, 0.5, 1.0};
GLfloat light_position1[] = {0.8, 0.8, 0.5, 1.0};
GLfloat ambient[] = { 0.2, 0.2, 0.2, 1.0 };
GLfloat diffuse[] = { 0.8, 0.8, 0.8, 1.0 };
GLfloat specular[] = { 0.0, 0.0, 0.0, 1.0 };
GLfloat shininess = 65.0;


GLMmodel* Rabbit = NULL;

struct Vertex
{
	Vector3f position_coordinate;
	Vector2f texture_coordinate;

	Vertex(){

	};

	Vertex(Vector3f  m_pos, Vector2f m_tex)
	{
		position_coordinate = m_pos;
		texture_coordinate = m_tex;
	}

};

static void LightInit()
{
	glLightfv(GL_LIGHT0, GL_POSITION, light_position0);
	glLightfv(GL_LIGHT1, GL_POSITION, light_position1);

	glEnable(GL_LIGHTING);//������Դ
	glEnable(GL_LIGHT0);//������0�Ź�Դ
	glEnable(GL_LIGHT1);//������1�Ź�Դ

	glMaterialfv(GL_FRONT, GL_AMBIENT, ambient);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, diffuse);
	glMaterialfv(GL_FRONT, GL_SPECULAR, specular);
	glMaterialf(GL_FRONT, GL_SHININESS, shininess);

}

static void ModelInit()
{
	if (!Rabbit)
	{
		Rabbit = glmReadOBJ("../Models/rabbit.obj");
	}
	if (!Rabbit)
	{
		printf("No obj loaded!");
		exit(0);
	}

	glmUnitize(Rabbit);
	glmScale(Rabbit, 0.5);
	glmFacetNormals(Rabbit);
	glmVertexNormals(Rabbit, 90.0);
}

static void RenderSceneCB()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glmDraw(Rabbit, GLM_SMOOTH | GLM_TEXTURE | GLM_MATERIAL);

	glutSwapBuffers();
}

static void SpecialKeyboardCB(int Key, int x, int y)
{

}

static void NormalKeyboardCB(unsigned char)
{

}

static void MouseCB(int button, int state, int x, int y)
{

}

static void NormalKeyboardCB(unsigned char Key, int x, int y)
{

}

static void InitializeGlutCallBacks()
{
	glutDisplayFunc(RenderSceneCB);
	glutIdleFunc(RenderSceneCB);

	glutSpecialFunc(SpecialKeyboardCB);
	glutKeyboardFunc(NormalKeyboardCB);
	glutMouseFunc(MouseCB);


}






//static void AddShader(GLuint ShaderProgram, const char* pShaderText, GLenum ShaderType)
//{
//	GLuint ShaderObj = glCreateShader(ShaderType);
//
//	if (ShaderObj == 0)
//	{
//		fprintf(stderr, "Error creating shader type %d", ShaderType);			//֮������fprintf���ǵ��㽫��׼����ض���󣬸������Ȼ�Ὣ�������������̨�ϡ�
//		exit(0);
//	}
//
//	const GLchar* p[1];
//	p[0] = pShaderText;
//	GLint Lengths[1];
//	Lengths[0] = strlen(pShaderText);
//
//	glShaderSource(ShaderObj, 1, p, Lengths);
//	glCompileShader(ShaderObj);
//
//	GLint success;
//	glGetShaderiv(ShaderObj, GL_COMPILE_STATUS, &success);
//	if (!success){
//		char logInfo[1024];
//		glGetShaderInfoLog(ShaderObj, 1024, NULL, logInfo);
//		fprintf(stderr, "fail to  compile the shader type %d : %s", ShaderType, logInfo);
//		exit(1);
//	}
//	glAttachShader(ShaderProgram, ShaderObj);
//}
//
//static void CompileShaders()
//{
//	GLuint ShaderProgram = glCreateProgram();
//
//	if (ShaderProgram == 0){
//		fprintf(stderr, "Error creating shader program\n");
//	}
//
//	string vs, fs;
//
//	if (!ReadFile(pVSFilename, vs)){
//		exit(1);
//	}
//
//	if (!ReadFile(pFSFilename, fs)){
//		exit(1);
//	}
//
//	AddShader(ShaderProgram, vs.c_str(), GL_VERTEX_SHADER);
//	AddShader(ShaderProgram, fs.c_str(), GL_FRAGMENT_SHADER);
//
//	GLint Success = 0;
//	GLchar ErrorLog[1024] = { ' ' };
//	glLinkProgram(ShaderProgram);
//	glGetProgramiv(ShaderProgram, GL_LINK_STATUS, &Success);
//	if (Success == 0){
//		glGetProgramInfoLog(ShaderProgram, 1024, NULL, ErrorLog);
//		fprintf(stderr, "fail to link shader program : %s", ErrorLog);
//		exit(1);
//	}
//
//	glValidateProgram(ShaderProgram);
//	glGetProgramiv(ShaderProgram, GL_VALIDATE_STATUS, &Success);
//	if (Success == 0){
//		glGetProgramInfoLog(ShaderProgram, 1024, NULL, ErrorLog);
//		fprintf(stderr, "invalidate the shader program : %s", ErrorLog);
//		exit(1);
//	}
//
//	glUseProgram(ShaderProgram);
//
//	Color = glGetUniformLocation(ShaderProgram, "colorChange");
//	gSampler = glGetUniformLocation(ShaderProgram, "gSampler");
//	
//}

int main(int argc, char **argv)
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA);
	glutInitWindowSize(WINDOW_WIDTH, WINDOW_HEIGHT);
	glutInitWindowPosition(100, 100);
	glutCreateWindow("...");
	//glutGameModeString("1920x1200@32");
	//glutEnterGameMode();

	LightInit();
	ModelInit();
	InitializeGlutCallBacks();

	//Must be done after glut is initialized!
	GLenum res = glewInit();
	if (res != GLEW_OK){
		fprintf(stderr, "Error: '%s'\n ", glewGetErrorString(res));
		return 1;
	}

	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);

	//CompileShaders();



	glutMainLoop();

	return 0;
}