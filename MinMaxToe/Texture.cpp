#include "Texture.h"
#include "Errors.h"
#include <SOIL.h>
#include <iostream>

Texture::~Texture() {
	Unload();
}

void Texture::Load(const std::string &file)
{
	std::cout << "Loading Texture from " << file << "...\n";
	TexObj = SOIL_load_OGL_texture(file.c_str(), SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, SOIL_FLAG_MIPMAPS);

	if (TexObj == 0) {
		std::cout << "Texture failed to load.\n" << SOIL_last_result() << std::endl;
		FatalError("Texture failed to load.\n" + std::string(SOIL_last_result()));
	}

	glBindTexture(GL_TEXTURE_2D, TexObj);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
}

void Texture::Unload() {
	if (TexObj != 0) {
		glDeleteTextures(1, &TexObj);
	}
}

	 
void Texture::Bind() {
	glBindTexture(GL_TEXTURE_2D, TexObj);
}

void Texture::Unbind() {
	glBindTexture(GL_TEXTURE_2D, 0);
}
