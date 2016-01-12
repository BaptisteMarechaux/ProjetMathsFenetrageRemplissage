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

// Enregistre les coordonn�es de la souris
GLint mousex;
GLint mousey;

// Enregistre la taille de la fen�tre
GLint Win;

// Index du mode pour le tra�age des formes
int indexPolyMode=3;
// Index du mode g�n�ral
int mode; // 1 = Poly
bool modifying = false;
int indexOfModifyingPoly = 0;
int indexOfModifyingPoint = 0;
int modifyingMode = 0;
// Switch pour savoir si on coupe ou non et si on rempli ou non
bool fillPoly = false;
bool clipPoly = false;
bool fillingPart = false;

bool leftButtonPressed = false;

// Tableaux de points pour les formes � l'�cran
vector<PointArray> polys;
int currentPoly = 0;
PointArray win;
vector<PointArray> clippedPoly;

//Couleur selectionn�e pour les formes
vector<int>polyColor = { 2 };
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

// On cr�e un objet cercle
Circle circle;

int drawCircleMode = 0;

// Dimmension du rep�re de la fen�tre
int hmax = 800, vmax = 600, hmin=0, vmin=0;

// D�finition de la couleur de remplissager
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

	PointArray firstPoly;
	polys.push_back(firstPoly);
}

