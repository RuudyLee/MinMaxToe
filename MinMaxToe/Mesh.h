#pragma once
#include <string>
#include <vector>
#include <GL\glew.h>

class Mesh {
public:
	Mesh();
	~Mesh();

	// Load a mesh, and send to OpenGL
	void LoadFromFile(const std::string str);

	// Release data from OpenGL (VRAM)
	void Unload();
	void Unbind();

	unsigned int GetNumFaces() const;
	unsigned int GetNumVertices() const;
	GLuint GetVAO() const;

	float _Width = 0.0f;
	float _Length = 0.0f;
	float _Height = 0.0f;
private:
	// OpenGL buffers and objects
	GLuint _VBO_Vertices = 0;
	GLuint _VBO_UVs = 0;
	GLuint _VBO_Normals = 0;
	GLuint _VAO = 0;

	unsigned int _NumFaces = 0;
	unsigned int _NumVertices = 0;

};

