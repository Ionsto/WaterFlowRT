#pragma once
#include <glm/glm.hpp>
class Block
{
public:
	float RockHeight = 0;
	float SandHeight = 0;
	float SoilHeight = 0;
	float WaterHeight = 0;
	float SedimentContent = 0;
	glm::vec3 Velocity = glm::vec3(1);
	bool Entity = false;
	//glm::vec3 Colour = glm::vec3(1);
};