// Rendu des formes dans la fen�tre
void Render()
{
	// Nettoyage de la fen�tre
	glClear(GL_COLOR_BUFFER_BIT);

	// On trace le poly en fonction du mode de tra�age choisi
	if (!clipPoly) {
		switch (indexPolyMode) {
		case 1:
			for (int p = 0; p < polys.size(); p++)
			{
				// On choisi la couleur du poly
				switch (polyColor[p]) {
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
				glBegin(GL_POINTS);
				for (int i = 0; i < polys[p].points.size(); i++) {
					glVertex2i(polys[p].points[i].x, polys[p].points[i].y);
				}
				glEnd();
			}
			break;
		case 2:
			for (int p = 0; p < polys.size(); p++)
			{
				// On choisi la couleur du poly
				switch (polyColor[p]) {
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
				glBegin(GL_LINE_STRIP);
				for (int i = 0; i < polys[p].points.size(); i++) {
					glVertex2i(polys[p].points[i].x, polys[p].points[i].y);
				}
				glEnd();
			}
			glEnd();
			break;
		case 3:
			for (int p = 0; p < polys.size(); p++)
			{
				// On choisi la couleur du poly
				switch (polyColor[p]) {
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
				glBegin(GL_LINE_LOOP);
				for (int i = 0; i < polys[p].points.size(); i++) {
					glVertex2i(polys[p].points[i].x, polys[p].points[i].y);
				}
				glEnd();
			}
			break;

		default:
			break;
		}
	}

	// On choisi la couleur de la fen�tre
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

	// On trace la fen�tre
	glBegin(GL_LINE_LOOP);
	for (int i = 0; i < win.points.size(); i++) {
		glVertex2i(win.points[i].x, win.points[i].y);
	}
	glEnd();

	//Remplissage du polygon dessin�
	if (!clipPoly) {
		if (fillPoly) {
			for (int p = 0; p < polys.size(); p++)
			{
				if (polys[p].points.size() >= 3) {
					switch (polyColor[p]) {
					case 1:
						Fill(polys[p], redColor);
						break;
					case 2:
						Fill(polys[p], blueColor);
						break;
					case 3:
						Fill(polys[p], greenColor);
						break;
					case 4:
						Fill(polys[p], blackColor);
						break;
					case 5:
						Fill(polys[p], purpleColor);
					}
				}
			}
		}
	}

	clippedPoly.clear();
	for (int p = 0; p < polys.size(); p++)
	{
		if (polys[p].points.size() >= 3 && win.points.size() >= 3 && clipPoly) {
			UpdateClipping(polys[p]);
		}
	}

	if (clipPoly) {
		// Dessin du polygon clipp�
		glColor3f(1.0f, 0.0f, 1.0f);
		
		// On relie tout les points de la forme clipp�e
		for (int cp = 0; cp < clippedPoly.size(); cp++)
		{
			glBegin(GL_LINE_LOOP);
			for (int i = 0; i < clippedPoly[cp].points.size(); i++) {
				glVertex2i(clippedPoly[cp].points[i].x, clippedPoly[cp].points[i].y);//////////////////////////////////////////////
			}
			glEnd();
		}
		

		// Coloriage de la forme en fonction de la couleur choisie
		if (fillPoly) {
			for (int cp = 0; cp < clippedPoly.size(); cp++)
			{
				if (clippedPoly[cp].points.size() >= 3) {
					switch (polyColor[cp]) {
					case 1:
						Fill(clippedPoly[cp], redColor);
						break;
					case 2:
						Fill(clippedPoly[cp], blueColor);
						break;
					case 3:
						Fill(clippedPoly[cp], greenColor);
						break;
					case 4:
						Fill(clippedPoly[cp], blackColor);
						break;
					case 5:
						Fill(clippedPoly[cp], purpleColor);
						break;
					}
				}
			}
		}
	}
	
	//FillCircle(50, 50, 50);

	//Affiche les cr�dits
	if (showCredits) {
		glColor3f(0.25, 0.25, 0.25);
		// Donne la position du texte, le texte et les caract�ristiques de sa police
		drawText(10, 30, "Baptiste Marechaux & Julien Geiger", GLUT_BITMAP_9_BY_15);
	}

	glFlush();
}

// Fonction d'evenements � la souris
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
			polys[currentPoly].points.push_back(tmpPoint);
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
				circle.radius = sqrt((mousex - circle.x)*(mousex - circle.x) 
					+ (mousey - circle.y)*(mousey - circle.y));
				drawCircleMode++;

				std::cout << "ok";
				DrawCircle(circle.x, circle.y, circle.radius, 40);
				drawCircleMode = 0;
			}
		}	

		//std::cout << poly.points.size();

		leftButtonPressed = false;
	}

	if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN) {
		indexOfModifyingPoly = 0;
		indexOfModifyingPoint = 0;
		modifyingMode = 0;

		for (int p = 0; p < polys.size(); p++)
		{
			for (unsigned int i = 0; i < polys[p].points.size(); i++) {
				if (sqrt((x - polys[p].points[i].x)*(x - polys[p].points[i].x) + (y - polys[p].points[i].y)*(y - polys[p].points[i].y)) < 15) {
					indexOfModifyingPoly = p;
					indexOfModifyingPoint = i;
					modifyingMode = 1;
					leftButtonPressed = true;
					break;
				}
			}
			if (indexOfModifyingPoly != 0)
				break;
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
			polys[indexOfModifyingPoly].points[indexOfModifyingPoint].x = x;
			polys[indexOfModifyingPoly].points[indexOfModifyingPoint].y = y;
		}
		else if (modifyingMode == 2) {
			win.points[indexOfModifyingPoint].x = x;
			win.points[indexOfModifyingPoint].y = y;
		}
	}
	glutPostRedisplay();
}

void UpdateClipping(PointArray poly) {
	std::vector<glm::vec2> s = std::vector<glm::vec2>();
	std::vector<glm::vec2> f = std::vector<glm::vec2>();
	auto r = std::vector<std::vector<glm::vec2>>();

	for (unsigned int i = 0; i < poly.points.size(); i++) {
		s.push_back(glm::vec2(poly.points[i].x, poly.points[i].y)); //filling shape array
	}
	for (unsigned int i = 0; i < win.points.size(); i++) {
		f.push_back(glm::vec2(win.points[i].x, win.points[i].y)); //filling windowarray
	}

	r = maskInWindow(s, f);

	for (int j = 0; j < r.size(); j++) {
		_Point tmpPoint;

		PointArray tmpPointArray;
		tmpPointArray.points = std::vector<_Point>();
		for (int i = 0; i < r[j].size(); i++) {
			tmpPoint.x = r[j][i].x;
			tmpPoint.y = r[j][i].y;
			tmpPointArray.points.push_back(tmpPoint);
			//glVertex2i(s[i].x, s[i].y);
			
		}
		clippedPoly.push_back(tmpPointArray);
	}
	for (int i = 0; i < r.size(); i++) {
		
	}
}

