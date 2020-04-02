#include "Chunk.h"
void Chunk::GenerateData() {

	for (int ix = 0; ix < Size; ++ix)
	{
		for (int iy = 0; iy < Size; ++iy)
		{
			float x = ix + X;
			float y = iy + Y;
			float scale = 0.01;
			int height = 0;
			height += (0.1 * (1 + sinf(x)));
			height += (1 * (1 + sinf(0.1 * x)));
			height += (2 * (1 + sinf(0.1 * x)));
			float dy = y + 17;
			height += (0.1 * (1 + sinf(dy)));
			height += (1 * (1 + sinf(0.1 * dy)));
			height += (2 * (1 + sinf(0.1 * dy)));
			for (int z = 0; z < std::max(1,height); ++z)
			{
				auto& block = GetBlockLocal(ix, iy, z);
				block.Colour.r = 0.4;
				block.Colour.g = 1;
				block.Colour.b = 0.4;
				block.Solid = true;
			}
			for (int z = 0; z < 4; ++z)
			{
				auto& block = GetBlockLocal(ix, iy, z);
				if (!block.Solid)
				{
					block.WaterContent = 1;
					block.Water = true;
				}
			}
		}
	}
}
void Chunk::UpdateQuadTree() {

}
void Chunk::UpdateBlock(int xl, int yl, int zl) {
	int x = xl - X;
	int y = yl - Y;
	int z = zl;
	bool Solid = false;
	auto block = GetBlockLocal(x, y, z);
	auto PrevNode = GetNode(x, y, z, Size);
	for (int level = MaxLevels-2; level >= 0;--level)
	{
		auto &node = GetNode(x, y, z, level);
		int localsize = Size >> level;
		int node_x = floor(x / localsize);
		int node_y = floor(y / localsize);
		int node_z = floor(z / localsize);
		int localsizechild = Size >> (level+1);
		node.Solid = false;
		node.Homogonous = true;
		node.Water = false;
		for (int nx = 0; nx < localsizechild; ++nx)
		{
			for (int ny = 0; ny < localsizechild; ++ny)
			{
				for (int nz = 0; nz < localsizechild; ++nz)
				{
					auto child = GetNode(node_x + nx,node_y + ny,node_z + nz,level+1);
					if (nx == 0 && ny == 0 && nz == 0)
					{
						node.Solid = child.Solid;
						node.Water = child.Water;
					}
					if (child.Solid != node.Solid || child.Water != node.Water || child.Homogonous == false)
					{
						node.Homogonous = false;
						//early exit?
					}
				}
			}
		}
	}
}
QuadNode& Chunk::GetNode(int x, int y, int z, int level) 
{
	int offset = (1 << level) - 1;
	int localsize = Size >> level;
	int xl = floor(x / localsize);
	int yl = floor(y / localsize);
	int zl = floor(z / localsize);
	return QuadTree[offset + (z + (y * localsize) + (x * localsize * localsize))];
}
QuadNode& Chunk::GetNodeLocal(int x, int y, int z, int level) 
{
	int offset = (1 << level) - 1;
	int localsize = Size >> level;
	return QuadTree[offset + (z + (y * localsize) + (x * localsize * localsize))];
}
