#pragma once

#include <sdl/SDL.h>
#include <GL/glew.h>

enum class GameState{PLAY, EXIT};

class MainGame
{
public:
	MainGame();
	~MainGame();

	void run();

private:
	SDL_Window* _window;

	int _screenWidth;
	int _screenHeight;

	GameState _gameState;

	GLuint gVertexArrayObject = 0;
	GLuint gVertexBufferObject = 0;

	GLuint gGraphicsPipelineShaderProgram = 0;

	void displaySystemInfo();
	void initSystems();

	void vertexSpecs();
	void createGraphicsPipeline();

	void gameLoop();

	void processInput();

	void preDraw();
	void drawGame();

	void cleanUp();
};

