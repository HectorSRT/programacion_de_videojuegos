#include <iostream>

#include "glad/glad.h"
#include "GLFW/glfw3.h"

#include "Shader.h"
#include "stb_image.h"

using namespace std;

const unsigned width = 1000;
const unsigned height = 600;

float vertices[] = {
	//Posicion           Texture1     
	 1.0f,  1.0f, 0.0f,  1.0f, 1.0f,
	 1.0f, -1.0f, 0.0f,  1.0f, 0.0f,
	-1.0f, -1.0f, 0.0f,  0.0f, 0.0f,
	-1.0f,  1.0f, 0.0f,  0.0f, 1.0f,
};

unsigned int indices[] = {
	0,1,3,
	1,2,3
};

float vertices2[] = {
	// RECTÁNGULO A LO ANCHO
	// Posición           Color
	0.7f,  0.2f, 0.0f,        0.9f, 0.9f, 0.9f,  // Vértice 0 (Arriba derecha)
	0.6f,  -0.2f, 0.0f,       0.5f, 0.5f, 0.5f,  // Vértice 1 (Derecha abajo)
	-0.7f, -0.2f, 0.0f,       0.9f, 0.9f, 0.9f,  // Vértice 2 (Izquierda abajo)
	-0.6f,  0.2f, 0.0f,       0.5f, 0.5f, 0.5f,  // Vértice 3 (Arriba izquierda)

	-0.2f,  0.3f, 0.0f,       0.5f, 0.5f, 0.5f,  // Vértice 4 (tIn1 izquierdo)
	0.0f,   0.3f, 0.0f,        0.9f, 0.9f, 0.9f,  // Vértice 5 (tIn1 arriba)
	-0.2f,  -0.3f, 0.0f,      0.9f, 0.9f, 0.9f,  // Vértice 6 (centro)
	0.0f, -0.3f, 0.0f,         0.9f, 0.9f, 0.9f,  // Vértice 7 (derecha abajo)

	0.2f,  0.3f, 0.0f,        0.5f, 0.5f, 0.5f,  // Vértice 8 (derecha arriba)
	0.0f,  0.3f, 0.0f,         0.9f, 0.9f, 0.9f,  // Vértice 9 (izquierda arriba)
	0.2f, -0.3f, 0.0f,        0.5f, 0.5f, 0.5f,  // Vértice 10 (derecha abajo)
	0.0f, -0.3f, 0.0f,         0.5f, 0.5f, 0.5f,  // Vértice 11 (izquierda abajo)

	// RECTÁNGULO AMARILLO MÁS PEQUEÑO 
	0.6f,  0.14f, 0.0f,       1.0f, 0.8f, 0.0f,  // Vértice 12 (Arriba derecha)
	0.525f, -0.14f, 0.0f,      0.5f, 0.4f, 0.0f,  // Vértice 13 (Derecha abajo)
	-0.6f, -0.14f, 0.0f,      1.0f, 0.8f, 0.0f,  // Vértice 14 (Izquierda abajo)
	-0.525f,  0.14f, 0.0f,     0.5f, 0.4f, 0.0f,  // Vértice 15 (Arriba izquierda)

	-0.15f,  0.2f, 0.0f,        0.5f, 0.4f, 0.0f,  // Vértice 16 (tIn1 izquierdo)
	0.0f,   0.2f, 0.0f,        0.5f, 0.4f, 0.0f,  // Vértice 17 (tIn1 arriba)
	-0.15f,  -0.2f, 0.0f,       1.0f, 0.8f, 0.0f,  // Vértice 18 (centro)
	0.0f,  -0.2f, 0.0f,        1.0f, 0.8f, 0.0f,  // Vértice 19 (derecha abajo)

	0.15f,  0.2f, 0.0f,         1.0f, 0.8f, 0.0f,  // Vértice 20 (derecha arriba)
	0.0f,  0.2f, 0.0f,         1.0f, 0.8f, 0.0f,  // Vértice 21 (izquierda arriba)
	0.15f, -0.2f, 0.0f,         0.5f, 0.4f, 0.0f,  // Vértice 22 (derecha abajo)
	0.0f, -0.2f, 0.0f,         0.5f, 0.4f, 0.0f   // Vértice 23 (izquierda abajo)
};




