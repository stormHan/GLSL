#include "ObjLoader.h"

ObjLoader::ObjLoader(const char* path)
{
	filepath = path;
	Mesh_num = 0;
}

bool ObjLoader::Load()
{
	std::vector<Vector3f> temp_vertices, temp_normals;
	std::vector<Vector2f> temp_uvs;

	std::vector<Vector3i> vertexIndex, uvIndex, normalIndex;

	FILE* file = fopen(filepath, "r");
	if (file == NULL)
	{
		printf("Impossible to open the file!");
		return false;
	}

	//read the file until the end of ti
	while (1)
	{
		char lineHeader[128];	//we suppose that the length of each line is below 128
		//read the first word of the line
		int res = fscanf(file, "%s", lineHeader);
		if (res == EOF)
			break;	//end of the file

		if (strcmp(lineHeader, "v") == 0)
		{
			Vector3f vertex;
			fscanf(file, "%f %f %f", &vertex.x, &vertex.y, &vertex.z);
			temp_vertices.push_back(vertex);
		}
		else if (strcmp(lineHeader, "vt") == 0)
		{
			Vector2f uv;
			fscanf(file, "%f %f", &uv.x, &uv.y);
			temp_uvs.push_back(uv);
		}
		else if (strcmp(lineHeader, "vn") == 0)
		{
			Vector3f normal;
			fscanf(file, "%f %f %f", &normal.x, &normal.y, &normal.z);
			temp_normals.push_back(normal);
		}
		else if (strcmp(lineHeader, "f") == 0)
		{
			Vector3i t_vIndex, t_vtIndex, t_vnIndex;
			int matched = fscanf(file, "%d/%d/%d %d/%d/%d %d/%d/%d", &t_vIndex.x, &t_vtIndex.x, &t_vnIndex.x,
				&t_vIndex.y, &t_vtIndex.y, &t_vnIndex.y, &t_vIndex.z, &t_vtIndex.z, &t_vnIndex.z);
			if (matched != 9)
			{
				printf("File can not be read by the simple parser");
				return false;
			}

			vertexIndex.push_back(t_vIndex);
			uvIndex.push_back(t_vtIndex);
			normalIndex.push_back(t_vnIndex);

			Mesh_num += 3;
		}
		else{
			//Probably a comment, eat up the rest of the line
			char stupidBuffer[1000];
			fgets(stupidBuffer, 1000, file);
		}
	}

	for (unsigned int i = 0; i<vertexIndex.size(); i++){
		//--------------------v1--------------------
		// Get the indices of its attributes
		int vertex = vertexIndex[i].x;
		int vertex_uv = uvIndex[i].x;
		int vertex_normal = normalIndex[i].x;
		// Get the attributes thanks to the index
		Vector3f ver = temp_vertices[vertex - 1];
		Vector2f uv = temp_uvs[vertex_uv - 1];
		Vector3f normal = temp_normals[vertex_normal - 1];
		// Put the attributes in buffers
		out_vertices.push_back(ver);
		out_uvs.push_back(uv);
		out_normals.push_back(normal);

		//--------------------v2--------------------
		// Get the indices of its attributes
		vertex = vertexIndex[i].y;
		vertex_uv = uvIndex[i].y;
		vertex_normal = normalIndex[i].y;
		// Get the attributes thanks to the index
		ver = temp_vertices[vertex - 1];
		uv = temp_uvs[vertex_uv - 1];
		normal = temp_normals[vertex_normal - 1];
		// Put the attributes in buffers
		out_vertices.push_back(ver);
		out_uvs.push_back(uv);
		out_normals.push_back(normal);

		//--------------------v3--------------------
		// Get the indices of its attributes
		vertex = vertexIndex[i].z;
		vertex_uv = uvIndex[i].z;
		vertex_normal = normalIndex[i].z;
		// Get the attributes thanks to the index
		ver = temp_vertices[vertex - 1];
		uv = temp_uvs[vertex_uv - 1];
		normal = temp_normals[vertex_normal - 1];
		// Put the attributes in buffers
		out_vertices.push_back(ver);
		out_uvs.push_back(uv);
		out_normals.push_back(normal);
	}

	Load2VBO();

	return true;
}

void ObjLoader::Load2VBO()
{
	glGenBuffers(1, &loader_vertexbuffer);
	glBindBuffer(GL_ARRAY_BUFFER, loader_vertexbuffer);
	glBufferData(GL_ARRAY_BUFFER, out_vertices.size() * sizeof(Vector3f), &out_vertices[0], GL_STATIC_DRAW);

	glGenBuffers(1, &loader_uvbuffer);
	glBindBuffer(GL_ARRAY_BUFFER, loader_uvbuffer);
	glBufferData(GL_ARRAY_BUFFER, out_uvs.size() * sizeof(Vector2f), &out_uvs[0], GL_STATIC_DRAW);
}