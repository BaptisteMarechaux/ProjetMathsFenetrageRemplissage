#pragma once

// C, standard, dio
#include <cstdio>
// C, standard, int
#include <cstdint>
#include "GL/freeglut.h"
#include <iostream>
#include "libs\glm\glm.hpp"

// Pour les fonctions
#include "Draw.h"
#include "Clipping.h"

// Pour les tableaux
#include <vector>
using std::vector;

// Enregistre les coordonnées de la souris
GLint mousex;
GLint mousey;

// Enregistre la taille de la fenêtre
GLint Win;

// Index du mode pour le traçage des formes
int indexPolyMode=0;
int indexWinMode=0;
// Index du mode général
int mode; // 1 = Poly
bool modifying = false;
int indexOfModifyingPoint = 0;
int modifyingMode = 0;
// Switch pour savoir si on coupe ou non et si on rempli ou non
bool fillPoly = false;
bool clipPoly = false;

bool leftButtonPressed = false;

// Tableaux de points pour les formes à l'écran
PointArray poly;
PointArray win;
PointArray clippedPoly;
PointArray circlePoints;

//Couleur selectionnée pour les formes
int polyColor = 2;
int winColor = 1;

float x1, x2, x3, x4, y5, y2, y3, y4;

bool pointSelected = false;

bool showCredits;

// Structure du cercle
struct Circle
{
	GLint x, y;
	GLfloat radius;
};

// On crée un objet cercle
Circle circle;

int drawCircleMode = 0;

// Dimmension du repère de la fenêtre
int hmax = 800, vmax = 600, hmin=0, vmin=0;

// Définition de la couleur de remplissager
color redColor = { 1.0f, 0.0f, 0.0f };
color blueColor = { 0.0f, 0.0f, 1.0f };
color greenColor = { 0.0f, 1.0f, 0.0f };
color blackColor = { 0.0f, 0.0f, 0.0f };
color purpleColor = { 0.75f, 0.0f, 0.75f };

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

