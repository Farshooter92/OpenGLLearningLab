#include <iostream>
#include <stdio.h>
#include <stdlib.h>

#include <GL/glew.h>
#include <GLFW/glfw3.h>

static void error_callback(int error, const char* description)
{
	fputs(description, stderr);
}

static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
	{
		glfwSetWindowShouldClose(window, GL_TRUE);
	}

	if (key == GLFW_KEY_T && action == GLFW_PRESS)
	{
		std::cout << "Test" << std::endl;
	}

}

int main(void)
{
	GLFWwindow *window;

	// Set the error callback
	glfwSetErrorCallback(error_callback);

	// Initialize the library
	if (!glfwInit())
	{
		return -1;
	}

	// Create a windowed mode window and its OpenGL context
	window = glfwCreateWindow(640, 480, "OpenGL Learning Lab", NULL, NULL);

	if (!window)
	{
		glfwTerminate();
		return -1;
	}

	// Make the window's context current
	glfwMakeContextCurrent(window);
	
	// The minimum number of screen updates to wait for until the buffers are swapped
	glfwSwapInterval(1);

	// Set up the key callbacks
	glfwSetKeyCallback(window, key_callback);

	// start GLEW extension handler
	glewExperimental = true;

	// Setup GLEW
	if (glewInit() != GLEW_OK)
	{
		fprintf(stderr, "GLEW failed to initialize");
		glfwTerminate();
		return -1;
	}

	// get version info
	const GLubyte* renderer = glGetString(GL_RENDERER); // get renderer string
	const GLubyte* version = glGetString(GL_VERSION); // version as a string
	printf("Renderer: %s\n", renderer);
	printf("OpenGL version supported %s\n", version);

	// OpenGL Variables
	GLuint program;
	GLuint vao;

	// INTIALIZE OPENGL
	static const char * vs_source[] =
	{
		"#version 420 core                                                 \n"
		"                                                                  \n"
		"void main(void)                                                   \n"
		"{                                                                 \n"
		"    const vec4 vertices[] = vec4[](vec4( 0.25, -0.25, 0.5, 1.0),  \n"
		"                                   vec4(-0.25, -0.25, 0.5, 1.0),  \n"
		"                                   vec4( 0.25,  0.25, 0.5, 1.0)); \n"
		"                                                                  \n"
		"    gl_Position = vertices[gl_VertexID];                          \n"
		"}                                                                 \n"
	};

	static const char * fs_source[] =
	{
		"#version 420 core                                                 \n"
		"                                                                  \n"
		"out vec4 color;                                                   \n"
		"                                                                  \n"
		"void main(void)                                                   \n"
		"{                                                                 \n"
		"    color = vec4(0.0, 0.8, 1.0, 1.0);                             \n"
		"}                                                                 \n"
	};

	program = glCreateProgram();
	GLuint fs = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fs, 1, fs_source, NULL);
	glCompileShader(fs);

	GLuint vs = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vs, 1, vs_source, NULL);
	glCompileShader(vs);

	glAttachShader(program, vs);
	glAttachShader(program, fs);

	glLinkProgram(program);

	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);

	/* Loop until the user closes the window */
	while (!glfwWindowShouldClose(window))
	{
		float ratio;
		int width, height;
		
		glfwGetFramebufferSize(window, &width, &height);
		ratio = width / (float)height;

		// Sets the viewport to the screen dimensions
		glViewport(0, 0, width, height);
		
		// Clear the color and depth buffers
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		// Render OpenGL here
		static const GLfloat green[] = { 0.0f, 0.25f, 0.0f, 1.0f };
		glClearBufferfv(GL_COLOR, 0, green);

		glUseProgram(program);
		glDrawArrays(GL_TRIANGLES, 0, 3);

		// Swap front and back buffers
		glfwSwapBuffers(window);

		// Poll for and process events
		glfwPollEvents();
	}

	glDeleteVertexArrays(1, &vao);
	glDeleteProgram(program);
	glfwDestroyWindow(window);
	glfwTerminate();

	return 0;
}