#include <iostream>
#include <fstream>
#include <math.h>
#include <string> 
#include <GL/glut.h>
using namespace std;

GLfloat horizontalMovement = 0, verticalMovement = 0;    //Movement values

bool exe= 0;

//x, y, and z angles of the objects
int lax= 90, lay= 270, laz= -90;		//left arm x, y, z
int llax= 90, llay= 0, llaz= 0;		//left lower arm x, y, z
int rax= 90, ray= 90, raz= 90;		//right arm x, y, z
int rlax= 90, rlay= 0, rlaz= 0;		//right lower arm x, y, z

int llx= 0, lly= -15, llz= 0;		//left leg x, y, z
int lllx= 0, llly= -16, lllz= 0;		//left lower leg x, y, z
int rlx= 0, rly= 15, rlz= 0;		//right leg x, y, z
int rllx= 0, rlly= 16, rllz= 0;		//right lower leg x, y, z

int pinocchio[8][3]= {90, 90, -90,
						    90, 0, 0,
							90, 0, 90, 
							90, 0, 90,
							90, 0, -15,
							90, -16, 0,
							90, 0, 15,
							90, 16, 0};

int pinocchioStart[8][3]= {90, 90, -90,
						    90, 0, 0,
							90, 0, 90, 
							90, 0, 90,
							90, 0, -15,
							90, -16, 0,
							90, 0, 15,
							90, 16, 0};

int pinocchioEnd[8][3]= {90, 90, -90,
						    90, 0, 0,
							90, 0, 90, 
							90, 0, 90,
							90, 0, -15,
							90, -16, 0,
							90, 0, 15,
							90, 16, 0};

//Arm Angles
int leftArmAngle = 180;
int leftLowerArmAngle = 180;
int rightArmAngle = 180;
int rightLowerArmAngle = 180;

int llat= 0;  //Left lower arm y translate
int llaxt= 0;  //Left lower arm x translate
int rlat= 0;	//Right lower arm y translate
int rlaxt= 0;	//Right lower arm x translate
int lllt= 0;	//Left lower leg y translate
int lllxt= 0;	//Left lower leg x translate
int rllt= 0;	//Right lower leg y translate
int rllxt= 0;	//Right lower leg x translate

//Leg Angles
int leftLegAngle = 90;
int leftLowLegAngle = 90;
int rightLegAngle = 90;
int rightLowLegAngle = 90;

//Left click control
bool mouseLeftState = false;

//Right click control
bool mouseRightState = false;

//If right click, arms move 2 degree every second
int rightArmMove = 2;
int leftArmMove = 2;

//If left click, legs move 1 degree every second
int leftLegMove = 1;
int leftLowLegMove = -1;
int leftLowerLegTranslate = 0;
int rightLegMove = -1;
int rightLowLegMove = 1;
int rightLowerLegTranslate = 0;

//With walking pinocchio moves in z-coordinate system
float zMove = 0.0;


void init(void) {
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	glOrtho(-750.0, 750.0, -500.0, 500.0, -99999999999.0, 99999999999.0);

	glMatrixMode(GL_MODELVIEW);
	glutPostRedisplay();

}

void cameraMovement(int key, int x, int y) {
	switch(key) {
		case GLUT_KEY_UP:
			verticalMovement -= 5;    //If user press the up arrow key, then decrease verticalMocement by 5 
			break;                    
		case GLUT_KEY_LEFT:
			horizontalMovement -= 5;    //If user press the left arrow key, then decrease horizontalMocement by 5 
			break;                     
		case GLUT_KEY_DOWN:
			verticalMovement += 5;
			break;
		case GLUT_KEY_RIGHT:
			horizontalMovement += 5;
			break;
	}
}

void pinocchioSS() {	//Save pinocchio's start position
	for(int i=0; i<8; i++) {
		for(int j=0; j<3; j++) {
			pinocchioStart[i][j]= pinocchio[i][j];
		}		
	}
}

