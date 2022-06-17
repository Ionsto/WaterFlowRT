#include "Chunk.h"
#include "ChunkMap.h"
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
			auto& block = GetBlockLocal(ix, iy);
			block.SandHeight = height;
		}
	}
}
void Chunk::UpdateQuadTree() {

}
void Chunk::UpdateWaterPre(ChunkMap & chunks)
{

}
void Chunk::UpdateWaterPost(ChunkMap & chunks)
{

}
void Chunk::UpdateBlock(int xl, int yl) {
	int x = xl - X;
	int y = yl - Y;
//	bool Solid = false;
//	auto block = GetBlockLocal(x, y);
//	for (int level = MaxLevels-2; level >= 0;--level)
//	{
//		auto &node = GetNode(x, y, level);
//		int localsize = Size >> level;
//		int node_x = floor(x / localsize);
//		int node_y = floor(y / localsize);
//		int localsizechild = Size >> (level+1);
//		node.Solid = false;
//		node.Homogonous = true;
//		node.Water = false;
//		for (int nx = 0; nx < localsizechild; ++nx)
//		{
//			for (int ny = 0; ny < localsizechild; ++ny)
//			{
//				for (int nz = 0; nz < localsizechild; ++nz)
//				{
//					auto child = GetNode(node_x + nx,node_y + ny,level+1);
//					if (nx == 0 && ny == 0 && nz == 0)
//					{
//						node.Solid = child.Solid;
//						node.Water = child.Water;
//					}
//					if (child.Solid != node.Solid || child.Water != node.Water || child.Homogonous == false)
//					{
//						node.Homogonous = false;
//						//early exit?
//					}
//				}
//			}
//		}
//	}
}
