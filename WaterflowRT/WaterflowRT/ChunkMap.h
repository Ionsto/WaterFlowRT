#pragma once
#include "Chunk.h"
#include <iostream>
#include <array>
class ChunkMap
{
public:
	static constexpr int WorldSize = 10;
	std::array<Chunk, WorldSize* WorldSize> raw_data;
	Chunk& GetChunk(int x, int y)
	{
		if (x >= 0 && y >= 0 && x < WorldSize && y < WorldSize)
		{
			return raw_data[y + (x * WorldSize);
		}
		std::cout << "Erronous chunk access\n";
		return Chunk();
	}
};