void pinocchioES() {	//Save pinocchio's end position
	for(int i=0; i<8; i++) {
		for(int j=0; j<3; j++) {
			pinocchioEnd[i][j]= pinocchio[i][j];
		}		
	}
}

void executeAnimation() {	//Execute animation between start-end positions
	int ras, rae;
	int las, lae;
	int lls, lle;
	int rls, rle;
	
	//Right Lower Arm Start-End Angles
	if(pinocchioEnd[3][1]>=pinocchioStart[3][1]) {	
		rae= pinocchioEnd[3][1];
		ras= pinocchioStart[3][1];
	}
	else {
		rae= pinocchioStart[3][1];
		ras= pinocchioEnd[3][1];
	}

	//Left Lower Arm Start-End Angles
	if(pinocchioEnd[1][1]>=pinocchioStart[1][1]) {
		lae= pinocchioEnd[1][1];
		las= pinocchioStart[1][1];
	}
	else {
		lae= pinocchioStart[1][1];
		las= pinocchioEnd[1][1];
	}

	//Left Lower Leg Start-End Angles
	if(pinocchioEnd[5][1]>=pinocchioStart[5][1]) {
		lle= pinocchioEnd[5][1];
		rls= pinocchioStart[5][1];
	}
	else {
		lle= pinocchioStart[5][1];
		lls= pinocchioEnd[5][1];
	}

	//Right Lower Leg Start-End Angles
	if(pinocchioEnd[7][1]>=pinocchioStart[7][1]) {
		rle= pinocchioEnd[7][1];
		rls= pinocchioStart[7][1];
	}
	else {
		rle= pinocchioStart[7][1];
		rls= pinocchioEnd[7][1];
	}


	//Movement of lower arms
    if (rlay >= rae) { //Exchange direction
      rightArmMove = -rightArmMove;
    }
    else if (rlay <= ras) { //Exchange direction
      rightArmMove = -rightArmMove;
    }
    rightArmAngle = (rightArmAngle + rightArmMove) % 360; //Exchange right hand angle

    if (llay >= lae) { //Exchange direction
      leftArmMove = -leftArmMove;
    }
    else if (llay <= las){ //Exchange direction
      leftArmMove = -leftArmMove;
    }
    leftArmAngle = (leftArmAngle + leftArmMove) % 360; //Exchange left hand angle

	//Movement of lower legs
    if (llly > lle) {
      leftLowLegMove = -leftLowLegMove;
    }
    else if (llly < lls) {
      leftLowLegMove = -leftLowLegMove;
    }
    leftLowLegAngle = (leftLowLegAngle - leftLowLegMove) % 360; //Lower left leg angle

    if (rlly > rle) {
      rightLowLegMove = -rightLowLegMove;
    }
    else if (rlly < rls) {
      rightLowLegMove = -rightLowLegMove;
    }
    rightLowLegAngle = (rightLowLegAngle - rightLowLegMove) % 360; //Lower right leg angle

	glutPostRedisplay();
}

void readAnimation() {	//Read wanted pinocchio's start-end positions
	string animationName;
	ifstream pinocchio;
	string line;
	pinocchio.open("save.txt");

	if(pinocchio.is_open()) {
		cout<<"Which animation do you want to play?: ";
		cin>>animationName;

		while(getline(pinocchio,line)) {	//Get one line each time
            if(animationName == line) {		//If found wanted animation name in file, get start and end positions
				for(int j=0; j<8; j++) {
					for(int i = 0; i < 3; ++i) {
						pinocchio >> pinocchioStart[j][i];	//Get start position
						cout<<pinocchioStart[j][i]<<" ";
					}      
				} 
				cout<<endl;
				for(int j=0; j<8; j++) {
					for(int i = 0; i < 3; ++i) {
						pinocchio >> pinocchioEnd[j][i];	//Get end position
						cout<<pinocchioEnd[j][i]<<" ";
					}      
				}
				cout<<endl;
			}
		}				
		pinocchio.close();		//Close file	
	}
}

