#include "glad/glad.h"
#include "GLFW/glfw3.h"
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"
#include <thread> // Necesario para sleep_for
#include <chrono> // Necesario para definir la duración

#include "Shader.h"
#include "Cubo.h"
#include "Camera.h"

#include <iostream>

using namespace std;
using namespace glm;

const unsigned int w = 600;
const unsigned int h = 400;

void InitGLFWVersion();
bool GladLoad();
void UpdateWindow(GLFWwindow* window, Shader shader);
void processInput(GLFWwindow* window);

void Framebuffer_Size_Callback(GLFWwindow* window, int w, int h);

void Move(GLFWwindow* window);
void SistemaCoordenado(Shader shader);

void CameraInput(GLFWwindow* window);
void Mouse_callback(GLFWwindow* window, double xpos, double ypos);
void TransformCubo(Shader ourShader);
void TransformCamera(Shader ourShader);
void Scroll_callback(GLFWwindow* window,double xoffset, double yoffset);
//----------------------------------------------------------------

int v, f, s;
unsigned int VAO, VBO, EBO;
unsigned int VAO2, VBO2, EBO2;
//----------------------------------------------------------------

// Variables de posición para el movimiento
float posX = 0.0f, posY = 0.0f;
float moveSpeed = 1.0f;  // Velocidad de movimient

// Variable para el ángulo de rotación
float rotationAngle = 0.0f;
float rotationIncrement = radians(45.0f);  // Incremento de 45 grados en radianes

// Variables de posición para el escalado
float X = 0.5f, Y = 0.5f, Z = 0.5f;

//----------------------------------------------------------------

//Variables para el movimiento de la camara

vec3 cameraPos = vec3(0.0f,0.0f, 3.0f);
vec3 cameraFront = vec3(0.0f,0.0f,-1.0f);
vec3 cameraUp = vec3(0.0f, 1.0f, 0.0f);
float deltaTime = 0.0f;
float lastTime = 0.0f;

Camera camera(vec3(0.0f, 0.0f, 3.0f));
float lastX = w / 2.0f;
float lastY = h / 2.0f;
bool firstMouse = true;
//----------------------------------------------------------------

// Variables globales para los límites de la cámara
const float X_LIMIT = 1.0f; // Límite lateral (izquierda/derecha)
const float Y_LIMIT = 1.0f; // Límite vertical (arriba/abajo)


/*float vertex[] =
{
	// Parte superior
	0.0f, 1.0f, 0.0f,          1.0f, 0.0f, 0.0f, // Vértice 0 (cima, rojo)
	0.5f, 0.0f, 0.0f,          0.0f, 1.0f, 0.0f, // Vértice 1 (esquina inferior, verde)
	0.309f, 0.0f, 0.951f,      0.0f, 0.0f, 1.0f, // Vértice 2 (esquina inferior, azul)
	-0.809f, 0.0f, 0.588f,     1.0f, 1.0f, 0.0f, // Vértice 3 (esquina inferior, amarillo)
	-0.809f, 0.0f, -0.588f,    1.0f, 0.0f, 1.0f, // Vértice 4 (esquina inferior, magenta)
	0.309f, 0.0f, -0.951f,      0.5f, 0.5f, 0.5f, // Vértice 5 (esquina inferior, gris)

	// Parte inferior (invertida)
	0.0f, -1.0f, 0.0f,         1.0f, 0.0f, 0.0f, // Vértice 6 (cima invertida, rojo)
	0.5f, 0.0f, 0.0f,          0.0f, 1.0f, 0.0f, // Vértice 7 (esquina inferior, verd_e)
	0.309f, 0.0f, 0.951f,      0.0f, 0.0f, 1.0f, // Vértice 8 (esquina inferior, azul)
	-0.809f, 0.0f, 0.588f,     1.0f, 1.0f, 0.0f, // Vértice 9 (esquina inferior, amarillo)
	-0.809f, 0.0f, -0.588f,    1.0f, 0.0f, 1.0f, // Vértice 10 (esquina inferior, magenta)
	0.309f, 0.0f, -0.951f,      0.5f, 0.5f, 0.5f  // Vértice 11 (esquina inferior, gris)
};*

/*unsigned int indices[] =
{
	// Base de la pirámide (pentágono)
	1, 2, 3,
	1, 3, 4,
	1, 4, 5,
	1, 5, 2,

	// Caras laterales de la parte superior
	0, 1, 2, // Cara 1
	0, 2, 3, // Cara 2
	0, 3, 4, // Cara 3
	0, 4, 5, // Cara 4
	0, 5, 1, // Cara 5

	// Base de la pirámide invertida (pentágono)
	7, 8, 9,
	7, 9, 10,
	7, 10, 11,
	7, 11, 8,

	// Caras laterales de la parte inferior (invertida)
	6, 7, 11, // Cara 6
	6, 11, 10, // Cara 7
	6, 10, 9, // Cara 8
	6, 9, 8,  // Cara 9
	6, 8, 7   // Cara 10
};*/
float vertex2[] =
{
	// Posición          // Color
	-0.5f, 0.0f, -0.5f, 1.0f, 0.0f, 0.0f, // Vértice 0 (esquina inferior izquierda, rojo)
	 0.5f, 0.0f, -0.5f, 0.0f, 1.0f, 0.0f, // Vértice 1 (esquina inferior derecha, verde)
	 0.5f, 0.0f,  0.5f, 0.0f, 0.0f, 1.0f, // Vértice 2 (esquina superior derecha, azul)
	-0.5f, 0.0f,  0.5f, 1.0f, 1.0f, 0.0f, // Vértice 3 (esquina superior izquierda, amarillo)
	 0.0f, 2.0f,  0.0f, 1.0f, 0.0f, 1.0f  // Vértice 4 (cima, magenta)
};

