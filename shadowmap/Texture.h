#pragma once
#include <stdio.h>
#include <glut.h>
#include <GLAUX.H>
class Texture
{
public:
	Texture();

	GLubyte *imageData;//ͼ������  
	GLuint bpp;//�������  
	GLuint width;//ͼ����  
	GLuint height;//ͼ��߶�  
	GLuint texID;//��Ӧ������ID  

	GLuint textureBMP[1];
	GLuint texturestore;

	int loadBMPTexture(GLuint  texture[], CHAR *filename);

	AUX_RGBImageRec *LoadBMP(CHAR *Filename);

	bool loadTGATexture(char *filename);

	void setTexParament();
	~Texture();
};

