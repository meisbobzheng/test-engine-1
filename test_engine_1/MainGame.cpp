#include "MainGame.h"

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

GLuint MainGame::compileShader(GLuint type, const std::string& source) {
	GLuint shaderObject;
	
	if (type == GL_VERTEX_SHADER) {
		shaderObject = glCreateShader(GL_VERTEX_SHADER);
	}
	else if (type == GL_FRAGMENT_SHADER) {
		shaderObject = glCreateShader(GL_FRAGMENT_SHADER);
	}

	const char* src = source.c_str();
	glShaderSource(shaderObject, 1, &src, nullptr);
	glCompileShader(shaderObject);

	return shaderObject;
}

GLuint MainGame::createShaderProgram(
	const std::string& vertexshadersource, 
	const std::string& fragmentshadersource) {
	GLuint programObject = glCreateProgram();
	
	GLuint myVertexShader = compileShader(GL_VERTEX_SHADER, vertexshadersource);
	GLuint myFramentShader = compileShader(GL_FRAGMENT_SHADER, fragmentshadersource);

	glAttachShader(programObject, myVertexShader);
	glAttachShader(programObject, myFramentShader);
	glLinkProgram(programObject);

	// validate program
	glValidateProgram(programObject);


	//detach, delete shader

	return programObject;

}

void MainGame::createGraphicsPipeline() {
	gGraphicsPipelineShaderProgram = createShaderProgram(gVertexShaderSource, gFragmentShaderSource);
}

void MainGame::gameLoop() {
	while (_gameState != GameState::EXIT) {
		// handle inputs
		processInput();

		// draw game
		preDraw();
		drawGame();

		// update screen
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

void MainGame::preDraw() {
	glDisable(GL_DEPTH_TEST);
	glDisable(GL_CULL_FACE);

	glViewport(0, 0, _screenWidth, _screenHeight);

	//clear screen and set color
	glClearColor(0.0f, 0.0f, 1.0f, 1.0f);
	glClearDepth(1.0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glUseProgram(gGraphicsPipelineShaderProgram);
}

void MainGame::drawGame() {
	glBindVertexArray(gVertexArrayObject);
	glBindBuffer(GL_ARRAY_BUFFER, gVertexBufferObject);

	glDrawArrays(GL_TRIANGLES, 0, 3);
}

void MainGame::cleanUp() {
	SDL_DestroyWindow(_window);
	SDL_Quit();
}