// Rendu des formes dans la fenêtre
void Render() 
{
	// Nettoyage de la fenêtre
	glClear(GL_COLOR_BUFFER_BIT);

	// On choisi la couleur du poly
	switch (polyColor) {
		case 1:
			glColor3fv(redColor);
			break;
		case 2:
			glColor3fv(blueColor);
			break;
		case 3:
			glColor3fv(greenColor);
			break;
		case 4:
			glColor3fv(blackColor);
			break;
		case 5:
			glColor3fv(purpleColor);
			break;
	}

	// On trace le poly en fonction du mode de traçage choisi
	if (!clipPoly) {
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
			}
			glEnd();
			*/
			break;

		default:
			break;
		}
	}

	// On choisi la couleur de la fenêtre
	switch (winColor) {
		case 1:
			glColor3fv(redColor);
			break;
		case 2:
			glColor3fv(blueColor);
			break;
		case 3:
			glColor3fv(greenColor);
			break;
		case 4:
			glColor3fv(blackColor);
			break;
		case 5:
			glColor3fv(purpleColor);
			break;
	}

	// On trace la fenêtre en fonction du mode de traçage choisi
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

	//Remplissage du polygon dessiné
	if (!clipPoly) {
		if (fillPoly) {
			if (poly.points.size() >= 3) {
				switch (polyColor) {
				case 1:
					Fill(poly, redColor);
					break;
				case 2:
					Fill(poly, blueColor);
					break;
				case 3:
					Fill(poly, greenColor);
					break;
				case 4:
					Fill(poly, blackColor);
					break;
				case 5:
					Fill(poly, purpleColor);
					break;
				}
			}
		}
	}
	
	// Si on souhaite dessiner un cercle
	if (drawCircleMode == 3) {
		 {
			 switch (polyColor) {
			 case 1:
				 DrawCircle(circle.x, circle.y, circle.radius, 40, redColor);
				 if (fillPoly)
					 Fill(poly, redColor);
				 //FillCircle(circle.x, circle.y, circle.radius);
				 break;
			 case 2:
				 DrawCircle(circle.x, circle.y, circle.radius, 40, blueColor);
				 if (fillPoly)
					 Fill(poly, blueColor);
				 break;
			 case 3:
				 DrawCircle(circle.x, circle.y, circle.radius, 40, greenColor);
				 if (fillPoly)
					Fill(poly, greenColor);
				 break;
			 case 4:
				 DrawCircle(circle.x, circle.y, circle.radius, 40, blackColor);
				 if (fillPoly)
					Fill(poly, blackColor);
				 break;
			 case 5:
				 DrawCircle(circle.x, circle.y, circle.radius, 40, purpleColor);
				 if (fillPoly)
					 Fill(poly, purpleColor);
				 break;
			 }
		}	
	}

	if (poly.points.size() >= 3 && win.points.size() >= 3 && clipPoly) {
		UpdateClipping();
	}

	if (clipPoly) {
		// Dessin du polygon clippé
		glColor3f(1.0f, 0.0f, 1.0f);
		glBegin(GL_LINE_LOOP);
<<<<<<< HEAD
=======
		// On relie tout les points de la forme clippée
>>>>>>> f0b9f3f7b7a2989b318db2ae28aba82c11441f10
		for (int i = 0; i < clippedPoly.points.size(); i++) {
			glVertex2i(clippedPoly.points[i].x, clippedPoly.points[i].y);
		}
		glEnd();

		// Coloriage de la forme en fonction de la couleur choisie
		if (fillPoly) {
			if (clippedPoly.points.size() >= 3) {
				switch (polyColor) {
					case 1:
						Fill(clippedPoly, redColor);
						break;
					case 2:
						Fill(clippedPoly, blueColor);
						break;
					case 3:
						Fill(clippedPoly, greenColor);
						break;
					case 4:
						Fill(clippedPoly, blackColor);
						break;
					case 5:
						Fill(clippedPoly, purpleColor);
						break;
				}
				
				//scanfill(poly.points);
			}
		}
	}
	
	//FillCircle(50, 50, 50);

	//Affiche les crédits
	if (showCredits) {
		glColor3f(0.25, 0.25, 0.25);
		// Donne la position du texte, le texte et les caractéristiques de sa police
		drawText(10, 30, "Baptiste Marechaux & Julien Geiger", GLUT_BITMAP_9_BY_15);
	}

	glFlush();
}

// Fonction d'evenements à la souris
void mouse(int button, int state, int x, int y)
{
	//Si on effectue un clic gauche
	if (button == GLUT_LEFT_BUTTON && state == GLUT_UP)
	{
		mousex = x;
		mousey = y;
		std::cout << x << " " << y << "\n";
		_Point tmpPoint;
		tmpPoint.x = mousex;
		tmpPoint.y = mousey;
		if (mode == 1) {
			poly.points.push_back(tmpPoint);
		}else 
			if(mode == 2)
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

		//std::cout << poly.points.size();

		leftButtonPressed = false;
	}

	if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN) {
		indexOfModifyingPoint = 0;
		modifyingMode = 0;

		for (unsigned int i = 0; i < poly.points.size(); i++) {
			if (sqrt((x - poly.points[i].x)*(x - poly.points[i].x) + (y - poly.points[i].y)*(y - poly.points[i].y)) < 15) {
				indexOfModifyingPoint = i;
				modifyingMode = 1;
				leftButtonPressed = true;
				break;
			}
		}
		for (unsigned int i = 0; i < win.points.size(); i++) {
			if (sqrt((x - win.points[i].x)*(x - win.points[i].x) + (y - win.points[i].y)*(y - win.points[i].y)) < 15) {
				indexOfModifyingPoint = i;
				modifyingMode = 2;
				leftButtonPressed = true;
				break;
			}
		}
	}

	glutPostRedisplay();
}

void mouseMotion(int x, int y) {
	if (modifying && leftButtonPressed) {
		if (modifyingMode == 1) {
			poly.points[indexOfModifyingPoint].x = x;
			poly.points[indexOfModifyingPoint].y = y;
		}
		else if (modifyingMode == 2) {
			win.points[indexOfModifyingPoint].x = x;
			win.points[indexOfModifyingPoint].y = y;
		}
	}
	glutPostRedisplay();
}

