#include <vector>
#include <iostream>
//#ifndef OBJLOAD_H
//#define OBJLOAD_H

bool loadOBJ(
	const char * path,
	std::vector<glm::vec3> & out_vertices,
	std::vector<glm::vec2> & out_uvs,
	std::vector<glm::vec3> & out_normals,
	std::vector<glm::vec3> & out_color
);

//#endif
