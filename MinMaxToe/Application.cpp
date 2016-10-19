#include "Application.h"
#include "Utilities.h"

#include <GL\glew.h>
#include <GL\freeglut.h>
#include <glm\gtc\matrix_transform.hpp>

Application::Application() {
}

Application::~Application() {
	StaticGeometry->Unload();
}

void Application::Initialize() {
	updateTimer = std::unique_ptr<Timer>(new Timer());

	TextDisplay.Init();
	glEnable(GL_DEPTH_TEST);

	// Shader Programs
	(StaticGeometry = std::shared_ptr<ShaderProgram>(new ShaderProgram()))->Load("./Assets/Shaders/StaticGeometry.vert", "./Assets/Shaders/Phong.frag");
	
	// Meshes
	(DoorMesh = std::shared_ptr<Mesh>(new Mesh()))->LoadFromFile("./Assets/Models/Door.obj");
	(GroundMesh = std::shared_ptr<Mesh>(new Mesh()))->LoadFromFile("./Assets/Models/Ground.obj");

	// Textures
	(DoorTexture = std::shared_ptr<Texture>(new Texture()))->Load("./Assets/Textures/Door.tga");
	(GroundTexture = std::shared_ptr<Texture>(new Texture()))->Load("./Assets/Textures/Ground.png");

	// Entities
	Door.Init(DoorMesh, DoorTexture);
	Door.Scale(0.01f);
	Ground.Init(GroundMesh, GroundTexture);

	// Scenes
	BasicScene.Init(StaticGeometry);
	BasicScene.AddGameObject(Door);
	BasicScene.AddGameObject(Ground);

	// Sounds
	Sound.init();
	Sound.createSound("./Assets/Sounds/drumloop.wav", 0, glm::vec3(0.0f, 0.0f, 0.0f));
	Sound.createSound("./Assets/Sounds/jaguar.wav", 1, glm::vec3(0.0f, 0.0f, 10.0f));

	CameraProjection = glm::perspective(60.0f, (float)WINDOW_WIDTH / (float)WINDOW_HEIGHT, 1.0f, 10000.0f);
}

void Application::Update() {
	// update our clock so we have the delta time since the last update
	updateTimer->tick();

	float deltaTime = updateTimer->getElapsedTimeSeconds();
	TotalGameTime += deltaTime;

	if (CursorOn) {
		glutSetCursor(GLUT_CURSOR_INHERIT);
	}
	else {
		glutSetCursor(GLUT_CURSOR_NONE);
		glutWarpPointer(WINDOW_WIDTH_MID, WINDOW_HEIGHT_MID);

		/* FPS Control */
		horizontalAngle += MOUSE_SPEED * float(WINDOW_WIDTH_MID - xpos);
		verticalAngle += MOUSE_SPEED * float(WINDOW_HEIGHT_MID - ypos);

		glm::vec3 direction(
			cos(verticalAngle) * sin(horizontalAngle),
			sin(verticalAngle),
			cos(verticalAngle) * cos(horizontalAngle)
			);

		glm::vec3 right = glm::vec3(
			sin(horizontalAngle - 3.14f / 2.0f),
			0,
			cos(horizontalAngle - 3.14f / 2.0f)
			);

		glm::vec3 up = glm::cross(right, direction);

		if (KeyWDown) {
			position += glm::vec3(direction.x, 0.0f, direction.z) * deltaTime * MOVEMENT_SPEED;
		}
		if (KeySDown) {
			position -= glm::vec3(direction.x, 0.0f, direction.z) * deltaTime * MOVEMENT_SPEED;
		}
		if (KeyDDown) {
			position += right * deltaTime * MOVEMENT_SPEED;
		}
		if (KeyADown) {
			position -= right * deltaTime * MOVEMENT_SPEED;
		}

		std::cout << "Pos: " << position.x << ", " << position.y << ", " << position.z << std::endl;
		CameraTransform = glm::lookAt(position, position + direction, up);
		Sound.setListener(position, direction, up);
	}

	BasicScene.Update(deltaTime);
	
	Sound.update();
}

void Application::Draw() {
	/* Black background */
	glClearColor(0.1f, 0.2f, 0.3f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	/* Render the scene */
	glViewport(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT);

	/* States */
	if (WireframeOn) {
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	}
	else {
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	}

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

	BasicScene.Draw();

	StaticGeometry->Unbind();

	renderText();

	glutSwapBuffers();

}

void Application::renderText()
{
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	/* Render Text for mode and values */
	if (TextDisplayOn) {
		glViewport(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT);
		float sx = 2.0f / glutGet(GLUT_WINDOW_WIDTH);
		float sy = 2.0f / glutGet(GLUT_WINDOW_HEIGHT);

		TextDisplay.program.Bind();

		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

		glm::vec4 Color = { 1, 0, 0, 1 };
		FT_Set_Pixel_Sizes(TextDisplay.face, 0, 24);
		TextDisplay.program.SendUniform("uColor", Color);

		if (WireframeOn) {
			TextDisplay.Render("1: Wireframe mode is ON", -1 + 8 * sx, 1 - 50 * sy, sx, sy);
		}
		else {
			TextDisplay.Render("1: Wireframe mode is OFF", -1 + 8 * sx, 1 - 50 * sy, sx, sy);
		}

		glDisable(GL_BLEND);
		TextDisplay.program.Unbind();
	}
}

void Application::keyboardDown(unsigned char key, int mouseX, int mouseY) {
	switch (key)
	{
	case 27: // the escape key
	case 'q': // the 'q' key
		exit(1);
		break;
	case '1':
		WireframeOn = !WireframeOn;
		break;
	case '2':
		CursorOn = !CursorOn;
		break;
	case '3':
		BasicScene.RemoveGameObject(Door);
		break;
	case '4':
		break;
	case '6':
		TextDisplayOn = !TextDisplayOn;
		break;
	case 'w':
		KeyWDown = true;
		break;
	case 'a':
		KeyADown = true;
		break;
	case 's':
		KeySDown = true;
		break;
	case 'd':
		KeyDDown = true;
		break;
	}
}

void Application::keyboardUp(unsigned char key, int mouseX, int mouseY) {
	switch (key)
	{
	case 32: // the space bar
		break;
	case 27: // the escape key
	case 'q': // the 'q' key
		exit(1);
		break;
	case 'w':
		KeyWDown = false;
		break;
	case 'a':
		KeyADown = false;
		break;
	case 's':
		KeySDown = false;
		break;
	case 'd':
		KeyDDown = false;
		break;
	}
}

void Application::mouseClicked(int button, int state, int x, int y) {
	if (state == GLUT_DOWN)	{
		switch (button)	{
		case GLUT_LEFT_BUTTON:

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
 * mouseMoved(x,y)
 * - this occurs only when the mouse is pressed down
 *   and the mouse has moved.  you are given the x,y locations
 *   in window coordinates (from the top left corner) and thus
 *   must be converted to screen coordinates using the screen to window pixels ratio
 *   and the y must be flipped to make the bottom left corner the origin.
 */
void Application::mouseMoved(int x, int y) {
	xpos = (float)x;
	ypos = (float)y;
}