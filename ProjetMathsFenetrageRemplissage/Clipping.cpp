#pragma once

#include <vector>
#include <iostream>

#include "Clipping.h"
#include "Shape.h"
#include "libs\glm\glm.hpp"
#include "libs\glm\gtc\constants.hpp"


bool visible(const glm::vec2& checkedPoint, const glm::vec2& windowPointA, const glm::vec2& windowPointB) {
	glm::vec2 segWindow = windowPointB - windowPointA;

	glm::vec2 normal = glm::vec2(-segWindow.y, segWindow.x);

	glm::vec2 segVerif = checkedPoint - windowPointA;

	auto reo = glm::dot(normal, segVerif);

	//Calcul des normales pour savoir si on est à l'intérieur ou l'extérieur de la fenêtre
	if (glm::dot(normal, segVerif) > 0) {
		//intérieur
		return false;
	}
	else if (glm::dot(normal, segVerif) < 0) {
		//extérieur
		return true;
	}
	else if (glm::dot(normal, segVerif) == 0) {
		//sur le meme plan
		//retourner

		return true;

	}

	return false;
}

bool cuts(glm::vec2& checkedPointA, glm::vec2& checkedPointB, glm::vec2& windowPointA, glm::vec2& windowPointB) {
	//Retourne vrai si la droite prolongeant le coté de la fenêtre coupe un segment de la forme

	glm::mat2 matrixD = glm::mat2(
		checkedPointB.x - checkedPointA.x, checkedPointB.y - checkedPointA.y,
		windowPointA.x - windowPointB.x, windowPointA.y - windowPointB.y
		);

	glm::vec2 c2 = glm::vec2(
		windowPointA.x - checkedPointA.x, windowPointA.y - checkedPointA.y
		);

	float tempDeterminant = glm::determinant(matrixD);
	glm::vec2 res = glm::vec2(0, 0);

	if (tempDeterminant != 0) {

		glm::inverse(matrixD);
		res = glm::inverse(matrixD) * c2;
	}
	else {
		//erreur

		return false;
	}

	//std::cout << "t : " << res.x << " s : " << res.y << std::endl;

	if ((res.x >= 0 && res.x <= 1) || (res.y >= 0 && res.y <= 1)) {
		return true;
	}
	else {
		return false;
	}

}

bool strictCuts(glm::vec2& checkedPointA, glm::vec2& checkedPointB, glm::vec2& windowPointA, glm::vec2& windowPointB) {
	//Retourne vrai si la droite prolongeant le coté de la fenêtre coupe un segment de la forme

	glm::mat2 matrixD = glm::mat2(
		checkedPointB.x - checkedPointA.x, checkedPointB.y - checkedPointA.y,
		windowPointA.x - windowPointB.x, windowPointA.y - windowPointB.y
		);

	glm::vec2 c2 = glm::vec2(
		windowPointA.x - checkedPointA.x, windowPointA.y - checkedPointA.y
		);

	float tempDeterminant = glm::determinant(matrixD);
	glm::vec2 res = glm::vec2(0, 0);

	if (tempDeterminant != 0) {

		glm::inverse(matrixD);
		res = glm::inverse(matrixD) * c2;
	}
	else {
		//erreur

		return false;
	}

	//std::cout << "t : " << res.x << " s : " << res.y << std::endl;

	if ((res.x > 0 && res.x < 1) && (res.y > 0 && res.y < 1)) {
		return true;
	}
	else {
		return false;
	}
}

glm::vec2 intersection(const glm::vec2& checkedPointA, const glm::vec2& checkedPointB, const glm::vec2& windowPointA, const glm::vec2& windowPointB) {

	/*
	glm::mat2 matrixD = glm::mat2(
	checkedPointB.x - checkedPointA.x, windowPointA.x - windowPointB.x,
	checkedPointB.y - checkedPointA.y, windowPointA.y - windowPointB.y
	);
	*/

	glm::mat2 matrixD = glm::mat2(
		checkedPointB.x - checkedPointA.x, checkedPointB.y - checkedPointA.y,
		windowPointA.x - windowPointB.x, windowPointA.y - windowPointB.y
		);

	glm::vec2 c2 = glm::vec2(
		windowPointA.x - checkedPointA.x, windowPointA.y - checkedPointA.y
		);

	float tempDeterminant = glm::determinant(matrixD);
	glm::vec2 res = glm::vec2(0, 0);
	if (tempDeterminant != 0) {
		auto inverseMatrixD = glm::inverse(matrixD);
		res = inverseMatrixD * c2;
	}
	else {
		//erreur

		return windowPointB;
	}

	//return res;

	if ((res.x >= 0 && res.x <= 1) || (res.y >= 0 && res.y <= 1)) {
		return windowPointA + (windowPointB - windowPointA)*res.y;
	}
	else {
		return  windowPointB;
	}

}

