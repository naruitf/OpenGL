#include "stdio.h"
#include <stdlib.h>
#include <time.h>
#include "glut.h"
#include "glm\glm.hpp"

#include "font.h"
#include "Rect.h"
#include "Ball.h"
#include "Paddle.h"

using namespace glm;
#define BALL_MAX 128
ivec2 windowSize = { 800, 600 };

bool keys[256];

Paddle paddle;

Ball balls[BALL_MAX];

Rect rect1 = Rect(vec2(100, 100), vec2(100, 200));
Rect rect2 = Rect(vec2(windowSize.x / 2, windowSize.y / 2), vec2(200, 100));


void display(void) {
	glClear(GL_COLOR_BUFFER_BIT);// GLbitfield mask

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluOrtho2D(
		0,//GLdouble left
		windowSize.x,//GLdouble right
		windowSize.y,//GLdouble bottom
		0//GLdouble top
	);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	if (rect1.intersect(rect2))
		glColor3ub(0xff, 0x00, 0x00);
	else glColor3ub(0x00, 0x00, 0xff);
	//rect1.draw();

	glColor3ub(0x00, 0xff, 0x00);
	//rect2.draw();


	fontBegin();
	fontSetColor(0, 0xff, 0);
	fontSetSize(FONT_DEFAULT_SIZE / 1.5);
	float lineheight = fontGetSize() * 1.5;
	float y = windowSize.y - lineheight * 2;
	fontSetPosition(0, y);
	fontSetWeight(fontGetWeightMin());
	//fontDraw("min: %f", fontGetWeightMin());
	fontSetPosition(0, y += lineheight);
	fontSetWeight(fontGetWeightMax() / 3);
	//fontDraw("max: %f", fontGetWeightMax());
	fontEnd();

	glColor3ub(0xff,0xff,0xff);
	paddle.draw();

	unsigned char colors[6][3] = {
		{ 0xff, 0x00, 0x00},
		{ 0xff, 0xff, 0x00},
		{ 0xff, 0x00, 0xff},
		{ 0x00, 0xff, 0x00},
		{ 0x00, 0xff, 0xff},
		{ 0x00, 0x00, 0xff},
	};

	for (int i = 0; i < BALL_MAX; i++) {
		glColor3ub(colors[i % 6][0], colors[i % 6][1], colors[i % 6][2]);
		balls[i].draw();
	}
	
	glutSwapBuffers();
}

void idle(int value) {
	for (int i = 0; i < BALL_MAX; i++) {
		if (paddle.intersectBall(balls[i])) {
			balls[i].m_position = balls[i].m_lastposition;
			balls[i].m_speed.x *= -1;
		}
		balls[i].update();
		if (balls[i].m_position.x < 0) {
			balls[i].m_position = balls[i].m_lastposition;
			balls[i].m_speed.x = fabs(balls[i].m_speed.x);
		}
		if (balls[i].m_position.x >= windowSize.x) {
			balls[i].m_position = balls[i].m_lastposition;
			balls[i].m_speed.x = -fabs(balls[i].m_speed.x);
		}
		if (balls[i].m_position.y >= windowSize.y) {
			balls[i].m_position = balls[i].m_lastposition;
			balls[i].m_speed.y = -fabs(balls[i].m_speed.y);
		}
		if (balls[i].m_position.y < 0) {
			balls[i].m_position = balls[i].m_lastposition;
			balls[i].m_speed.y = fabs(balls[i].m_speed.y);
		}
	}
	float f = 1;
	if (keys['w'])	rect1.m_position.y -= f;
	if (keys['s'])	rect1.m_position.y += f;
	if (keys['a'])	rect1.m_position.x -= f;
	if (keys['d'])	rect1.m_position.x += f;
	glutPostRedisplay();
	glutTimerFunc(
		1000 / 60,// unsigned int mills
		idle,// void (GLUTCALLBACK *func)(int value)
		0// int value
	);
}


void reshape(int width, int height) {
	//printf("reshape: width %d, height %d", width, height);
	glViewport(
		0, 0,// GLint x, y
		width, height
	);
	windowSize = ivec2(width, height);
}

void keyboard(unsigned char key, int x, int y) {
	if (key == 0x1b)
		exit(0);
	//printf("keyboard \'%c\'(%#x)", key, key);
	keys[key] = true;
}

void keyboardUp(unsigned char key, int x, int y) {
	//printf("keyboard \'%c\'(%#x)", key, key);
	keys[key] = false;

}

void PassiveMotion(int x, int y) {
	paddle.m_position = vec2(x, y-paddle.m_height/2);
}

int main(int argc, char *argv[]) {
	srand(time(NULL));
	for (int i = 0; i < BALL_MAX; i++) {
		balls[i].m_position = vec2(
			rand() % windowSize.x,
			rand() % windowSize.y
		);
		balls[i].m_speed =
			normalize(
				vec2(
					(float)rand() / RAND_MAX - .5,
					(float)rand() / RAND_MAX - .5
					)
			)*2.f;
	}

	paddle.m_height = 300;

	glutInit(
		&argc,//int *argcp
		argv//char **argv
	);
	glutInitDisplayMode(GL_DOUBLE);// unsigned int mode
	glutInitWindowPosition(640, 0); // int x,y
	glutInitWindowSize(windowSize.x, windowSize.y);// int width, height
	glutCreateWindow("title");// const char *title
	glutDisplayFunc(display);// void (GLUTCALLBACK *func) (void)
	glutKeyboardFunc(keyboard);
	glutIgnoreKeyRepeat(GL_TRUE);
	glutKeyboardUpFunc(keyboardUp);
	glutPassiveMotionFunc(PassiveMotion);
	glutTimerFunc(
		0,// unsigned int mills
		idle,// void (GLUTCALLBACK *func)(int value)
		0// int value
	);
	glutReshapeFunc(reshape);
	glutMainLoop();
}