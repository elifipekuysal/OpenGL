#include <windows.h>
#include <GL/glut.h>
#include <stdio.h>
#include <iostream>
using namespace std;

int mouseXp, mouseYp;   //Mouse's position
int tx1 = 400.0, tx2 = 620.0, tx3 = 510.0, ty1 = 90.0, ty2 = 270.0;   //Triangle vertex points
int red = 0, green = 119, blue = 255;   // Triangle color variables
int isHit = 0;   //Is mouse clicking the triangle?
int mClickSx, mClickBx;   //Smaller and bigger x coordinates that triangle has when mouse click somewhere on the window
int rx1 = 60.0, rx2 = 280.0, ry1 = 90.0, ry2 = 270.0;   //Rectangle vertex points
int isDragging = 0;   //Is mouse clicking the rectangle?


void init(void) {
	glClearColor(1.0, 1.0, 1.0, 0.0);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluOrtho2D(0.0, 680.0, 0.0, 360.0);
}

void rectangle() {   //Draw rectangle
	glColor3d(0, 0, 0);
	glBegin(GL_LINE_LOOP);
	glVertex2f(rx1, ry1);
	glVertex2f(rx2, ry1);
	glVertex2f(rx2, ry2);
	glVertex2f(rx1, ry2);
	glEnd();
}

void triangle() {   //Draw triangle
	glColor3ub(red, green, blue);
	glBegin(GL_TRIANGLES);
	glVertex2f(tx1, ty1);
	glVertex2f(tx2, ty1);
	glVertex2f(tx3, ty2);
	glEnd();
}

void display() {
	glClearColor(1.0, 1.0, 1.0, 0.0);
	glClear(GL_COLOR_BUFFER_BIT);

	rectangle();
	triangle();

	glFlush();
}

void findTriangleX(int y) {   //Find the x coordinates that triangle has when mouse click somewhere on the window   
	int tDifX;   //Difference between triangle's bigger x coordinate and smaller x coordinate
	tDifX = ((tx2 - tx1) / (ty2 - ty1)) * (mouseYp - ty1);  //Geometric equation of the isosceles triangle's ratio formula
	mClickSx = 510 - (tDifX / 2);   //Triangle's smaller x coordinate
	mClickBx = 510 + (tDifX / 2);   //Triangle's bigger x coordinate
}

void hitTest(int y) {
	if (tx1 - mouseXp <= 0 && tx2 - mouseXp >= 0) {
		if (mouseYp - ty1 >= 0 && mouseYp - ty2 <= 0) {
			findTriangleX(y);
			if (mClickSx - mouseXp <= 0 && mClickBx - mouseXp >= 0) {   //If the mouse's x coordinate inside the triangle's x coordinates
				isHit = 1;   //Set isHit to 1
			}
		}
	}
}

void mouseControl(int button, int state, int x, int y) {
	mouseXp = x;   //Give the old mouse coordinate values to the mouseXp and mouseYp
	mouseYp = y;
	if (button == GLUT_LEFT_BUTTON)	{
		if (state == GLUT_DOWN) {   //If mouse's left button is pressed
			if (rx1 - mouseXp <= 0 && rx2 - mouseXp >= 0){   //If the mouse is inside the rectangle then set isDragging to 1
				if (mouseYp - ry1 >= 0 && mouseYp - ry2 <= 0){
					isDragging = 1;
				}
			}
			hitTest(y);   //Call hitTest function to find if the mouse is inside the triangle
			if (isHit == 1) {   //If the mouse is inside the triangle
				red = (rand() % 256);   //Give a value to the color variables, from 0 to 255            
				green = (rand() % 256);
				blue = (rand() % 256);
			}
		}
		else {   //If mouse's left button released then set isDragging and isHit to 0
			isDragging = 0;
			isHit = 0;
		}
	}
	glutPostRedisplay();
}

void mouseAction(int x, int y) {
	if (isDragging == 1) {   //If mouse is dragging the rectangle
		int xDelta = x - mouseXp;  //Find the difference between new mouse coordinates and old ones then assign this values to the Delta variables
		int yDelta = y - mouseYp;
		rx1 += xDelta;   //Change the rectangle's vertex point coordinates
		rx2 += xDelta;
		ry1 -= yDelta;
		ry2 -= yDelta;
		mouseXp = x;   //Give the new mouse coordinate values to the mouseXp and mouseYp
		mouseYp = y;
	}
	glutPostRedisplay();
}

void keyboardExit(unsigned char key, int x, int y) {
	switch (key) {
		case 'q':   //If user click the 'q' character on the keyboard then exit
			exit(0);
			break;
		case 'Q':
			exit(0);
			break;
	}
}

int main(int argc, char** argv) {
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);

	glutInitWindowPosition(200, 200);
	glutInitWindowSize(680, 360);
	glutCreateWindow("Homework-I");

	glutDisplayFunc(display);

	glutKeyboardFunc(keyboardExit);
	glutMouseFunc(mouseControl);
	glutMotionFunc(mouseAction);

	init();
	glutMainLoop();

	return 1;
}