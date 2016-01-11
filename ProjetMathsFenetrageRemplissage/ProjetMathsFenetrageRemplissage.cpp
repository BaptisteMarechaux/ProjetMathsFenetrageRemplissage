// ProjetMathsFenetrageRemplissage.cpp : Defines the entry point for the console application.
//
#pragma once

#include "stdafx.h"

#include <iostream>
#include <vector>

#include "libs\glew\include\GL\glew.h"
#include "libs\freeglut\include\GL\freeglut.h"

#include "common\EsgiShader.h"
#include <math.h>
#include "libs\glm\glm.hpp"

#include "Point.h"
#include "Shape.h"
#include "Clipping.h"
#include "Filling.h"
#include "Draw.h"


int main(int argc, char* argv[])
{
	int j(0);
	std::vector<glm::vec2> s = std::vector<glm::vec2>();
	std::vector<glm::vec2> f = std::vector<glm::vec2>();

	s.push_back(glm::vec2(0, 4));
	s.push_back(glm::vec2(0, 7));
	s.push_back(glm::vec2(5, 7));
	s.push_back(glm::vec2(5, 4));

	f.push_back(glm::vec2(1, 1));
	f.push_back(glm::vec2(1, 5));
	f.push_back(glm::vec2(4, 5));
	f.push_back(glm::vec2(4, 1));

	s = maskInWindow(s, f);

	//auto t = intersection(glm::vec2(0, 0), glm::vec2(3, 0), glm::vec2(1, -1), glm::vec2(1, 1));

	//On est censé obtenir le point d'intersection suivant : (1, 0)


	//std::cin >> j;

	//glutInitDisplayindexPolyMode(GLUT_RGBA | GLUT_DOUBLE);

	// Fonction appelée quand aucun n'evenement particulier ne se produit
	//glutIdleFunc(Update);

	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
	glutInitWindowSize(800, 600);
	glutInitWindowPosition(100, 100);
	_CreateWindow();


	glutDisplayFunc(Render);
	// glutReshapeFunc(myReshape);
	initMenu();
	glutMouseFunc(mouse);
	glutMotionFunc(mouseMotion);
	glutKeyboardFunc(keyboard);

	Initialize();

	glutMainLoop();

	return 0;
}


