#pragma once
#include <glm/glm.hpp>
class Ray
{
public:
	glm::fvec3 Direction;
	glm::fvec3 Colour = glm::fvec3(0,0,0);
	glm::fvec3 Origin;
};

