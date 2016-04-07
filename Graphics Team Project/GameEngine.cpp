/*
*
* Demonstrates how to load and display an Wavefront OBJ file.
* Using triangles and normals as static object. No texture mapping.
*
* OBJ files must be triangulated!!!
* Non triangulated objects wont work!
* You can use Blender to triangulate
*
Look into the main method to change the path to the moel file (shoud have no path) and the model should be at the
same location of this file
*/

#include <windows.h>
#include <iostream>
#include <stdio.h>
#include <string.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>
#include <iostream>
#include <sstream>
#include <fstream>
#include <string>
#include <vector>
#include "cobject.h"
#include "glcamera.h"

#define KEY_ESCAPE 27

using namespace std;

typedef struct {
	int width;
	int height;
	char* title;

	float field_of_view_angle;
	float z_near;
	float z_far;
} glutWindow;


//Model_OBJ obj;
Object3D *obj;
Object3D obj2("spray can 2.obj");
Object3D obj3("spray can 2.obj");
glutWindow win;
Camera cam;
int mouse_x, mouse_y;





/***************************************************************************
* Program code
Spray can added
***************************************************************************/

void initObjects(){
	obj= new Object3D("spray can 2.obj");
}

/***************************************************************************
* Game Logic
need to make some movement for the spray cans and may need to remove the other
3 to test the movements
***************************************************************************/
void updateGame(){
	//cam.setLocation(0, 1, 4);
	if ((*obj).rotY < 3){
		cam.setLocation(0, 1, 4);
		cam.lookAt(0, 0, 0);
	}
	//cam.lookAt((mouse_x - 600.0) / 1200.0, (mouse_y - 400.0) / 800.0, 0);
	(*obj).rotY++;
	(*obj).xPos = 0;
	obj2.rotY--;
	obj3.rotY--;
	obj2.setLocation(-4, 0, 0);
	obj3.setLocation(4, 0, 0);	
	glutPostRedisplay(); // must be last line of code
}

void display()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glLoadIdentity(); // camera functionality must be in display method
	cam.Update();//MUST UPDATE CAMERA BEFORE DRAWING OBJECTS
	glScalef(.1, .1, .1);
	glColor3f(1.0, 0.0, 0.0); //Set object 1 to red
	(*obj).Draw();
	glColor3f(0.0, 1.0, 0.0); //Set object 2 to Green
	obj2.Draw();
	glColor3f(0.0, 0.0, 1.0); //Set object 3 to blue
	obj3.Draw();
	glutSwapBuffers();
}


void mouseInfo(int button, int state, int x, int y)
{
	mouse_x = x; //place current mouse pos in mouse_x
	mouse_y = y;
}

void mousePosUpdate(int x, int y)
{
	mouse_x = x; //place current mouse pos in mouse_x
	mouse_y = y;
}

void initialize()
{
	glMatrixMode(GL_PROJECTION);
	glViewport(0, 0, win.width, win.height);
	GLfloat aspect = (GLfloat)win.width / win.height;
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(win.field_of_view_angle, aspect, win.z_near, win.z_far);
	glMatrixMode(GL_MODELVIEW);
	glShadeModel(GL_SMOOTH);
	glClearColor(0.0f, 0.1f, 0.0f, 0.5f);
	glClearDepth(1.0f);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_NORMALIZE);
	glDepthFunc(GL_LEQUAL);
	glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);

	GLfloat amb_light[] = { 0.1, 0.1, .1, 1.0 };
	//GLfloat amb_light[] = { 0.7, 0.7, .7, 1.0 };
	GLfloat diffuse[] = { 0.6, 0.6, 0.6, 1 };
	GLfloat specular[] = { 0.7, 0.7, 0.3, 1 };
	glLightModelfv(GL_LIGHT_MODEL_AMBIENT, amb_light);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, diffuse);
	glLightfv(GL_LIGHT0, GL_SPECULAR, specular);
	//glEnable(GL_LIGHT0);
	glEnable(GL_COLOR_MATERIAL);
	glShadeModel(GL_SMOOTH);
	glLightModeli(GL_LIGHT_MODEL_TWO_SIDE, GL_FALSE);
	glDepthFunc(GL_LEQUAL);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);
}


void keyboard(unsigned char key, int x, int y)
{
	switch (key) {
	case KEY_ESCAPE:
		exit(0);
		break;
	case 'w':
		cam.move(.1);
		break;
	case 's':
		cam.move(-.1);
		break;
	case 'a':
		cam.moveSide(-.1);
		break;
	case 'd':
		cam.moveSide(.1);
		break;
	default:
		break;
	}
}

void SpecialKey(int key, int x, int y)
{

	switch (key) {
	case GLUT_KEY_UP:
		cam.rotate(1, 1,0,0);
		break;
	case GLUT_KEY_DOWN:
		cam.rotate(-1, 1, 0, 0);

		break;
	case GLUT_KEY_LEFT:
		cam.rotate(1, 0, 1, 0);
		break;
	case GLUT_KEY_RIGHT:
		cam.rotate(-1, 0, 1, 0);
		break;
	case GLUT_KEY_PAGE_UP:
		cam.setLocation(cam.xPos, cam.yPos + .1, cam.zPos);
		break;
	case GLUT_KEY_PAGE_DOWN:
		cam.setLocation(cam.xPos, cam.yPos - .1, cam.zPos);
		break;
	}
}

int main(int argc, char **argv)
{
	// set window values
	win.width = 1200;
	win.height = 800;
	win.title = "OpenGL/GLUT OBJ Loader";
	win.field_of_view_angle = 45;
	win.z_near = 1.0f;
	win.z_far = 500.0f;

	// initialize and run program
	glutInit(&argc, argv);                                      // GLUT initialization
	glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH);  // Display Mode
	glutInitWindowSize(win.width, win.height);					// set window size
	glutCreateWindow(win.title);								// create Window
	glutDisplayFunc(display);									// register Display Function
	glutIdleFunc(updateGame);									// register Idle Function
	glutKeyboardFunc(keyboard);								// register Keyboard Handler
	glutSpecialFunc(SpecialKey);
	glutPassiveMotionFunc(mousePosUpdate);              //register mouse tracking
	glutMouseFunc(mouseInfo);

	initialize();
	initObjects();
	
	glutMainLoop();												// run GLUT mainloop
	return 0;
}