#pragma once
#include "VoxelGrid.h"
#include "Entity.h"
class World
{
	float Accumulator;
	float cloudnoise = 0;
	float SunCounter = 0;
	int WaterCounter = 0;
public:
	float DeltaTime = 0.01;
	VoxelGrid grid;
	glm::fvec3 SunPosition = glm::fvec3(0,0,19.9);
	glm::fvec3 SunColour = glm::fvec3(1,0.9,0.9);
	static constexpr int MaxEntities = 1000;
	std::array<std::unique_ptr<Entity>, MaxEntities> EntityList;
	World();
	void GenerateCloudNoise(float dt);
	void UpdateSun(float dt);
	void UpdateWater(float dt);
	void Update(float dt);
};

