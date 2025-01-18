#include <iostream>

#include "glad/glad.h"
#include "GLFW/glfw3.h"

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"

#include "Shader.h"
#include "Cubo.h"
#include "Camera.h"

using namespace std;
using namespace glm;

const unsigned width = 500;
const unsigned height = 300;

vec3 cameraPos = vec3(0.0f, 0.0f, 3.0f);
vec3 cameraFront = vec3(0.0f, 0.0f, -1.0f);
vec3 cameraUp = vec3(0.0f, 1.0f, 0.0f);

float deltaTime = 0.0f;
float lastTime = 0.0f;

Camera camera(vec3(0.0f, 0.0f, 3.0f));

float lastX = width / 2.0f;
float lastY = height / 2.0f;
bool firstMouse = true;

unsigned int VBO, VAO, EBO;
unsigned int VAO_L;
float moveX = 0.0f, moveY = 0.0f;

void initGLFWVersion();
bool gladLoad();
void updateWindow(GLFWwindow* window, Shader ourShader, Shader ourLight);

void framebuffer_size_callback(GLFWwindow* window, int w, int h);
void processInput(GLFWwindow* window);

void GeneracionBuffer();
void VertexAttribute(int layout, int data, int total, int start);

void Move(GLFWwindow* window);
void SistemaCoordenado(Shader ourShader);

void CameraInput(GLFWwindow* window);
void Mouse_callback(GLFWwindow* window, double xpos, double ypos);
void TransformCubo(Shader ourShader);
void TransformCamera(Shader ourShader);
void Scroll_callback(GLFWwindow* window, double xoffset, double yoffset);

void GeneracionBufferLight();
void DeleteVertexArrays(GLuint& VA);
void DeleteBuffer(GLuint& VBO, GLuint& EBO);
void TransformCuboLight(Shader ourLight);
void CameraUniform(Shader ourLight);

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

	glfwSetCursorPosCallback(window, Mouse_callback);
	glfwSetScrollCallback(window, Scroll_callback);
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	glEnable(GL_DEPTH_TEST);

	Shader ourShader("vertexShader.vs", "fragmentShader.fs");
	Shader ourLight("vertexLight.vl", "fragmentLight.fl");

	GeneracionBuffer();

	updateWindow(window, ourShader, ourLight);

	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(VBO ,&EBO);

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

//FUNCIONES ILUMINACION
void DeleteVertexArrays(GLuint& VA) {
	glDeleteVertexArrays(1, &VA);
}

void DeleteBuffer(GLuint& VBO, GLuint& EBO) {
	glDeleteBuffers(1, &VBO);
	glDeleteBuffers(1, &EBO);
}

void GeneracionBufferLight() {
	// Light
	glGenVertexArrays(1, &VAO_L);
	glBindVertexArray(VAO_L);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	VertexAttribute(0, 3, 3, 0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}

void TransformCuboLight(Shader ourLight) {
	int tam = sizeof(posCubeLight) / sizeof(posCubeLight[0]);
	glBindVertexArray(VAO_L);

	for (int i = 0; i < tam; i++) {
		mat4 modelo = mat4(1.0f);
		modelo = translate(modelo, posCubeLight[i]);
		modelo = scale(modelo, vec3(0.2f));
		ourLight.setMat4("model", modelo);
		glDrawElements(GL_TRIANGLES, sizeof(indices) / sizeof(indices[0]), GL_UNSIGNED_INT, 0);
	}
}




void Move(GLFWwindow* window)
{
	if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS)
	{
		moveY -= 0.001f;
	}
	if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS)
	{
		moveY += 0.001f;
	}
	if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS)
	{
		moveX -= 0.001f;
	}
	if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS)
	{
		moveX += 0.001f;
	}
}

void SistemaCoordenado(Shader ourShader)
{
	mat4 transform = mat4(1.0f);
	transform = translate(transform, vec3(0.0f, 0.0f, -3.0f));
	transform = rotate(transform, (float)glfwGetTime(), vec3(3.0f, 2.0f, 0.0f));

	ourShader.setMat4("model", transform);

	mat4 view = mat4(1.0f);
	view = translate(view, vec3(0.0f, 0.0f, -3.0f));
	ourShader.setMat4("view", view);

	mat4 projection;

	projection = perspective(radians(45.0f), (float)width / (float)height, 0.1f, 100.0f);
	ourShader.setMat4("projection", projection);
}


void updateWindow(GLFWwindow* window, Shader ourShader, Shader ourLight) {
	while (!glfwWindowShouldClose(window)) {
		float currentTime = glfwGetTime();
		deltaTime = currentTime - lastTime;
		lastTime = currentTime;

		processInput(window);

		glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		ourShader.use();
		ourShader.setVec3("objectColor", 0.0f, 0.0f, 1.0f);
		ourShader.setVec3("lightColor", 1.0f, 1.0f, 1.0f);
		TransformCamera(ourShader);
		TransformCubo(ourShader);

		ourLight.use();
		CameraUniform(ourLight);
		TransformCuboLight(ourLight);

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

	VertexAttribute(0, 3, 6, 0);
	VertexAttribute(1, 3, 6, 3);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}

void VertexAttribute(int layout, int data, int total, int start)
{
	glVertexAttribPointer(layout, data, GL_FLOAT, GL_FALSE, total * sizeof(float), (void*)(start * sizeof(float)));
	glEnableVertexAttribArray(layout);
}

void CameraInput(GLFWwindow* window) {
	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS || glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS) {
		camera.ProcessKeyboard(FORWARD, deltaTime);
	}

	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS || glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS) {
		camera.ProcessKeyboard(BACKWARD, deltaTime);
	}

	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS || glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS) {
		camera.ProcessKeyboard(LEFT, deltaTime);
	}

	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS || glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS) {
		camera.ProcessKeyboard(RIGHT, deltaTime);
	}
}

void Mouse_callback(GLFWwindow* window, double xposIn, double yposIn) {
	float xpos = xposIn;
	float ypos = yposIn;

	if (firstMouse) {
		lastX = xpos;
		lastY = ypos;
		firstMouse = false;
	}

	float xoffset = xpos - lastX;
	float yoffset = lastY - ypos;

	lastX = xpos;
	lastY = ypos;

	camera.ProcessMouseMovement(xoffset, yoffset);
}

void TransformCubo(Shader ourShader) {
	int tam = sizeof(posCube) / sizeof(posCube[0]);
	glBindVertexArray(VAO);

	for (int i = 0; i < tam; i++) {
		mat4 modelo = mat4(1.0f);
		modelo = translate(modelo, posCube[i]);
		modelo = rotate(modelo, radians(-45.0f), vec3(0.3f, 0.7f, 0.0f));
		ourShader.setMat4("modelo", modelo);
		glDrawElements(GL_TRIANGLES, sizeof(indices) / sizeof(indices[0]), GL_UNSIGNED_INT, 0);
	}
}

void TransformCamera(Shader ourShader) {
	mat4 projection;
	projection = perspective(radians(camera.Zoom), (float)width / (float)height, 0.1f, 100.0f);
	ourShader.setMat4("projection", projection);

	mat4 view = camera.GetViewMatrix();
	ourShader.setMat4("view", view);
}

void Scroll_callback(GLFWwindow* window, double xoffset, double yoffset) {
	camera.ProcessMouseScroll(yoffset);
}

void CameraUniform(Shader ourLight)
{
	mat4 projection;
	ourLight.setMat4("projection", projection);
}