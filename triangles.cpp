#include <iostream>
#include <string>

#include <math.h>
#include <GL\glew.h>
#include <glut.h>

#include "math_3d.h"
#include "HAN_UTIL.h"

#pragma comment (lib, "glew32.lib")

#define WINDOW_WIDTH  1024
#define WINDOW_HEIGHT  768

using namespace std;

enum VAO_IDs { Triangles, NumVAOs };
enum Buffer_IDs { ArrayBuffer, NumBuffers };
enum Attrib_IDs { vPosition = 0};

GLuint VAOs[NumVAOs];
GLuint Buffers[NumBuffers];

const GLuint NumVertices = 6;

const char* pVSFilename = "shader.vs";
const char* pFSFilename = "shader.fs";

GLuint Color;
GLfloat colorChange = 0.0;

static void RenderSceneCB()
{
	glClear(GL_COLOR_BUFFER_BIT);

	colorChange += 0.001;

	glUniform1f(Color, colorChange);
	glEnableVertexAttribArray(vPosition);
	glBindBuffer(GL_ARRAY_BUFFER, Buffers[ArrayBuffer]);
	glVertexAttribPointer(vPosition, 3, GL_FLOAT, GL_FALSE, 0, 0);

	glDrawArrays(GL_TRIANGLES, 0, 6);

	glDisableVertexAttribArray(vPosition);

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

static void CreateVertexBuffer()
{
	Vector3f Vertices[6];
	Vertices[0] = Vector3f(-0.90f, -0.90f, 0.0f);			//Triangle 1
	Vertices[1] = Vector3f( 0.85f, -0.90f, 0.0f);
	Vertices[2] = Vector3f(-0.90f,  0.85f, 0.0f);			//Triangle 2
	Vertices[3] = Vector3f( 0.90f, -0.85f, 0.0f);
	Vertices[4] = Vector3f( 0.90f,  0.90f, 0.0f);
	Vertices[5] = Vector3f(-0.85f,  0.90f, 0.0f);

	glGenBuffers(NumBuffers, Buffers);
	glBindBuffer(GL_ARRAY_BUFFER, Buffers[ArrayBuffer]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(Vertices), Vertices, GL_STATIC_DRAW);
}


static void AddShader(GLuint ShaderProgram, const char* pShaderText, GLenum ShaderType)
{
	GLuint ShaderObj = glCreateShader(ShaderType);

	if (ShaderObj == 0)
	{
		fprintf(stderr, "Error creating shader type %d", ShaderType);			//之所以用fprintf，是当你将标准输出重定向后，该语句仍然会将错误输出到控制台上。
		exit(0);
	}

	const GLchar* p[1];
	p[0] = pShaderText;
	GLint Lengths[1];
	Lengths[0] = strlen(pShaderText);

	glShaderSource(ShaderObj, 1, p, Lengths);
	glCompileShader(ShaderObj);

	GLint success;
	glGetShaderiv(ShaderObj, GL_COMPILE_STATUS, &success);
	if (!success){
		char logInfo[1024];
		glGetShaderInfoLog(ShaderObj, 1024, NULL, logInfo);
		fprintf(stderr, "fail to  compile the shader type %d : %s", ShaderType, logInfo);
		exit(1);
	}
	glAttachShader(ShaderProgram, ShaderObj);
}

static void CompileShaders()
{
	GLuint ShaderProgram = glCreateProgram();

	if (ShaderProgram == 0){
		fprintf(stderr, "Error creating shader program\n");
	}

	string vs, fs;

	if (!ReadFile(pVSFilename, vs)){
		exit(1);
	}

	if (!ReadFile(pFSFilename, fs)){
		exit(1);
	}

	AddShader(ShaderProgram, vs.c_str(), GL_VERTEX_SHADER);
	AddShader(ShaderProgram, fs.c_str(), GL_FRAGMENT_SHADER);

	GLint Success = 0;
	GLchar ErrorLog[1024] = {' '};
	glLinkProgram(ShaderProgram);
	glGetProgramiv(ShaderProgram, GL_LINK_STATUS, &Success);
	if (Success == 0){
		glGetProgramInfoLog(ShaderProgram, 1024, NULL, ErrorLog);
		fprintf(stderr, "fail to link shader program : %s", ErrorLog);
		exit(1); 
	}

	glValidateProgram(ShaderProgram);
	glGetProgramiv(ShaderProgram, GL_VALIDATE_STATUS, &Success);
	if (Success == 0){
		glGetProgramInfoLog(ShaderProgram, 1024, NULL, ErrorLog);
		fprintf(stderr, "invalidate the shader program : %s", ErrorLog);
		exit(1);
	}

	glUseProgram(ShaderProgram);

	Color = glGetUniformLocation(ShaderProgram, "colorChange");
;}

int main(int argc, char **argv)
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA);
	glutInitWindowSize(WINDOW_WIDTH, WINDOW_HEIGHT);
	glutInitWindowPosition(100, 100);
	glutCreateWindow("...");
	//glutGameModeString("1920x1200@32");
	//glutEnterGameMode();

	InitializeGlutCallBacks();

	//Must be done after glut is initialized!
	GLenum res = glewInit();
	if (res != GLEW_OK){
		fprintf(stderr, "Error: '%s'\n ", glewGetErrorString(res));
		return 1;
	}

	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);

	CreateVertexBuffer();

	CompileShaders();

	glutMainLoop();

	return 0;
}