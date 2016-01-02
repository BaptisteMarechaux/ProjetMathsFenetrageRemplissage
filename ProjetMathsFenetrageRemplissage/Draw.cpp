// C, standard, dio
#include <cstdio>
#include <cstdint>

#include "GL/freeglut.h"

#include <iostream>

#include "Draw.h"

GLint mousex;
GLint mousey;

GLint Win;

#include <vector>
using std::vector;

int indexPolyMode=0;
int indexWinMode=0;
int mode; // 1 = Poly
bool fillPoly = false;

struct Point
{
	float x;
	float y;
};

vector<Point> polyPoints;
vector<Point> winPoints;

int polyColor = 0;
int winColor = 0;

float x1, x2, x3, x4, y5, y2, y3, y4;

void Initialize() 
{
	glClearColor(1.0, 1.0, 1.0, 0.0);
	glColor3f(0.0f, 0.0f, 1.0f);
	glPointSize(2.0);			
	glMatrixMode(GL_PROJECTION);	
	glLoadIdentity();	

	gluOrtho2D(0.0, (GLdouble)800,
		(GLdouble)600, 0.0);
}

void Terminate() 
{

}

void draw_pixel(int x, int y)
{
	glColor3f(0.0, 1.0, 1.0);
	glPointSize(1.0);
	glBegin(GL_POINTS);
	glVertex2i(x, y);
	glEnd();
}

void edgedetect(float x1, float y1, float x2, float y2, int *le, int *re)
{
	float temp, x, mx;
	int i;

	if (y1>y2)
	{
		temp = x1, x1 = x2, x2 = temp;
		temp = y1, y1 = y2, y2 = temp;
	}

	if (y1 == y2)
		mx = x2 - x1;
	else
		mx = (x2 - x1) / (y2 - y1);

	x = x1;

	for (i = int(y1); i <= (int)y2; i++)
	{
		if (x<(float)le[i]) le[i] = (int)x;
		if (x>(float)re[i]) re[i] = (int)x;
		x += mx;
	}
}

void scanfill(vector<Point> pois)
{
	int le[1000], re[1000], i, j;

	for (i = 0; i<1000; i++)
		le[i] = 1000, re[i] = 0;

	int k;
	for (k = 0; k < pois.size() -1; k++)
	{
		edgedetect(pois[k].x, pois[k].y, pois[k+1].x, pois[k+1].y, le, re);
	}

	edgedetect(pois[k].x, pois[k].y, pois[0].x, pois[0].y, le, re);

	for (j = 0; j<1000; j++)
	{
		if (le[j] <= re[j])
			for (i = le[j]; i<re[j]; i++)
				draw_pixel(i, j);
	}
}

void Render() 
{
	glClear(GL_COLOR_BUFFER_BIT);

	switch (polyColor) {
	case 1:
		glColor3f(1.0f, 0.0f, 0.0f);
		break;
	case 2:
		glColor3f(0.0f, 0.0f, 1.0f);
		break;
	case 3:
		glColor3f(0.0f, 1.0f, 0.0f);
		break;
	}

	switch (indexPolyMode) {
		case 1:
			glBegin(GL_POINTS);
			for (int i = 0; i < polyPoints.size(); i++) {
				glVertex2i(polyPoints[i].x, polyPoints[i].y);
			}
			glEnd();
			break;

		case 2:
			glBegin(GL_LINE_STRIP);
			for (int i = 0; i < polyPoints.size(); i++) {
				glVertex2i(polyPoints[i].x, polyPoints[i].y);
			}
			glEnd();
			break;

		case 3:
			glBegin(GL_LINE_LOOP);
			for (int i = 0; i < polyPoints.size(); i++) {
				glVertex2i(polyPoints[i].x, polyPoints[i].y);
			}
			glEnd();
			/*glBegin(GL_POINTS);
			for (int i = 0; i < polyPoints.size(); i++) {
				glVertex2i(polyPoints[i].x, polyPoints[i].y);
			}*/
			glEnd();
			break;

		default:
			break;
	}

	switch (winColor) {
	case 1:
		glColor3f(1.0f, 0.0f, 0.0f);
		break;
	case 2:
		glColor3f(0.0f, 0.0f, 1.0f);
		break;
	case 3:
		glColor3f(0.0f, 1.0f, 0.0f);
		break;
	}

	switch (indexWinMode) {
	case 1:
		glBegin(GL_POINTS);
		for (int i = 0; i < winPoints.size(); i++) {
			glVertex2i(winPoints[i].x, winPoints[i].y);
		}
		glEnd();
		break;

	case 2:
		glBegin(GL_LINE_STRIP);
		for (int i = 0; i < winPoints.size(); i++) {
			glVertex2i(winPoints[i].x, winPoints[i].y);
		}
		glEnd();
		break;

	case 3:
		glBegin(GL_LINE_LOOP);
		for (int i = 0; i < winPoints.size(); i++) {
			glVertex2i(winPoints[i].x, winPoints[i].y);
		}
		glEnd();
		/*glBegin(GL_POINTS);
		for (int i = 0; i < winPoints.size(); i++) {
			glVertex2i(winPoints[i].x, winPoints[i].y);
		}
		glEnd();*/
		break;

	default:
		break;
	}

	if (fillPoly) {
		if (polyPoints.size() >= 4)
			scanfill(polyPoints);
			//scanfill(polyPoints[0].x, polyPoints[0].y, polyPoints[1].x, polyPoints[1].y, polyPoints[2].x, polyPoints[2].y, polyPoints[3].x, polyPoints[3].y, polyPoints[4].x, polyPoints[4].y);
	}

	glFlush();
}

