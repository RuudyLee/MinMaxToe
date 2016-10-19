#define _CRT_SECURE_NO_WARNINGS
#include "Mesh.h"
#include <fstream>
#include <iostream>
#include <glm\glm.hpp>
#include <cmath>

#include "Objloader.h"

#define CHAR_BUFFER_SIZE 128
#define BUFFER_OFFSET(i) ((char *)0 + (i))

struct MeshFace {
	MeshFace() {
		vertices[0] = 0;
		vertices[1] = 0;
		vertices[2] = 0;

		textureUVs[0] = 0;
		textureUVs[1] = 0;
		textureUVs[2] = 0;

		normals[0] = 0;
		normals[1] = 0;
		normals[2] = 0;
	}

	MeshFace(
		unsigned v1, unsigned v2, unsigned v3,
		unsigned t1, unsigned t2, unsigned t3,
		unsigned n1, unsigned n2, unsigned n3)
	{
		vertices[0] = v1;
		vertices[1] = v2;
		vertices[2] = v3;

		textureUVs[0] = t1;
		textureUVs[1] = t2;
		textureUVs[2] = t3;

		normals[0] = n1;
		normals[1] = n2;
		normals[2] = n3;
	}

	unsigned vertices[3];
	unsigned textureUVs[3];
	unsigned normals[3];
};

Mesh::Mesh() {
}


Mesh::~Mesh() {
	Unload();
}

// Load a mesh, and send to OpenGL
void Mesh::LoadFromFile(const std::string str) {
	std::vector<glm::vec3> vertices;
	std::vector<glm::vec2> uvs;
	std::vector<glm::vec3> normals;

	glm::vec2 xCons, yCons, zCons;

	loadOBJ(str.c_str(), vertices, uvs, normals, xCons, yCons, zCons);

	_Width = std::abs(xCons.y - xCons.x);
	_Height = std::abs(yCons.y - yCons.x);
	_Length = std::abs(zCons.y - zCons.x);
	

	_NumVertices = vertices.size();
	_NumFaces = _NumVertices / 3;

	// Send data to OpenGL
	glGenVertexArrays(1, &_VAO);
	glGenBuffers(1, &_VBO_Vertices);
	glGenBuffers(1, &_VBO_UVs);
	glGenBuffers(1, &_VBO_Normals);

	glBindVertexArray(_VAO);

	glEnableVertexAttribArray(0); // Vertices
	glEnableVertexAttribArray(1); // UVs
	glEnableVertexAttribArray(2); // Normals

	glBindBuffer(GL_ARRAY_BUFFER, _VBO_Vertices);
	glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(glm::vec3), &vertices[0], GL_STATIC_DRAW);
	glVertexAttribPointer((GLuint)0, 3, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(0));

	glBindBuffer(GL_ARRAY_BUFFER, _VBO_UVs);
	glBufferData(GL_ARRAY_BUFFER, uvs.size() * sizeof(glm::vec2), &uvs[0], GL_STATIC_DRAW);
	glVertexAttribPointer((GLuint)1, 2, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(0));

	glBindBuffer(GL_ARRAY_BUFFER, _VBO_Normals);
	glBufferData(GL_ARRAY_BUFFER, normals.size() * sizeof(glm::vec3), &normals[0], GL_STATIC_DRAW);
	glVertexAttribPointer((GLuint)2, 3, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(0));


	// Cleanup
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}

// Release data from OpenGL (VRAM)
void Mesh::Unload() {
	glDeleteBuffers(1, &_VBO_Normals);
	glDeleteBuffers(1, &_VBO_UVs);
	glDeleteBuffers(1, &_VBO_Vertices);
	glDeleteVertexArrays(1, &_VAO);

	_VBO_Normals = 0;
	_VBO_UVs = 0;
	_VBO_Vertices = 0;
	_VAO = 0;

	_NumFaces = 0;
	_NumVertices = 0;
}

void Mesh::Unbind() {
	glBindVertexArray(0);
}

unsigned int Mesh::GetNumFaces() const {
	return _NumFaces;
}

unsigned int Mesh::GetNumVertices() const {
	return _NumVertices;
}

GLuint Mesh::GetVAO() const {
	return _VAO;
}