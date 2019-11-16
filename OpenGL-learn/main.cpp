#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include "VertexBuffer.h"
#include "shader.h"

#include <fstream>

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow *window);

const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;

///-------------------------------------------------------------------------------------------

int main() {
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	#ifdef __APPLE__
		glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
	#endif

	GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "LearnOpenGL", NULL, NULL);
	if (window == nullptr) {
		std::cout << "Failed to create GLFW window\n";
		glfwTerminate();
		return -1;
	}

	glfwMakeContextCurrent(window);
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

	if (auto err = glewInit() != GLEW_OK) {
		std::cout << "Failed to initialize GLEW\n";
		return -1;
	}

	glEnable(GL_DEPTH_TEST);

	Shader shader("resources/shaders/vshader2.glsl", "resources/shaders/fshader2.glsl");

	/*
	std::vector<float> vertices = {
		-0.5f,  0.25f, 0.f, 240.f, 157.f, 2.f, 255.f, //0
		 0.5f,  0.25f, 0.f, 240.f, 157.f, 2.f, 255.f, //1
		 0.5f, -0.75f, 0.f, 240.f, 157.f, 2.f, 255.f, //2
		-0.5f, -0.75f, 0.f, 240.f, 157.f, 2.f, 255.f, //3
		-0.5f,  0.25f, 0.f, 240.f, 157.f, 2.f, 255.f, //4
		0.5f,  0.25f, 0.f,  240.f, 157.f, 2.f, 255.f, //5
		 0.f,   0.75f, 0.f, 240.f, 157.f, 2.f, 255.f, //6
	};

	std::vector<unsigned> indicies {
		0, 1, 3, 1, 2, 3, 4, 5, 6
	};

	VertexBufferManagement vao(DrawMode::Triangles, {
		VertexBufferAttribute("position", DataType::Float, 3),
		VertexBufferAttribute("color", DataType::Float, 4),
	});

	vao.setVertexDataPointer(reinterpret_cast<VertexBufferManagement::BytesPtr>(vertices.data()),
		vertices.size() * sizeof(float));
	vao.setElementDataPointer(reinterpret_cast<VertexBufferManagement::BytesPtr>(indicies.data()),
		indicies.size() * sizeof(unsigned), DataType::UInt);

	vao.saveAllToBuffer();
	vao.setBufferAttributes();

	saveModelToFile("model.omod", vao);
	*/

	char* vertexDataPointer = nullptr;
	char* elementDataPointer = nullptr; 
	// auto vao = loadModelFromFile("model.omod", vertexDataPointer, elementDataPointer);

	shader.use();
	shader.seti("pos", 0);
	shader.seti("col", 1);

	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	while (!glfwWindowShouldClose(window)) {

		processInput(window);

		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		shader.use(); 

		//vao.draw();
		// vao->draw(); 

		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	glfwTerminate();
	return 0;
}

///-------------------------------------------------------------------------------------------

void processInput(GLFWwindow *window) {
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);
}

///-------------------------------------------------------------------------------------------

void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
	glViewport(0, 0, width, height);
}