void UpdateClipping() {
	std::vector<glm::vec2> s = std::vector<glm::vec2>();
	std::vector<glm::vec2> f = std::vector<glm::vec2>();

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

}

// Procédés appelés au clavier
void keyboard(unsigned char key, int xmouse, int ymouse)
{
	// En fonction de la touche
	switch (key) {
	// On change la couleur de fond
	case 'a':
		glClearColor(0.0f, 0.5f, 0.25f, 1.0f);
		std::cout << "clear";
		break;
	// On change la couleur de fond
	case 'z':
		glClearColor(0.6f, 0.6f, 0.6f, 1.0f);
		std::cout << "clear";
		break;
	// On quitte l'application avec la touche echap
	case 27:
		glutDestroyWindow(Win);
		exit(0);
		break;
	// On zoom avec le + du pavé numérique
	case '+':
		hmax -= 100;
		vmax -= 100;
		hmin += 100;
		vmin += 100;
		// Réécriture de la matrice de projection (camera, != model view (transformation des objets)
		glMatrixMode(GL_PROJECTION);
		// Remplace la matrice actuelle par la matrice identité (1 0 0 0 0 1 0 0 0 0 1 0 0 0 0 1)
		glLoadIdentity();
		// Défini les coordonées des points gauche, droite, bas, haut
		gluOrtho2D(hmin, (GLdouble)hmax,
			(GLdouble)vmax, vmin);
		break;
	// On dé-zoom avec le - du pavé numérique
	case '-':
		hmax += 100;
		vmax += 100;
		hmin -= 100;
		vmin -= 100;
		// Réécriture de la matrice de projection (camera, != model view (transformation des objets)
		glMatrixMode(GL_PROJECTION);
		// Remplace la matrice actuelle par la matrice identité (1 0 0 0 0 1 0 0 0 0 1 0 0 0 0 1)
		glLoadIdentity();
		// Défini les coordonées des points gauche, droite, bas, haut
		gluOrtho2D(hmin, (GLdouble)hmax,
			(GLdouble)vmax, vmin);
		break;

	default:
		break;
	}
	// Demande de le redessin de la fenêtre
	glutPostRedisplay();
}

// Fonction de menu principal
void processMenuEvents(int option) {
	switch (option) {
	// Bouton qui permet de quitter l'application
	case 0:
		glutDestroyWindow(Win);
		exit(0);
		break;

	case 1:
<<<<<<< HEAD
		
		for (unsigned int i = 0; i < poly.points.size(); i++) {
			s.push_back(glm::vec2(poly.points[i].x, poly.points[i].y)); //filling shape array
		}
		for (unsigned int i = 0; i < win.points.size(); i++) {
			f.push_back(glm::vec2(win.points[i].x, win.points[i].y)); //filling windowarray
		}

		mode = 56;
		
		s = maskInWindow(s, f);

		_Point tmpPoint;

		clippedPoly.points = std::vector<_Point>();

		for (int i = 0; i < s.size(); i++) {
			
			tmpPoint.x = s[i].x;
			tmpPoint.y = s[i].y;
			clippedPoly.points.push_back(tmpPoint);
			//glVertex2i(s[i].x, s[i].y);
		}

=======
>>>>>>> f0b9f3f7b7a2989b318db2ae28aba82c11441f10
		clipPoly = !clipPoly;
		break;

	// Permet de choisir entre remplir ou non la forme
	case 2:
		fillPoly = !fillPoly;
		break;
	}
	// Demande de le redessin de la fenêtre
	glutPostRedisplay();
}

// Fonction du menu qui permet de changer la couleur de la forme dessinée
void colors_menu(int option) {
	if (mode == 2) {
		winColor = option;
	}
	else
	{
		polyColor = option;
	}
	// Demande de le redessin de la fenêtre
	glutPostRedisplay();
}

