#pragma once
#include "QuadNode.h"
#include "Block.h"
#include <array>
class ChunkMap;
class Chunk
{
public:
	static constexpr float MaxHeight = 64;
	static constexpr int Size = 16;
	static constexpr int MeshSize = Size * Size;
	int X, Y;
	std::array<Block, MeshSize> Mesh;
	static constexpr int MaxEntitiesPerChunk = 100;
	int EntityCount = 0;
	std::array<int, MaxEntitiesPerChunk> EntityRef;
	void UpdateQuadTree();
	Block& GetBlockLocal(int x, int y)
	{
		return Mesh[y + (x * Size)];
	}
	Block& GetBlock(float xl, float yl)
	{
		return GetBlockLocal(std::floor(xl - X), std::floor(yl - Y));
	}
	void UpdateBlock(int xl, int yl);
	void GenerateData();
	void UpdateWaterPre(ChunkMap & Chunks);
	void UpdateWaterPost(ChunkMap & Chunks);
};

