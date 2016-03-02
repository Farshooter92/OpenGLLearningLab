#pragma once
#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "Enums.h"

class Game
{
public:
	Game();
	~Game();

	void run();

protected:
	void initSystems();
	void initShaders();
	void gameLoop();
	void updateGame(float deltaTime);
	void drawGame();

	static void errorCallback(int error, const char* description);
	static void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods);

private:
	GLFWwindow* _window;
	int _screenWidth;
	int _screenHeight;
	GameState _gameState;
	double _previousTime;

	// OpenGL Variables
	GLuint _program;
	GLuint _vao;

public:
	// Getters
	GameState GetGameState() const { return _gameState; }

	// Setters
	void setGameState(GameState gs) { _gameState = gs; }
};

