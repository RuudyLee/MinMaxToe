#pragma once
#include <windows.h>
#include <memory>
#include "ShaderProgram.h"
#include "Mesh.h"
#include "Texture.h"
#include "Text.h"
#include "Entity.h"
#include "Scene.h"
#include "SoundSystem.h"
#include "Board.h"
#include "MinMaxAI.h"

#include <glm\glm.hpp>

#include "Timer.h"


#define WINDOW_WIDTH			800
#define WINDOW_WIDTH_MID		WINDOW_WIDTH / 2
#define WINDOW_HEIGHT			600
#define WINDOW_HEIGHT_MID		WINDOW_HEIGHT / 2
#define FRAMES_PER_SECOND		60

#define MOVEMENT_SPEED			10.0f
#define MOUSE_SPEED				0.001f

class Application {
public:
	Application();
	~Application();

	void Initialize();
	void Update();
	void Draw();
	void renderText();

	/* input callback functions */
	void keyboardDown(unsigned char key, int mouseX, int mouseY);
	void keyboardUp(unsigned char key, int mouseX, int mouseY);
	void mouseClicked(int button, int state, int x, int y);
	void mouseMoved(int x, int y);

	/* Data Members */
	std::unique_ptr<Timer> updateTimer;
	float TotalGameTime = 0.0f;
	float InterpolationParam = 0.0f;

	std::shared_ptr<ShaderProgram> StaticGeometry;

	std::shared_ptr<Mesh> BoardMesh;
	std::shared_ptr<Mesh> XMesh;
	std::shared_ptr<Mesh> OMesh;
	std::shared_ptr<Texture> BoardTexture;
	std::shared_ptr<Texture> RedTexture;
	std::shared_ptr<Texture> BlueTexture;
	Entity *BoardEntity;
	Entity *XEntity;
	Entity *OEntity;
	glm::mat4 CameraTransform;
	glm::mat4 CameraProjection;

	// Assignment 3
	Board *_Board;
	MinMaxAI _MMAI;
	TTTVal _Player;
	bool _PlayerTurn;

	// states
	bool WireframeOn = false;
	bool CursorOn = false;
	bool TextDisplayOn = false;

	Text TextDisplay;
	bool oneTime = false;
	/* For FPS Control */
	bool KeyWDown = false;
	bool KeyADown = false;
	bool KeySDown = false;
	bool KeyDDown = false;
	float xpos = 0.0f;
	float ypos = 0.0f;
	glm::vec3 position = glm::vec3(0, 2, -5);
	float horizontalAngle = 0.0f;
	float verticalAngle = 0.0f;

private:
};
