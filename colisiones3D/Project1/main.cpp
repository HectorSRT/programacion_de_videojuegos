#include <iostream>

#include "glad/glad.h"
#include "GLFW/glfw3.h"

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"

#include "Shader.h"
#include "Cubo.h"
#include "Piso.h"
#include "Camera.h"

using namespace std;

const unsigned width = 500;
const unsigned height = 300;

unsigned int VBO, VAO, EBO;
unsigned int VBO_P, VAO_P, EBO_P;
unsigned int VAO_L;

mat4 projection, view;

vec3 cameraPos = vec3(0.0f, 0.0f, 3.0f);
vec3 cameraFront = vec3(0.0f, 0.0f, -1.0f);
vec3 cameraUp = vec3(0.0f, 1.0f, 0.0f);
float deltaTime = 0.0f;
float lastTime = 0.0f;

Camera camera(vec3(0.0f, 2.0f, 7.0f));
float lastX = width / 2.0f;
float lastY = height / 2.0f;
bool firstMouse = true;
bool gravedadActive = false;

struct AABB {
	vec3 min;
	vec3 max;
};

void initGLFWVersion();
bool gladLoad();
void updateWindow(GLFWwindow* window, Shader ourShader, Shader ourLight, Shader ourShaderPiso);

void framebuffer_size_callback(GLFWwindow* window, int w, int h);
void processInput(GLFWwindow* window);
void CameraInput(GLFWwindow* window);
void PlayerInput(GLFWwindow* window);
void Mouse_callback(GLFWwindow* window, double xpos, double ypos);
void Scroll_callback(GLFWwindow* window, double xoffset, double yoffset);

void GeneracionBuffer(
	GLuint& VAO,
	GLuint& VBO,
	GLuint& EBO,
	const GLfloat* vertices,
	size_t verticesSize,
	const GLuint* indices,
	size_t indicesSize,
	GLuint& VAO_L);
void VertexAttribute(int layout, int data, int total, int start);
void DeleteVertexArrays(GLuint& VA);
void DeleteBuffer(GLuint& VBO, GLuint& EBO);
void TransformCubo(Shader ourShader);
void TransformPiso(Shader ourShader);
void TransformCuboLight(Shader ourLight);
void TransformCamera(Shader ourShader);
void CameraUniform(Shader shaderName);
void updatePhysics(float deltaTime);

