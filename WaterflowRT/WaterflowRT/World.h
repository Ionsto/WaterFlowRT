#pragma once
#include "VoxelGrid.h"
class World
{
	float Accumulator;
	float cloudnoise = 0;
	float SunCounter = 0;
	int WaterCounter = 0;
public:
	VoxelGrid grid;
	glm::fvec3 SunPosition = glm::fvec3(0,0,19.9);
	glm::fvec3 SunColour = glm::fvec3(1,0.9,0.9);
	World();
	void GenerateCloudNoise(float dt);
	void UpdateSun(float dt);
	void UpdateWater(float dt);
	void Update(float dt);
};

