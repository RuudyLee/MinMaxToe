/* Created by: Rudy Lee */

#include "Application.h"
#include "Utilities.h"

#include <GL\glew.h>
#include <GL\freeglut.h>
#include <glm\gtc\matrix_transform.hpp>

Application::Application() {
}

Application::~Application() {
	// smart pointers will be handled by the compiler
	StaticGeometry->Unload();
	delete BoardEntity;
	delete XEntity;
	delete OEntity;
	delete _Board;
}

/*
*  Initializes OpenGL rendering tasks, and misc. services
*/
void Application::Initialize() {
	updateTimer = std::unique_ptr<Timer>(new Timer());

	TextDisplay.Init();
	glEnable(GL_DEPTH_TEST);

	// Shader Programs
	(StaticGeometry = std::shared_ptr<ShaderProgram>(new ShaderProgram()))->Load("./Assets/Shaders/StaticGeometry.vert", "./Assets/Shaders/Phong.frag");

	// Meshes
	(BoardMesh = std::shared_ptr<Mesh>(new Mesh()))->LoadFromFile("./Assets/Models/Board.obj");
	(XMesh = std::shared_ptr<Mesh>(new Mesh()))->LoadFromFile("./Assets/Models/X.obj");
	(OMesh = std::shared_ptr<Mesh>(new Mesh()))->LoadFromFile("./Assets/Models/O.obj");

	// Textures
	(BoardTexture = std::shared_ptr<Texture>(new Texture()))->Load("./Assets/Textures/Ground.png");
	(BlueTexture = std::shared_ptr<Texture>(new Texture()))->Load("./Assets/Textures/Blue.png");
	(RedTexture = std::shared_ptr<Texture>(new Texture()))->Load("./Assets/Textures/Red.png");

	// Entities
	BoardEntity = new Entity();
	BoardEntity->Init(BoardMesh, BoardTexture);
	XEntity = new Entity();
	XEntity->Init(XMesh, RedTexture);
	OEntity = new Entity();
	OEntity->Init(OMesh, BlueTexture);

	// Initialize a game of tic-tac-toe
	InitializeTicTacToe();

	// Prepare Camera Transforms
	CameraProjection = glm::perspective(60.0f, (float)WINDOW_WIDTH / (float)WINDOW_HEIGHT, 1.0f, 10000.0f);
	CameraTransform = glm::lookAt(glm::vec3(0.0f, 10.0f, 1.0f),
		glm::vec3(0.0f, 0.0f, 0.0f),
		glm::vec3(0.0f, 1.0f, 0.0f));
}

/*
*  Initializes all the variables for tic-tac-toe
*/
void Application::InitializeTicTacToe() {
	_Board = new Board();
	_Board->Init();

	// Choose side
	char choice;
	do {
		std::cout << "Do you want to be X or O? (X/O): ";
		std::cin >> choice;
	} while (choice != 'X' && choice != 'x' && choice != 'O' && choice != 'o');

	switch (choice) {
	case 'X':
	case 'x':
		_Player = TTTVal::X;
		_MMAI.init(TTTVal::O);
		_PlayerTurn = true;
		break;
	case 'O':
	case 'o':
		_Player = TTTVal::O;
		_MMAI.init(TTTVal::X);
		_PlayerTurn = false;
		break;
	}
}

/*
*  Game update loop
*/
void Application::Update() {
	// update our clock so we have the delta time since the last update
	updateTimer->tick();
	float deltaTime = updateTimer->getElapsedTimeSeconds();
	TotalGameTime += deltaTime;

	// AI's turn
	if (!_PlayerTurn) {
		// First check to see if game is over
		CheckGameOver();

		// AI's move. Requires one more check, for game loop
		// If it game overs one line up, it will call this without
		// this check on new game
		if (!_PlayerTurn) {
			_MMAI.performMove(_Board);
		}

		// It is now the player's turn
		_PlayerTurn = true;

		// Check to see if game is over once more
		CheckGameOver();
	}
}

