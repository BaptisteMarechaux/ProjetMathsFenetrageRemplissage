#include "Filling.h"

#include <list>
#include <iostream>

///////Structures 
class Node
{
public:
	int ymax;
	float xmin, invertCoef;
	Node() : ymax(-1), xmin(0.0), invertCoef(0.0) { };
};

class SI
{
public:
	void buildSI(PointArray &);
	int yNext(int, vector<_Point>);
	void recordOfSide(_Point, _Point, int);

	vector<list<Node> > SIList; // tableau de list de noeuds, soit la structure principale de SI
};


////////////Méthodes pour le SI
// Insertion avec classement de même ordre pour le SI et LCA
void insertInTheList(list<Node>& orderedList, const Node& item)
{
	list<Node>::iterator currentNode = orderedList.begin();
	while ((currentNode != orderedList.end()) && ((*currentNode).xmin < item.xmin)) {
		currentNode++;
	}
	orderedList.insert(currentNode, item);
}

// Trouve le point suivant
int SI::yNext(int k, vector<_Point> p)
{
	int j; // Point suivant

		   // Retourne au premier point
	if ((k + 1) >(p.size() - 1))
		j = 0;
	else
		j = k + 1;
	while (p[k].y == p[j].y)
		if ((j + 1) > (p.size() - 1))
			j = 0;
		else
			j++;
	return (p[j].y);
}

// Enregistre les données sur les cotés en créant les nodes
void SI::recordOfSide(_Point minPoint, _Point maxPoint, int yComp)
{
	Node n;
	n.invertCoef = (float)(maxPoint.x - minPoint.x) / (maxPoint.y - minPoint.y);
	n.xmin = minPoint.x;

	if (maxPoint.y < yComp)
		n.ymax = maxPoint.y - 1;
	else
		n.ymax = maxPoint.y;
	insertInTheList(SIList[minPoint.y], n);
}

void SI::buildSI(PointArray& Poly) //const
{
	_Point v1, v2;
	int i, yPrev;

	yPrev = Poly.points[Poly.points.size() - 2].y;
	v1.x = Poly.points[Poly.points.size() - 1].x; // x du dernier point
	v1.y = Poly.points[Poly.points.size() - 1].y; // y du dernier point

	// On compare chaque point chacun des autres. Lors de la première boucle, on compare le premier et le dernier.
	for (i = 0; i < Poly.points.size(); i++)
	{
		v2 = Poly.points[i];
		if (v1.y != v2.y) // Si on detecte une droite horizontale, on ne l'ajoute pas
		{
			if (v1.y < v2.y)
				recordOfSide(v1, v2, yNext(i, Poly.points)); //up edge
			else
				recordOfSide(v2, v1, yPrev); // down edge
		}
		yPrev = v1.y;
		v1 = v2;
	}
}


//////////////LCA
void buildLCA(list<Node> &LCA, list<Node> ET)
{
	list<Node>::iterator iter;

	iter = ET.begin();
	iter++;
	while (iter != ET.end())
	{
		// On ajoute tous les éléments d'un y du SI dans le LCA
		insertInTheList(LCA, *iter);
		iter++;
	}
}

void drawLine(int y, list<Node> L)
{
	list<Node>::iterator iter1 = L.begin();
	int x1, x2;
	// Tant qu'il reste des noeuds
	while (iter1 != L.end())
	{
		list<Node>::iterator iter2 = iter1;
		iter2++;
		// On récupère les x
		x1 = (int)(*iter1).xmin; // xmin qui a evolué avec le coef
		x2 = (int)(*iter2).xmin;

		// On trace d'un point à un autre (de gauche à droite)
		glBegin(GL_LINES);
		glVertex2i(x1, y); // y est le y en cours
		glVertex2i(x2, y);
		glEnd();

		// On traite les points deux à deux donc on incrémente de deux points ajdacents
		iter1++;
		iter1++;
	}
}

void OutLCA(int y, list<Node>& L)
{
	list<Node>::iterator iter = L.begin();
	while (iter != L.end()) {
		// On retire les points pour lesquel ont a dépassé le y
		if (y >= (*iter).ymax)
			L.erase(iter++);
		else
		{
			// On incrémente le x (y min) avec le coef calculé (pour les entre deux points)
			(*iter).xmin += (*iter).invertCoef;
			iter++;
		}
	}
}

void resortLCA(list<Node>& L)
{
	Node n;
	list<Node> L1;
	list<Node>::iterator iter = L.begin();
	while (iter != L.end())
	{
		// Permet de retrier les noeuds
		insertInTheList(L1, *iter);
		L.erase(iter++);
	}
	L = L1;
}

//////////////////Fonction de filling
void Fill(PointArray P, colorType c)
{
	SI _SI;
	list<Node> LCA;

	// Pour la création d'un tableau (SIList) vide.
	Node EmptyNode;
	list<Node> EmptyList;

	EmptyList.push_front(EmptyNode); // initialisation à vide

	for (int i = 0; i < 600; i++) // Trouver ici le y max
		_SI.SIList.push_back(EmptyList); // Autant de liste de noeud vide que de y

	// Construit la SI
	_SI.buildSI(P);

	// On défini la couleur de dessin
	glColor3fv(c);

	for (int yOfLine = 0; yOfLine < 600; yOfLine++)
	{
		// Création d'un nouveau LCA en fonction du Y en cours
		buildLCA(LCA, _SI.SIList[yOfLine]);
		if (!LCA.empty())
		{
			drawLine(yOfLine, LCA);
			OutLCA(yOfLine, LCA);
			resortLCA(LCA);
		}
	}
}


# define PI           3.14159265358979323846
void FillCircle(GLfloat centerX, GLfloat centerY, GLfloat radius)
{
	int i;
	int triangleAmount = 1000;
	GLfloat twicePi = 2.0f * PI;

	glEnable(GL_LINE_SMOOTH);
	glLineWidth(5.0);

	glBegin(GL_LINES);
	for (i = 0; i <= triangleAmount; i++)
	{
		glVertex2f(centerX, centerY);
		glVertex2f(centerX + (radius * cos(i * twicePi / triangleAmount)), centerY + (radius * sin(i * twicePi / triangleAmount)));
	}
	glEnd();

	glDisable(GL_LINE_SMOOTH);
}