unsigned int indices2[] = {
	0, 1, 2,
	0, 2, 3,
	4, 5, 6,
	5, 7, 6,
	8, 9, 10,
	9, 11, 10,
	12, 13, 14,
	12, 14, 15,
	16, 17, 18,
	17, 19, 18,
	20, 21, 22,
	21, 23, 22
};



string nombre[] = {
	"img/logo.png"
};

int tipo[] = {
	1
};

int expa[] = {
	0
};

unsigned int VBO, VAO, EBO;
unsigned int VBO2, VAO2, EBO2;
unsigned int texture[1];
int limite;

void initGLFWVersion();
bool gladLoad();
void updateWindow(GLFWwindow* window, Shader ourShader);

void framebuffer_size_callback(GLFWwindow* window, int w, int h);
void processInput(GLFWwindow* window);

void GeneracionBuffer();
void GeneracionBuffer2();
void VertexAttribute(int layout, int data, int total, int start);
void GeneraTextura(int n, string path, int td, int e);

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
	GeneracionBuffer2();

	limite = (sizeof(texture) / sizeof(texture[0]));

	for (int i = 0; i < limite; i++)
	{
		GeneraTextura(i, nombre[i], tipo[i], expa[i]);
	}

	if (limite > 1)
	{
		ourShader.use();
		ourShader.setInt("Texture1", 0);
		ourShader.setInt("Texture2", 1);
	}

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

		/*for (int i = 0; i < limite; i++)
		{
			glActiveTexture(GL_TEXTURE0 + i);
			glBindTexture(GL_TEXTURE_2D, texture[i]);
		}*/

		glBindVertexArray(VAO);
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
		// Dibuja el rectángulo con textura
		glUniform1i(glGetUniformLocation(ourShader.ID, "useTexture"), true);
		glBindVertexArray(VAO);
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

		glBindVertexArray(VAO);
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

		// Dibujar el rectángulo amarillo sin textura
		glUniform1i(glGetUniformLocation(ourShader.ID, "useTexture"), false);

		glBindVertexArray(VAO2);
		glDrawElements(GL_TRIANGLES, 50, GL_UNSIGNED_INT, 0);

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

	VertexAttribute(0, 3, 5, 0);
	VertexAttribute(2, 2, 5, 3);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}

void GeneracionBuffer2() {
	glGenVertexArrays(1, &VAO2);
	glGenBuffers(1, &VBO2);
	glGenBuffers(1, &EBO2);

	glBindVertexArray(VAO2);

	glBindBuffer(GL_ARRAY_BUFFER, VBO2);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices2), vertices2, GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO2);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices2), indices2, GL_STATIC_DRAW);

	// Atributos de posición
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	// Atributos de color
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}


void VertexAttribute(int layout, int data, int total, int start)
{
	glVertexAttribPointer(layout, data, GL_FLOAT, GL_FALSE, total * sizeof(float), (void*)(start * sizeof(float)));
	glEnableVertexAttribArray(layout);
}

void GeneraTextura(int n, string path, int td, int e)
{
	glGenTextures(1, &texture[n]);
	glBindTexture(GL_TEXTURE_2D, texture[n]);

	if (e == 0)
	{
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	}
	else if (e == 1)
	{
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_MIRRORED_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_MIRRORED_REPEAT);
	}
	else if (e == 2)
	{
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	}
	else if (e == 3)
	{
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);

		float borderColor[] = { 0.0f,0.0f,0.0f,0.0f };
		glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, borderColor);
	}
	else
	{
		cout << "Valores de 0 a 3 en r" << endl;
	}

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	int width, height, nrChannels;
	stbi_set_flip_vertically_on_load(true);
	unsigned char* data = stbi_load(path.c_str(), &width, &height, &nrChannels, 0);

	if (data)
	{
		if (td == 0)
		{
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
		}
		else
		{
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
		}
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
	{
		cout << "No se puede cargar la textura" << endl;
	}
	stbi_image_free(data);
}