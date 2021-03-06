#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/vec2.hpp>

#include <iostream>

#include "Resources/ResourceManager.h"
#include "Renderer/ShaderProgram.h"
#include "Renderer/Texture2D.h"


glm::ivec2 m_WindowSize(640, 480*2);

//int g_windowSizeX = 640;
//int g_windowSizeY = 480;


GLfloat point[] = {
	 0.0f,  0.5f, 0.0f,
	 0.5f, -0.5f, 0.0f,
	-0.5f, -0.5f, 0.0f
};
GLfloat colors[] = {
	1.0f, 0.0f, 0.0f,
	0.0f, 1.0f, 0.0f,
	0.0f, 0.0f, 1.0f
};
GLfloat texCoord[] = {
	0.5f, 1.0f,
	1.0f, 0.0f,
	0.0f, 0.0f
};


void glfwWindowSizeCallback(GLFWwindow* window, int width, int height) {
	m_WindowSize.x = width;
	m_WindowSize.y = height;
	glViewport(0, 0, m_WindowSize.x, m_WindowSize.y);
}

void glfwKeyCallback(GLFWwindow* window, int key, int scancode, int action, int mode) {
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) {
		glfwSetWindowShouldClose(window, GL_TRUE);
	}

}

int main(int argc, char** argv){
	/* Initialize the library */
	if (!glfwInit()) {
		std::cout << "glfwInit failed!" << std::endl;
		return -1;
	}

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);


	/* Create a windowed mode window and its OpenGL context */
	GLFWwindow* window = glfwCreateWindow(m_WindowSize.x, m_WindowSize.y, "BattleCity", nullptr, nullptr);
	if (!window){
		std::cout << "glfwCreateWindow failed" << std::endl;
		glfwTerminate();
		return -1;
	}

	glfwSetWindowSizeCallback(window, glfwWindowSizeCallback);
	glfwSetKeyCallback(window, glfwKeyCallback);
	/* Make the window's context current */
	glfwMakeContextCurrent(window);


	if (!gladLoadGL()) {
		std::cout << "Can't load GLAD" << std::endl;
		return -1;
	}

	std::cout << "Renderer: " << glGetString(GL_RENDERER) << std::endl;
	std::cout << "Opengl version: " << glGetString(GL_VERSION) << std::endl;
	

	glClearColor(1, 1, 0, 1);

	{
		ResourceManager resourceManager(argv[0]);
		auto DefaulShaderProgram = resourceManager.loadShaders("DefaultShader", "res/Shaders/vertex.txt", "res/Shaders/fragment.txt");
		if (!DefaulShaderProgram) {
			std::cerr << "Can't create shader program: " << "DefaultShader" << std::endl;
		}
		
		auto tex = resourceManager.loatTexture("DefaultTexture", "res/Textures/atlas256x256.png");
		
		GLuint points_vbo = 0;
		glGenBuffers(1, &points_vbo);
		glBindBuffer(GL_ARRAY_BUFFER, points_vbo);
		glBufferData(GL_ARRAY_BUFFER, sizeof(point), point, GL_STATIC_DRAW);


		GLuint colors_vbo = 0;
		glGenBuffers(1, &colors_vbo);
		glBindBuffer(GL_ARRAY_BUFFER, colors_vbo);
		glBufferData(GL_ARRAY_BUFFER, sizeof(colors), colors, GL_STATIC_DRAW);

		GLuint texCoord_vbo = 0;
		glGenBuffers(1, &texCoord_vbo);
		glBindBuffer(GL_ARRAY_BUFFER, texCoord_vbo);
		glBufferData(GL_ARRAY_BUFFER, sizeof(texCoord), texCoord, GL_STATIC_DRAW);


		GLuint vao = 0;
		glGenVertexArrays(1, &vao);
		glBindVertexArray(vao);

		glEnableVertexAttribArray(0);
		glBindBuffer(GL_ARRAY_BUFFER, points_vbo);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, nullptr);

		glEnableVertexAttribArray(1);
		glBindBuffer(GL_ARRAY_BUFFER, colors_vbo);
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, nullptr);

		glEnableVertexAttribArray(2);
		glBindBuffer(GL_ARRAY_BUFFER, texCoord_vbo);
		glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 0, nullptr);



		DefaulShaderProgram->use();
		DefaulShaderProgram->setInt("tex", 0);

		/* Loop until the user closes the window */
		while (!glfwWindowShouldClose(window)) {
			/* Render here */
			glClear(GL_COLOR_BUFFER_BIT);


			DefaulShaderProgram->use();
			glBindVertexArray(vao);
			tex->bind();
			glDrawArrays(GL_TRIANGLES, 0, 3);

			/* Swap front and back buffers */
			glfwSwapBuffers(window);

			/* Poll for and process events */
			glfwPollEvents();
		}
	}

	glfwTerminate();
	return 0;
}