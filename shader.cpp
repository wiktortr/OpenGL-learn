#include "shader.h"

///--------------------------------------------------------------------------------

void Shader::checkCompileError(const GLuint& shader, const std::string& type) const {
	int success;
	char infoLog[1024];

	if (type != "PROGRAM") {
		glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
		if (!success) {
			glGetShaderInfoLog(shader, 1024, NULL, infoLog);
			std::cout << "ERROR::SHADER_COMPILATION_ERROR of type: " << type << "\n" << infoLog << "\n";
		}
	}
	else {
		glGetProgramiv(shader, GL_LINK_STATUS, &success);
		if (!success)
		{
			glGetProgramInfoLog(shader, 1024, NULL, infoLog);
			std::cout << "ERROR::PROGRAM_LINKING_ERROR of type: " << type << "\n" << infoLog << "\n";
		}
	}
}

///--------------------------------------------------------------------------------

Shader::Shader(const GLchar* vertexPath, const GLchar* fragmentPath) {

	std::string vertexCode;
	std::string fragmentCode;
	std::ifstream vShaderFile;
	std::ifstream fShaderFile;

	vShaderFile.exceptions(std::ifstream::failbit | std::istream::badbit);
	fShaderFile.exceptions(std::ifstream::failbit | std::istream::badbit);

	try {
		vShaderFile.open(vertexPath);
		fShaderFile.open(fragmentPath);

		std::stringstream vShaderStream, fShaderStream;
		vShaderStream << vShaderFile.rdbuf();
		fShaderStream << fShaderFile.rdbuf();

		vShaderFile.close();
		fShaderFile.close();

		vertexCode = vShaderStream.str();
		fragmentCode = fShaderStream.str();
	}

	catch (const std::ifstream::failure& e) {
		std::cout << "ERROR::SHADER::FILE_NOT_SUCCESFULLY_READ\n";
	}

	const char* vShaderCode = vertexCode.c_str();
	const char* fShaderCode = fragmentCode.c_str();

	GLuint vertex, fragment;
	int success;
	char infoLog[512];

	vertex = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertex, 1, &vShaderCode, nullptr);
	glCompileShader(vertex);
	checkCompileError(vertex, "vertex");

	fragment = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragment, 1, &fShaderCode, nullptr);
	glCompileShader(fragment);
	checkCompileError(fragment, "fragment");

	ID = glCreateProgram();
	glAttachShader(ID, vertex);
	glAttachShader(ID, fragment);
	glLinkProgram(ID);

	checkCompileError(ID, "PROGRAM");

	glDeleteShader(vertex);
	glDeleteShader(fragment);
}

///--------------------------------------------------------------------------------

void Shader::use() {
	glUseProgram(ID);
}

///--------------------------------------------------------------------------------

void Shader::setb(const std::string& name, bool value) const {
	glUniform1i(glGetUniformLocation(ID, name.c_str()), static_cast<int>(value));
}

///--------------------------------------------------------------------------------

void Shader::seti(const std::string& name, int value) const {
	glUniform1i(glGetUniformLocation(ID, name.c_str()), value);
}

///--------------------------------------------------------------------------------

void Shader::setf(const std::string& name, float value) const {
	glUniform1f(glGetUniformLocation(ID, name.c_str()), value);
}

///--------------------------------------------------------------------------------

void Shader::set4b(const std::string& name, const std::array<bool, 4>& vals) const {
	glUniform4i(glGetUniformLocation(ID, name.c_str()),
		static_cast<bool>(vals[0]),
		static_cast<bool>(vals[1]),
		static_cast<bool>(vals[2]),
		static_cast<bool>(vals[3]));
}

///--------------------------------------------------------------------------------

void Shader::set4i(const std::string& name, const std::array<int, 4>& vals) const {
	glUniform4i(glGetUniformLocation(ID, name.c_str()), vals[0], vals[1], vals[2], vals[3]);
}

///--------------------------------------------------------------------------------

void Shader::set4f(const std::string& name, const std::array<float, 4>& vals) const {
	glUniform4f(glGetUniformLocation(ID, name.c_str()), vals[0], vals[1], vals[2], vals[3]);
}