// Proc�d�s appel�s au clavier
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
	// On zoom avec le + du pav� num�rique
	case '+':
		hmax -= 100;
		vmax -= 100;
		hmin += 100;
		vmin += 100;
		// R��criture de la matrice de projection (camera, != model view (transformation des objets)
		glMatrixMode(GL_PROJECTION);
		// Remplace la matrice actuelle par la matrice identit� (1 0 0 0 0 1 0 0 0 0 1 0 0 0 0 1)
		glLoadIdentity();
		// D�fini les coordon�es des points gauche, droite, bas, haut
		gluOrtho2D(hmin, (GLdouble)hmax,
			(GLdouble)vmax, vmin);
		break;
	// On d�-zoom avec le - du pav� num�rique
	case '-':
		hmax += 100;
		vmax += 100;
		hmin -= 100;
		vmin -= 100;
		// R��criture de la matrice de projection (camera, != model view (transformation des objets)
		glMatrixMode(GL_PROJECTION);
		// Remplace la matrice actuelle par la matrice identit� (1 0 0 0 0 1 0 0 0 0 1 0 0 0 0 1)
		glLoadIdentity();
		// D�fini les coordon�es des points gauche, droite, bas, haut
		gluOrtho2D(hmin, (GLdouble)hmax,
			(GLdouble)vmax, vmin);
		break;

	default:
		break;
	}
	// Demande de le redessin de la fen�tre
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
		win.points.clear();
		mode = 2;
		glutPostRedisplay();
		clippedPoly.clear();
		break;
	case 2:
		clipPoly = !clipPoly;
		break;

	// Permet de choisir entre remplir ou non la forme
	case 3:
		fillPoly = !fillPoly;
		break;
	}
	// Demande de le redessin de la fen�tre
	glutPostRedisplay();
}

// Fonction du menu qui permet de changer la couleur de la forme dessin�e
void colors_menu(int option) {
	if (mode == 2) {
		winColor = option;
	}
	else
	{
		if (polyColor.size() != 0) {
			polyColor[currentPoly] = option;
		}
		else
		{
			polyColor.push_back(option);
		}
	}
	// Demande de le redessin de la fen�tre
	glutPostRedisplay();
}

// Fonction du menu qui permet de changer la nature de la forme dessin�e
void polyCut_menu(int option) {

	switch (option) {
	case 1:
		drawCircleMode = 0;
		break;
	case 2:
		drawCircleMode = 1;
		break;
	}
	if (drawCircleMode == 1)
		mode = 0;
	else
		mode = 1;
	clippedPoly.clear();

	if (polys.size() != 0) {
		if (polys[currentPoly].points.size() > 3) {
			PointArray tmpPoly;
			polys.push_back(tmpPoly);
			polyColor.push_back(2);
			currentPoly++;
		}
		else
		{
			polys[currentPoly].points.clear();
		}
	}
	
	//std::cout << polys.size();

	glutPostRedisplay();
}

void render_menu(int option) {
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

	glutPostRedisplay();
}

// Menu doption suppl�mentaires
void option_menu(int option) {
	switch (option) {
	// Nettoie la fen�tre et les structures
	case 1:
		modifying = !modifying;
		mode = 0;
		break;
	case 2:
		fillingPart = !fillingPart;
		mode = 0;
		break;
	case 3:
		polys.clear();
		Initialize();
		currentPoly = 0;
		win.points.clear();
		clippedPoly.clear();
		indexPolyMode = 0;
		drawCircleMode = 0;
		mode = 0;
		clipPoly = false;
		fillPoly = false;
		break;
	// Affiche les cr�dits
	case 4:
		showCredits = !showCredits;
		break;
	}
	glutPostRedisplay();
}