/*
*  Game display loop
*/
void Application::Draw() {
	/* Black background */
	glClearColor(0.1f, 0.2f, 0.3f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	/* Render the scene */
	glViewport(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT);

	// States
	if (WireframeOn) {
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	}
	else {
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	}

	// Set the shader program values
	StaticGeometry->Bind();
	StaticGeometry->SendUniformMat4("uView", &CameraTransform[0][0], false);
	StaticGeometry->SendUniformMat4("uProj", &CameraProjection[0][0], false);
	StaticGeometry->SendUniform("uTex", 0);
	StaticGeometry->SendUniformMat4("CameraPosition", &CameraTransform[0][0], false);
	StaticGeometry->SendUniform("LightPosition", glm::vec4(0.0f, 3.0f, 0.0f, 1.0f));
	StaticGeometry->SendUniform("LightAmbient", glm::vec3(0.15f, 0.15f, 0.15f));
	StaticGeometry->SendUniform("LightDiffuse", glm::vec3(0.7f, 0.7f, 0.7f));
	StaticGeometry->SendUniform("LightSpecular", glm::vec3(0.8f, 0.8f, 0.8f));
	StaticGeometry->SendUniform("LightAttenuationConstant", 1.0f);
	StaticGeometry->SendUniform("LightAttenuationLinear", 0.1f);
	StaticGeometry->SendUniform("LightAttenuationQuadratic", 0.01f);
	StaticGeometry->SendUniform("LightSpecularExponent", 50.0f);

	// Render the board state
	StaticGeometry->SendUniformMat4("uModel", &BoardEntity->GetTransform()[0][0], false);
	BoardEntity->Draw();

	for (int y = 0; y < 3; y++) {
		for (int x = 0; x < 3; x++) {
			TTTVal value = _Board->GetValue(x, y);

			switch (value) {
			case TTTVal::X:
				XEntity->Translate(3.0f * (x - 1), 0.0f, 3.0f * (y - 1));
				StaticGeometry->SendUniformMat4("uModel", &XEntity->GetTransform()[0][0], false);
				XEntity->Draw();
				XEntity->Translate(-3.0f * (x - 1), 0.0f, -3.0f * (y - 1));
				break;

			case TTTVal::O:
				OEntity->Translate(3.0f * (x - 1), 0.0f, 3.0f * (y - 1));
				StaticGeometry->SendUniformMat4("uModel", &OEntity->GetTransform()[0][0], false);
				OEntity->Draw();
				OEntity->Translate(-3.0f * (x - 1), 0.0f, -3.0f * (y - 1));
				break;
			}
		}
	}

	StaticGeometry->Unbind();

	// Render text
	RenderText();

	glutSwapBuffers();
}

/*
*  Trys to do a move at the given position. Returns true if
*  the move was successful. False otherwise.
*/
bool Application::TryMove(int x, int y) {
	if (_Board->GetValue(x, y) == TTTVal::NIL) {
		_Board->SetValue(x, y, _Player);
		return true;
	}
	else {
		return false;
	}
}

/*
*  Checks board state for end condition. Sets _GameOver flag
*  to true if end condition is reached.
*/
void Application::CheckGameOver() {
	if (_Board->GameOver()) {
		_PlayerTurn = true;
		_GameOver = true;
	}
}

/*
*  Calls to the text display program
*/
void Application::RenderText()
{
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	/* Render Text for mode and values */
	if (_GameOver) {
		TTTVal winner = _Board->CheckWinner();
		std::string display;

		if (winner == TTTVal::TIE) {
			display = "Tie Game!";
		}
		else {
			display = (winner == _Player) ? "You Win!" : "You Lose. :(";
		}

		glViewport(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT);
		float sx = 2.0f / glutGet(GLUT_WINDOW_WIDTH);
		float sy = 2.0f / glutGet(GLUT_WINDOW_HEIGHT);

		TextDisplay.program.Bind();

		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

		glm::vec4 Color = { 1, 0, 0, 1 };
		FT_Set_Pixel_Sizes(TextDisplay.face, 0, 24);
		TextDisplay.program.SendUniform("uColor", Color);

		TextDisplay.Render(display.c_str(), -1 + 8 * sx, 1 - 50 * sy, sx, sy);
		TextDisplay.Render("Press 'R' to restart", -1 + 8 * sx, 1 - 75 * sy, sx, sy);


		glDisable(GL_BLEND);
		TextDisplay.program.Unbind();
	}
}

/*
*  Key down callback
*/
void Application::keyboardDown(unsigned char key, int mouseX, int mouseY) {
	switch (key)
	{
	case 27: // the escape key
	case 'q': // the 'q' key
		glutLeaveMainLoop();
		break;
	case '1':
		WireframeOn = !WireframeOn;
		break;
	case '2':
		_MMAI.toggleAlphaBeta();
		break;
	case '3':
		break;
	case '4':
		break;
	case '6':
		break;
	case 'w':
		break;
	case 'a':
		break;
	case 's':
		break;
	case 'd':
		break;
	case 'r': // Reset the game
		_GameOver = false;
		delete _Board;
		InitializeTicTacToe();
		break;
	}
}

/*
*  Key up callback
*/
void Application::keyboardUp(unsigned char key, int mouseX, int mouseY) {
	switch (key)
	{
	case 32: // the space bar
		break;
	case 27: // the escape key
	case 'q': // the 'q' key
		exit(1);
		break;
	case '2':
		break;
	case 'w':
		break;
	case 'a':
		break;
	case 's':
		break;
	case 'd':
		break;
	}
}

/*
*  Mouse click callback
*/
void Application::mouseClicked(int button, int state, int x, int y) {
	if (state == GLUT_DOWN) {
		switch (button) {
		case GLUT_LEFT_BUTTON:
			// Checks for where the player clicked
			if (_PlayerTurn) {
				// left column
				if (xpos > 150 && xpos < 320) {
					// top row
					if (ypos > 72 && ypos < 215) {
						_PlayerTurn = !TryMove(0, 0);
					}
					// middle row
					else if (ypos > 215 && ypos < 375) {
						_PlayerTurn = !TryMove(0, 1);
					}
					// bottom row
					else if (ypos > 375 && ypos < 550) {
						_PlayerTurn = !TryMove(0, 2);
					}
				}
				// middle column
				else if (xpos > 320 && xpos < 480) {
					// top row
					if (ypos > 72 && ypos < 215) {
						_PlayerTurn = !TryMove(1, 0);
					}
					// middle row
					else if (ypos > 215 && ypos < 375) {
						_PlayerTurn = !TryMove(1, 1);
					}
					// bottom row
					else if (ypos > 375 && ypos < 550) {
						_PlayerTurn = !TryMove(1, 2);
					}
				}
				// right column
				else if (xpos > 480 && xpos < 630) {
					// top row
					if (ypos > 72 && ypos < 215) {
						_PlayerTurn = !TryMove(2, 0);
					}
					// middle row
					else if (ypos > 215 && ypos < 375) {
						_PlayerTurn = !TryMove(2, 1);
					}
					// bottom row
					else if (ypos > 375 && ypos < 550) {
						_PlayerTurn = !TryMove(2, 2);
					}
				}
			}
			break;
		case GLUT_RIGHT_BUTTON:

			break;
		case GLUT_MIDDLE_BUTTON:

			break;
		}
	}
	else {

	}
}

/*
*  Mouse movement callback
*/
void Application::mouseMoved(int x, int y) {
	xpos = (float)x;
	ypos = (float)y;
}