#include "MainGame.h"

#include <vector>
#include <iostream>

void fatalError(std::string errorString) {
	std::cout << errorString << std::endl;
	std::cout << "Enter any key to quit...";
	int tmp;
	std::cin >> tmp;
	SDL_Quit();
}

MainGame::MainGame() {
	_window = nullptr;
	_screenWidth = 1024;
	_screenHeight = 728;
	_gameState = GameState::PLAY;
}

MainGame::~MainGame() {

}

void MainGame::run() {

	initSystems();

	vertexSpecs();

	createGraphicsPipeline();

	gameLoop();

	cleanUp();
}

void MainGame::displaySystemInfo() {
	std::cout << "Vendor: " << glGetString(GL_VENDOR) << std::endl;
	std::cout << "Renderer: " << glGetString(GL_RENDERER) << std::endl;
	std::cout << "Version: " << glGetString(GL_VERSION) << std::endl;
	std::cout << "Shading Language: " << glGetString(GL_VERSION) << std::endl;
}

void MainGame::initSystems() {

	//Initialize SDL
	SDL_Init(SDL_INIT_EVERYTHING);

	_window = SDL_CreateWindow(
		"test_engine", 
		SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 
		_screenWidth, _screenHeight, 
		SDL_WINDOW_OPENGL);

	if (_window == nullptr) {
		fatalError("Window failed to load");
	}

	//openGL context
	SDL_GLContext glContext = SDL_GL_CreateContext(_window);

	if (glContext == nullptr) {
		fatalError("SLD_GL Context could not be created");
	}

	//glew initalization
	GLenum glewError = glewInit();
	if (glewError != GLEW_OK) {
		fatalError("Glew initialization error");
	}

	//setting attributes using open GL 4.10
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 1);
	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);

	//clear screen
	glClearColor(0.0f, 0.0f, 1.0f, 1.0f);

	//system info
	displaySystemInfo();
}

void MainGame::vertexSpecs() {

	// Lives on CPU
	const std::vector<GLfloat> vertexPosition{
		// x	 y	   z
		-0.8f, -0.8f, 0.0f,
		0.8f, -0.8f, 0.0f,
		0.0f, 0.8f, 0.0f
	};

	//GPU setup
	glGenVertexArrays(1, &gVertexArrayObject);
	glBindVertexArray(gVertexArrayObject);

	// start generating vbo
	glGenBuffers(1, &gVertexBufferObject);
	glBindBuffer(GL_ARRAY_BUFFER, gVertexBufferObject);
	glBufferData(
		GL_ARRAY_BUFFER,
		vertexPosition.size() * sizeof(GLfloat),
		vertexPosition.data(),
		GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(
		0,
		3,
		GL_FLOAT,
		GL_FALSE,
		0,
		(void*)0);

	//cleanup
	glBindVertexArray(0);
	glDisableVertexAttribArray(0);

}

void MainGame::createGraphicsPipeline() {

}

void MainGame::gameLoop() {
	while (_gameState != GameState::EXIT) {
		processInput();
		drawGame();
		SDL_GL_SwapWindow(_window);
	}
}

void MainGame::processInput() {
	SDL_Event evnt;

	while (SDL_PollEvent(&evnt)) {
		switch (evnt.type) {
			case SDL_QUIT:
				_gameState = GameState::EXIT;
				break;
			case SDL_MOUSEBUTTONDOWN:
				std::cout << evnt.motion.x << " " << evnt.motion.y << std::endl;
				break;
		}
	}
}

void MainGame::drawGame() {
	glClearDepth(1.0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void MainGame::cleanUp() {
	SDL_DestroyWindow(_window);
	SDL_Quit();
}

