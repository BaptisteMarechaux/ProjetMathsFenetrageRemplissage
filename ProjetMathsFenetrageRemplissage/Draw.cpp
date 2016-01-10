// C, standard, dio
#pragma once

#include <cstdio>
#include <cstdint>

#include "GL/freeglut.h"

#include <iostream>

#include "Draw.h"
#include "Filling.h"

#include "libs\glm\glm.hpp"
#include "Clipping.h"

#include <vector>

GLint mousex;
GLint mousey;

GLint Win;

#include <vector>
using std::vector;

int indexPolyMode=0;
int indexWinMode=0;
int mode; // 1 = Poly
bool fillPoly = false;

bool clipPoly = false;

/*struct _Point
{
	GLint x, y;
};

class PointArray
{
public:
	vector<_Point> points;
};*/

PointArray poly;
PointArray win;
PointArray clippedPoly;
PointArray circlePoints;

int polyColor = 0;
int winColor = 0;

float x1, x2, x3, x4, y5, y2, y3, y4;

bool pointSelected = false;


struct Circle
{
	GLint x, y;
	GLfloat radius;
};

Circle circle;

int drawCircleMode = 0;

void Initialize() 
{
	glClearColor(1.0, 0.984, 0.906, 0.961);
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

void DrawCircle(float cx, float cy, float r, int num_segments)
{

	float theta = 2 * 3.1415926 / float(num_segments);
	float c = cosf(theta);//precalculate the sine and cosine
	float s = sinf(theta);
	float t;

	float x = r;//we start at angle = 0 
	float y = 0;

	glBegin(GL_LINE_LOOP);
	poly.points = std::vector<_Point>();
	for (int ii = 0; ii < num_segments; ii++)
	{
		_Point tempP = _Point();
		tempP.x = x + cx;
		tempP.y = y + cy;
		poly.points.push_back(tempP);
		glVertex2f(x + cx, y + cy);//output vertex 

								   //apply the rotation matrix
		t = x;
		x = c * x - s * y;
		y = s * t + c * y;
	}
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

void scanfill(vector<_Point> pois)
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
			for (int i = 0; i < poly.points.size(); i++) {
				glVertex2i(poly.points[i].x, poly.points[i].y);
			}
			glEnd();
			break;

		case 2:
			glBegin(GL_LINE_STRIP);
			for (int i = 0; i < poly.points.size(); i++) {
				glVertex2i(poly.points[i].x, poly.points[i].y);
			}
			glEnd();
			break;

		case 3:
			glBegin(GL_LINE_LOOP);
			for (int i = 0; i < poly.points.size(); i++) {
				glVertex2i(poly.points[i].x, poly.points[i].y);
			}
			glEnd();
			/*glBegin(GL_POINTS);
			for (int i = 0; i < poly.points.size(); i++) {
				glVertex2i(poly.points[i].x, poly.points[i].y);
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
		for (int i = 0; i < win.points.size(); i++) {
			glVertex2i(win.points[i].x, win.points[i].y);
		}
		glEnd();
		break;

	case 2:
		glBegin(GL_LINE_STRIP);
		for (int i = 0; i < win.points.size(); i++) {
			glVertex2i(win.points[i].x, win.points[i].y);
		}
		glEnd();
		break;

	case 3:
		glBegin(GL_LINE_LOOP);
		for (int i = 0; i < win.points.size(); i++) {
			glVertex2i(win.points[i].x, win.points[i].y);
		}
		glEnd();
		/*glBegin(GL_POINTS);
		for (int i = 0; i < win.points.size(); i++) {
			glVertex2i(win.points[i].x, win.points[i].y);
		}
		glEnd();*/
		break;

	default:
		break;
	}

	if (fillPoly) {
		if (poly.points.size() >= 3) {
			colorType fillC = { 1.0f,0.0f,0.0f };
			Fill(poly, fillC);
			//scanfill(poly.points);
		}
	}

	if (drawCircleMode == 3) {
		DrawCircle(circle.x, circle.y, circle.radius, 40);
		if (fillPoly) {
			glColor4f(1.0, 1.0, 0.0, 1.0);
			FillCircle(circle.x, circle.y, circle.radius);
		}	
	}

	if (clipPoly) {

		glColor3f(1.0f, 0.0f, 1.0f);
		glBegin(GL_LINE_STRIP);
		for (int i = 0; i < clippedPoly.points.size(); i++) {
			glVertex2i(clippedPoly.points[i].x, clippedPoly.points[i].y);
		}
		glEnd();

		if (fillPoly) {
			if (clippedPoly.points.size() >= 3) {
				colorType fillC = { 1.0f,0.0f,1.0f };
				Fill(clippedPoly, fillC);
				//scanfill(poly.points);
			}
		}
	}

	
	//FillCircle(50, 50, 50);

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
		_Point tmpPoint;
		tmpPoint.x = mousex;
		tmpPoint.y = mousey;
		if(mode == 1)
			poly.points.push_back(tmpPoint);
		else
		{
			win.points.push_back(tmpPoint);
		}
		if (drawCircleMode == 1) {
			circle.x = mousex;
			circle.y = mousey;
			drawCircleMode++;
		}
		else
		{
			if(drawCircleMode == 2){
				circle.radius = sqrt((mousex - circle.x)*(mousex - circle.x) + (mousey - circle.y)*(mousey - circle.y));
				drawCircleMode++;
				std::cout << "ok";
			}
		}

		std::cout << poly.points.size();
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
	std::vector<glm::vec2> s = std::vector<glm::vec2>();
	std::vector<glm::vec2> f = std::vector<glm::vec2>();
	switch (option) {
	case 0:
		glutDestroyWindow(Win);
		exit(0);
		break;

	case 1:
		
		for (unsigned int i = 0; i < poly.points.size(); i++) {
			s.push_back(glm::vec2(poly.points[i].x, poly.points[i].y)); //filling shape array
		}
		for (unsigned int i = 0; i < win.points.size(); i++) {
			f.push_back(glm::vec2(win.points[i].x, win.points[i].y)); //filling windowarray
		}
		
		s = maskInWindow(s, f);

		_Point tmpPoint;

		clippedPoly.points = std::vector<_Point>();

		for (int i = 0; i < s.size(); i++) {
			
			tmpPoint.x = s[i].x;
			tmpPoint.y = s[i].y;
			clippedPoly.points.push_back(tmpPoint);
			//glVertex2i(s[i].x, s[i].y);
		}

		clipPoly = !clipPoly;

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
	case 4:
		drawCircleMode = 1;
		break;
	}
	poly.points.clear();
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
	win.points.clear();
	mode = 2;
	glutPostRedisplay();
}

void option_menu(int option) {

	switch (option) {
	case 1:
		poly.points.clear();
		win.points.clear();
		clippedPoly.points.clear();
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
	glutAddMenuEntry("Circle", 4);

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

