#pragma once
#include "VoxelGrid.h"
#include "Chunk.h"
#include "Entity.h"
#include <memory>
#include "ChunkMap.h"
#include "SwapList.h"
class World
{
	float Accumulator;
	float cloudnoise = 0;
	int WaterCounter = 0;
public:
	float DeltaTime = 0.01;
	float DeltaTimeAccumulator = 0;
	VoxelGrid grid;
	ChunkMap Chunks;
	Chunk TestChonk;
	float SunCounter = 0;
	glm::fvec3 SunPosition = glm::fvec3(0,0,19.9);
	glm::fvec3 SunColour = glm::fvec3(1,0.9,0.9);
	static constexpr int MaxEntities = 1000;
	SwapList<Entity, MaxEntities> EntityList;
	World();
	void GenerateCloudNoise(float dt);
	void UpdateSun(float dt);
	void UpdateWater(float dt);
	void UpdateEntityChunks();
	void UpdateEntity(float dt);
	void Update(float dt);
	int AddEntity(std::unique_ptr<Entity> && entity);
	bool InBounds(glm::vec3 pos);
	Block& GetBlock(glm::vec3 pos)
	{
		if (Chunks.InBounds(pos.x/Chunk::Size, pos.y/Chunk::Size))
		{
			return Chunks.GetChunkPos(pos.x, pos.y).GetBlock(pos.x, pos.y, pos.z);
		}
		return Chunks.GetChunkPos(0,0).GetBlock(0, 0, 0);
	}
};

