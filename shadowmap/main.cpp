#include <Windows.h>
#include <math.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "GLee\GLee.h"
#include <glut.h>
#include <GLAUX.H>
#include "camera.h"
#include "Light.h"
#include "Object.h"
#include "TIMER.h"
#include "FPS_COUNTER.h"
#include "Maths\Maths.h"

#define PI 3.1415926

const int shadowMapSize = 512;
int winWidth, winHeight;

//Timer used for frame rate independent movement
TIMER timer;

//Frames per second counter
FPS_COUNTER fpsCounter;

camera cameraView(0.0f, 0.0f, 4.0f);
Light light(0.0f, 0.0f, 0.0f);

Object pirate;
Object fish;

Texture pirateTex;
Texture fishTex;
Texture shadowMapTex;

static float spin = 0.0;
void init(void)
{	
	glClearColor(0.0, 0.0, 0.0, 0.0);
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LEQUAL);

	light.startLight();
	light.setMaterial();

	pirate.loadObjfromFile("src/pirate.obj");
	pirate.initialization();

	fish.loadObjfromFile("src/fish.obj");
	fish.initialization();

}

void reDisplayAll(void)
{
	glutPostRedisplay();
}

void myDisplay(void)
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);//���������ɫ�������Ȼ���
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
	glColor3f(1.0f, 0.0f, 0.0f);
	//glutSolidCube(0.5);

	pirate.LoadTextureViaTGA(pirateTex, "src/pirate.tga");
	pirateTex.setTexParament();
	glEnable(GL_TEXTURE_2D);

	glPushMatrix();
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	cameraView.cameraShow();

	//pirate.glmDraw(pirate.pMode, GLM_SMOOTH | GLM_TEXTURE | GLM_MATERIAL);
	glPopMatrix();

	fish.LoadTextureViaBMP(fishTex, "src/fish.bmp");
	fishTex.setTexParament();
	glEnable(GL_TEXTURE_2D);


	glPushMatrix();
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	cameraView.cameraShow();

	glRotatef(spin, 0.0, 1.0, 0.0);
	glTranslatef(1.0, 0.0, 0.0);

	//fish.glmDraw(fish.pMode, GLM_SMOOTH | GLM_TEXTURE | GLM_MATERIAL);
	glPopMatrix();

	
	glColor3f(0.0f, 0.0f, 1.0f);
	glPushMatrix();
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	cameraView.cameraShow();
	glTranslatef(0.0, -0.7, 0.0);
	glScalef(3.0, 0.1, 3.0);

	glutSolidCube(1.0f);
	glPopMatrix();


	
	glFlush();
	glutSwapBuffers();
	reDisplayAll();
}

void myResize(int width, int height)
{

	//get the viewport of GLUI and then set the viewport back to that after the resize
	glViewport(0, 0, width, height);//viewport���������ڴ������趨һ���ӽǵĴ�С������������һ�����ڻ���Ϊ����ӽ�
	glMatrixMode(GL_PROJECTION);

	glPushMatrix();
	glLoadIdentity();
	gluPerspective(60.0, (float)width / height, 0.5f, 20.0f);//�����趨�ӽ�
	glPopMatrix();
	cameraView.cameraShow();
	glClearColor((GLclampf)1 / 255, (GLclampf)1 / 255, (GLclampf)1 / 255, 0.0);//������ˢ�ɻ�ɫ
	glEnable(GL_DEPTH_TEST);
}

void spinDisplay(void)
{
	spin += 4;
	if (spin > 360)
		spin = spin - 360;
	glutPostRedisplay();
}

void myMouse(int button, int state, int x, int y)
{
	switch (button)
	{
	case GLUT_LEFT_BUTTON:
		if (state == GLUT_DOWN)
			glutIdleFunc(spinDisplay);
		break;
	case GLUT_MIDDLE_BUTTON:
		if (state == GLUT_DOWN)
			glutIdleFunc(NULL);
		break;
	default:
		break;
	}
}

