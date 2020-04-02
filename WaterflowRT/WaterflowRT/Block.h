#pragma once
#include <glm/glm.hpp>
class Block
{
public:
	bool Solid = false;
	bool Water = false;
	bool Entity = false;
	float WaterContent = false;
	glm::vec3 Colour = glm::vec3(1);
};

