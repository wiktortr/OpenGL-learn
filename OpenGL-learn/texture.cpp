#include "texture.h"

///-------------------------------------------------------------------------------------------

Texture2D::Texture2D(const std::string& name, GLint colorType, int desiredChannels,
	GLint wrapx, GLint wrapy, GLint minFilter, GLint maxFilter) 
	: m_uniformTextureName(""), m_uniformTextureLocate(0) {
	glGenTextures(1, &m_ID);
	glBindTexture(GL_TEXTURE_2D, m_ID);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, wrapx);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, wrapy);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, minFilter);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, maxFilter);

	unsigned char* data = stbi_load(name.c_str(), &m_width, &m_height, &m_numberOfChannels, desiredChannels);

	if (data) {
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, m_width, m_height, 0, colorType, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else std::cerr << "TEXTURE ERROR -> Failed to load texture: " << name << "\n";

	if (desiredChannels != m_numberOfChannels) {
		std::cerr << "TEXTURE WARNING -> input desired channels != output desired channels:\n" <<
			"input desired channels: " << desiredChannels << "\noutput desired channels: " << m_numberOfChannels << "\n";
	}

	stbi_image_free(data);
}

///-------------------------------------------------------------------------------------------

GLuint Texture2D::getID() const {
	return m_ID; 
}

///-------------------------------------------------------------------------------------------

int Texture2D::getWidth() const {
	return m_width; 
}

///-------------------------------------------------------------------------------------------

int Texture2D::getHeight() const {
	return m_height; 
}

///-------------------------------------------------------------------------------------------

int Texture2D::getNumberOfChannels() const {
	return m_numberOfChannels;
}

///-------------------------------------------------------------------------------------------

void Texture2D::setUniformName(const std::string& name) {
	m_uniformTextureName = name;
}

///-------------------------------------------------------------------------------------------

void Texture2D::setUniformLocate(Shader& shader, int locate) {
	shader.use(); 
	shader.seti(m_uniformTextureName.c_str(), locate); 
	m_uniformTextureLocate = locate; 
}

///-------------------------------------------------------------------------------------------

void Texture2D::bindTexture() const {
	glActiveTexture(GL_TEXTURE0 + m_uniformTextureLocate);
	glBindTexture(GL_TEXTURE_2D, m_ID);
}

///-------------------------------------------------------------------------------------------
///static

void Texture2D::init() {
	stbi_set_flip_vertically_on_load(true);
	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
}

