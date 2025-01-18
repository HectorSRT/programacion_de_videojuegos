#include "glad/glad.h"
#include "GLFW/glfw3.h"

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"

#include <iostream>

#include "Shader.h"

using namespace std;

const unsigned int w = 600;
const unsigned int h = 400;

unsigned int VAO_1, VBO_1, EBO_1;
unsigned int VAO_2, VBO_2, EBO_2;

vec3 sizeC1 = vec3(0.3f, 0.3f, 0.0f);
vec3 sizeC2 = vec3(0.3f, 0.3f, 0.0f);

vec3 colorC1 = vec3(1.0f, 0.0f, 0.0f);
vec3 colorC2 = vec3(0.0f, 1.0f, 0.0f);

float vertex1[] = {
	//Posicion                       //Color
	 sizeC1.x,  sizeC1.y, sizeC1.z,  colorC1.x, colorC1.y, colorC1.z,
	 sizeC1.x, -sizeC1.y, sizeC1.z,  colorC1.x, colorC1.y, colorC1.z,
	-sizeC1.x, -sizeC1.y, sizeC1.z,  colorC1.x, colorC1.y, colorC1.z,
	-sizeC1.x,  sizeC1.y, sizeC1.z,  colorC1.x, colorC1.y, colorC1.z
};

float vertex2[] = {
	//Posicion                       //Color
	sizeC2.x,  sizeC2.y, sizeC2.z,  colorC2.x, colorC2.y, colorC2.z,
	sizeC2.x, -sizeC2.y, sizeC2.z,  colorC2.x, colorC2.y, colorC2.z,
   -sizeC2.x, -sizeC2.y, sizeC2.z,  colorC2.x, colorC2.y, colorC2.z,
   -sizeC2.x,  sizeC2.y, sizeC2.z,  colorC2.x, colorC2.y, colorC2.z
};

unsigned int indice[] = {
	0, 1, 3,
	1, 2, 3
};

float moveX = 0.0f, moveY = 0.0f;
bool isColliding = false;

void InitGLFWVersion();
bool GladLoad();
void UpdateWindow(GLFWwindow* window, Shader shader);

void Framebuffer_Size_Callback(GLFWwindow* window, int w, int h);
void ProcessInput(GLFWwindow* window);
void Move(GLFWwindow* window);

void GeneraBuffer(unsigned int& VAO, unsigned int& VBO, unsigned int& EBO, float v[], size_t vertexSize);
void VertexAttribPointer(int layout, int data, int total, int start);
bool CheckCollisionAABB(vec3 pos1, vec3 pos2, vec2 size1, vec2 size2);

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

	Shader shader("vertexShader.vs", "fragmentShader.fs");

	GeneraBuffer(VAO_1, VBO_1, EBO_1, vertex1, sizeof(vertex1));
	GeneraBuffer(VAO_2, VBO_2, EBO_2, vertex2, sizeof(vertex2));

	UpdateWindow(window, shader);

	glDeleteVertexArrays(1, &VAO_1);
	glDeleteBuffers(1, &EBO_1);
	glDeleteBuffers(1, &VBO_1);

	glDeleteVertexArrays(1, &VAO_2);
	glDeleteBuffers(1, &EBO_2);
	glDeleteBuffers(1, &VBO_2);

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
void UpdateWindow(GLFWwindow* window, Shader shader)
{
	while (!glfwWindowShouldClose(window))
	{
		ProcessInput(window);

		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		shader.use();
		vec3 pos1 = vec3(-0.7f, 0.0f, 0.0f);
		vec3 pos2 = vec3(moveX, moveY, 0.0f);

		isColliding = CheckCollisionAABB(pos1, pos2, sizeC1, sizeC2);
		if (isColliding)
		{
			cout << "Colisión detectada" << endl;
		}

		mat4 transform1 = mat4(1.0f);
		transform1 = translate(transform1, pos1);
		transform1 = scale(transform1, vec3(0.5f, 0.5f, 0.5f));

		shader.setMat4("transform", transform1);
		glBindVertexArray(VAO_1);
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

		mat4 transform2 = mat4(1.0f);
		transform2 = translate(transform2, pos2);
		transform2 = scale(transform2, vec3(0.5f, 0.5f, 0.5f));
		shader.setMat4("transform", transform2);

		glBindVertexArray(VAO_2);
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

		glfwSwapBuffers(window);
		glfwPollEvents();
	}
}
void ProcessInput(GLFWwindow* window)
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

	Move(window);
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
void GeneraBuffer(unsigned int& VAO, unsigned int& VBO, unsigned int& EBO, float v[], size_t vertexSize)
{
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);

	glBindVertexArray(VAO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, vertexSize, v, GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indice), indice, GL_STATIC_DRAW);

	VertexAttribPointer(0, 3, 6, 0);
	VertexAttribPointer(1, 3, 6, 3);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}
void VertexAttribPointer(int layout, int data, int total, int start)
{
	glVertexAttribPointer(layout, data, GL_FLOAT, GL_FALSE, total * sizeof(float), (void*)(start * sizeof(float)));
	glEnableVertexAttribArray(layout);
}
bool CheckCollisionAABB(vec3 pos1, vec3 pos2, vec2 size1, vec2 size2)
{
	float left1 = pos1.x - size1.x / 2.0f;
	float right1 = pos1.x + size1.x / 2.0f;
	float top1 = pos1.y + size1.y / 2.0f;
	float bottom1 = pos1.y - size1.y / 2.0f;

	float left2 = pos2.x - size2.x / 2.0f;
	float right2 = pos2.x + size2.x / 2.0f;
	float top2 = pos2.y + size2.y / 2.0f;
	float bottom2 = pos2.y - size2.y / 2.0f;

	bool collisionX = right1 >= left2 && left1 <= right2;
	bool collisionY = top1 >= bottom2 && bottom1 <= top2;

	return collisionX && collisionY;
}