//---------------------------------------
//
//    stormhan  texture.h
//
//	opengl 用于解析图片信息
//
//	format: .bmp/ .dds
//--------------------------------------
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <GL\glew.h>
#include <glut.h>


#ifndef H_TEXTURE_H
#define H_TEXTURE_H

class Texture
{
public:
	Texture(GLenum TextureTarget);

	GLuint loadBMP_custom(const char * imagepath);

	GLuint loadDDS(const char * imagepath);

	void Bind(GLenum TextureUnit);
private:
	GLenum m_textureTarget;
	GLuint m_textrueObj;

};

#endif