void saveAnimation() {		//Save start-end positions
	string animationName;
	fstream pinocchio;
	pinocchio.open("save.txt");		//Open save file

	if(pinocchio.is_open()) {	 //If file opened
		cout<<"Enter a name for new animation: ";
		cin>>animationName;
		
		pinocchio.seekg (0, pinocchio.end);		//Go to end of file
		pinocchio<<animationName<<"\n";		//Add animation name to the last line
		for(int i=0; i<8; i++) {
			for(int j=0; j<3; j++) {
				pinocchio << pinocchioStart[i][j]<<" ";		//Save start positions
				cout<<pinocchioStart[i][j]<<" ";			
			}
		}	
		cout<<endl;
		pinocchio<<"\n";
		for(int j=0; j<8; j++) {
			for(int i= 0; i < 3; ++i) {
				pinocchio << pinocchioEnd[j][i]<<" ";		//Save end positions
				cout<<pinocchioEnd[j][i]<<" ";
			}
		} 	
		pinocchio<<"\n";		
		pinocchio.close();		//Close file 
	}	
	cout<<endl<<"Animation saved!"<<endl;
}

void humanoid(){
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  glPushMatrix(); 	//Torso
    glColor3f(0.0, 0.0, 0.0);
    glTranslatef(0.0, 221, zMove);
    glRotatef(90, 1.0, 0.0, 0.0);
    GLUquadricObj* body = gluNewQuadric();
    gluQuadricDrawStyle(body, GLU_SILHOUETTE);
	gluCylinder(body, 120, 120, 300, 30, 30);
  glPopMatrix();

  glPushMatrix(); 	//Left lower arm --> Left upper arm
    glColor3f(1.0, 0.0, 1.0);
    glPushMatrix();
      glTranslatef(llaxt-280, llat+160, zMove);
      glutSolidSphere(16, 30, 30);
    glPopMatrix();	
  glPushMatrix(); 	//Left upper arm
    glColor3f(0.0, 0.0, 0.0);
    glTranslatef(-80, 160, zMove);
    glRotatef(lax, 1.0, 0.0, 0.0);
    glRotatef(lay, 0.0, 1.0, 0.0);
    glRotatef(laz, 0.0, 0.0, 1.0);
    GLUquadricObj* leftUpperArm = gluNewQuadric();
    gluQuadricDrawStyle(leftUpperArm, GLU_SILHOUETTE);
    gluCylinder(leftUpperArm, 16, 16, 200, 30, 30);
  glPopMatrix();
    glPushMatrix(); 	//Left lower arm
    glColor3f(0.0, 0.0, 1.0);
    glTranslatef(llaxt-280, llat+160, zMove);
    glRotatef((GLfloat)leftArmAngle, 0.0, 0.0, 1.0);
    glRotatef(llax, 1.0, 0.0, 0.0);
    glRotatef(llay, 0.0, 1.0, 0.0);
    glRotatef(llaz, 0.0, 0.0, 1.0);
    GLUquadricObj* leftLowerArm = gluNewQuadric();
    gluQuadricDrawStyle(leftLowerArm, GLU_SILHOUETTE);
    gluCylinder(leftLowerArm, 16, 16, 200, 30, 30);
  glPopMatrix();

      glPushMatrix(); 	   //Right lower arm --> Right upper arm
        glColor3f(1.0, 0.0, 1.0);
        glPushMatrix();
          glTranslatef(rlaxt+280, rlat+160, zMove);
          glutSolidSphere(16, 30, 30);
        glPopMatrix();
		glPushMatrix(); 	//Right upper arm
		  glColor3f(0.0, 0.0, 0.0);
		  glTranslatef(80, 160, zMove);
		  glRotatef(rax, 1.0, 0.0, 0.0);
		  glRotatef(ray, 0.0, 1.0, 0.0);
		  glRotatef(raz, 0.0, 0.0, 1.0);
		  GLUquadricObj* rightUpperArm = gluNewQuadric();
		  gluQuadricDrawStyle(rightUpperArm, GLU_SILHOUETTE);
		  gluCylinder(rightUpperArm, 16, 16, 200, 30, 30);
		glPopMatrix();
		  glPushMatrix(); 	//Right lower arm
		    glColor3f(0.0, 0.0, 1.0);
		    glTranslatef(rlaxt+280, rlat+160, zMove);
		    glRotatef((GLfloat)rightArmAngle, 0.0, 0.0, 1.0);
			glRotatef(rlax, 1.0, 0.0, 0.0);
			glRotatef(rlay, 0.0, 1.0, 0.0);
			glRotatef(rlaz, 0.0, 0.0, 1.0);
		    GLUquadricObj* rightLowerArm = gluNewQuadric();
		    gluQuadricDrawStyle(rightLowerArm, GLU_SILHOUETTE);
		    gluCylinder(rightLowerArm, 16, 16, 200, 30, 30);
		  glPopMatrix();
	
	 	glPushMatrix(); 	//Left lower leg --> Left upper leg
   	 	glColor3f(1.0, 0.0, 1.0);
   	 	glPushMatrix();
   	 	  glTranslatef(lllxt-87, lllt-220, leftLowerLegTranslate+zMove);
   	 	  glutSolidSphere(20, 30, 30);
   	 	glPopMatrix();
        glPushMatrix(); 	//Left upper leg
          glColor3f(0.0, 0.0, 0.0);
          glTranslatef(-35, -20, zMove);
          glRotatef((GLfloat)leftLegAngle, 1.0, 0.0, 0.0);
     	  glRotatef(llx, 1.0, 0.0, 0.0);
		  glRotatef(lly, 0.0, 1.0, 0.0);
		  glRotatef(llz, 0.0, 0.0, 1.0);
          GLUquadricObj* leftLeg = gluNewQuadric();
          gluQuadricDrawStyle(leftLeg, GLU_SILHOUETTE);
          gluCylinder(leftLeg, 16, 16, 210, 30, 30);
        glPopMatrix();
		glPopMatrix();
		glPushMatrix(); 	//Left lower leg
		  glColor3f(0.0, 0.0, 1.0);
		  glTranslatef(lllxt-85, lllt-210, leftLowerLegTranslate+zMove);
          glRotatef((GLfloat)leftLowLegAngle, 1.0, 0.0, 0.0);
		  GLUquadricObj* leftLowerLeg = gluNewQuadric();
          glRotatef(lllx, 1.0, 0.0, 0.0);
          glRotatef(llly, 0.0, 1.0, 0.0);
    	  glRotatef(lllz, 0.0, 0.0, 1.0);
		  gluQuadricDrawStyle(leftLowerLeg, GLU_SILHOUETTE);
		  gluCylinder(leftLowerLeg, 16, 16, 200, 30, 30);
		glPopMatrix();

	 	glPushMatrix(); 	//Right lower leg --> Right upper leg
   	 	glColor3f(1.0, 0.0, 1.0);
   	 	glPushMatrix();
   	 	  glTranslatef(rllxt+87, rllt-220, rightLowerLegTranslate+zMove);
   	 	  glutSolidSphere(20, 30, 30);
   	 	glPopMatrix();
        glPushMatrix(); 	//Right upper leg
          glColor3f(0.0, 0.0, 0.0);
          glTranslatef(35, -20, zMove);
          glRotatef((GLfloat)rightLegAngle, 1.0, 0.0, 0.0);
		  glRotatef(rlx, 1.0, 0.0, 0.0);
		  glRotatef(rly, 0.0, 1.0, 0.0);
		  glRotatef(rlz, 0.0, 0.0, 1.0);
          GLUquadricObj* rightLeg = gluNewQuadric();
          gluQuadricDrawStyle(rightLeg, GLU_SILHOUETTE);
          gluCylinder(rightLeg, 16, 16, 210, 30, 30);
        glPopMatrix();
		glPushMatrix(); 	//Right lower leg
		  glColor3f(0.0, 0.0, 1.0);
		  glTranslatef(rllxt+85, rllt-210, rightLowerLegTranslate+zMove);
          glRotatef((GLfloat)rightLowLegAngle, 1.0, 0.0, 0.0);
		  glRotatef(rllx, 1.0, 0.0, 0.0);
		  glRotatef(rlly, 0.0, 1.0, 0.0);
		  glRotatef(rllz, 0.0, 0.0, 1.0);
		  GLUquadricObj* rightLowerLeg = gluNewQuadric();
		  gluQuadricDrawStyle(rightLowerLeg, GLU_SILHOUETTE);
		  gluCylinder(rightLowerLeg, 16, 16, 200, 30, 30);
		glPopMatrix();

        glPushMatrix(); 	//Neck
          glColor3f(0.0, 0.0, 0.0);
          glTranslatef(0.0, 251, zMove);
          glRotatef(90, 1.0, 0.0, 0.0);
          GLUquadricObj* neck = gluNewQuadric();
          gluQuadricDrawStyle(neck, GLU_SILHOUETTE);
          gluCylinder(neck, 20, 20, 30, 30, 30);
        glPopMatrix();

        glPushMatrix(); 	//Head
          glColor3f(0.0, 0.0, 0.0);
          glPushMatrix();
          glTranslatef(0.0, 350, zMove);
          glutSolidSphere(100, 30, 30);
        glPopMatrix();
      glPopMatrix();
    glPopMatrix();
  glPopMatrix();

}

