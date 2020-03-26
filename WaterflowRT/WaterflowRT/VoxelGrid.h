#pragma once
#include <array>
#include <vector>
#include "Voxel.h"
#include <iostream>
class VoxelGrid
{
public:
	constexpr static int RealSize = 50;
	constexpr static float GridDim = 1;
	constexpr static int GridSize = static_cast<int>(static_cast<float>(RealSize) / GridDim);
	static constexpr int SizeX = 200;
	static constexpr int SizeY = 200;
	static constexpr int SizeZ = 50;
	//static constexpr float ScaleFactor = 0.1;
//	std::array<Voxel, SizeX * SizeY> Raw_Data;
	std::vector<Voxel> Raw_Data;
	VoxelGrid() : Raw_Data(SizeX * SizeY * SizeZ) {};
	~VoxelGrid() {};
	bool InFlowBounds(int x, int y)
	{
		return x > 0 && y > 0 && x < SizeX - 1 && y < SizeY - 1;
	}
	bool InBounds(int x, int y,int z)
	{
		return x >= 0 && y >= 0 && z >= 0 && x < SizeX && y < SizeY && z < SizeZ;
	}
	Voxel& Get(int x, int y,int z)
	{
		if (!InBounds(x,y,z)) {
			std::cout << "Out of grid access violation\n";
		}
		return Raw_Data[z + (y * SizeZ) + (x * SizeY * SizeZ)];
	}
};