std::vector<std::vector<glm::vec2>> maskInWindow(std::vector<glm::vec2>& s, std::vector<glm::vec2>& f) {
	std::vector<glm::vec2> outShape; //polygone retourné
	std::vector<glm::vec2> inShape; //polygone d'entrée temporaire

	glm::vec2 f1; //premier point sélectionné de la fenetre dans la boucle
	glm::vec2 f2; //point courant de la fenetre sélectionné 

	glm::vec2 sIn; //point courant de la forme que l'on veut couper
	glm::vec2 sLast; //dernier point de la forme que l'on veut couper

	//On vérifie si le polygone est concave
	//Si le polygone est concave on lance n(coté de f) - 2 fois la procédure de clipping
	if (f.size() > 3) {
		if (!isConvex(f)) {
			auto vecList = std::vector< std::vector<glm::vec2> >();
			auto winList = std::vector< std::vector<glm::vec2> >();

			//O, découpe en triangle la winList
			winList = triangulation(f);

			int k = 0;
			for (int i = 2; i < f.size() - 2; i++) {

				k++;
			}

			for (int i = 0; i < winList.size(); i++) {
				//On ajoute chacun des polygones obtenus avec tous les clipping à une liste
				if (winList.size() > 0) {

					vecList.push_back(maskInWindow(s, winList[i])[0]); 

				}

			}

			return vecList;
		}
	}

	

	std::cout << "Points de la forme coupée :" << std::endl;

	for (unsigned int i = 0; i < s.size(); i++) {
		std::cout << "X : " << s[i].x << " Y : " << s[i].y << std::endl;
	}
	std::cout << std::endl;

	outShape = s;
	f1 = f[f.size() - 1];

	for (unsigned int i = 0; i < f.size(); i++) {

		f2 = f[i];
		inShape = outShape;
		outShape = std::vector<glm::vec2>();
		if (inShape.size() == 0) {
			break;
		}
		sLast = inShape[inShape.size() - 1];

		for (unsigned int j = 0; j < inShape.size(); j++) {
			sIn = inShape[j];

			if (visible(sIn, f1, f2)) {

				if (!visible(sLast, f1, f2)) {
					if (cuts(sIn, sLast, f1, f2))
						outShape.push_back(intersection(sIn, sLast, f1, f2));
				}

				outShape.push_back(sIn);

			}
			else if (visible(sLast, f1, f2)) {
				if (cuts(sIn, sLast, f1, f2))
					outShape.push_back(intersection(sIn, sLast, f1, f2));
			}

			sLast = sIn;

		}

		f1 = f2;

	}

	std::cout << "Points de la forme obtenue :" << std::endl;
	for (unsigned int i = 0; i < outShape.size(); i++) {
		std::cout << "X : " << outShape[i].x << " Y : " << outShape[i].y << std::endl;
	}
	std::cout << std::endl;
	
	auto g = std::vector < std::vector<glm::vec2 >> ();
	g.push_back(outShape);
	return g;
}

bool isInShape(glm::vec2 p, std::vector<glm::vec2> shape) {
	for (int i = 0; i < shape.size(); i++) {
		if (p == shape[i]) {
			return true;
		}
	}

	return false;
}

/*
bool isConvex(std::vector<glm::vec2> s) {
	
	float r=0;
	float pii = 3.14159265358979323846;
	for (int i = 0; i < s.size(); i++) {
		if (i == 0) {
			r+=(2*pii) - glm::acos(glm::dot(
					s[s.size() - 1] - s[0],
					s[1] - s[0]
				) /
				(glm::distance(
					s[s.size() - 1],
					s[0]
				) * 
				glm::distance(
					s[1],
					s[0]
				))
			);
		}
		else if (i == s.size()-1) {
			r+= (2 * pii) - glm::acos(glm::dot(
					s[0] - s[s.size()-1],
					s[s.size()-2] - s[s.size()-1]
				) /
				(glm::distance(
					s[s.size() - 2],
					s[s.size() -1]
					) *
					glm::distance(
						s[0],
						s[s.size()-1]
						))
				);
		}
		else {
			r += (2 * pii) - glm::acos(glm::dot(
				s[i+1] - s[i],
				s[i] - s[i-1]
				) /
				(glm::distance(
					s[i-1],
					s[i]
					) *
					glm::distance(
						s[i+1],
						s[i]
						))
				);
		}
	}


	if (r != 4*pii ){
		std::cout << "Forme Concave" << std::endl;
		std::cout << r << std::endl;
		return false;
	}

	std::cout << "Forme Convexe" << std::endl;
	std::cout << r << std::endl;

	return true;
}

*/

