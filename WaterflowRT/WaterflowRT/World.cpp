#include "World.h"
#include <glm/glm.hpp>

World::World()
{
	for (int x = 0; x < grid.SizeX; ++x)
	{
		for (int y = 0; y < grid.SizeY; ++y)
		{
			float scale = 0.01;

			int height = 0;
			height += (0.1 * (1 + sinf(x)));
			height += (1 * (1 + sinf(0.1 * x)));
			height += (2 * (1 + sinf(0.1 * x)));
			int dy = y + 17;
			height += (0.1 * (1 + sinf(dy)));
			height += (1 * (1 + sinf(0.1 * dy)));
			height += (2 * (1 + sinf(0.1 * dy)));
			for (int z = 0; z < std::max(1,height); ++z)
			{
				auto& voxel = grid.Get(x, y, z);
				voxel.Colour.r = 0.1;
				voxel.Colour.g = 1;
				voxel.Colour.b = 0.1;
				voxel.Density = 1;
				voxel.Solid = true;
				voxel.Pressure = 1;
			}
			for (int z = 0; z < 4; ++z)
			{
				auto& voxel = grid.Get(x, y, z);
				if (!voxel.Solid)
				{
					voxel.WaterContent = 1;
				}
			}
		}
	}
}
void World::Update(float dt) {
	GenerateCloudNoise(dt);
	if (WaterCounter++ > 10)
	{
		UpdateWater(dt);
	}
	UpdateSun(dt);
}
void World::UpdateWater(float dt)
{
	for (auto& v : grid.Raw_Data)
	{
	//	v.WaterContent -= glm::length(v.Force) * dt;
		if (v.Solid)
		{
			v.Pressure = 1;
		}
	}
#pragma omp parallel for
	for (int x = 1; x < grid.SizeX-1; ++x)
	{
		for (int y = 1; y < grid.SizeY-1; ++y)
		{
			for (int z = 1; z < grid.SizeZ-1; ++z)
			{
				grid.Get(x, y, z).BackWaterContent = 0;
				grid.Get(x, y, z).Force.x = grid.Get(x - 1, y, z).WaterContent - (2 * grid.Get(x, y, z).WaterContent) + grid.Get(x + 1, y, z).WaterContent;
				grid.Get(x, y, z).Force.z = -9.81;
//				grid.Get(x, y, z).Force.x = grid.Get(x - 1, y, z).WaterContent - (2 * grid.Get(x, y, z).WaterContent) + grid.Get(x + 1, y, z).WaterContent;
//				grid.Get(x, y, z).Force.x = grid.Get(x - 1, y, z).WaterContent - (2 * grid.Get(x, y, z).WaterContent) + grid.Get(x + 1, y, z).WaterContent;
			}
		}
	}
}
void World::UpdateSun(float dt)
{
	float sunspeed = 0.1;
	SunCounter += dt * sunspeed;
	SunPosition.x = grid.SizeX/2.0;
	float sundist = grid.SizeX;
	SunPosition.y = sundist * (sinf(SunCounter)) + (grid.SizeY/2.0);
	SunPosition.z = sundist * (cosf(SunCounter)) + (grid.SizeZ/2.0);
}
void World::GenerateCloudNoise(float dt) {
	float speed = 5;
	cloudnoise += dt * speed;
	float densityrolloff = 0;
	float scale = 0.1;
	for (int x = 0; x < grid.SizeX; ++x)
	{
		for (int y = 0; y < grid.SizeY; ++y)
		{
			for (int z = 10; z < grid.SizeZ; ++z)
			{
				auto& vox = grid.Get(x, y, z);
				if (!vox.Solid)
				{
					float noise = sinf(scale * (125 + x + cloudnoise)) * sinf(scale * (253 + y) - 0.2* cloudnoise) * sinf(scale * (972 + z));
					noise = (1.0 + noise) / 2.0;
					noise = powf(noise,4);
					vox.Density = 0.08 * noise * (((float)z)/grid.SizeZ);
					vox.Density = std::max(vox.Density, 0.004f);
				}
			}
		}
	}
}
