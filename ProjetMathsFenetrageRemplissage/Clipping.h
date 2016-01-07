#pragma once

#include <vector>

#include "libs\glm\glm.hpp"


bool visible(const glm::vec2& checkedPoint, const glm::vec2& windowPointA, const glm::vec2& windowPointB);

bool cuts(glm::vec2& s, glm::vec2& ss, glm::vec2& f, glm::vec2& f2);

glm::vec2 intersection(const glm::vec2& checkedPointA, const glm::vec2& checkedPointB, const glm::vec2& windowPointA, const glm::vec2& windowPointB);

std::vector<glm::vec2> maskInWindow(std::vector<glm::vec2>& s, std::vector<glm::vec2>& f);