bool isConvex(std::vector<glm::vec2> s) {
	bool sign = false;

	glm::vec2 v1,v2;

	if (s.size() < 4)
		return true;

	for (int i = 0; i<s.size(); i++)
	{
		if (i == 0) {
			
			v2 = s[s.size() - 1] - s[0];
			v1 = s[1] - s[0];
		}
		else if (i == s.size() - 1) {
		
			v1 = s[0] - s[s.size() - 1];
			v2 = s[s.size() - 2] - s[s.size() - 1];
	
		}
		else {
			
			v1 = s[i + 1] - s[i];
			v2 = s[i-1] - s[i];
				
		}

		glm::vec3 b1 = glm::vec3(v1.x, v1.y, 0);
		glm::vec3 b2 = glm::vec3(v2.x, v2.y, 0);
		auto cross = glm::cross(b1, b2);
		if (i == 0)
			sign = cross.z>0;
		else
		{
			if (sign != (cross.z>0))
				return false;
		}
	}
	return true;
}

float vectorAngle(glm::vec2 c, glm::vec2 p1, glm::vec2 p2) {
	return glm::acos(glm::dot(
		p1 - c,
		p2 - c
		) /
		(glm::distance(
			p1,
			c
			) *
			glm::distance(
				p2,
				c
				)
		)
	);
}

bool isInsideTriangle(glm::vec2 cur, glm::vec2 prev, glm::vec2 next, std::vector<glm::vec2> v) {
	float pii = 3.14159265358979323846;

	

	for (int j = 0; j < v.size(); j++) {
		if (visible(v[j], cur, next) && visible(v[j], next, prev) && visible(v[j], prev, cur)) {
			if (v[j] != cur && v[j] != next && v[j] != prev) {
				return true;
			}
			
		}
	}

	

	return false;
}

std::vector<std::vector<glm::vec2> > triangulation(std::vector<glm::vec2>& v) {
	glm::vec2 cur, prev, next;

	std::vector<std::vector<glm::vec2>> newTriangleList;

	std::vector<glm::vec2> newTriangle;

	std::vector<glm::vec2> newV = v;

	int leftPoints = v.size();

	int i = 0;

	newTriangle = std::vector<glm::vec2>();

	while (leftPoints > 3) {

		cur = newV[i];
		if (i == 0) {
			prev = newV[newV.size() - 1];
			next = newV[i + 1];
		}
		else if (i == newV.size() - 1) {
			prev = newV[newV.size() - 2];
			next = newV[0];
		}
		else {
			prev = newV[i - 1];
			next = newV[i + 1];
		}

		if (isInsideTriangle(cur, prev, next, newV)) {
			i++;
		}
		else {
			leftPoints -= 1;
			newTriangle.push_back(cur);
			newTriangle.push_back(next);
			newTriangle.push_back(prev);

			newTriangleList.push_back(newTriangle);

			newTriangle.clear();

			v.clear();

			for (int k = 0; k < newV.size(); k++) {
				if (k!=i) {
					v.push_back(newV[k]);
				}
				
			}
			newV = v;

			i = 0;
		}

	}

	if (leftPoints == 3) {
		newTriangle.push_back(newV[0]);
		newTriangle.push_back(newV[1]);
		newTriangle.push_back(newV[2]);

		newTriangleList.push_back(newTriangle);
	}

	std::cout << "Nombre de triangles : " << newTriangleList.size() << std::endl;

	for (int i = 0; i < newTriangleList.size(); i++) {
		std::cout << "Triangle" << std::endl;
		for (int j = 0; j < newTriangleList[i].size(); j++) {
			std::cout << " x : " << newTriangleList[i][j].x << " y : " << newTriangleList[i][j].y << std::endl;
		}
		std::cout << std::endl;
		
	}

	return newTriangleList;
}