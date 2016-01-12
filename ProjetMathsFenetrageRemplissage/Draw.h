#pragma once

#include "Filling.h"

void Render();

void initMenu();

void Initialize();

void mouse(int button, int state, int x, int y);

void mouseMotion(int x, int y);

void keyboard(unsigned char key, int xmouse, int ymouse);

void _CreateWindow();

void drawCircle(GLfloat x, GLfloat y, GLfloat radius);

// Fonction qui permet d'afficher du texte
void drawText(int x, int y, char *string, void *font);

void DrawCircle(float cx, float cy, float r, int numberOfSegments);

void UpdateClipping(PointArray poly, PointArray win);