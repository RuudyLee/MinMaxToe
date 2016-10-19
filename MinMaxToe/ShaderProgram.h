#pragma once
#include <GL\glew.h>
#include <glm\glm.hpp>
#include <string>

class ShaderProgram {
public:
	ShaderProgram();
	~ShaderProgram();

	// loads a vertex shader and a fragment shader, and places them in a program
	void Load(const std::string &vertFile, const std::string &fragFile);
	bool IsLoaded() const;
	void Unload();
	bool LinkProgram();

	void Bind() const;
	void Unbind();

	void AddAttribute(unsigned int index, const std::string &attribName);

	// returns -1 if the attribute does not exist
	int GetAttribLocation(const std::string &attrbName);
	// returns -1 if uniform does not exist
	int GetUniformLocation(const std::string &uniformName);

	// send data to shaders
	void SendUniform(const std::string &name, int integer);
	void SendUniform(const std::string &name, unsigned int integer);
	void SendUniform(const std::string &name, float scalar);
	void SendUniform(const std::string &name, const glm::vec2 &vector);
	void SendUniform(const std::string &name, const glm::vec3 &vector);
	void SendUniform(const std::string &name, const glm::vec4 &vector);
	void SendUniformMat3(const std::string &name, float *matrix, bool transpose);
	void SendUniformMat4(const std::string &name, float *matrix, bool transpose);

private:
	bool _IsInit = false;
	GLuint _VertexShader = 0;
	GLuint _FragShader = 0;
	GLuint _ProgramID = 0;

	std::string ReadFile(const std::string &fileName) const;
	bool CompileShader(GLuint shader) const;
	void OutputShaderLog(GLuint shader) const;
	void OutputProgramLog() const;
};

