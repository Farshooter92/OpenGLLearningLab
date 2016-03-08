#include "Game.h"
#include <iostream>
#include <stdio.h>
#include <stdlib.h>

Game::Game() :
	_window(nullptr),
	_screenWidth(640),
	_screenHeight(480),
	_gameState(GameState::PLAY),
	_previousTime(0.0)
{
}

Game::~Game()
{
}

void Game::run()
{
	initSystems();
	initShaders();
	gameLoop();

	// Clean up
	glDeleteVertexArrays(1, &_vao);
	glDeleteProgram(_program);
	glfwDestroyWindow(_window);
	glfwTerminate();
}

void Game::initSystems()
{
	// Set the error callback
	glfwSetErrorCallback(errorCallback);

	// Initialize the library
	if (!glfwInit())
	{
		exit(EXIT_FAILURE);
	}

	// Create a windowed mode window and its OpenGL context
	_window = glfwCreateWindow(640, 480, "OpenGL Learning Lab", NULL, NULL);

	if (!_window)
	{
		glfwTerminate();
		exit(EXIT_FAILURE);
	}

	// Make the window's context current
	glfwMakeContextCurrent(_window);

	// Set the data in the window, we will use this for input
	glfwSetWindowUserPointer(_window, this);

	// The minimum number of screen updates to wait for until the buffers are swapped
	glfwSwapInterval(1);

	// Set up the key callbacks
	glfwSetKeyCallback(_window, keyCallback);

	// start GLEW extension handler
	glewExperimental = true;

	// Setup GLEW
	GLenum err = glewInit();
	
	if (err != GLEW_OK)
	{
		fprintf(stderr, "Error: %s\n", glewGetErrorString(err));
		glfwTerminate();
		exit(EXIT_FAILURE);
	}

	// get version info
	const GLubyte* renderer = glGetString(GL_RENDERER); // get renderer string
	const GLubyte* version = glGetString(GL_VERSION); // version as a string
	printf("Renderer: %s\n", renderer);
	printf("OpenGL version supported %s\n", version);

	//Set the background color to blue
	glClearColor(0.0f, 0.0f, 1.0f, 1.0f);

	// Create our OpenGL program
	_program = glCreateProgram();
}

void Game::initShaders()
{
	// INTIALIZE OPENGL
	static const char * vs_source[] =
	{
		"#version 450 core                                                 \n"
		"                                                                  \n"
		"// 'offset' is an input vertex attribute                          \n"
		"layout (location = 0) in vec4 offset;                             \n"
		"                                                                  \n"
		"void main(void)                                                   \n"
		"{                                                                 \n"
		"    const vec4 vertices[] = vec4[](vec4( 0.25, -0.25, 0.5, 1.0),  \n"
		"                                   vec4(-0.25, -0.25, 0.5, 1.0),  \n"
		"                                   vec4( 0.25,  0.25, 0.5, 1.0)); \n"
		"                                                                  \n"
		"    // Add 'offset' to our hard-coded vertex position             \n"
		"    gl_Position = vertices[gl_VertexID] + offset;                 \n"
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

	GLuint fs = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fs, 1, fs_source, NULL);
	glCompileShader(fs);

	GLuint vs = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vs, 1, vs_source, NULL);
	glCompileShader(vs);

	glAttachShader(_program, vs);
	glAttachShader(_program, fs);

	glLinkProgram(_program);

	glGenVertexArrays(1, &_vao);
	glBindVertexArray(_vao);
}

void Game::gameLoop()
{
	//Will loop until we set _gameState to EXIT
	while (_gameState != GameState::EXIT && !glfwWindowShouldClose(_window))
	{
		updateGame((float)(glfwGetTime() - _previousTime));
		drawGame();

		// Poll for and process events
		glfwPollEvents();

		// Set up previous time
		_previousTime = glfwGetTime();
	}
}

void Game::updateGame(float deltaTime)
{
}

void Game::drawGame()
{
	float ratio;
	int width, height;

	glfwGetFramebufferSize(_window, &width, &height);
	ratio = width / (float)height;

	// Sets the viewport to the screen dimensions
	glViewport(0, 0, width, height);

	//Set the base depth to 1.0
	glClearDepth(1.0);

	// Clear the color and depth buffers
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// Render OpenGL here
	static const GLfloat green[] = { 0.0f, 0.25f, 0.0f, 1.0f };
	glClearBufferfv(GL_COLOR, 0, green);

	// Use the program object we created earlier for rendering
	glUseProgram(_program);

	GLfloat attrib[] = { (float)sin(glfwGetTime()) * 0.5f,
		(float)cos(glfwGetTime()) * 0.6f,
		0.0f, 0.0f };

	// Pass our offset to vertex data to the vertex shader
	glVertexAttrib4fv(0, attrib);

	glDrawArrays(GL_TRIANGLES, 0, 3);

	// Swap front and back buffers
	glfwSwapBuffers(_window);
}

void Game::errorCallback(int error, const char * description)
{
	fputs(description, stderr);
}

void Game::keyCallback(GLFWwindow * window, int key, int scancode, int action, int mods)
{
	
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
	{
		glfwSetWindowShouldClose(window, GL_TRUE);
	}

	Game* game = reinterpret_cast<Game*>(glfwGetWindowUserPointer(window));

	if (game != nullptr)
	{
		if (key == GLFW_KEY_Q && action == GLFW_PRESS)
		{
			std::cout << "Quit" << std::endl;
			game->setGameState(GameState::EXIT);
		}
	}

	
}
