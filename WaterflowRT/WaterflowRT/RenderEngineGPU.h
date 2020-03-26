#pragma once
#include "RenderEngine.h"
#include "ShaderProgram.h"
#include <array>
class OctNode {
//	int SizeX,SizeY,SizeZ;
	bool Solid;
	bool Homogonous;
};
class OctTree {
	static constexpr int SizeX = 32;
	static constexpr int SizeY = 32;
	static constexpr int SizeZ = 32;
	std::array <OctNode,SizeX * SizeY * SizeZ * 2 - 1> raw_data;
	OctNode GetNode(int x, int y, int z, int level)
	{
		int LevelOffset = 1 << level;
		return raw_data[(LevelOffset - 1) + z + (y * SizeZ) + (x * SizeY * SizeZ)];
	}
	int GetSize(int level)
	{
		return SizeX >> level;
	}
};
class RenderEngineGPU : public RenderEngine
{
private:
	Shader compute;
	ShaderProgram program_compute;
	GLuint TextureFrameBuffer[3];
	int CurrentBuffer = 0;
	GLuint FB_VBO;
	GLuint FB_VAO;
	GLuint FB_EBO;
	float fb_verts[16] = {
		-1,-1, 0,0,
		-1, 1, 0,1,
		 1, 1, 1,1,
		 1,-1, 1,0
	};
	unsigned int fb_ebo[6] = {
		0,1,2,
		0,2,3
	};
	virtual ~RenderEngineGPU();
	virtual void RenderWorld(World& world) override;
	void CreateQuadTree(World& world);
};

