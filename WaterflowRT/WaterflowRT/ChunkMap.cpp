#include "ChunkMap.h"
ChunkMap::ChunkMap()
{
	for (int x = 0; x < WorldSize; ++x)
	{
		for (int y = 0; y < WorldSize; ++y)
		{
			GetChunk(x, y).X = x * Chunk::Size;
			GetChunk(x, y).Y = y * Chunk::Size;
		}
	}

}
