#ifndef TEXTURE_H_
#define TEXTURE_H_

#include <glad/glad.h>
#include "stb_image.h"
#include "shader.h"
#include <string>
#include <iostream>

class Texture2D {
private: 
	GLuint m_ID;
	int m_width;
	int m_height;
	int m_numberOfChannels; 
	std::string m_uniformTextureName; 
	int m_uniformTextureLocate; 

public:
	Texture2D(const std::string& name, GLint colorType, int desiredChannels,
		GLint wrapx, GLint wrapy, GLint minFilter, GLint maxFilter); 

	GLuint getID() const;
	int getWidth() const; 
	int getHeight() const;
	int getNumberOfChannels() const;

	void setUniformName(const std::string& name);
	void setUniformLocate(Shader& shader, int locate);
	void bindTexture() const; 

public:
	static void init(); 
};

#endif