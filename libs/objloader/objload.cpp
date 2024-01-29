#define _CRT_SECURE_NO_WARNINGS
#include <vector>

#include <stdio.h>
#include <string>
#include <cstring>

#include <glm/glm.hpp>

#include "objload.hpp"

// Very, VERY simple OBJ loader.

bool loadOBJ(
	const char *path,
	std::vector<glm::vec3> &out_vertices,
	std::vector<glm::vec2> &out_uvs,
	std::vector<glm::vec3> &out_normals,
	std::vector<glm::vec3> &out_color)
{
	// printf("Loading OBJ file %s...\n", path);

	std::vector<unsigned int> vertexIndices, uvIndices, normalIndices;
	std::vector<glm::vec3> temp_vertices;
	std::vector<glm::vec2> temp_uvs;
	std::vector<glm::vec3> temp_normals;
	std::vector<glm::vec3> temp_color;

	FILE *file = fopen(path, "r");
	if (file == NULL)
	{
		printf("Impossible to open the file !\n");
		getchar();
		return false;
	}

	while (1)
	{

		char lineHeader[128];
		int res = fscanf(file, "%s", lineHeader);
		if (res == EOF)
			break; // EOF
		if (strcmp(lineHeader, "v") == 0)
		{
			glm::vec3 vertex;
			glm::vec3 color;
			// check if there is 6 values or 3 values
			int check = fscanf(file, "%f %f %f %f %f %f\n", &vertex.x, &vertex.y, &vertex.z, &color.x, &color.y, &color.z);
			if (check == 6)
			{
				temp_vertices.push_back(vertex);
				temp_color.push_back(color);
			}
			else
			{
				temp_vertices.push_back(vertex);
				temp_color.push_back(glm::vec3(1.0f, 1.0f, 1.0f));
			}
		}
		else if (strcmp(lineHeader, "vt") == 0)
		{
			glm::vec2 uv;
			fscanf(file, "%f %f\n", &uv.x, &uv.y);
			uv.y = -uv.y;
			temp_uvs.push_back(uv);
		}
		else if (strcmp(lineHeader, "vn") == 0)
		{
			glm::vec3 normal;
			fscanf(file, "%f %f %f\n", &normal.x, &normal.y, &normal.z);
			temp_normals.push_back(normal);
		}
		else if (strcmp(lineHeader, "f") == 0)
		{
			std::string vertex1, vertex2, vertex3;
			unsigned int vertexIndex[3], uvIndex[3], normalIndex[3];
			int matches = fscanf(file, "%d/%d/%d %d/%d/%d %d/%d/%d\n", &vertexIndex[0], &uvIndex[0], &normalIndex[0], &vertexIndex[1], &uvIndex[1], &normalIndex[1], &vertexIndex[2], &uvIndex[2], &normalIndex[2]);
			if (matches != 9)
			{
				printf("File can't be read \n");
				return false;
			}
			vertexIndices.push_back(vertexIndex[0]);
			vertexIndices.push_back(vertexIndex[1]);
			vertexIndices.push_back(vertexIndex[2]);
			uvIndices.push_back(uvIndex[0]);
			uvIndices.push_back(uvIndex[1]);
			uvIndices.push_back(uvIndex[2]);
			normalIndices.push_back(normalIndex[0]);
			normalIndices.push_back(normalIndex[1]);
			normalIndices.push_back(normalIndex[2]);
		}
		else
		{
			// Probably a comment
			char stupidBuffer[1000];
			fgets(stupidBuffer, 1000, file);
		}
	}

	// std::cout << vertexIndices.size() << std::endl;

	for (unsigned int i = 0; i < vertexIndices.size(); i++)
	{
		// Get the indices of its attributes
		unsigned int vertexIndex = vertexIndices[i];
		unsigned int uvIndex = uvIndices[i];
		unsigned int normalIndex = normalIndices[i];

		// Get the attributes thanks to the index
		glm::vec3 vertex = temp_vertices[vertexIndex - 1];
		glm::vec2 uv = temp_uvs[uvIndex - 1];
		glm::vec3 normal = temp_normals[normalIndex - 1];

		// Put the attributes in buffers
		out_vertices.push_back(vertex); // std::cout<<vertex.x<< " " << vertex.y<< " " << vertex.z << "       ";

		out_uvs.push_back(uv); // std::cout<< uv.x<<" " << uv.y << "       ";

		out_normals.push_back(normal); // std::cout<<normal.x<<" " <<normal.y<<" "<<normal.z<<std::endl;

		out_color.push_back(temp_color[vertexIndex - 1]);
	}

	return true;
}
