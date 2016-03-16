#include <iostream>
#include <string>

#include <math.h>
#include <GL\glew.h>
#include <glut.h>
#include "glm.h"

#include "utils_sampling.hpp"
#include "math_3d.h"
#include "ViewControl.h"

#pragma comment (lib, "glew32.lib")
#pragma comment (lib, "CORE_DB_Magick++_.lib")
#pragma comment (lib, "CORE_RL_Magick++_.lib")

using namespace std;

#define WINDOW_WIDTH  1024
#define WINDOW_HEIGHT  768

//------------------Light parameters---------------------
GLfloat light_position0[] = { 0.8, 0.8, 0.5, 1.0 };
GLfloat light_position1[] = { 0.8, 0.8, 0.5, 1.0 };
GLfloat ambient[] = { 0.2, 0.2, 0.2, 1.0 };
GLfloat diffuse[] = { 0.8, 0.8, 0.8, 1.0 };
GLfloat specular[] = { 0.0, 0.0, 0.0, 1.0 };
GLfloat shininess = 65.0;



GLdouble projection[16], modelview[16];//通过glGetDoublev()函数得到的矩阵信息存在在数组中
//-------------------Model Infomation---------------------
GLMmodel* Rabbit = NULL;

char* Rabbit_path = "../Models/rabbit.obj";
char* Robot_path = "../Models/robot_normalize.obj";

vector<Utils_sampling::Vec3> samples_pos;
vector<Utils_sampling::Vec3> samples_nol;

bool loadObj(const char* path, vector<Utils_sampling::Vec3>& out_vertices,
	vector<Utils_sampling::Vec3>& out_normals, vector<int>& out_tris){

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
				out_vertices.push_back(vertex);
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
				fscanf(file, "%d/%d/%d %d/%d/%d %d/%d/%d", &vertexIndex[0], &normalIndex[0], &uvIndex[0],
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
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LEQUAL);
	glLightfv(GL_LIGHT0, GL_POSITION, light_position0);
	//glLightfv(GL_LIGHT1, GL_POSITION, light_position1);

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
		Rabbit = glmReadOBJ(Robot_path);
	}
	if (!Rabbit)
	{
		printf("No obj loaded!");
		exit(0);
	}

	//glmUnitize(Rabbit);
	//glmScale(Rabbit, 0.5);
	//glmFacetNormals(Rabbit);
	//glmVertexNormals(Rabbit, 90.0);
}

static void DrawSamples()
{
	for(int i = 0; i < samples_pos.size(); i++)
	{
		glPushMatrix();
		glColor3f(0.0f, 0.0f, 0.0f);
		glPointSize(2.0f);
		glBegin(GL_POINTS);
			glVertex3f(samples_pos[i].x, samples_pos[i].y, samples_pos[i].z);
		glEnd();
		
		//glTranslatef(samples_pos[i].x / 10, samples_pos[i].y / 10, samples_pos[i].z / 10);
		//glutSolidSphere(0.01f, 10, 10);
		glPopMatrix();
	}
	
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
	
	Utils_sampling::poisson_disk(0.0, 300, _vertices, _normals, _tris, samples_pos, samples_nol);
}

static void RenderSceneCB()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	glPushMatrix();
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	CalEyePosition();
	gluLookAt(Eye[0], Eye[1], Eye[2], Center[0], Center[1], Center[2], Up[0], Up[1], Up[2]);
	glmDraw(Rabbit, GLM_SMOOTH | GLM_TEXTURE | GLM_MATERIAL);
	DrawSamples();
	glPopMatrix();

	glFlush();
	glutSwapBuffers();
}

static void ReRenderScene(int width, int height)
{
	//get the viewport of GLUI and then set the viewport back to that after the resize
	glViewport(0, 0, width, height);//viewport函数用于在窗口中设定一个视角的大小，可以用来将一个窗口划分为多个视角
	glMatrixMode(GL_PROJECTION);

	glLoadIdentity();
	gluPerspective(60.0, (float)width / height, 0.1, 20.0);//重新设定视角
	glGetDoublev(GL_PROJECTION_MATRIX, projection);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	gluLookAt(Eye[0], Eye[1], Eye[2], Center[0], Center[1], Center[2], Up[0], Up[1], Up[2]);
	glGetDoublev(GL_MODELVIEW_MATRIX, modelview);
	glClearColor((GLclampf)1 / 255, (GLclampf)1 / 255, (GLclampf)1 / 255, 0.0);//将背景刷成灰色
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);
}

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
	glutReshapeFunc(ReRenderScene);

	glutSpecialFunc(SpecialKeyboardCB);
	glutKeyboardFunc(NormalKeyboardCB);//
	glutMotionFunc(MouseMotion); //鼠标移动。
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
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH);
	glutInitWindowSize(WINDOW_WIDTH, WINDOW_HEIGHT);
	glutInitWindowPosition(100, 100);
	glutCreateWindow("...");
	//glutGameModeString("1920x1200@32");
	//glutEnterGameMode();

	LightInit();
	ModelInit();
	TextureInit();
	UniformSampling(Robot_path);

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
