#include <windows.h>
#include <GL/glut.h>
#include <stdio.h>
#include <string.h>
#include <iostream>
#include <stdlib.h>
#include <math.h>
using namespace std;

GLfloat horizontalMovement = 0, verticalMovement = 0;    //Movement values of cube
GLfloat  xAngle = 60.0, yAngle = -45.0, zAngle = 0.0;    //Angles which cube has with x,y, and z coordinates
bool projectionType = 0;    //Used to know which projection type will be executed.


void init(void) {
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	if (projectionType == 0) {
		gluPerspective(30, 1.5, 1, 10);    //If projectionType==0, then execute Perspective mode
	}
	else {
		glOrtho(-1.0, 1.0, -1.0, 1.0, 1.0, 200);     //If projectionType==1, then execute Orthogonal mode
	}

	glMatrixMode(GL_MODELVIEW);
	glutPostRedisplay();

}

void drawCube(void) {
	glBegin(GL_QUADS);
	glColor3f(0.3f, 0.5f, 1.0f);    //Fixed color
	glVertex3f(1.0f, 1.0f, -1.0f);
	glVertex3f(-1.0f, 1.0f, -1.0f);
	glVertex3f(-1.0f, 1.0f, 1.0f);
	glVertex3f(1.0f, 1.0f, 1.0f);

	glColor3f(0.8f, 0.2f, 0.3f);    //Fixed color
	glVertex3f(1.0f, -1.0f, 1.0f);
	glVertex3f(-1.0f, -1.0f, 1.0f);
	glVertex3f(-1.0f, -1.0f, -1.0f);
	glVertex3f(1.0f, -1.0f, -1.0f);

	glColor3f(0.8f, 0.5f, 0.8f);    //Fixed color 
	glVertex3f(1.0f, 1.0f, 1.0f);
	glVertex3f(-1.0f, 1.0f, 1.0f);
	glVertex3f(-1.0f, -1.0f, 1.0f);
	glVertex3f(1.0f, -1.0f, 1.0f);

	glColor3f(0.7f, 0.3f, 0.7f);    //Fixed color 
	glVertex3f(1.0f, -1.0f, -1.0f);
	glVertex3f(-1.0f, -1.0f, -1.0f);
	glVertex3f(-1.0f, 1.0f, -1.0f);
	glVertex3f(1.0f, 1.0f, -1.0f);

	glColor3f(0.5f, 0.0f, 1.0f);    glVertex3f(-1.0f, 1.0f, 1.0f);    //Mixed color 
	glColor3f(0.0f, 0.5f, 1.0f);	glVertex3f(-1.0f, 1.0f, -1.0f);
	glColor3f(1.0f, 0.5f, 0.5f);	glVertex3f(-1.0f, -1.0f, -1.0f);
	glColor3f(0.5f, 0.5f, 0.5f);	glVertex3f(-1.0f, -1.0f, 1.0f);

	glColor3f(0.8f, 0.2f, 0.3f);	glVertex3f(1.0f, 1.0f, -1.0f);    //Mixed color
	glColor3f(0.3f, 0.5f, 1.0f);	glVertex3f(1.0f, 1.0f, 1.0f);
	glColor3f(0.8f, 0.5f, 0.8f);	glVertex3f(1.0f, -1.0f, 1.0f);
	glColor3f(0.7f, 0.3f, 0.7f);    glVertex3f(1.0f, -1.0f, -1.0f);
	glEnd();
}

void keyboard(unsigned char key, int x, int y)
{
	switch (key) {
	case 'w':
		verticalMovement -= 5;    //If user press the 'w' button, then decrease verticalMocement by 5 
		break;                    //Camera will be moved to the down, view of the cube will be moved to the up 
	case 'a':
		horizontalMovement -= 5;    //If user press the 'a' button, then decrease horizontalMocement by 5 
		break;                      //Camera will be moved to the right, view of the cube will be moved to the left
	case 's':
		verticalMovement += 5;
		break;
	case 'd':
		horizontalMovement += 5;
		break;
	case 'p':    //If user presses 'p' button, then change the projectionType
		if (projectionType == 0) {
			cout << "Projection type changed to Orthogonal mode!" << endl;
			projectionType = 1;
		}
		else {
			cout << "Projection type changed to Perspective mode!" << endl;
			projectionType = 0;
		}
		break;
	}
	glutPostRedisplay();
}

void display(void) {

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glEnable(GL_DEPTH_TEST);

	glLoadIdentity();

	glTranslatef(0, 0, -3);    //Place cube inside the window
	glRotatef(verticalMovement, 1, 0, 0);  // 'w' and 's' keyboard keys 'tip' view.
	glRotatef(horizontalMovement, 0, 1, 0);  // 'a' and 'd' keyboard keys 'turn' view.

	glPushMatrix();
	glScalef(0.2, 0.2, 0.2);    //Scale the cube by 0.2
	glRotatef(zAngle, 0, 0, 1);    //Use zAngle variable to specify the angle which cube has with z-coordinate
	glRotatef(yAngle, 0, 1, 0);    //Use yAngle variable to specify the angle which cube has with y-coordinate
	glRotatef(xAngle, 1, 0, 0);    //Use xAngle variable to specify the angle which cube has with x-coordinate
	drawCube();
	glPopMatrix();

	init();
	glutSwapBuffers();
}

void reshape(int width, int height) {
	double hScale = static_cast<double>(height) / static_cast<double>(480);
	double wScale = static_cast<double>(width) / static_cast<double>(640);
	glOrtho(0, wScale, 0, hScale, -1, 1);
}

int main(int argc, char **argv) {
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);

	glutInitWindowSize(640, 480);
	glutInitWindowPosition(200, 100);
	glutCreateWindow("Homework - II");

	glutDisplayFunc(display);
	glutReshapeFunc(reshape);  
	glutKeyboardFunc(keyboard);

	init();
	glutMainLoop();

	return 1;
}