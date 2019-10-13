#ifndef SHADER_H_
#define SHADER_H_

#include <glad\glad.h>
#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <array>

class Shader {

private:
	GLuint m_ID;
	void checkCompileError(const GLuint& shader, const std::string& type) const; 

public:
	Shader(const char* vertexPath, const char* fragmentPath);

	void use();
	GLuint getID() const; 

	///1 param
	void setb(const std::string& name, bool value) const;
	void seti(const std::string& name, int value) const;
	void setf(const std::string& name, float value) const;

	///2 param
	void setb(const std::string& name, const std::array<bool, 2>& vals) const;
	void seti(const std::string& name, const std::array<int, 2>& vals) const;
	void setf(const std::string& name, const std::array<float, 2>& vals) const;

	///3 param
	void setb(const std::string& name, const std::array<bool, 3>& vals) const;
	void seti(const std::string& name, const std::array<int, 3>& vals) const;
	void setf(const std::string& name, const std::array<float, 3>& vals) const;

	///4 param
	void setb(const std::string& name, const std::array<bool, 4>& vals) const;
	void seti(const std::string& name, const std::array<int, 4>& vals) const;
	void setf(const std::string& name, const std::array<float, 4>& vals) const; 
};

#endif