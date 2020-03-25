#pragma once
#include <glm/glm.hpp>
struct Voxel
{
	glm::fvec3 Colour = glm::fvec3(1,1,1);
	float Density = 0.004;
	bool Solid = false;
	float Pressure = 0;
	//float Mass;
	glm::fvec3 Force = glm::vec3(0);
	glm::fvec3 Velocity = glm::vec3(0);
	float WaterContent = 0;
	float BackWaterContent = 0;
};


