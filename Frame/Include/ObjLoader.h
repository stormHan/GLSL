//---------------------------------------
//
//    stormhan  pipeline.h
//
//	opengl my own model(.obj) loader
//--------------------------------------

#ifndef H_OBJLOADER_H
#define H_OBJLOADER_H
#include <stdio.h>
#include <vector>

#include <GL\glew.h>
#include <glut.h>

#include "math_3d.h"

class ObjLoader{

public:
	ObjLoader(const char* path);

	bool Load();

	GLuint Mesh_num;

	GLuint loader_vertexbuffer;
	GLuint loader_uvbuffer;

private:
	const char* filepath;

	std::vector<Vector3f> out_vertices;
	std::vector<Vector3f> out_normals;
	std::vector<Vector2f> out_uvs;

	void Load2VBO();
};


#endif //H_OBJLOADER_H