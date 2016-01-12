#pragma once

#include <vector>

#include "libs\glm\glm.hpp"


bool visible(const glm::vec2& checkedPoint, const glm::vec2& windowPointA, const glm::vec2& windowPointB);

bool cuts(glm::vec2& s, glm::vec2& ss, glm::vec2& f, glm::vec2& f2);

bool strictCuts(glm::vec2& s, glm::vec2& ss, glm::vec2& f, glm::vec2& f2);

glm::vec2 intersection(const glm::vec2& checkedPointA, const glm::vec2& checkedPointB, const glm::vec2& windowPointA, const glm::vec2& windowPointB);

std::vector<std::vector<glm::vec2>> maskInWindow(std::vector<glm::vec2>& s, std::vector<glm::vec2>& f);

bool isInShape(glm::vec2 p, std::vector<glm::vec2> shape);

bool isConvex(std::vector<glm::vec2> s);

std::vector<std::vector<glm::vec2> > triangulation(std::vector<glm::vec2>& v);

