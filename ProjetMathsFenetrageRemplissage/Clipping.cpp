#pragma once

#include "Clipping.h"
#include "Shape.h"
#include "libs\glm\glm.hpp"
#include <iostream>

bool visible(const glm::vec2& checkedPoint, const glm::vec2& windowPointA, const glm::vec2& windowPointB) {
	glm::vec2 segWindow = windowPointB;
	segWindow -= windowPointA;

	glm::vec2 normal = glm::vec2(-segWindow.y, segWindow.x);

	glm::vec2 segVerif = checkedPoint;
	segVerif -= windowPointA;

	//Calcul des normales pour savoir si on est à l'intérieur ou l'extérieur de la fenêtre
	if (glm::dot(normal, segVerif) > 0) {
		//extérieur
		normal = -normal;
	}
	else if (glm::dot(normal, segVerif) < 0) {
		//intérieur
		//garder la normale telle qu'elle est
	}
	else if (glm::dot(normal, segVerif) == 0) {
		//sur le meme plan
		//retourner

	}

	return false;
}

bool cuts(glm::vec2& s, glm::vec2& ss, glm::vec2& f, glm::vec2& f2) {
	//Retourne vrai si la droite prolongeant le coté de la fenêtre coupe un segment de la forme



	return false;
}

glm::vec2 intersection(const glm::vec2& checkedPointA, const glm::vec2& checkedPointB, const glm::vec2& windowPointA, const glm::vec2& windowPointB) {

	glm::mat2x2 matrixD = glm::mat2x2(
		checkedPointB.x - checkedPointA.x, -(windowPointB.x - windowPointA.x),
		checkedPointB.y - checkedPointA.y, -(windowPointB.y - windowPointA.y)
		);

	glm::vec2 c2 = glm::vec2(
		windowPointA.x - checkedPointA.x, windowPointA.y - checkedPointA.x
		);

	float tempDeterminant = glm::determinant(matrixD);
	glm::vec2 res = glm::vec2(0, 0);
	if (tempDeterminant != 0) {

		glm::inverse(matrixD);
		res = glm::inverse(matrixD) * c2;
	}
	else {
		//erreur

		return glm::vec2(0, 0);
	}

	std::cout << "t : " << res.x << " s : " << res.y << std::endl;

	glm::vec2 finalRes = windowPointA + (windowPointB - windowPointA)*res.y;

	std::cout << "X : " << finalRes.x << " Y : " << finalRes.y << std::endl;

	return res;
}

void maskInWindow(Shape& s, Shape& f) {
	Shape finalShape;
	glm::vec2 F; //Premier et dernier point de la forme
	glm::vec2 S, I;
	for (int i = 0; i < f.pointCount - 1; i++) {
		finalShape = Shape();
		for (int j = 0; j < s.pointCount; j++) {
			if (j == 0) {
				F = s[j];
			}
			else {
				if (cuts(S, s[j], f[i], f[i + 1])) {
					I = intersection(S, s[j], f[i], f[i + 1]);
					finalShape.push(I);
				}
			}
			S = s[j];
			if (visible(S, f[i], f[i + 1])) {
				finalShape.push(S);
			}
		}
		if (finalShape.pointCount > 0) {
			//Traitement du dernier coté de s
			if (cuts(S, f[1], f[i], f[i + 1])) {
				I = intersection(S, f[1], f[i], f[i + 1]);
				s.push(I);
			}
			s = finalShape;

		}
	}
}