void processNormalKeys(unsigned char key, int x, int y)//���������Ϣ��Key������̵����ݣ�x��y�ֱ�����û�����ʱ�������������Ͻǵ�����
{
	GLfloat spinEye = 0.0;
	GLfloat storeEyeX, storeEyeY, storeEyeZ, storeEyeR, r4XandZ;
	
	switch (key)
	{
	case 'a':
		spinEye = 5.0;
		storeEyeX = cameraView.cameraPosition[0] * cos(spinEye * PI / 180) - cameraView.cameraPosition[2] * sin(spinEye * PI / 180);
		storeEyeZ = cameraView.cameraPosition[2] * cos(spinEye * PI / 180) + cameraView.cameraPosition[0] * sin(spinEye * PI / 180);
		cameraView.cameraPosition[0] = storeEyeX;
		cameraView.cameraPosition[2] = storeEyeZ;
		break;

	case 'd':
		spinEye = -5.0;
		storeEyeX = cameraView.cameraPosition[0] * cos(spinEye * PI / 180) - cameraView.cameraPosition[2] * sin(spinEye * PI / 180);
		storeEyeZ = cameraView.cameraPosition[2] * cos(spinEye * PI / 180) + cameraView.cameraPosition[0] * sin(spinEye * PI / 180);
		cameraView.cameraPosition[0] = storeEyeX;
		cameraView.cameraPosition[2] = storeEyeZ;
		break;

	case 'q':
		cameraView.cameraPosition[0] *= 0.95;
		cameraView.cameraPosition[1] *= 0.95;
		cameraView.cameraPosition[2] *= 0.95;
		break;

	case 'e':
		cameraView.cameraPosition[0] /= 0.95;
		cameraView.cameraPosition[1] /= 0.95;
		cameraView.cameraPosition[2] /= 0.95;
		break;

	case 'w':
		spinEye = 5.0;
		r4XandZ = sqrt(cameraView.cameraPosition[0] * cameraView.cameraPosition[0] + cameraView.cameraPosition[2] * cameraView.cameraPosition[2]);
		storeEyeR = r4XandZ * cos(spinEye * PI / 180) - cameraView.cameraPosition[1] * sin(spinEye * PI / 180);
		storeEyeY = cameraView.cameraPosition[1] * cos(spinEye * PI / 180) + r4XandZ * sin(spinEye * PI / 180);
		cameraView.cameraPosition[0] = cameraView.cameraPosition[0] * storeEyeR / r4XandZ;
		cameraView.cameraPosition[1] = storeEyeY;
		cameraView.cameraPosition[2] = cameraView.cameraPosition[2] * storeEyeR / r4XandZ;
		break;

	case 's':
		spinEye = -5.0;
		r4XandZ = sqrt(cameraView.cameraPosition[0] * cameraView.cameraPosition[0] + cameraView.cameraPosition[2] * cameraView.cameraPosition[2]);
		storeEyeR = r4XandZ * cos(spinEye * PI / 180) - cameraView.cameraPosition[1] * sin(spinEye * PI / 180);
		storeEyeY = cameraView.cameraPosition[1] * cos(spinEye * PI / 180) + r4XandZ * sin(spinEye * PI / 180);
		cameraView.cameraPosition[0] = cameraView.cameraPosition[0] * storeEyeR / r4XandZ;
		cameraView.cameraPosition[1] = storeEyeY;
		cameraView.cameraPosition[2] = cameraView.cameraPosition[2] * storeEyeR / r4XandZ;
		break;
		

	//case 'a':
	//	spinEye = 5.0;
	//	storeEyeX = light.light_position[0] * cos(spinEye * PI / 180) - light.light_position[2] * sin(spinEye * PI / 180);
	//	storeEyeZ = light.light_position[2] * cos(spinEye * PI / 180) + light.light_position[0] * sin(spinEye * PI / 180);
	//	light.light_position[0] = storeEyeX;
	//	light.light_position[2] = storeEyeZ;
	//	break;

	//case 'd':
	//	spinEye = -5.0;
	//	storeEyeX = light.light_position[0] * cos(spinEye * PI / 180) - light.light_position[2] * sin(spinEye * PI / 180);
	//	storeEyeZ = light.light_position[2] * cos(spinEye * PI / 180) + light.light_position[0] * sin(spinEye * PI / 180);
	//	light.light_position[0] = storeEyeX;
	//	light.light_position[2] = storeEyeZ;
	//	break;

	//case 'q':
	//	light.light_position[0] *= 0.95;
	//	light.light_position[1] *= 0.95;
	//	light.light_position[2] *= 0.95;
	//	break;

	//case 'e':
	//	light.light_position[0] /= 0.95;
	//	light.light_position[1] /= 0.95;
	//	light.light_position[2] /= 0.95;
	//	break;

	//case 'w':
	//	spinEye = 5.0;
	//	r4XandZ = sqrt(light.light_position[0] * light.light_position[0] + light.light_position[2] * light.light_position[2]);
	//	storeEyeR = r4XandZ * cos(spinEye * PI / 180) - light.light_position[1] * sin(spinEye * PI / 180);
	//	storeEyeY = light.light_position[1] * cos(spinEye * PI / 180) + r4XandZ * sin(spinEye * PI / 180);
	//	light.light_position[0] = light.light_position[0] * storeEyeR / r4XandZ;
	//	light.light_position[1] = storeEyeY;
	//	light.light_position[2] = light.light_position[2] * storeEyeR / r4XandZ;
	//	break;

	//case 's':
	//	spinEye = -5.0;
	//	r4XandZ = sqrt(light.light_position[0] * light.light_position[0] + light.light_position[2] * light.light_position[2]);
	//	storeEyeR = r4XandZ * cos(spinEye * PI / 180) - light.light_position[1] * sin(spinEye * PI / 180);
	//	storeEyeY = light.light_position[1] * cos(spinEye * PI / 180) + r4XandZ * sin(spinEye * PI / 180);
	//	light.light_position[0] = light.light_position[0] * storeEyeR / r4XandZ;
	//	light.light_position[1] = storeEyeY;
	//	light.light_position[2] = light.light_position[2] * storeEyeR / r4XandZ;
	//	break;
	}
	reDisplayAll();
}

void processSpecialKeys(int key, int x, int y)//����һЩ����ļ�������
{

}

int main(int argc, char** argv)
{
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);//������ͼ����ʾģʽ
	glutInitWindowPosition(0, 0);//������ʾ���ڵ�top-leftλ��
	glutInitWindowSize(800, 600);//���ô��ڴ�С
	glutInit(&argc, argv);
	glutCreateWindow("OpenGL_objControl");

	glutKeyboardFunc(processNormalKeys);//����������ڽ��ռ����������Ϣ��������Ϣ����processNormalKeys��������
	glutSpecialFunc(processSpecialKeys);//��������ļ�������

	init();

	glutDisplayFunc(&myDisplay);
	glutReshapeFunc(myResize);//�ڸı䴰�ڴ�С��ʱ����������
	glutMouseFunc(myMouse);

	reDisplayAll();

	glutMainLoop();

	return 0;
}