// Fonction du menu qui permet de changer la nature de la forme dessinée
void polyCut_menu(int option) {

	switch (option) {
	case 1:
		indexPolyMode = 1;
		drawCircleMode = 0;
		break;
	case 2:
		indexPolyMode = 2;
		drawCircleMode = 0;
		break;
	case 3:
		indexPolyMode = 3;
		drawCircleMode = 0;
		break;
	case 4:
		drawCircleMode = 1;
		indexPolyMode = 0;
		break;
	}
	poly.points.clear();
	mode = 1;
	glutPostRedisplay();
	clippedPoly.points.clear();
}

// Fonction du menu qui permet de changer la nature de la fenêtre dessinée
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
	clippedPoly.points.clear();
}

// Menu doption supplémentaires
void option_menu(int option) {
	switch (option) {
	// Nettoie la fenêtre et les structures
	case 1:
		modifying = !modifying;
		mode = 0;
		break;
	case 2:
		poly.points.clear();
		win.points.clear();
		clippedPoly.points.clear();
		indexPolyMode = 0;
		indexWinMode = 0;
		drawCircleMode = 0;
		mode = 0;
		clipPoly = false;
		fillPoly = false;
		break;
	// Affiche les crédits
	case 3:
		showCredits = !showCredits;
		break;
	}
	glutPostRedisplay();
}

// Initialise le menu
void initMenu() {

	// Index du menu
	int menu;
	// Création du menu
	menu = glutCreateMenu(processMenuEvents);

	// Liste des index des sous menus
	GLint colorsMenu, polyCutMenu, winLayMenu, winWingMenu, fillMenu, optionMenu;

	// Menu pour changer la couleur
	colorsMenu = glutCreateMenu(colors_menu);
	// Liste des couleurs disponibles
	glutAddMenuEntry("Rouge", 1);
	glutAddMenuEntry("Bleu", 2);
	glutAddMenuEntry("Vert", 3);
	glutAddMenuEntry("Noir", 4);
	glutAddMenuEntry("Violet", 5);

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
	glutAddMenuEntry("Modifier", 1);
	glutAddMenuEntry("Effacer", 2);
	glutAddMenuEntry("Credits", 3);

	glutCreateMenu(processMenuEvents);
	glutAddSubMenu("Colours", colorsMenu);
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

// Fonction qui permet d'afficher du texte
void drawText(int x, int y, char *string, void *font)
{
	// Positionne le premier caractère de la chaîne
	glRasterPos2f(x, y);
	// Calcule la longueur de la chaîne de caractères
	int len = (int)strlen(string);
	// Affiche tout les caractères en fonctin de la police
	for (int i = 0; i < len; i++) {
		glutBitmapCharacter(font, string[i]);
	}
}

void DrawCircle(float cx, float cy, float r, int numberOfSegments, color _color)
{
	// Angle entre deux points (2pi/x)
	float theta = 2 * 3.1415926 / float(numberOfSegments);
	// Précalcule les sinus et cosinus
	float c = cosf(theta);
	float s = sinf(theta);
	float tmpx;

	//Angle 0
	float x = r;
	float y = 0;

	// Les points seront reliés par une ligne
	glBegin(GL_LINE_LOOP);
	// Nettoyage de la liste
	poly.points = std::vector<_Point>();
	// Placement d'autant de points que de segments demandés
	for (int i = 0; i < numberOfSegments; i++)
	{
		// Création d'un nouveau point
		_Point tempP = _Point();
		// Placement du point sur le cercle et en fonction du centre de ce cercle
		tempP.x = x + cx;
		tempP.y = y + cy;
		// Dessin du point
		glColor3fv(_color);
		glVertex2f(tempP.x, tempP.y);//output vertex 
		 // Insertion du point dessiné dans dans la liste
		poly.points.push_back(tempP);

		//Modification des coordonées
		// Sauvergarde du x
		tmpx = x;
		// x se déplace de droite à gauche (puis inverse)
		x = c * x - s * y;
		// y se déplace de haut en base (puis inverse)
		y = s * tmpx + c * y;
	}
	glEnd();
}
