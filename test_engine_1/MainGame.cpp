#include "MainGame.h"

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

	gameLoop();
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

	SDL_GLContext glContext = SDL_GL_CreateContext(_window);

	if (glContext == nullptr) {
		fatalError("SLD_GL Context could not be created");
	}

	GLenum glewError = glewInit();
	if (glewError != GLEW_OK) {
		fatalError("Glew initialization error");
	}
}

void MainGame::gameLoop() {
	while (_gameState != GameState::EXIT) {
		processInput();
	}
}

void MainGame::processInput() {
	SDL_Event evnt;

	while (SDL_PollEvent(&evnt)) {
		switch (evnt.type) {
			case SDL_QUIT:
				_gameState = GameState::EXIT;
				break;
			case SDL_MOUSEMOTION:
				std::cout << evnt.motion.xrel << " " << evnt.motion.yrel << std::endl;
				break;
		}
	}
}

void MainGame::drawGame() {
	glClearDepth(1.0);
}

