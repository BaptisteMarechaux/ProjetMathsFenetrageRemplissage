#pragma once

#include <vector>
#include <iostream>

#include "Clipping.h"
#include "Shape.h"
#include "libs\glm\glm.hpp"


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

	if (res.x < 0)
		return false;
	else if (res.x > 1)
		return false;
	else if (res.y < 0)
		return false;
	else if (res.y > 1)
		return false;
	else
		return true;
}

glm::vec2 intersection(const glm::vec2& checkedPointA, const glm::vec2& checkedPointB, const glm::vec2& windowPointA, const glm::vec2& windowPointB) {

	std::cout << "intersection" << std::endl;
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

		return checkedPointB;
	}

	//return res;

	if (res.x < 0)
		return checkedPointB;
	else if (res.x > 1)
		return checkedPointB;
	else if (res.y < 0)
		return checkedPointB;
	else if (res.y > 1)
		return checkedPointB;
	else
		return windowPointA + (windowPointB - windowPointA)*res.y;
}

std::vector<glm::vec2> maskInWindow(std::vector<glm::vec2>& s, std::vector<glm::vec2>& f) {
	std::vector<glm::vec2> outShape; //polygone retourné
	std::vector<glm::vec2> inShape; //polygone d'entrée temporaire

	glm::vec2 f1; //premier point sélectionné de la fenetre dans la boucle
	glm::vec2 f2; //point courant de la fenetre sélectionné 

	glm::vec2 sIn; //point courant de la forme que l'on veut couper
	glm::vec2 sLast; //dernier point de la forme que l'on veut couper

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

	return outShape;
}