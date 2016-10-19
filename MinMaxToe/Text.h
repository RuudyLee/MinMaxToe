#pragma once
#include "ShaderProgram.h"
#include "Texture.h"

#include <ft2build.h>
#include FT_FREETYPE_H

struct point {
	GLfloat x;
	GLfloat y;
	GLfloat s;
	GLfloat t;
};

class Text {
public:
	Text();
	~Text();

	int Init();
	void Render(const char *text, float x, float y, float sx, float sy);

public:
	ShaderProgram program;

	GLuint vao;
	GLuint vbo;

	FT_Library ft;
	FT_Face face;


private:
};

