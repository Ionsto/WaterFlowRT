#pragma once
#include "Chunk.h"
#include <iostream>
#include <array>
class ChunkMap
{
public:
	ChunkMap();
	static constexpr int WorldSize = 20;
	std::array<Chunk, WorldSize* WorldSize> raw_data;
	Chunk& GetChunk(int x, int y)
	{
		if (x >= 0 && y >= 0 && x < WorldSize && y < WorldSize)
		{
			return raw_data[y + (x * WorldSize)];
		}
		std::cout << "Erronous chunk access\n";
		throw;
	}
	Chunk& GetChunkPos(float x, float y)
	{
		if (x >= 0 && y >= 0 && x < WorldSize * Chunk::Size && y < WorldSize * Chunk::Size)
		{
			int ix = floor(x / Chunk::Size);
			int iy = floor(y / Chunk::Size);
			return raw_data[iy + (ix * WorldSize)];
		}
		std::cout << "Erronous chunk access\n";
		throw;
	}
	bool InBounds(int cx, int cy)
	{
		return cx >= 0 && cy >= 0 && cx < WorldSize && cy < WorldSize;
	}
	bool InBoundsPos(float cx, float cy)
	{
		return cx >= 0 && cy >= 0 && cx < WorldSize * Chunk::Size && cy < WorldSize * Chunk::Size;
	}
};

