#include <iostream>

#include "glad/glad.h"
#include "GLFW/glfw3.h"

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"

#include "Shader.h"

using namespace std;

const unsigned width = 500;
const unsigned height = 300;

float vertices[] = {
	// Posición           Color
	 0.0f,  1.0f, 0.0f,  1.0f, 1.0f, 0.0f,  // Vértice 0 (Superior)
	 0.5f,  0.0f, 0.0f,  1.0f, 0.0f, 0.0f,  // Vértice 1 (Derecho)
	 0.0f, -1.0f, 0.0f,  1.0f, 1.0f, 0.0f,  // Vértice 2 (Inferior)
	-0.5f,  0.0f, 0.0f,  1.0f, 0.0f, 0.0f,   // Vértice 3 (Izquierdo)
	-0.2f,  0.4f, 0.0f,  1.0f, 0.0f, 0.0f,	//vertice 4 (tIn1 izquierdo)
	0.0f,   0.7f, 0.0f,  1.0f, 1.0f, 0.0f,  //vertice 5 (tIn1 arriba)
	0.0f,   0.0f, 0.0f,  1.0f, 1.0f, 0.0f,   //vertice 6(centro)
	0.0f,   -0.7f, 0.0f,  1.0f, 1.0f, 0.0f,  //vertice 7 (tIn2 abajo)
	0.2f,  -0.4f,  0.0f, 1.0f, 0.0f, 0.0f   // vertice 8(tIn2 derecha)
};



unsigned int indices[] = {
	0,1,2,
	0,2,3,
	5,4,6,
	6,7,8
};

unsigned int VBO, VAO, EBO;

void initGLFWVersion();
bool gladLoad();
void updateWindow(GLFWwindow* window, Shader ourShader);

void framebuffer_size_callback(GLFWwindow* window, int w, int h);
void processInput(GLFWwindow* window);

void GeneracionBuffer();
void VertexAttribute(int layout, int data, int total, int start);

int main()
{
	initGLFWVersion();

	//Creacion de ventana
	GLFWwindow* window = glfwCreateWindow(width, height, "OpenGl", NULL, NULL);
	if (window == NULL)
	{
		cout << "Falla al cargar ventana" << endl;
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);

	if (!gladLoad())
	{
		return -1;
	}

	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

	Shader ourShader("vertexShader.vs", "fragmentShader.fs");

	GeneracionBuffer();

	updateWindow(window, ourShader);

	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);
	glDeleteBuffers(1, &EBO);

	glfwTerminate();
	return 0;
}

void initGLFWVersion()
{
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif // __APPLE__
}
bool gladLoad()
{
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		cout << "Falla al cargar glad" << endl;
		return false;
	}
	return true;
}
void framebuffer_size_callback(GLFWwindow* window, int w, int h)
{
	glViewport(0, 0, w, h);
}
void processInput(GLFWwindow* window)
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
	{
		glfwSetWindowShouldClose(window, true);
	}
	if (glfwGetKey(window, GLFW_KEY_1) == GLFW_PRESS)
	{
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	}
	if (glfwGetKey(window, GLFW_KEY_2) == GLFW_PRESS)
	{
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	}
}
void updateWindow(GLFWwindow* window, Shader ourShader)
{
	while (!glfwWindowShouldClose(window))
	{
		processInput(window);

		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		ourShader.use();

		double  timeValue = glfwGetTime();
		float greenValue = static_cast<float>(sin(timeValue) / 2.0 + 0.5);
		ourShader.setVec4("ourColor", 0.0f, greenValue, 0.0f, 1.0f);

		glBindVertexArray(VAO);
		glDrawElements(GL_TRIANGLES, sizeof(indices), GL_UNSIGNED_INT, 0);

		glfwSwapBuffers(window);
		glfwPollEvents();
	}
}

void GeneracionBuffer()
{
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);

	glBindVertexArray(VAO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	VertexAttribute(0, 3, 6, 0); // Posición, 3 floats (x, y, z)
	VertexAttribute(1, 3, 6, 3); // Color, 3 floats (r, g, b)


	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}
void VertexAttribute(int layout, int data, int total, int start)
{
	glVertexAttribPointer(layout, data, GL_FLOAT, GL_FALSE, total * sizeof(float), (void*)(start * sizeof(float)));
	glEnableVertexAttribArray(layout);
}