#pragma once

#include <sdl/SDL.h>
#include <GL/glew.h>
#include <vector>
#include <iostream>

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

	// VAO
	GLuint gVertexArrayObject = 0;
	// VBO
	GLuint gVertexBufferObject = 0;

	// Program Object for shaders
	GLuint gGraphicsPipelineShaderProgram = 0;

	//TEMP SHADERS
	const std::string gVertexShaderSource =
		"#version 410 core\n"
		"in vec4 position;\n"
		"void main()\n"
		"{\n"
		"    gl_Position = vec4(position.x, position.y, position.z, position.w);\n"
		"}\n";

	const std::string gFragmentShaderSource =
		"#version 410 core\n"
		"out vec4 color;\n"
		"void main()\n"
		"{\n"
		"    color = vec4(1.0f, 0.5f, 0.0f, 1.0f);\n"
		"}\n";



	void displaySystemInfo();
	void initSystems();

	void vertexSpecs();
	GLuint compileShader(GLuint type, const std::string& source);
	GLuint createShaderProgram(
		const std::string& vertexshadersource,
		const std::string& fragmentshadersource);

	void createGraphicsPipeline();

	void gameLoop();

	void processInput();

	void preDraw();
	void drawGame();

	void cleanUp();
};

