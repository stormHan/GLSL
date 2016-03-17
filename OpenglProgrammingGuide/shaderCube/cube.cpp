#include <GL\glew.h>
#include <GL\freeglut.h>
#include <math.h>

#include <HAN_UTIL.h>
#include <math_3d.h>
#include <ShaderLoading.h>

#pragma comment(lib, "glew32.lib")

using namespace std;

#define WINDOW_WIDTH	800
#define WINDOW_HEIGHT	600

Shader cubeshader("../shader/cube.vs", "../shader/cube.fs");
static GLfloat scale = 0.0f;
enum vertexIndex
{
	v_postion,
	v_color
};

GLuint VBO, VIO;

void RenderSceneCB()
{
	glClear(GL_COLOR_BUFFER_BIT);

	static float Scale = 0.0f;

	Scale += 0.001f;

	Matrix4f World;

	World.m[0][0] = cosf(Scale); World.m[0][1] = 0.0f; World.m[0][2] = -sinf(Scale); World.m[0][3] = 0.0f;
	World.m[1][0] = 0.0;         World.m[1][1] = 1.0f; World.m[1][2] = 0.0f; World.m[1][3] = 0.0f;
	World.m[2][0] = sinf(Scale); World.m[2][1] = 0.0f; World.m[2][2] = cosf(Scale); World.m[2][3] = 0.0f;
	World.m[3][0] = 0.0f;        World.m[3][1] = 0.0f; World.m[3][2] = 0.0f; World.m[3][3] = 1.0f;

	glUniformMatrix4fv(cubeshader.gWorldLocation, 1, GL_TRUE, &World.m[0][0]);

	glEnableVertexAttribArray(v_postion);
	glEnableVertexAttribArray(v_color);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glVertexAttribPointer(v_postion, 4, GL_FLOAT, GL_FALSE, 0, NULL);
	glVertexAttribPointer(v_color, 4, GL_FLOAT, GL_FALSE, 0, (const GLvoid*)(32 * sizeof(GLfloat)));

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, VIO);
	glDrawElements(GL_TRIANGLE_STRIP, 8, GL_UNSIGNED_SHORT, NULL);
	glDrawElements(GL_TRIANGLE_STRIP, 8, GL_UNSIGNED_SHORT, (const GLvoid*)(9 * sizeof(GLushort)));

	glDisableVertexAttribArray(v_color);
	glDisableVertexAttribArray(v_postion);

	glutSwapBuffers();
}

void CreateVBO()
{
	static const GLfloat cube_position[] =
	{
		-1.0f, -1.0f, -1.0f, 1.0f,
		-1.0f, -1.0f, 1.0f, 1.0f,
		-1.0f, 1.0f, -1.0f, 1.0f,
		-1.0f, 1.0f, 1.0f, 1.0f,
		1.0f, -1.0f, -1.0f, 1.0f,
		1.0f, -1.0f, 1.0f, 1.0f,
		1.0f, 1.0f, -1.0f, 1.0f,
		1.0f, 1.0f, 1.0f, 1.0f
	};
	
	static const GLfloat cube_colors[] =
	{
		1.0f, 1.0f, 1.0f, 1.0f,
		1.0f, 1.0f, 0.0f, 1.0f,
		1.0f, 0.0f, 1.0f, 1.0f,
		1.0f, 0.0f, 0.0f, 1.0f,
		0.0f, 1.0f, 1.0f, 1.0f,
		0.0f, 1.0f, 0.0f, 1.0f,
		0.0f, 0.0f, 1.0f, 1.0f,
		0.5f, 0.5f, 0.5f, 1.0f,
	};
	//glGenBuffers(1, &VBO);
	//glBindBuffer(GL_ARRAY_BUFFER, VBO);
	//glBufferData(GL_ARRAY_BUFFER, sizeof(cube_position), cube_position, GL_STATIC_DRAW);
	//glBufferData(GL_ARRAY_BUFFER, sizeof(cube_colors), cube_colors, GL_STATIC_DRAW);

	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(cube_position) + sizeof(cube_colors), NULL, GL_STATIC_DRAW);
	glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(cube_position), cube_position);
	glBufferSubData(GL_ARRAY_BUFFER, sizeof(cube_position), sizeof(cube_colors), cube_colors);
	
}

void CreateVIO()
{
	static const GLushort cube_indices[] =
	{
		0, 1, 2, 3, 6, 7, 4, 5,	//cube can be described as 2 strips 
		0xffff,
		2, 6, 0, 4, 1, 5, 3, 7
	};
	glGenBuffers(1, &VIO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, VIO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(cube_indices), cube_indices, GL_STATIC_DRAW);
}

int main(int argc, char ** argv)
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA);
	glutInitWindowSize(WINDOW_WIDTH, WINDOW_HEIGHT);
	glutInitWindowPosition(100, 100);
	glutCreateWindow("...");

	GLenum res = glewInit();
	if (res != GLEW_OK) {
		fprintf(stderr, "Error: '%s'\n", glewGetErrorString(res));
		return 1;
	}

	CreateVBO();
	CreateVIO();

	cubeshader.CompileShaders();

	glColor3f(0.0f, 0.0f, 0.0f);

	glutDisplayFunc(RenderSceneCB);
	glutIdleFunc(RenderSceneCB);

	glutMainLoop();
	
}