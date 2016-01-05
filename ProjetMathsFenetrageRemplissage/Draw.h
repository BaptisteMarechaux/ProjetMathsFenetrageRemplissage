#pragma once

void Render();

void initMenu();

void Initialize();

void mouse(int button, int state, int x, int y);

void keyboard(unsigned char key, int xmouse, int ymouse);

void _CreateWindow();

void drawCircle(GLfloat x, GLfloat y, GLfloat radius);