void handWaving() {
  if (mouseLeftState == true) { //Hand waving
    if (rightArmAngle >= 225) { //Exchange direction
      rightArmMove = -rightArmMove;
    }
    else if (rightArmAngle <= 135) { //Exchange direction
      rightArmMove = -rightArmMove;
    }
    rightArmAngle = (rightArmAngle + rightArmMove) % 360; //Exchange right hand angle

    if (leftArmAngle >= 225) { //Exchange direction
      leftArmMove = -leftArmMove;
    }
    else if (leftArmAngle <= 135){ //Exchange direction
      leftArmMove = -leftArmMove;
    }
    leftArmAngle = (leftArmAngle + leftArmMove) % 360; //Exchange left hand angle
  }
}

void walking() {
  if (mouseRightState == true) { // Walking
	//Movement of higher legs
    if (leftLegAngle > 110) {
      leftLegMove = -leftLegMove;
    }
    else if (leftLegAngle < 70) {
      leftLegMove = -leftLegMove;
    }
    leftLegAngle = (leftLegAngle + leftLegMove) % 360; //Left leg angle

    if (rightLegAngle > 110) {
      rightLegMove = -rightLegMove;
    }
    else if (rightLegAngle < 70) {
      rightLegMove = -rightLegMove;
    }
    rightLegAngle = (rightLegAngle + rightLegMove) % 360; //Right leg angle   

	//Movement of lower legs
    if (leftLowLegAngle > 110) {
      leftLowLegMove = -leftLowLegMove;
    }
    else if (leftLowLegAngle < 70) {
      leftLowLegMove = -leftLowLegMove;
    }
    leftLowerLegTranslate += (-3)*leftLegMove;
    leftLowLegAngle = (leftLowLegAngle - leftLowLegMove) % 360; //Lower left leg angle

    if (rightLowLegAngle > 110) {
      rightLowLegMove = -rightLowLegMove;
    }
    else if (rightLowLegAngle < 70) {
      rightLowLegMove = -rightLowLegMove;
    }
	rightLowerLegTranslate += (-3)*rightLegMove;
    rightLowLegAngle = (rightLowLegAngle - rightLowLegMove) % 360; //Lower right leg angle

	zMove += 1.5f; //Movement in z-coordinate system
  }

}

