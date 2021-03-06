#include "Filling.h"

#include <list>
#include <iostream>

///////Structures 
class Node
{
public:
	int ymax;
	float xmin, invertCoef;
	// Constructeur
	Node() : ymax(-1), xmin(0.0), invertCoef(0.0) { };
};

class SI
{
public:
	// Contruit la structure
	void buildSI(PointArray &);
	// Cherche le point suivant
	int yNext(int, vector<_Point>);
	// Enregistre les cot�s
	void recordOfSide(_Point, _Point, int);

	vector<list<Node> > SIList; // tableau de list de noeuds, soit la structure principale de SI
};


////////////M�thodes pour le SI
// Insertion avec classement de m�me ordre pour le SI et LCA
void insertInTheList(list<Node>& orderedList, const Node& item)
{
	// On prend le premier �l�ment
	list<Node>::iterator currentNode = orderedList.begin();
	// Tant qu'on a un y plus petit
	while ((currentNode != orderedList.end()) && ((*currentNode).xmin < item.xmin)) {
		//On va chercher un noeud suivant
		currentNode++;
	}
	// On peut placer notre noeud en bout de chaine
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

// Enregistre les donn�es sur les cot�s en cr�ant les nodes
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

// Construction de la structure intermediaire
void SI::buildSI(PointArray& Poly) //const
{
	_Point v1, v2;
	int i, yPrev;

	yPrev = Poly.points[Poly.points.size() - 2].y;
	v1.x = Poly.points[Poly.points.size() - 1].x; // x du dernier point
	v1.y = Poly.points[Poly.points.size() - 1].y; // y du dernier point

	// On compare chaque point chacun des autres. Lors de la premi�re boucle, on compare le premier et le dernier.
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
void buildLCA(list<Node> &LCA, list<Node> Element)
{
	// On d�fini un iterator
	list<Node>::iterator iter;
	iter = Element.begin();
	iter++;

	// Tant qu'on est pas au bout de la sous list du SI
	while (iter != Element.end())
	{
		// On ajoute tous les �l�ments d'un y du SI dans le LCA
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
		// On r�cup�re les x
		x1 = (int)(*iter1).xmin; // xmin qui a evolu� avec le coef
		x2 = (int)(*iter2).xmin;

		// On trace d'un point � un autre (de gauche � droite)
		glBegin(GL_LINES);
		glVertex2i(x1, y); // y est le y en cours
		glVertex2i(x2, y);
		glEnd();

		// On traite les points deux � deux donc on incr�mente de deux points ajdacents
		iter1++;
		iter1++;
	}
}

// Mise � jour du LCA
void outLCA(int y, list<Node>& L)
{
	list<Node>::iterator iter = L.begin();
	while (iter != L.end()) {
		// On retire les points pour lesquel ont a d�pass� le y
		if (y >= (*iter).ymax)
			L.erase(iter++);
		else
		{
			// On incr�mente le x (y min) avec le coef calcul� (pour les entre deux points)
			(*iter).xmin += (*iter).invertCoef;
			iter++;
		}
	}
}

// R�ordonne les noeuds
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
void Fill(PointArray P, color c)
{
	// D�finission des listes
	SI _SI;
	list<Node> LCA;

	// Pour la cr�ation d'un tableau (SIList) vide.
	Node EmptyNode;
	list<Node> EmptyList;

	EmptyList.push_front(EmptyNode); // initialisation � vide

	for (int i = 0; i < 600; i++) // Trouver ici le y max
		_SI.SIList.push_back(EmptyList); // Autant de liste de noeud vide que de y

	// Construit la SI
	_SI.buildSI(P);

	// On d�fini la couleur de dessin
	glColor3fv(c);

	for (int yOfLine = 0; yOfLine < 600; yOfLine++)
	{
		// Cr�ation d'un nouveau LCA en fonction du Y en cours
		buildLCA(LCA, _SI.SIList[yOfLine]);
		if (!LCA.empty())
		{
			// Dessin du remplissage
			drawLine(yOfLine, LCA);
			// Mise � jour du LCA
			outLCA(yOfLine, LCA);
			// On reordonne le LCA
			resortLCA(LCA);
		}
	}
}


// Autre version du dessin du cercle
# define PI 3.14159265358979323846
void FillCircle(GLfloat centerX, GLfloat centerY, GLfloat radius)
{
	int i;
	// De m�me que pour le dessin du cercle, on calcule la pr�cision de l'angle entre les points
	int iterations = 1000;
	GLfloat twoPi = 2.0f * PI;
	GLfloat theta = twoPi / iterations;

	// On r�gle l'�paisseur des traits
	glEnable(GL_LINE_SMOOTH);
	glLineWidth(5.0);

	glBegin(GL_LINES);
	for (i = 0; i <= iterations; i++)
	{
		// En partant du centre du cercle
		glVertex2f(centerX, centerY);
		// Et en arrivant sur un point du cercle
		glVertex2f(centerX + (radius * cos(i * theta)), centerY + (radius * sin(i * theta)));
	}
	glEnd();

	// On d�sactive l'augmentation de l'�paisseur des lignes
	glDisable(GL_LINE_SMOOTH);
}


