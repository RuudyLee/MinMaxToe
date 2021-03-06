/* Created by: Rudy Lee */

#include <windows.h>
#include <iostream>
#include <memory>
#include "Application.h"
#include <GL\glew.h>
#include <GL\freeglut.h>

const int FRAME_DELAY_SPRITE = 1000 / FRAMES_PER_SECOND;

std::unique_ptr<Application> theGame;

/* function DisplayCallbackFunction(void)
 * Description:
 *  - this is the openGL display routine
 *  - this draws the sprites appropriately
 */
void DisplayCallbackFunction(void) {
	theGame->Draw();
}

/* function void KeyboardCallbackFunction(unsigned char, int, int)
 * Description:
 *   - this handles keyboard input when a button is pressed
 */
void KeyboardCallbackFunction(unsigned char key, int x, int y) {
	theGame->keyboardDown(key, x, y);

}
/* function void KeyboardUpCallbackFunction(unsigned char, int, int)
 * Description:
 *   - this handles keyboard input when a button is lifted
 */
void KeyboardUpCallbackFunction(unsigned char key, int x, int y) {
	theGame->keyboardUp(key, x, y);
}

/* function TimerCallbackFunction(int value)
 * Description:
 *  - this is called many times per second
 *  - this enables you to animate things
 *  - no drawing, just changing the state
 *  - changes the frame number and calls for a redisplay
 *  - FRAME_DELAY_SPRITE is the number of milliseconds to wait before calling the timer again
 */
void TimerCallbackFunction(int value) {
	theGame->Update();

	glutPostRedisplay();
	glutTimerFunc(FRAME_DELAY_SPRITE, TimerCallbackFunction, 0);
}

/*
*   - this is called when the mouse is clicked
*/
void MouseClickCallbackFunction(int button, int state, int x, int y) {
	theGame->mouseClicked(button, state, x, y);
	glutPostRedisplay();
}

/* function MouseMotionCallbackFunction()
 * Description:
 *   - this is called when the mouse is clicked and moves
 */
void MouseMotionCallbackFunction(int x, int y) {
	theGame->mouseMoved(x, y);
	glutPostRedisplay();
}

int main(int argc, char **argv) {
	/* initialize the window and OpenGL properly */
	glutInit(&argc, argv);
	glutInitContextVersion(3, 3);
	glutInitWindowSize(WINDOW_WIDTH, WINDOW_HEIGHT);
	glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE);
	glutCreateWindow("OpenGL Framework");

	glewExperimental = true;
	if (glewInit() != GLEW_OK) {
		std::cout << "GLEW failed to initialize\n";
	}

	std::cout << "OpenGL version: " << glGetString(GL_VERSION) << std::endl;

	/* set up our function callbacks */
	glutSetKeyRepeat(GLUT_KEY_REPEAT_OFF);
	glutDisplayFunc(DisplayCallbackFunction);
	glutKeyboardFunc(KeyboardCallbackFunction);
	glutKeyboardUpFunc(KeyboardUpCallbackFunction);
	glutMouseFunc(MouseClickCallbackFunction);
	glutPassiveMotionFunc(MouseMotionCallbackFunction);
	glutTimerFunc(1, TimerCallbackFunction, 0);

	/* init the game */
	theGame = std::unique_ptr<Application>(new Application());
	theGame->Initialize();

	/* start the game */
	glutMainLoop();
	return 0;
}