unsigned int indices2[] =
{
	// Base de la pirámide (cuadrado)
   0, 1, 2,
   0, 2, 3,

   // Caras laterales (triángulos)
   0, 1, 4, // Cara 1
   1, 2, 4, // Cara 2
   2, 3, 4, // Cara 3
   3, 0, 4  // Cara 4
};

void CheckShader(int shader);
int UnionShader(int v, int f);
void GeneraBuffer();
void GeneraBuffer2();

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
	
	glfwSetCursorPosCallback(window, Mouse_callback);
	glfwSetScrollCallback(window, Scroll_callback);
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);


	glEnable(GL_DEPTH_TEST);
	Shader shader("vertexShader.vs", "fragmentShader.fs");

	GeneraBuffer();
	GeneraBuffer2();

	UpdateWindow(window, shader);

	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);
	glDeleteBuffers(1, &EBO);
	glDeleteVertexArrays(1, &VAO2);
	glDeleteBuffers(1, &VBO2);
	glDeleteBuffers(1, &EBO2);
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

void CheckShader(int shader)
{
	int sucess;
	char infoLog[512];

	glGetShaderiv(shader, GL_COMPILE_STATUS, &sucess);
	if (!sucess)
	{
		glGetShaderInfoLog(shader, 512, NULL, infoLog);
		cout << "No se pudo crea el shader" << endl;
		cout << infoLog << endl;
	}
}

int UnionShader(int v, int f)
{
	unsigned int shaderProgram;
	shaderProgram = glCreateProgram();
	glAttachShader(shaderProgram, v);
	glAttachShader(shaderProgram, f);
	glLinkProgram(shaderProgram);

	int sucess;
	char infoLog[512];

	glGetProgramiv(shaderProgram, GL_LINK_STATUS, &sucess);
	if (!sucess)
	{
		glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
		cout << "No se pudi crear el Program" << endl;
		cout << infoLog << endl;
	}

	glDeleteShader(v);
	glDeleteShader(f);

	return shaderProgram;
}

void GeneraBuffer()
{
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);

	glBindVertexArray(VAO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertex), vertex, GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	//POSISIONES
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	//COLORES
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}
void GeneraBuffer2()
{
	glGenVertexArrays(1, &VAO2);
	glGenBuffers(1, &VBO2);
	glGenBuffers(1, &EBO2);

	glBindVertexArray(VAO2);

	glBindBuffer(GL_ARRAY_BUFFER, VBO2);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertex2), vertex2, GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO2);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices2), indices2, GL_STATIC_DRAW);

	//POSISIONES
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	//COLORES
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}

