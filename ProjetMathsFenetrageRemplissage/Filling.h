#pragma once

#include <windows.h>
#include <GL/gl.h>
#include <GL/glut.h>
#include <vector>
using namespace std;

typedef GLfloat colorType[3];

struct _Point
{
	GLint x, y;
};

class PointArray
{
public:
	vector<_Point> points;
};

void Fill(PointArray P, colorType c);

void FillCircle(GLfloat centerX, GLfloat centerY, GLfloat radius);

