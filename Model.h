//#include <vector>
//#include <stdio.h>
//#include <iostream>
//
//#include "math_3d.h"
//#include "utils_sampling.hpp"
//#include "vcg_mesh.hpp"
//
//using namespace std;
//
//bool loadObj(const char* path, vector<Utils_sampling::Vec3>& out_vertices,
//	vector<Utils_sampling::Vec3>& out_normals, vector<int> out_tris){
//	
//	printf("Loading OBJ file %s", path);
//
//	vector<unsigned int> vertexIndices, uvIndices, normalIndices;
//	vector<Utils_sampling::Vec3> temp_vertices;
//	vector<Vector2f> temp_uvs;
//	vector<Utils_sampling::Vec3> temp_normals;
//	vector<int> tri;
//
//	FILE* file = fopen(path, "r");
//
//	if (file == NULL){
//		printf("Impossible to open the file!");
//
//		return false;
//	}
//
//	while (1){
//		char lineHeader[128];
//		//read the first word of the line
//		int res = fscanf(file, "%s", lineHeader); 
//		if (res == EOF)
//			break;
//
//		if (strcmp(lineHeader, "v") == 0)
//		{
//			cout << "Get v" << endl;
//			Utils_sampling::Vec3  vertex;
//			fscanf(file, "%f %f %f", &vertex.x, &vertex.y, &vertex.z);
//			temp_vertices.push_back(vertex);
//		}
//		else if (strcmp(lineHeader, "vt") == 0)
//		{
//			cout << "Get vt" << endl;
//			Vector2f tex_coordinate;
//			fscanf(file, "%f %f", &tex_coordinate.x, &tex_coordinate.y);
//			temp_uvs.push_back(tex_coordinate);
//		}
//		else if (strcmp(lineHeader, "vn") == 0)
//		{ 
//			cout << "Get vn" << endl;
//			Utils_sampling::Vec3 vertex_normal;
//			fscanf(file, "%f %f %f", &vertex_normal.x, &vertex_normal.y, &vertex_normal.z);
//			out_normals.push_back(vertex_normal);
//		}
//		else if (strcmp(lineHeader, "f") == 0)
//		{
//			std::string vertex1, vertex2, vertex3;
//			unsigned int vertexIndex[3], uvIndex[3], normalIndex[3];
//			fscanf(file, "%d//%d//%d %d//%d//%d %d//%d//%d", &vertexIndex[0], &normalIndex[0], &uvIndex[0],
//				&vertexIndex[1], &normalIndex[1], &uvIndex[1],
//				&vertexIndex[2], &normalIndex[2], &uvIndex[2]);
//
//			out_tris.push_back(vertexIndex[0]);
//			out_tris.push_back(vertexIndex[1]);
//			out_tris.push_back(vertexIndex[2]);
//		}
//	}
//	
//	return true;
//}
//
//
//void __poisson_disk(float radius,
//	int nb_samples,
//	const std::vector<Utils_sampling::Vec3>& verts,
//	const std::vector<Utils_sampling::Vec3>& nors,
//	const std::vector<int>& tris,
//	std::vector<Utils_sampling::Vec3>& samples_pos,
//	std::vector<Utils_sampling::Vec3>& samples_nor)
//{
//	Utils_sampling::poisson_disk(radius, nb_samples, nors, verts, tris, samples_pos, samples_nor);
//}