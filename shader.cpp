#include "shader.h"

///-------------------------------------------------------------------------------------------

Shader::Shader(const char* vertexPath, const char* fragmentPath) {

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

	catch (const std::ifstream::failure&) {
		std::cerr << "ERROR::SHADER::FILE_NOT_SUCCESFULLY_READ\n";
	}

	const char* vShaderCode = vertexCode.c_str();
	const char* fShaderCode = fragmentCode.c_str();

	GLuint vertex, fragment;

	vertex = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertex, 1, &vShaderCode, nullptr);
	glCompileShader(vertex);
	checkCompileError(vertex, "vertex");

	fragment = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragment, 1, &fShaderCode, nullptr);
	glCompileShader(fragment);
	checkCompileError(fragment, "fragment");

	m_ID = glCreateProgram();
	glAttachShader(m_ID, vertex);
	glAttachShader(m_ID, fragment);
	glLinkProgram(m_ID);

	checkCompileError(m_ID, "PROGRAM");

	glDeleteShader(vertex);
	glDeleteShader(fragment);
}

///-------------------------------------------------------------------------------------------

void Shader::checkCompileError(const GLuint& shader, const std::string& type) const {
	int success;
	char infoLog[1024];

	if (type != "PROGRAM") {
		glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
		if (!success) {
			glGetShaderInfoLog(shader, 1024, NULL, infoLog);
			std::cerr << "ERROR::SHADER_COMPILATION_ERROR of type: " << type << "\n" << infoLog << "\n";
		}
	}
	else {
		glGetProgramiv(shader, GL_LINK_STATUS, &success);
		if (!success)
		{
			glGetProgramInfoLog(shader, 1024, NULL, infoLog);
			std::cerr << "ERROR::PROGRAM_LINKING_ERROR of type: " << type << "\n" << infoLog << "\n";
		}
	}
}

///-------------------------------------------------------------------------------------------

void Shader::use() {
	glUseProgram(m_ID);
}

///-------------------------------------------------------------------------------------------

GLuint Shader::getID() const {
	return m_ID; 
}

///-------------------------------------------------------------------------------------------
///1 param

void Shader::setb(const std::string& name, bool value) const {
	glUniform1i(glGetUniformLocation(m_ID, name.c_str()), static_cast<int>(value));
}

void Shader::seti(const std::string& name, int value) const {
	glUniform1i(glGetUniformLocation(m_ID, name.c_str()), value);
}

void Shader::setf(const std::string& name, float value) const {
	glUniform1f(glGetUniformLocation(m_ID, name.c_str()), value);
}

///-------------------------------------------------------------------------------------------
///2 param

void Shader::setb(const std::string& name, const std::array<bool, 2>& vals) const {
	glUniform2i(glGetUniformLocation(m_ID, name.c_str()),
		static_cast<bool>(vals[0]),
		static_cast<bool>(vals[1]));
}

void Shader::seti(const std::string& name, const std::array<int, 2>& vals) const {
	glUniform2i(glGetUniformLocation(m_ID, name.c_str()), vals[0], vals[1]);
}

void Shader::setf(const std::string& name, const std::array<float, 2>& vals) const {
	glUniform2f(glGetUniformLocation(m_ID, name.c_str()), vals[0], vals[1]);
}

///-------------------------------------------------------------------------------------------
///3 param

void Shader::setb(const std::string& name, const std::array<bool, 3>& vals) const {
	glUniform3i(glGetUniformLocation(m_ID, name.c_str()),
		static_cast<bool>(vals[0]),
		static_cast<bool>(vals[1]),
		static_cast<bool>(vals[2]));
}

void Shader::seti(const std::string& name, const std::array<int, 3>& vals) const {
	glUniform3i(glGetUniformLocation(m_ID, name.c_str()), vals[0], vals[1], vals[2]);
}

void Shader::setf(const std::string& name, const std::array<float, 3>& vals) const {
	glUniform3f(glGetUniformLocation(m_ID, name.c_str()), vals[0], vals[1], vals[2]);
}

///-------------------------------------------------------------------------------------------
///4 param

void Shader::setb(const std::string& name, const std::array<bool, 4>& vals) const {
	glUniform4i(glGetUniformLocation(m_ID, name.c_str()),
		static_cast<bool>(vals[0]),
		static_cast<bool>(vals[1]),
		static_cast<bool>(vals[2]),
		static_cast<bool>(vals[3]));
}

void Shader::seti(const std::string& name, const std::array<int, 4>& vals) const {
	glUniform4i(glGetUniformLocation(m_ID, name.c_str()), vals[0], vals[1], vals[2], vals[3]);
}

void Shader::setf(const std::string& name, const std::array<float, 4>& vals) const {
	glUniform4f(glGetUniformLocation(m_ID, name.c_str()), vals[0], vals[1], vals[2], vals[3]);
}

///--------------------------------------------------------------------------------