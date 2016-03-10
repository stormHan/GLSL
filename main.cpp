<<<<<<< HEAD
#include <iostream>
#include <string>

#include <math.h>
#include <GL\glew.h>
#include <glut.h>
#include "glm.h"

#include "math_3d.h"
#include "HAN_UTIL.h"
#include "HAN_texture.h"

#include "utils_sampling.hpp"
#include "vcg_mesh.hpp"


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

char* Rabbit_path = ".. / Models / rabbit.obj";

vector<Utils_sampling::Vec3> samples_pos;
vector<Utils_sampling::Vec3> samples_nol;

bool loadObj(const char* path, vector<Utils_sampling::Vec3>& out_vertices,
	vector<Utils_sampling::Vec3>& out_normals, vector<int> out_tris){

	printf("Loading OBJ file %s", path);

	vector<unsigned int> vertexIndices, uvIndices, normalIndices;
	vector<Utils_sampling::Vec3> temp_vertices;
	vector<Vector2f> temp_uvs;
	vector<Utils_sampling::Vec3> temp_normals;
	vector<int> tri;

	FILE* file = fopen(path, "r");

	if (file == NULL){
		printf("Impossible to open the file!");

		return false;
	}

	while (1){
		char lineHeader[128];
		//read the first word of the line
		int res = fscanf(file, "%s", lineHeader);
		if (res == EOF)
			break;

		if (strcmp(lineHeader, "v") == 0)
		{
			cout << "Get v" << endl;
			Utils_sampling::Vec3  vertex;
			fscanf(file, "%f %f %f", &vertex.x, &vertex.y, &vertex.z);
			temp_vertices.push_back(vertex);
		}
		else if (strcmp(lineHeader, "vt") == 0)
		{
			cout << "Get vt" << endl;
			Vector2f tex_coordinate;
			fscanf(file, "%f %f", &tex_coordinate.x, &tex_coordinate.y);
			temp_uvs.push_back(tex_coordinate);
		}
		else if (strcmp(lineHeader, "vn") == 0)
		{
			cout << "Get vn" << endl;
			Utils_sampling::Vec3 vertex_normal;
			fscanf(file, "%f %f %f", &vertex_normal.x, &vertex_normal.y, &vertex_normal.z);
			out_normals.push_back(vertex_normal);
		}
		else if (strcmp(lineHeader, "f") == 0)
		{
			std::string vertex1, vertex2, vertex3;
			unsigned int vertexIndex[3], uvIndex[3], normalIndex[3];
			fscanf(file, "%d//%d//%d %d//%d//%d %d//%d//%d", &vertexIndex[0], &normalIndex[0], &uvIndex[0],
				&vertexIndex[1], &normalIndex[1], &uvIndex[1],
				&vertexIndex[2], &normalIndex[2], &uvIndex[2]);

			out_tris.push_back(vertexIndex[0]);
			out_tris.push_back(vertexIndex[1]);
			out_tris.push_back(vertexIndex[2]);
		}
	}

	return true;
}


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

	glEnable(GL_LIGHTING);//启动光源
	glEnable(GL_LIGHT0);//启动第0号光源
	glEnable(GL_LIGHT1);//启动第1号光源

}

static void TextureInit()
{
	glMaterialfv(GL_FRONT, GL_AMBIENT, ambient);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, diffuse);
	glMaterialfv(GL_FRONT, GL_SPECULAR, specular);
	glMaterialf(GL_FRONT, GL_SHININESS, shininess);
}

static void ModelInit()
{
	if (!Rabbit)
	{
		Rabbit = glmReadOBJ(Rabbit_path);
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

static void UniformSampling(char* path)
{
	vector<Utils_sampling::Vec3> _vertices;
	vector<Utils_sampling::Vec3> _normals;
	vector<int> _tris;
	
	if (!loadObj(path, _vertices, _normals, _tris))
	{
		printf("fail to load OBJ!");
		exit(0);
	}
	
	Utils_sampling::poisson_disk(1.0, 1000, _vertices, _normals, _tris, samples_pos, samples_nol);
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
//		fprintf(stderr, "Error creating shader type %d", ShaderType);			//之所以用fprintf，是当你将标准输出重定向后，该语句仍然会将错误输出到控制台上。
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
	TextureInit();
	UniformSampling(Rabbit_path);

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
=======
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

	glEnable(GL_LIGHTING);//启动光源
	glEnable(GL_LIGHT0);//启动第0号光源
	glEnable(GL_LIGHT1);//启动第1号光源

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
//		fprintf(stderr, "Error creating shader type %d", ShaderType);			//之所以用fprintf，是当你将标准输出重定向后，该语句仍然会将错误输出到控制台上。
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
>>>>>>> 05a30e7e9a2be33148e319bd2d297feb9831b391
}