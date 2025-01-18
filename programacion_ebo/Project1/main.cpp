#include "glad/glad.h"
#include "GLFW/glfw3.h"

#include <iostream>

using namespace std;

const unsigned int w = 600;
const unsigned int h = 400;

void InitGLFWVersion();
bool GladLoad();
void UpdateWindow(GLFWwindow* window);

void Framebuffer_Size_Callback(GLFWwindow* window, int w, int h);

int Vertex();
int Fragmen();
void CheckShader(int shader);
int UnionShader(int v, int f);
void GeneraBuffer();
void ProcessInput(GLFWwindow* window);

void VertexAttribPointer(int layout, int data, int total, int start);

const char* vertexShaderSource =
"#version 330 core \n"
"layout(location = 0) in vec3 aPos;\n"
"void main()\n"
"{\n"
"gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0f);\n"
"}\n\0";

const char* fragmenShaderSource =
"#version 330 core\n"
"out vec4 FragColor;\n"
"void main()\n"
"{\n"
"FragColor = vec4(1.0f, 0.0f, 0.0f, 1.0f);\n"
"}\n\0";

int v, f, s;
unsigned int VAO, VBO;
unsigned int EBO;

float vertex[] = {
	// Triángulo 1
	 0.5f,  -0.5f, 0.0f,  // Centro
	 0.0f,  0.5f, 0.0f,  // Derecha
	 -0.5f,  -0.5f, 0.0f, // Arriba derecha
};

unsigned int indice[] = {
	0,1,2
};

int main()
{
	InitGLFWVersion();

	GLFWwindow* window = glfwCreateWindow(w, h, "OpenGL", NULL, NULL);
	if (window == NULL)
	{
		cout << "Falla al crear ventana" << endl;
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);

	if (!GladLoad())
	{
		return -1;
	}

	glfwSetFramebufferSizeCallback(window, Framebuffer_Size_Callback);

	v = Vertex();
	f = Fragmen();
	s = UnionShader(v, f);

	GeneraBuffer();

	UpdateWindow(window);

	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &EBO);
	glDeleteBuffers(1, &VBO);
	glDeleteProgram(s);

	glfwTerminate();
	return 0;
}

void InitGLFWVersion()
{
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif // __APPLE__
}

void VertexAttribPointer(int layout, int data, int total, int start) {
	glVertexAttribPointer(layout, data, GL_FLOAT, GL_FALSE, total * sizeof(float), (void*)(start * sizeof(float)));
	glEnableVertexAttribArray(layout);
}

bool GladLoad()
{
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		cout << "Falla al utilizar glad" << endl;
		return false;
	}
	return true;
}
void Framebuffer_Size_Callback(GLFWwindow* window, int w, int h)
{
	glViewport(0, 0, w, h);
}

void UpdateWindow(GLFWwindow* window) {
	while (!glfwWindowShouldClose(window)) {
		ProcessInput(window);

		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		glUseProgram(s);

		glBindVertexArray(VAO);
		//glDrawArrays(GL_TRIANGLES, 0, 3);
		glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_INT, 0);

		glfwSwapBuffers(window);
		glfwPollEvents();
	}
}


int Vertex() {
	unsigned int vertexShader;
	vertexShader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
	glCompileShader(vertexShader);
	CheckShader(vertexShader);
	return vertexShader;
}

int Fragmen() {
	unsigned int fragmenShader;
	fragmenShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmenShader, 1, &fragmenShaderSource, NULL);
	glCompileShader(fragmenShader);
	CheckShader(fragmenShader);
	return fragmenShader;
}

void CheckShader(int shader) {
	int sucess;
	char infoLog[512];

	glGetShaderiv(shader, GL_COMPILE_STATUS, &sucess);
	if (!sucess) {
		glGetShaderInfoLog(shader, 512, NULL, infoLog);
		cout << "No se pudo crear el shader" << endl;
		cout << infoLog << endl;
	}
}

int UnionShader(int v, int f) {
	unsigned int shaderProgram;
	shaderProgram = glCreateProgram();
	glAttachShader(shaderProgram, v);
	glAttachShader(shaderProgram, f);
	glLinkProgram(shaderProgram);

	int sucess;
	char infoLog[512];

	glGetProgramiv(shaderProgram, GL_LINK_STATUS, &sucess);
	if (!sucess) {
		glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
		cout << "No se pudo crear el programa" << endl;
		cout << infoLog << endl;
	}

	glDeleteShader(v);
	glDeleteShader(f);

	return shaderProgram;
}

void GeneraBuffer() {
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);

	glBindVertexArray(VAO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertex), vertex, GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indice), indice, GL_STATIC_DRAW);

	VertexAttribPointer(0, 3, 3, 0);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}

void ProcessInput(GLFWwindow* window) {
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
		glfwSetWindowShouldClose(window, true);
	}
	if (glfwGetKey(window, GLFW_KEY_1) == GLFW_PRESS) {
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	}
	if (glfwGetKey(window, GLFW_KEY_2) == GLFW_PRESS) {
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	}
}