// Initialise le menu
void initMenu() {

	// Index du menu
	int menu;
	// Cr�ation du menu
	menu = glutCreateMenu(processMenuEvents);

	// Liste des index des sous menus
	GLint colorsMenu, polyCutMenu, winLayMenu, winWingMenu, fillMenu, renderMenu, optionMenu;

	// Menu pour changer la couleur
	colorsMenu = glutCreateMenu(colors_menu);
	// Liste des couleurs disponibles
	glutAddMenuEntry("Rouge", 1);
	glutAddMenuEntry("Bleu", 2);
	glutAddMenuEntry("Vert", 3);
	glutAddMenuEntry("Noir", 4);
	glutAddMenuEntry("Violet", 5);

	polyCutMenu = glutCreateMenu(polyCut_menu);
	glutAddMenuEntry("Polygon", 1);
	glutAddMenuEntry("Cercle", 2);

	renderMenu = glutCreateMenu(render_menu);
	glutAddMenuEntry("Point", 1);
	glutAddMenuEntry("Ligne", 2);
	glutAddMenuEntry("Polygone", 3);

	optionMenu = glutCreateMenu(option_menu);
	glutAddMenuEntry("Modifier", 1);
	glutAddMenuEntry("Remplir une partie", 2);
	glutAddMenuEntry("Remise � zero", 3);
	glutAddMenuEntry("Credits", 4);

	glutCreateMenu(processMenuEvents);
	glutAddSubMenu("Colours", colorsMenu);
	glutAddSubMenu("Cr�er une forme", polyCutMenu);
	glutAddMenuEntry("Dessiner une fen�tre", 1);
	glutAddMenuEntry("D�couper les formes", 2);
	glutAddMenuEntry("Remplir les formes", 3);
	glutAddSubMenu("Type de rendu", renderMenu);
	glutAddSubMenu("Options", optionMenu);
	glutAddMenuEntry("Quitter", 0);

	glutAttachMenu(GLUT_RIGHT_BUTTON);
}

void _CreateWindow() {
	Win = glutCreateWindow("Window");
}

// Fonction qui permet d'afficher du texte
void drawText(int x, int y, char *string, void *font)
{
	// Positionne le premier caract�re de la cha�ne
	glRasterPos2f(x, y);
	// Calcule la longueur de la cha�ne de caract�res
	int len = (int)strlen(string);
	// Affiche tout les caract�res en fonctin de la police
	for (int i = 0; i < len; i++) {
		glutBitmapCharacter(font, string[i]);
	}
}

void DrawCircle(float cx, float cy, float r, int numberOfSegments)
{
	// Angle entre deux points (2pi/x)
	float theta = 2 * 3.1415926 / float(numberOfSegments);
	// Pr�calcule les sinus et cosinus
	float c = cosf(theta);
	float s = sinf(theta);
	float tmpx;

	//Angle 0
	float x = r;
	float y = 0;

	// Les points seront reli�s par une ligne
	glBegin(GL_LINE_LOOP);
	// Nettoyage de la liste
	polys[currentPoly].points = std::vector<_Point>();
	// Placement d'autant de points que de segments demand�s
	for (int i = 0; i < numberOfSegments; i++)
	{
		// Cr�ation d'un nouveau point
		_Point tempP = _Point();
		// Placement du point sur le cercle et en fonction du centre de ce cercle
		tempP.x = x + cx;
		tempP.y = y + cy;
		 // Insertion du point dessin� dans dans la liste
		polys[currentPoly].points.push_back(tempP);

		//Modification des coordon�es
		// Sauvergarde du x
		tmpx = x;
		// x se d�place de droite � gauche (puis inverse)
		x = c * x - s * y;
		// y se d�place de haut en base (puis inverse)
		y = s * tmpx + c * y;
	}
	glEnd();
}
