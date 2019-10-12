#ifndef SHADER_H_
#define SHADER_H_

#include <glad\glad.h>
#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <array>

class Shader {

public:
	unsigned int ID;
	void checkCompileError(const GLuint& shader, const std::string& type) const; 

public:
	Shader(const GLchar* vertexPath, const GLchar* fragmentPath);

	void use();

	void setb(const std::string& name, bool value) const;
	void seti(const std::string& name, int value) const;
	void setf(const std::string& name, float value) const;

	void set4b(const std::string& name, const std::array<bool, 4>& vals) const;
	void set4i(const std::string& name, const std::array<int, 4>& vals) const;
	void set4f(const std::string& name, const std::array<float, 4>& vals) const; 
};

#endif