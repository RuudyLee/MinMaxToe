#include "ShaderProgram.h"
#include "Errors.h"
#include <fstream>
#include <iostream>
#include <vector>

ShaderProgram::ShaderProgram() {
}


ShaderProgram::~ShaderProgram() {
}

void ShaderProgram::Load(const std::string &vertFile, const std::string &fragFile) {
	// create shader and program objects
	_VertexShader = glCreateShader(GL_VERTEX_SHADER);
	_FragShader = glCreateShader(GL_FRAGMENT_SHADER);
	_ProgramID = glCreateProgram();

	// Load our source code
	std::string source = ReadFile(vertFile);
	const GLchar *temp = static_cast<const GLchar *>(source.c_str());
	glShaderSource(_VertexShader, 1, &temp, NULL);

	source = ReadFile(fragFile);
	temp = static_cast<const GLchar *>(source.c_str());
	glShaderSource(_FragShader, 1, &temp, NULL);

	// Compile the code
	if (!CompileShader(_VertexShader)) {
		OutputShaderLog(_VertexShader);
		Unload();
		FatalError("Vertex Shader failed to compile.");
	}

	if (!CompileShader(_FragShader)) {
		OutputShaderLog(_FragShader);
		Unload();
		FatalError("Fragment Shader failed to compile.");
	}

	// setup our program object
	glAttachShader(_ProgramID, _VertexShader);
	glAttachShader(_ProgramID, _FragShader);

	if (!LinkProgram()) {
		OutputProgramLog();
		Unload();
		FatalError("Shader program failed to link.");
	}

	_IsInit = true;
}

bool ShaderProgram::IsLoaded() const {
	return _IsInit;
}

void ShaderProgram::Unload() {
	if (_VertexShader != 0) {
		glDetachShader(_ProgramID, _VertexShader);
		glDeleteShader(_VertexShader);
		_VertexShader = 0;
	}

	if (_FragShader != 0) {
		glDetachShader(_ProgramID, _FragShader);
		glDeleteShader(_FragShader);
		_FragShader = 0;
	}

	if (_ProgramID != 0)
	{
		glDeleteProgram(_ProgramID);
		_ProgramID = 0;
	}

	_IsInit = false;
}

bool ShaderProgram::LinkProgram() {
	glLinkProgram(_ProgramID);

	GLint success;
	glGetProgramiv(_ProgramID, GL_LINK_STATUS, &success);

	return success == GL_TRUE;
}

void ShaderProgram::Bind() const {
	glUseProgram(_ProgramID);
}

void ShaderProgram::Unbind() {
	glUseProgram(0);
}

void ShaderProgram::AddAttribute(unsigned int index, const std::string &attribName) {
	glBindAttribLocation(_ProgramID, index, attribName.c_str());
}

// returns -1 if the attribute does not exist
int ShaderProgram::GetAttribLocation(const std::string &attribName) {
	return glGetAttribLocation(_ProgramID, attribName.c_str());
}

// returns -1 if uniform does not exist
int ShaderProgram::GetUniformLocation(const std::string &uniformName) {
	return glGetUniformLocation(_ProgramID, uniformName.c_str());
}

void ShaderProgram::SendUniform(const std::string &name, int integer) {
	GLint location = GetUniformLocation(name);
	glUniform1i(location, integer);
}

void ShaderProgram::SendUniform(const std::string &name, unsigned int integer) {
	GLint location = GetUniformLocation(name);
	glUniform1ui(location, integer);
}

void ShaderProgram::SendUniform(const std::string &name, float scalar) {
	GLint location = GetUniformLocation(name);
	glUniform1f(location, scalar);
}

void ShaderProgram::SendUniform(const std::string &name, const glm::vec2 &vector) {
	GLint location = GetUniformLocation(name);
	glUniform2f(location, vector.x, vector.y);
}

void ShaderProgram::SendUniform(const std::string &name, const glm::vec3 &vector) {
	GLint location = GetUniformLocation(name);
	glUniform3f(location, vector.x, vector.y, vector.z);
}

void ShaderProgram::SendUniform(const std::string &name, const glm::vec4 &vector) {
	GLint location = GetUniformLocation(name);
	glUniform4f(location, vector.x, vector.y, vector.z, vector.w);
}

void ShaderProgram::SendUniformMat3(const std::string &name, float *matrix, bool transpose) {
	GLint location = GetUniformLocation(name);
	glUniformMatrix3fv(location, 1, transpose, matrix);
}

void ShaderProgram::SendUniformMat4(const std::string &name, float *matrix, bool transpose) {
	GLint location = GetUniformLocation(name);
	glUniformMatrix4fv(location, 1, transpose, matrix);
}

std::string ShaderProgram::ReadFile(const std::string &fileName) const {
	std::ifstream inStream(fileName);

	if (!inStream.good()) {
		std::cout << "Shader file not found.\n";
		return std::string();
	}

	std::string data(std::istreambuf_iterator<char>(inStream), (std::istreambuf_iterator<char>()));
	return data;
}

bool ShaderProgram::CompileShader(GLuint shader) const {
	glCompileShader(shader);

	GLint success;
	glGetShaderiv(shader, GL_COMPILE_STATUS, &success);

	return success == GL_TRUE;
}

void ShaderProgram::OutputShaderLog(GLuint shader) const {
	std::vector<char> infoLog;
	infoLog.resize(512);

	GLint infoLen;
	glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &infoLen);

	glGetShaderInfoLog(shader, sizeof(char) * 512, &infoLen, &infoLog[0]);

	std::cout << std::string(infoLog.begin(), infoLog.end()) << std::endl;
}

void ShaderProgram::OutputProgramLog() const {
	std::vector<char> infoLog;
	infoLog.resize(512);

	GLint infoLen;
	glGetProgramiv(_ProgramID, GL_INFO_LOG_LENGTH, &infoLen);

	glGetProgramInfoLog(_ProgramID, sizeof(char) * 512, &infoLen, &infoLog[0]);

	std::cout << std::string(infoLog.begin(), infoLog.end()) << std::endl;
}
