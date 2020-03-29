#include "Chunk.h"
void Chunk::UpdateQuadTree() {

}
Block& Chunk::UpdateBlock(int xl, int yl, int zl) {
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
