#pragma once
#include "QuadNode.h"
#include "Block.h"
#include <array>
class Chunk
{
public:
	static constexpr int Size = 16;
	static constexpr int MaxLevels = 5;
	static constexpr int Volume = Size * Size * Size;
	int X, Y;
	std::array<QuadNode, Volume * 2 + 1> QuadTree;
	std::array<Block, Volume> block_array;
	void UpdateQuadTree();
	Block& GetBlockLocal(int x, int y, int z)
	{
		return block_array[z + (y * Size) + (x * Size * Size)];
	}
	Block& GetBlock(int xl, int yl, int zl) 
	{
		return GetBlockLocal(X - xl, Y - yl, zl);
	}
	Block& UpdateBlock(int xl, int yl, int zl);
	QuadNode& GetNode(int x, int y, int z, int level);
	QuadNode& GetNodeLocal(int i, int j, int k, int level);
};

