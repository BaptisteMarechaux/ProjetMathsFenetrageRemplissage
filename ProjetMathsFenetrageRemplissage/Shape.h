#pragma once

#include "Point.h"
#include "libs\glm\glm.hpp"
#include <vector>


class Shape
{

private:
	
public:
	Shape() { pointCount = 0;}
	~Shape();
	int pointCount;
	std::vector<glm::vec2> points;

	void push(const glm::vec2& p);

	glm::vec2 operator[] (int i) const;
};

