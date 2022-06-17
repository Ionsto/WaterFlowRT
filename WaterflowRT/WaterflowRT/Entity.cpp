#include "Entity.h"
#include "World.h"
#include <algorithm>

Entity::Entity(World& world) : world(world)
{

}
Entity::~Entity() {

}
void Entity::Intergrate()
{
	auto temppos = Position;
	glm::vec3 dv = (world.DeltaTime * world.DeltaTime * Acceleration);
	glm::vec3 v = (1 - Damping) * (Position - PositionOld) + dv;
	glm::vec3 CurrentBlockPos = glm::floor(Position);
	if (glm::dot(v, v) != 0)
	{
		float Size = 0;
		glm::vec3 BlockPos = glm::floor(Position + v + (Size * glm::normalize(v)));
		bool Solid = false;
		if (world.Chunks.InBounds(BlockPos.x, BlockPos.y))
		{
			auto& intersectchunk = world.Chunks.GetChunkPos(BlockPos.x, BlockPos.y);
			auto block = intersectchunk.GetBlock(BlockPos.x, BlockPos.y);
			if (Position.z < block.RockHeight + block.SoilHeight + block.SandHeight)
			{
				Solid = true;
			}
		}
		else {
			Solid = true;
		}
		if (Solid)
		{
			auto delta = BlockPos - CurrentBlockPos;
			if (glm::dot(delta, delta) != 0)
			{
				auto bd = glm::normalize(delta);
				v -= bd * glm::dot(bd, v);
				Position -= bd * 0.01f;
				temppos -= bd * 0.01f;
			}
		}
	}
	Position += v;
	PositionOld = temppos;
	Acceleration = glm::vec3(0);
	Acceleration.z = -9.8;
	for (int i = 0; i < 2; ++i) {
		Position[i] = std::clamp(Position[i], 0.0f, world.Chunks.WorldSize-0.1f);
	}
	Position[2] = std::clamp(Position[2], 0.0f, Chunk::Size-0.1f);
	for (int x = -1; x < 2; ++x)
	{
		for (int y = -1; y < 2; ++y)
		{
			for (int z = -1; z < 2; ++z)
			{
				if (world.InBounds(Position + glm::vec3(x, y, z)))
				{
					world.GetBlock(Position + glm::vec3(x, y, z)).Entity = true;
				}
			}
		}
	}
}
void Entity::UpdateCollisions()
{
	//We good
	for (int i = 0; i < 2; ++i) {
		Position[i] = std::clamp(Position[i], 0.0f, world.Chunks.WorldSize-0.1f);
		PositionOld[i] = std::clamp(PositionOld[i], 0.0f, world.Chunks.WorldSize-0.1f);
	}
	Position[2] = std::clamp(Position[2], 0.0f, Chunk::Size-0.1f);
	PositionOld[2] = std::clamp(PositionOld[2], 0.0f, Chunk::Size-0.1f);
	world.GetBlock(Position).Entity = true;
}
void Entity::Update()
{
	Intergrate();
	UpdateCollisions();
	DirectionCounter += world.DeltaTime;
	Acceleration += Direction;
	if (DirectionCounter > 2)
	{
		Direction = 20.0f * ((glm::vec3(rand() % 100, rand() % 100, 0) / 100.0f) - 0.5f);
		Direction.z = 0;
		DirectionCounter = 0;
	}
	auto block = world.GetBlock(Position);
	if (abs(Position.z - block.RockHeight + block.SoilHeight + block.SandHeight) < 1)
	{
		if (rand() % 1000 == 0)
		{
			Acceleration.z += 600;
		}
	}
	if (Position.z > block.RockHeight + block.SoilHeight + block.SandHeight && Position.z < block.WaterHeight + block.RockHeight + block.SoilHeight + block.SandHeight)
	{
		Acceleration.z += 2.8;
	}
}
