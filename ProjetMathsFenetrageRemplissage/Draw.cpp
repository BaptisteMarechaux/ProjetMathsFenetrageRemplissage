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
int indexPolyMode=3;
// Index du mode général
int mode; // 1 = Poly
bool modifying = false;
int indexOfModifyingPoly = 0;
int indexOfModifyingWin = 0;
int indexOfModifyingPoint = 0;
int modifyingMode = 0;
// Switch pour savoir si on coupe ou non et si on rempli ou non
bool fillPoly = false;
bool clipPoly = false;
bool fillingPart = false;

bool leftButtonPressed = false;

// Tableaux de points pour les formes à l'écran
vector<PointArray> polys;
int currentPoly = 0;
vector<PointArray> wins;
int currentWin = 0;
vector<PointArray> clippedPoly;
std::vector<std::vector<glm::vec2>> triangles;

//Couleur selectionnée pour les formes
vector<int>polyColor = { 2 };
vector<int>winColor = { 1 };

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

	PointArray firstPoly;
	polys.push_back(firstPoly);
	PointArray firstWin;
	wins.push_back(firstWin);
}

// Rendu des formes dans la fenêtre
void Render()
{
	// Nettoyage de la fenêtre
	glClear(GL_COLOR_BUFFER_BIT);

	// On trace le poly en fonction du mode de traçage choisi
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

	// On trace la fenêtre
	for (int w = 0; w < wins.size(); w++) {
		// On choisi la couleur de la fenêtre
		switch (winColor[w]) {
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
		for (int p = 0; p < wins[w].points.size(); p++) {
			glVertex2i(wins[w].points[p].x, wins[w].points[p].y);
		}
		glEnd();
	}


	//Remplissage du polygon dessiné
	if (!clipPoly && !fillingPart) {
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

	if (clipPoly) {
		clippedPoly.clear();
		for (int p = 0; p < polys.size(); p++)
		{
			for (int w = 0; w < wins.size(); w++)
			{
				if (polys[p].points.size() >= 3 && wins[w].points.size() >= 3 && clipPoly) {
					UpdateClipping(polys[p], wins[w]);
				}
			}
		}

		// Dessin du polygon clippé
		glColor3f(1.0f, 0.0f, 1.0f);
		
		// On relie tout les points de la forme clippée
		for (int cp = 0; cp < clippedPoly.size(); cp++)
		{
			glBegin(GL_LINE_LOOP);
			for (int i = 0; i < clippedPoly[cp].points.size(); i++) {
				glVertex2i(clippedPoly[cp].points[i].x, clippedPoly[cp].points[i].y);//////////////////////////////////////////////
			}
			glEnd();
		}
		

		// Coloriage de la forme en fonction de la couleur choisie
		if (fillPoly && !fillingPart) {
			for (int cp = 0; cp < clippedPoly.size(); cp++)
			{
				if (clippedPoly[cp].points.size() >= 3) {
					if (cp < polyColor.size()) {
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
					else
					{
						Fill(clippedPoly[cp], blueColor);
					}
				}
			}
		}
	}
	
	// Triangulation et remplissage d'une partie du polygone
	if (fillingPart) {
		std::vector<glm::vec2> tmpPoints;
		if (!clipPoly) {
			for (int p = 0; p < polys[currentPoly].points.size(); p++)
			{
				tmpPoints.push_back(glm::vec2(polys[currentPoly].points[p].x, polys[currentPoly].points[p].y));
			}
		}
		else
		{
			if(clippedPoly.size() != 0)
			for (int p = 0; p < clippedPoly[clippedPoly.size()-1].points.size(); p++)
			{
				tmpPoints.push_back(glm::vec2(clippedPoly[clippedPoly.size() - 1].points[p].x, clippedPoly[clippedPoly.size() - 1].points[p].y));
			}
		}
		triangles = triangulation(tmpPoints);
		for (int t = 0; t < triangles.size(); t++) {
			glBegin(GL_LINE_LOOP);
			for (int i = 0; i < triangles[t].size(); i++) {
				glVertex2i(triangles[t][i].x, triangles[t][i].y);
			}
			glEnd();
		}

		for (int t = 0; t < triangles.size(); t++) {
			if (visible(glm::vec2(mousex, mousey), triangles[t][0], triangles[t][1])
				&& visible(glm::vec2(mousex, mousey), triangles[t][1], triangles[t][2])
				&& visible(glm::vec2(mousex, mousey), triangles[t][2], triangles[t][0])) {
				_Point tmpPoint;
				PointArray tmpPointArray;
				tmpPointArray.points = std::vector<_Point>();
				for (int i = 0; i < triangles[t].size(); i++) {
					tmpPoint.x = triangles[t][i].x;
					tmpPoint.y = triangles[t][i].y;
					tmpPointArray.points.push_back(tmpPoint);
				}

				switch (polyColor[currentPoly]) {
				case 1:
					Fill(tmpPointArray, redColor);
					break;
				case 2:
					Fill(tmpPointArray, blueColor);
					break;
				case 3:
					Fill(tmpPointArray, greenColor);
					break;
				case 4:
					Fill(tmpPointArray, blackColor);
					break;
				case 5:
					Fill(tmpPointArray, purpleColor);
					break;
				}
				break;
			}
		}
	}

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
			polys[currentPoly].points.push_back(tmpPoint);
		}else 
			if(mode == 2)
			{
				wins[currentWin].points.push_back(tmpPoint);
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
		indexOfModifyingWin = 0;
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
		for (int w = 0; w < wins.size(); w++)
		{
			for (unsigned int i = 0; i < wins[w].points.size(); i++) {
				if (sqrt((x - wins[w].points[i].x)*(x - wins[w].points[i].x) + (y - wins[w].points[i].y)*(y - wins[w].points[i].y)) < 15) {
					indexOfModifyingWin = w;
					indexOfModifyingPoint = i;
					modifyingMode = 2;
					leftButtonPressed = true;
					break;
				}
			}
			if (indexOfModifyingWin != 0)
				break;
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
			wins[indexOfModifyingWin].points[indexOfModifyingPoint].x = x;
			wins[indexOfModifyingWin].points[indexOfModifyingPoint].y = y;
		}
	}
	glutPostRedisplay();
}

// Fonction de mise à jour de la liste de polygons clippés
void UpdateClipping(PointArray poly, PointArray win) {
	// On défini une structure pour passer les fenêtres et une pour les polygones
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
		// Modification des coordonées du repère
		hmax -= 50;
		vmax -= 50;
		hmin += 50;
		vmin += 50;
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
		// Modification des coordonées du repère
		hmax += 50;
		vmax += 50;
		hmin -= 50;
		vmin -= 50;
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
		if (wins.size() != 0) {
			if (wins[currentWin].points.size() > 3) {
				PointArray tmpWin;
				wins.push_back(tmpWin);
				winColor.push_back(1);
				currentWin++;
			}
			else
			{
				wins[currentWin].points.clear();
			}
		}
		mode = 2;
		drawCircleMode = 0;
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
	modifying = false;

	// Demande de le redessin de la fenêtre
	glutPostRedisplay();
}

// Fonction du menu qui permet de changer la couleur de la forme dessinée
void colors_menu(int option) {
	if (mode == 2) {
		if (winColor.size() != 0) {
			winColor[currentWin] = option;
		}
		else
		{
			winColor.push_back(option);
		}
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
	// Demande de le redessin de la fenêtre
	glutPostRedisplay();
}

// Fonction du menu qui permet de changer la nature de la forme dessinée
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

	modifying = false;

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

// Menu doption supplémentaires
void option_menu(int option) {
	switch (option) {
	// Nettoie la fenêtre et les structures
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
		currentPoly = 0;
		wins.clear();
		currentWin = 0;
		Initialize();
		clippedPoly.clear();
		indexPolyMode = 3;
		drawCircleMode = 0;
		mode = 0;
		clipPoly = false;
		fillPoly = false;
		fillingPart = false;
		break;
	// Affiche les crédits
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
	// Création du menu
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
	glutAddMenuEntry("Remise à zero", 3);
	glutAddMenuEntry("Credits", 4);

	glutCreateMenu(processMenuEvents);
	glutAddSubMenu("Colours", colorsMenu);
	glutAddSubMenu("Créer une forme", polyCutMenu);
	glutAddMenuEntry("Dessiner une fenêtre", 1);
	glutAddMenuEntry("Découper les formes", 2);
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
	// Positionne le premier caractère de la chaîne
	glRasterPos2f(x, y);
	// Calcule la longueur de la chaîne de caractères
	int len = (int)strlen(string);
	// Affiche tout les caractères en fonctin de la police
	for (int i = 0; i < len; i++) {
		glutBitmapCharacter(font, string[i]);
	}
}

// Dessin du cercle
void DrawCircle(float cx, float cy, float r, int numberOfSegments)
{
	// Angle entre deux points (2pi/x)
	float theta = 2 * 3.1415926 / float(numberOfSegments);
	// On calcule les valeurs d'augmentation de x et y pour cet angle
	float c = cosf(theta);
	float s = sinf(theta);
	//Pour l'angle 0
	float x = r;
	float y = 0;

	// Les points seront reliés par une ligne
	glBegin(GL_LINE_LOOP);
	// Nettoyage de la liste
	polys[currentPoly].points = std::vector<_Point>();

	// Placement d'autant de points que de segments demandés
	for (int i = 0; i < numberOfSegments; i++)
	{
		// Création d'un nouveau point
		_Point tempP = _Point();
		// Placement du point sur le cercle et en fonction du centre du cercle
		tempP.x = x + cx;
		tempP.y = y + cy;
		 // Insertion du point dessiné dans dans la liste
		polys[currentPoly].points.push_back(tempP);

		//Modification des coordonées
		// Sauvergarde du x
		float tmpx = x;
		// x se déplace de droite à gauche (puis inverse)
		x = c * x - s * y;
		// y se déplace de haut en base (puis inverse)
		y = s * tmpx + c * y;
	}
	glEnd();
}
