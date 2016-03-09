#pragma once
#include <stdio.h>
#include <glut.h>
#include <GLAUX.H>
class Texture
{
public:
	Texture();

	GLubyte *imageData;//图像数据  
	GLuint bpp;//像素深度  
	GLuint width;//图像宽度  
	GLuint height;//图像高度  
	GLuint texID;//对应的纹理ID  

	GLuint textureBMP[1];
	GLuint texturestore;

	int loadBMPTexture(GLuint  texture[], CHAR *filename);

	AUX_RGBImageRec *LoadBMP(CHAR *Filename);

	bool loadTGATexture(char *filename);

	void setTexParament();
	~Texture();
};

