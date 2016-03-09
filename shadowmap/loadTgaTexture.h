#ifndef TGATEXTURE_H  
#define TGATEXTURE_H  

#include <glut.h>  
#include <iostream>  
#include <GLAUX.H>

using namespace std;

//����ṹ�嶨��  
typedef struct
{
	GLubyte *imageData;//ͼ������  
	GLuint bpp;//�������  
	GLuint width;//ͼ����  
	GLuint height;//ͼ��߶�  
	GLuint texID;//��Ӧ������ID  
}TextureImage;

//����TGAͼ����������  
bool LoadTGA(TextureImage *texture, char *fileName);

AUX_RGBImageRec *LoadBMP(CHAR *Filename);

int LoadGLTextures(GLuint  texture[], CHAR *filename);
#endif