void display() {
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  glEnable(GL_DEPTH_TEST);
  
  glLoadIdentity();

  glRotatef(verticalMovement, 1, 0, 0);  // Up and down arrow keys 'tip' view.
  glRotatef(horizontalMovement, 0, 1, 0);  // Left and right keyboard keys 'turn' view.

  humanoid();
  handWaving();
  walking();

  if(exe == true)		//When pressed 'w' call executeAnimation function
  	executeAnimation();

  init();
  glutSwapBuffers();
}

void mouse(int button, int state, int x, int y) {
  if (GLUT_LEFT_BUTTON == button && state == GLUT_DOWN && mouseLeftState == false) {	//Execute handWaving function
    mouseLeftState = true;
  }
  else if (GLUT_LEFT_BUTTON == button && state == GLUT_DOWN && mouseLeftState == true) {
    mouseLeftState = false;
  }
  else if (GLUT_RIGHT_BUTTON == button && state == GLUT_DOWN && mouseRightState == false) {		//Execute walking function
    mouseRightState = true;
  }
  else if (GLUT_RIGHT_BUTTON == button && state == GLUT_DOWN && mouseRightState == true) {
    mouseRightState = false;
  }
}

void keyboard(unsigned char key, int x, int y) {
	int add;
	double translate;
	switch(key) {
		case 'e':	//Left arm keyboard movement
			cout<<"Enter a value in to add: ";
			cin>>add;
			lay-=add;
			pinocchio[0][1]= lay;
			llat+= add*3.2;
			llaxt+= add+2;
			break;
		case 'd':	//Left lower arm keyboard movement
			cout<<"Enter a value in to add: ";
			cin>>add;
			llay-=add;
			pinocchio[1][1]= llay;
			break;
		case 'r':	//Right arm keyboard movement
			cout<<"Enter a value in to add: ";
			cin>>add;
			ray+=add;
			pinocchio[2][1]= ray;
			rlat+= add*3.0;
			rlaxt-= add*1.5;
			break;		
		case 'f':	//Right lower arm keyboard movement
			cout<<"Enter a value in to add: ";
			cin>>add;
			rlay+=add;
			pinocchio[3][1]= rlay;
			break;

		case 't':	//Left leg keyboard movement
			cout<<"Enter a value in to add: ";
			cin>>add;
			lly-=add;
			pinocchio[4][1]= lly;
			lllt+= add*1.6;
			lllxt-= add*2.9;
			break;
		case 'g':	//Left lower leg keyboard movement
			cout<<"Enter a value in to add: ";
			cin>>add;
			llly-=add;
			pinocchio[5][1]= llly;
			break;
		case 'y':	//Right leg keyboard movement
			cout<<"Enter a value in to add: ";
			cin>>add;
			rly+=add;
			pinocchio[6][1]= rly;
			rllt+= add*1.6;
			rllxt+= add*2.9;
			break;	
		case 'h':	//Right lower leg keyboard movement
			cout<<"Enter a value in to add: ";
			cin>>add;
			rlly+=add;
			pinocchio[7][1]= rlly;
			break;

		case 'x':	//Read animation
			readAnimation();
			break;	
		case 'w':	//Execute animation
			exe!= exe;
			executeAnimation();
			break;
		case 'a':	//Save start position
			pinocchioSS();
			break;
		case 'z':	//Save end position
			pinocchioES();
			break;
		case 's':	//Save animation
			saveAnimation();
			break;

		case 'q':	//If user presses 'q' button, then exit
			exit(EXIT_SUCCESS);
	}	
	glutPostRedisplay();
}

void timer(int notUsed) {
  glutPostRedisplay();
  glutTimerFunc(100, timer, 0);
}

int main(int argc, char *argv[]) {
  glutInit(&argc, argv);
  glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
  glEnable(GL_DEPTH_TEST);

  glutInitWindowSize(1200, 700);
  glutInitWindowPosition(200, 100);
  glutCreateWindow("Homework - III");
  
  glClearColor(1.0, 1.0, 1.0, 0.0);
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();

  glutDisplayFunc(display);
  glutMouseFunc(mouse);
  glutKeyboardFunc(keyboard);
  glutSpecialFunc(cameraMovement);

  timer(0);
  glutMainLoop();

  return EXIT_SUCCESS;
}








