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

string nombre[] = {
	"img/caja.jpg",
	"img/fragil.png",
};

int tipo[] = {
	0,
	1
};

int expa[] = {
	0,
	3
};

unsigned int VBO, VAO, EBO;
unsigned int texture[2];
int limite;

void initGLFWVersion();
bool gladLoad();
void updateWindow(GLFWwindow* window, Shader ourShader);

void framebuffer_size_callback(GLFWwindow* window, int w, int h);
void processInput(GLFWwindow* window);

void GeneracionBuffer();
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

	ourShader.use();

	GeneracionBuffer();

	GeneraTextura(0, nombre[0], tipo[0], expa[0]);
	ourShader.setInt("Texture1", 0);

	glUniform1i(glGetUniformLocation(ourShader.ID, "Texture1"), 0);

	GeneraTextura(1, nombre[1], tipo[1], expa[1]);
	ourShader.setInt("Texture2", 1);




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
	if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS)
	{
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, texture[0]);
	}
	if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS)
	{
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, texture[1]);
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




		glBindVertexArray(VAO);
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

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
	VertexAttribute(1, 2, 5, 3);

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