void mouse(int button, int state, int x, int y)
{
	// Save the left button state
	if (button == GLUT_LEFT_BUTTON && state == GLUT_UP)
	{
		mousex = x;
		mousey = y;
		std::cout << x << " " << y << "\n";
		Point tmpPoint;
		tmpPoint.x = mousex;
		tmpPoint.y = mousey;
		if(mode == 1)
			polyPoints.push_back(tmpPoint);
		else
		{
			winPoints.push_back(tmpPoint);
		}

		std::cout << polyPoints.size();
	}
}

void keyboard(unsigned char key, int xmouse, int ymouse)
{
	switch (key) {
	case 'w':
		glClearColor(0.0f, 0.5f, 0.25f, 1.0f);
		std::cout << "clear";
		break;

	case 'r':
		glClearColor(0.6f, 0.6f, 0.6f, 1.0f);
		std::cout << "clear";
		break;

	case 27: // Escape key
		glutDestroyWindow(Win);
		exit(0);
		break;

	default:
		break;
	}
	glutPostRedisplay();
}

void processMenuEvents(int option) {

	switch (option) {
	case 0:
		glutDestroyWindow(Win);
		exit(0);
		break;
	case 2:
		fillPoly = !fillPoly;
		break;
	}
}

void colors_menu(int option) {
	if (mode == 1) {
		polyColor = option;
	}
	else
	{
		winColor = option;
	}
	glutPostRedisplay();
}

void polyCut_menu(int option) {

	switch (option) {
	case 1:
		indexPolyMode = 1;
		break;
	case 2:
		indexPolyMode = 2;
		break;
	case 3:
		indexPolyMode = 3;
		break;
	}
	polyPoints.clear();
	mode = 1;
	glutPostRedisplay();
}

void winLay_menu(int option) {

	switch (option) {
	case 1:
		indexWinMode = 1;
		break;
	case 2:
		indexWinMode = 2;
		break;
	case 3:
		indexWinMode = 3;
		break;
	}
	winPoints.clear();
	mode = 2;
	glutPostRedisplay();
}

void option_menu(int option) {

	switch (option) {
	case 1:
		polyPoints.clear();
		winPoints.clear();
		indexPolyMode = 0;
		indexWinMode = 0;
		break;
	case 2:
		indexPolyMode = 2;
		break;
	case 3:
		indexPolyMode = 3;
		break;
	}
	glutPostRedisplay();
}

void initMenu() {

	int menu;

	menu = glutCreateMenu(processMenuEvents);

	GLint colorsMenu, polyCutMenu, winLayMenu, winWingMenu, fillMenu, optionMenu;

	colorsMenu = glutCreateMenu(colors_menu);
	glutAddMenuEntry("Red", 1);
	glutAddMenuEntry("Blue", 2);
	glutAddMenuEntry("Green", 3);

	polyCutMenu = glutCreateMenu(polyCut_menu);
	glutAddMenuEntry("Point", 1);
	glutAddMenuEntry("Line", 2);
	glutAddMenuEntry("Polygon", 3);

	winLayMenu = glutCreateMenu(winLay_menu);
	glutAddMenuEntry("Point", 1);
	glutAddMenuEntry("Line", 2);
	glutAddMenuEntry("Polygon", 3);

	optionMenu = glutCreateMenu(option_menu);
	glutAddMenuEntry("Clear", 1);
	glutAddMenuEntry("Credits", 2);

	glutCreateMenu(processMenuEvents);
	glutAddSubMenu("Colors", colorsMenu);
	glutAddSubMenu("Polygon Cutting", polyCutMenu);
	glutAddSubMenu("Window Layout", winLayMenu);
	glutAddMenuEntry("Windowing", 1);
	glutAddMenuEntry("Filling", 2);
	glutAddSubMenu("Options", optionMenu);
	glutAddMenuEntry("Exit", 0);

	glutAttachMenu(GLUT_RIGHT_BUTTON);
}

void _CreateWindow() {
	Win = glutCreateWindow("Window");
}