bool AABBIntersect(AABB box1, AABB box2);
AABB GenerateBoindingBox(vec3 position, float w, float h, float d);
bool DetecCollision();

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
	Shader ourShaderPiso("vertexShader.vs", "fragmentShader.fs");
	Shader ourLight("vertexLight.vl", "fragmentLight.fl");

	GeneracionBuffer(VAO, VBO, EBO, vertices, sizeof(vertices), indices, sizeof(indices), VAO_L);
	GeneracionBuffer(VAO_P, VBO_P, EBO_P, pisoVertices, sizeof(pisoVertices), pisoIndices, sizeof(pisoIndices), VAO_L);

	updateWindow(window, ourShader, ourLight, ourShaderPiso);

	DeleteVertexArrays(VAO);
	DeleteVertexArrays(VAO_L);
	DeleteBuffer(VBO, EBO);

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

	CameraInput(window);
	PlayerInput(window);
}
void CameraInput(GLFWwindow* window)
{
	if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS)
	{
		camera.ProcessKeyboard(FORWARD, deltaTime);
	}
	if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS)
	{
		camera.ProcessKeyboard(BACKWARD, deltaTime);
	}
	if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS)
	{
		camera.ProcessKeyboard(LEFT, deltaTime);
	}
	if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS)
	{
		camera.ProcessKeyboard(RIGHT, deltaTime);
	}
}
void PlayerInput(GLFWwindow* window)
{
	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
	{
		posCube[0].z -= 0.01f;
	}
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
	{
		posCube[0].z += 0.01f;
	}
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
	{
		posCube[0].x -= 0.01f;
	}
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
	{
		posCube[0].x += 0.01f;
	}
	if (glfwGetKey(window, GLFW_KEY_F) == GLFW_PRESS)
	{
		gravedadActive = true;
	}
	if (DetecCollision())
	{
		cuboVel.y = 0;
		posCube[0].y = pisoPosCube[0].y;
	}
}
void Mouse_callback(GLFWwindow* window, double xposIn, double yposIn)
{
	float xpos = xposIn;
	float ypos = yposIn;

	if (firstMouse)
	{
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
void Scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
	camera.ProcessMouseScroll(yoffset);
}
void updateWindow(GLFWwindow* window, Shader ourShader, Shader ourLight, Shader ourShaderPiso)
{
	while (!glfwWindowShouldClose(window))
	{
		float currenTime = glfwGetTime();
		deltaTime = currenTime - lastTime;
		lastTime = currenTime;

		processInput(window);

		if (gravedadActive)
		{
			updatePhysics(deltaTime);
		}

		glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		ourShader.use();

		ourShader.setVec3("dirLight.direction", -0.2f, -1.0f, -0.3f);
		ourShader.setVec3("dirLight.ambient", 0.05f, 0.05f, 0.05f);
		ourShader.setVec3("dirLight.diffuse", 0.3f, 0.3f, 0.3f);
		ourShader.setVec3("dirLight.specular", 0.5f, 0.5f, 0.5f);

		ourShader.setVec3("viewPos", camera.Position);

		ourShader.setVec3("material.diffuse", 0.0f, 0.5f, 1.0f);
		ourShader.setVec3("material.specular", 0.5f, 0.5f, 1.5f);
		ourShader.setFloat("material.shininess", 32.0f);

		TransformCamera(ourShader);
		TransformCubo(ourShader);

		ourShaderPiso.use();

		ourShaderPiso.setVec3("dirLight.direction", -0.2f, -1.0f, -0.3f);
		ourShaderPiso.setVec3("dirLight.ambient", 0.05f, 0.05f, 0.05f);
		ourShaderPiso.setVec3("dirLight.diffuse", 0.3f, 0.3f, 0.3f);
		ourShaderPiso.setVec3("dirLight.specular", 0.5f, 0.5f, 0.5f);

		ourShaderPiso.setVec3("viewPos", camera.Position);

		ourShaderPiso.setVec3("material.diffuse", 0.2f, 1.0f, 0.2f);
		ourShaderPiso.setVec3("material.specular", 0.5f, 0.5f, 1.5f);
		ourShaderPiso.setFloat("material.shininess", 32.0f);

		CameraUniform(ourShaderPiso);
		TransformPiso(ourShaderPiso);

		glfwSwapBuffers(window);
		glfwPollEvents();
	}
}
void GeneracionBuffer(GLuint& VAO,
	GLuint& VBO,
	GLuint& EBO,
	const GLfloat* vertices,
	size_t verticesSize,
	const GLuint* indices,
	size_t indicesSize,
	GLuint& VAO_L)
{
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);

	glBindVertexArray(VAO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, verticesSize, vertices, GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indicesSize, indices, GL_STATIC_DRAW);

	VertexAttribute(0, 3, 6, 0);
	VertexAttribute(1, 3, 6, 3);

	//light
	glGenVertexArrays(1, &VAO_L);
	glBindVertexArray(VAO_L);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);

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
void DeleteVertexArrays(GLuint& VA)
{
	glDeleteVertexArrays(1, &VA);
}
void DeleteBuffer(GLuint& VBO, GLuint& EBO)
{
	glDeleteBuffers(1, &VBO);
	glDeleteBuffers(1, &EBO);
}
void TransformCubo(Shader ourShader)
{
	int tam = sizeof(posCube) / sizeof(posCube[0]);
	glBindVertexArray(VAO);
	for (int i = 0; i < tam; i++)
	{
		mat4 modelo = mat4(1.0f);
		modelo = translate(modelo, posCube[i]);
		//modelo = rotate(modelo, radians(20.0f * i), vec3(1.0f, 0.3f, 0.5f));
		ourShader.setMat4("model", modelo);
		glDrawElements(GL_TRIANGLES, sizeof(indices) / sizeof(indices[0]), GL_UNSIGNED_INT, 0);
	}

}
void TransformPiso(Shader ourShader)
{
	glBindVertexArray(VAO_P);
	mat4 modelo = mat4(1.0f);
	modelo = translate(modelo, pisoPosCube[0]);
	ourShader.setMat4("model", modelo);
	glDrawElements(GL_TRIANGLES, sizeof(pisoIndices) / sizeof(pisoIndices[0]), GL_UNSIGNED_INT, 0);
}
void TransformCuboLight(Shader ourLight)
{
	int tam = sizeof(posCubeLight) / sizeof(posCubeLight[0]);
	glBindVertexArray(VAO_L);
	for (int i = 0; i < tam; i++)
	{
		mat4 modelo = mat4(1.0f);
		modelo = translate(modelo, posCubeLight[i]);
		modelo = scale(modelo, vec3(0.2f));
		ourLight.setMat4("model", modelo);
		glDrawElements(GL_TRIANGLES, sizeof(indices) / sizeof(indices[0]), GL_UNSIGNED_INT, 0);
	}
}
void TransformCamera(Shader ourShader)
{
	projection = perspective(radians(camera.Zoom), (float)width / (float)height, 0.1f, 100.0f);
	view = camera.GetViewMatrix();

	CameraUniform(ourShader);
}
void CameraUniform(Shader shaderName)
{
	shaderName.setMat4("projection", projection);
	shaderName.setMat4("view", view);
}

void updatePhysics(float deltaTime)
{
	cuboVel.y += gravedad * deltaTime;
	posCube[0].y += cuboVel.y * deltaTime;

	/*if (posCube[0].y < 0.0f)
	{
		posCube[0].y = 0.0f;
		cuboVel.y = 0;
	}*/
}

bool AABBIntersect(AABB box1, AABB box2)
{
	bool col;
	bool xOverlap = box1.min.x <= box2.max.x && box1.max.x >= box2.min.x;
	bool yOverlap = box1.min.y <= box2.max.y && box1.max.y >= box2.min.y;
	bool zOverlap = box1.min.z <= box2.max.z && box1.max.z >= box2.min.z;

	col = xOverlap && yOverlap && zOverlap;
	return col;
}
AABB GenerateBoindingBox(vec3 position, float w, float h, float d)
{
	AABB box;
	box.min = vec3((position.x - w) / 2.0f, position.y, (position.z - d) / 2.0f);
	box.max = vec3((position.x + w) / 2.0f, position.y + h, (position.z + d) / 2.0f);
	return box;
}
bool DetecCollision()
{
	bool col = false;
	AABB cubeBox = GenerateBoindingBox(posCube[0], 1.0f, 1.0f, 1.0f);
	AABB floorBox = GenerateBoindingBox(pisoPosCube[0], 5.0f, 0.1f, 5.0f);

	if (AABBIntersect(cubeBox, floorBox))
	{
		col = true;
	}
	return col;
}