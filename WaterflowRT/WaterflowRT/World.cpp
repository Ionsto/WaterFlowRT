#include "World.h"
#include <iostream>
#include <glm/glm.hpp>
#include "Entity.h"

World::World()
{
	std::cout << "Generating world" << std::endl;
	for (auto& chunk : Chunks.raw_data)
	{
		chunk.GenerateData();
	}
}
void World::Update(float dt) {
	DeltaTimeAccumulator += dt;
	//GenerateCloudNoise(dt);
	int DeltaSteps = floor(DeltaTimeAccumulator / DeltaTime);
	for(int i = 0;i < std::min(1,DeltaSteps);++i)
	{
		if (WaterCounter++ > 10)
		{
			//UpdateWater(dt);
		}
		UpdateEntity(dt);
		UpdateEntityChunks();
		DeltaTimeAccumulator -= DeltaTime;
		UpdateSun(dt);
	}
}
void World::UpdateWater(float dt)
{
	for (auto& chunk : Chunks.raw_data)
	{
		chunk.UpdateWaterPre(Chunks);
	}
	for (auto& chunk : Chunks.raw_data)
	{
		chunk.UpdateWaterPost(Chunks);
	}
	//for (auto& v : grid.Raw_Data)
	//{
	////	v.WaterContent -= glm::length(v.Force) * dt;
	//	if (v.Solid)
	//	{
	//		v.Pressure = 1;
	//	}
	//}
//#pragma omp parallel for
//	for (int x = 1; x < grid.SizeX-1; ++x)
//	{
//		for (int y = 1; y < grid.SizeY-1; ++y)
//		{
//			for (int z = 1; z < grid.SizeZ-1; ++z)
//			{
//				grid.Get(x, y, z).BackWaterContent = 0;
//				grid.Get(x, y, z).Force.x = grid.Get(x - 1, y, z).WaterContent - (2 * grid.Get(x, y, z).WaterContent) + grid.Get(x + 1, y, z).WaterContent;
//				grid.Get(x, y, z).Force.z = -9.81;
////				grid.Get(x, y, z).Force.x = grid.Get(x - 1, y, z).WaterContent - (2 * grid.Get(x, y, z).WaterContent) + grid.Get(x + 1, y, z).WaterContent;
////				grid.Get(x, y, z).Force.x = grid.Get(x - 1, y, z).WaterContent - (2 * grid.Get(x, y, z).WaterContent) + grid.Get(x + 1, y, z).WaterContent;
//			}
//		}
//	}
}
void World::UpdateSun(float dt)
{
	float sunspeed = 5;
	SunCounter += dt * sunspeed;
	SunPosition.x = Chunks.WorldSize/2.0;
	float sundist = Chunks.WorldSize;
	SunPosition.y = sundist * (sinf(SunCounter)) + (Chunks.WorldSize/2.0);
	SunPosition.z = sundist * (cosf(SunCounter)) + (Chunks.WorldSize/2.0);
	if (cosf(SunCounter * (3.14/180)) < 0)
	{
		SunCounter += dt * sunspeed * 10;
	}
	SunColour = glm::fvec3(1,0.9,0.9);
}
void World::GenerateCloudNoise(float dt) {
	float speed = 5;
	cloudnoise += dt * speed;
	float densityrolloff = 0;
	float scale = 0.1;
//	for (int x = 0; x < grid.SizeX; ++x)
//	{
//		for (int y = 0; y < grid.SizeY; ++y)
//		{
//			for (int z = 10; z < grid.SizeZ; ++z)
//			{
//				auto& vox = grid.Get(x, y, z);
//				if (!vox.Solid)
//				{
//					float noise = sinf(scale * (125 + x + cloudnoise)) * sinf(scale * (253 + y) - 0.2* cloudnoise) * sinf(scale * (972 + z));
//					noise = (1.0 + noise) / 2.0;
//					noise = powf(noise,4);
//					vox.Density = 0.08 * noise * (((float)z)/grid.SizeZ);
//					vox.Density = std::max(vox.Density, 0.004f);
//				}
//			}
//		}
//	}
}
void World::UpdateEntityChunks() {
	for (auto & chunk : Chunks.raw_data)
	{
		chunk.EntityCount = 0;
	}
	for (int i = 0; i < EntityList.ParticleCount; ++i)
	{
		auto& entity = EntityList.RawData()[i];
		auto& chunk = Chunks.GetChunkPos(entity->Position.x, entity->Position.y);
		if (chunk.EntityCount < chunk.MaxEntitiesPerChunk)
		{
				chunk.EntityRef[chunk.EntityCount++] = EntityList.ReverseIds[i];
		}
		auto dist = (glm::round(entity->Position / float(Chunk::Size))*float(Chunk::Size)) - entity->Position;
		if (Chunks.InBoundsPos((entity->Position.x + (dist.x * 1.2)), entity->Position.y))
		{
			auto & chunk = Chunks.GetChunkPos(entity->Position.x + (dist.x * 1.2), entity->Position.y);
			if (chunk.EntityCount < chunk.MaxEntitiesPerChunk)
			{
				chunk.EntityRef[chunk.EntityCount++] = EntityList.ReverseIds[i];
			}
		}
		if (Chunks.InBoundsPos(entity->Position.x, (entity->Position.y + (dist.y * 1.2))))
		{
			auto& chunk = Chunks.GetChunkPos(entity->Position.x, entity->Position.y + (dist.y * 1.2));
			if (chunk.EntityCount < chunk.MaxEntitiesPerChunk)
			{
				chunk.EntityRef[chunk.EntityCount++] = EntityList.ReverseIds[i];
			}
		}
	}
}
void World::UpdateEntity(float dt) {
	for (auto & chunk : Chunks.raw_data)
	{
		for (Block& b : chunk.Mesh)
		{
			b.Entity = false;
		}
	}
	for (int i = 0; i < EntityList.ParticleCount; ++i)
	{
		auto& entity = EntityList.RawData()[i];
		entity->Update();
	}
}
int World::AddEntity(std::unique_ptr<Entity>&& entity)
{
	return EntityList.AddParticle(std::move(entity));
}
bool World::InBounds(glm::vec3 Pos)
{
	return (Pos.x >= 0 && Pos.y >= 0 && Pos.z >= 0 && Pos.x < Chunks.WorldSize * Chunk::Size && Pos.y < Chunks.WorldSize * Chunk::Size && Pos.z < Chunk::Size);
}