void UpdateWindow(GLFWwindow* window, Shader shader)
{
	while (!glfwWindowShouldClose(window))
	{
		float currentTime = glfwGetTime();
		deltaTime = currentTime - lastTime;
		lastTime = currentTime;

		processInput(window);

		glClearColor(0.031f, 0.40f, 0.52f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


		shader.use();

		TransformCamera(shader);
		TransformCubo(shader);
/*
		SistemaCoordenado(shader);


		// Cubo grande
		mat4 transform1 = mat4(1.0f);
		transform1 = translate(transform1, vec3(-1.5f, 0.0f, -3.0f)); // Mover cubo grande a la izquierda
		//transform1 = rotate(transform1, (float)glfwGetTime(), vec3(1.0f, 1.0f, 0.0f));
		shader.setMat4("model", transform1);

		// Establecer el color para el cubo grande
		//shader.setVec4("ourColor", vec4(1.0f, 0.0f, 0.0f, 1.0f)); // Rojo
		glBindVertexArray(VAO);
		glDrawElements(GL_TRIANGLES, sizeof(indices) / sizeof(indices[0]), GL_UNSIGNED_INT, 0);

		// Cubo pequeño
		mat4 transform2 = mat4(1.0f);
		transform2 = translate(transform2, vec3(1.5f, 0.0f, -3.0f)); // Mover cubo pequeño a la derecha
		//transform2 = rotate(transform2, (float)glfwGetTime(), vec3(1.0f, 0.0f, 0.0f));
		shader.setMat4("model", transform2);

		// Establecer el color para el cubo pequeño
		//shader.setVec4("ourColor", vec4(0.0f, 1.0f, 0.0f, 1.0f)); // Verde
		glBindVertexArray(VAO2); // Dibujar cubo pequeño
		glDrawElements(GL_TRIANGLES, sizeof(indices2) / sizeof(indices2[0]), GL_UNSIGNED_INT, 0);

		*/

		glfwSwapBuffers(window);
		glfwPollEvents();
	}
}

void processInput(GLFWwindow* window)
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
	{
		glfwSetWindowShouldClose(window, true);
	}/*if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
	{
		posY += moveSpeed;  // Mover hacia arriba
	}
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
	{
		posY -= moveSpeed;  // Mover hacia abajo
	}
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
	{
		posX -= moveSpeed;  // Mover hacia la izquierda
	}
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
	{
		posX += moveSpeed;  // Mover hacia la derecha
	}if (glfwGetKey(window, GLFW_KEY_1) == GLFW_PRESS)
	{
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	}
	if (glfwGetKey(window, GLFW_KEY_2) == GLFW_PRESS)
	{
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	}if (glfwGetKey(window, GLFW_KEY_R) == GLFW_PRESS)
	{
		std::this_thread::sleep_for(std::chrono::milliseconds(200));
		rotationAngle += rotationIncrement;  // Rotar 45 grados
	}if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS)
	{
		std::this_thread::sleep_for(std::chrono::milliseconds(200));
		X += 0.5; Y += 0.5; Z += 0.5; // Escala en 5
	}if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS)
	{
		std::this_thread::sleep_for(std::chrono::milliseconds(200));
		X -= 0.5; Y -= 0.5; Z -= 0.5;   // Descala en 5
	}*/
	Move(window);
	CameraInput(window);
}

void Move(GLFWwindow* window)
{

}

void SistemaCoordenado(Shader shader)
{
	mat4 transform = mat4(1.0f);
	transform = translate(transform, vec3(0.0f, 0.0f, -3.0f));
	transform = rotate(transform, (float)glfwGetTime(), vec3(3.0f, 2.0f, 0.0f));
	shader.setMat4("model", transform);

	mat4 view = mat4(1.0f);
	view = translate(view, vec3(0.0f, 0.0f, -3.0f));
	shader.setMat4("view", view);

	mat4 projection;
	projection = perspective(radians(45.0f), (float)w / (float)h, 0.1f, 100.0f);
	shader.setMat4("projection", projection);
}

void CameraInput(GLFWwindow* window)
{
	// Limita el movimiento de la cámara a izquierda, derecha, arriba y abajo
	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS || glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS)
	{
		if (cameraPos.y < Y_LIMIT)  // Límite superior
			cameraPos.y += moveSpeed * deltaTime;
	}
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS || glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS)
	{
		if (cameraPos.y > -Y_LIMIT)  // Límite inferior
			cameraPos.y -= moveSpeed * deltaTime;
	}
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS || glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS)
	{
		if (cameraPos.x > -X_LIMIT)  // Límite izquierdo
			cameraPos.x -= moveSpeed * deltaTime;
	}
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS || glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS)
	{
		if (cameraPos.x < X_LIMIT)  // Límite derecho
			cameraPos.x += moveSpeed * deltaTime;
	}

	// Actualizamos la posición de la cámara en el shader
	cameraFront = vec3(0.0f, 0.0f, -1.0f); // Fijamos la dirección de la cámara para que siempre mire hacia adelante

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

void TransformCubo(Shader ourShader)
{
	int tam = sizeof(posCube) / sizeof(posCube[0]);
	glBindVertexArray(VAO);
	for (int i = 0; i < tam; i++)
	{
		mat4 modelo = mat4(1.0f);
		modelo = translate(modelo, posCube[i]);
		modelo = rotate(modelo, radians(-45.0f), vec3(0.3f, 0.7f, 0.0f));
		ourShader.setMat4("model", modelo);
		glDrawElements(GL_TRIANGLES, sizeof(indices) / sizeof(indices[0]), GL_UNSIGNED_INT, 0);
	}
}

void TransformCamera(Shader ourShader)
{
	/*mat4 projection;
	projection = perspective(radians(camera.Zoom), (float)w / (float)h, 0.1f, 100.0f);
	ourShader.setMat4("projection", projection);

	mat4 view = camera.GetViewMatrix();
	ourShader.setMat4("view", view);*/
	
		// La proyección se mantiene igual (sin cambios)
		mat4 projection;
		projection = perspective(radians(camera.Zoom), (float)w / (float)h, 0.1f, 100.0f);
		ourShader.setMat4("projection", projection);

		// Actualizamos la vista, pero la cámara solo puede moverse en X y Y, sin rotación
		mat4 view = lookAt(cameraPos, cameraPos + cameraFront, cameraUp);
		ourShader.setMat4("view", view);

	
}



void Scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
	camera.ProcessMouseScroll(yoffset);
}