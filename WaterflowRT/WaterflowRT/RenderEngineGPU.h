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
struct GPUBlock{
	float x;
	float y;
	float z;
	float w;
	float WaterContent;
	float Solid;
	float Entity;
	float padding;
};
struct GPUEntityPrimitive{
	float ColourR;
	float ColourG;
	float ColourB;
	float Colourw;
	float PosX;
	float PosY;
	float PosZ;
	float PosW;
	float SizeX;
	float SizeY;
	float SizeZ;
	float SizeW;
};
struct GPUChunk {
	int EntityCount;
	std::array<int, Chunk::MaxEntitiesPerChunk> EntityIds;
};
class RenderEngineGPU : public RenderEngine
{
private:
	static constexpr int FrameSizeX = 128;
	static constexpr int FrameSizeY = 128;
	static constexpr int LocalSize = 32;
	static constexpr int RenderGridSize = 16;
	static constexpr int ViewChunkCount = 5;
	static constexpr int MaxEntityPrimitiveCount = 100;
	int EntityCounter = 0;
	GLuint BlockBufferSSBO;
	GLuint EntityBufferSSBO;
	GLuint ChunkBufferSSBO;
	std::vector<GPUBlock> GPUBlockBuffer = std::vector<GPUBlock>(RenderGridSize* RenderGridSize* RenderGridSize * ViewChunkCount * ViewChunkCount);
	std::vector<GPUEntityPrimitive> GPUEntityBuffer = std::vector<GPUEntityPrimitive>(MaxEntityPrimitiveCount);
	std::vector<GPUChunk> GPUChunkBuffer = std::vector<GPUChunk>(ViewChunkCount * ViewChunkCount);
	glm::vec3 CurrentChunkPos;
	glm::ivec2 CurrentChunkIds;
	Shader compute;
	ShaderProgram program_compute;
	GLuint UniformTilt;
	GLuint UniformYaw;
	GLuint UniformSunTime;
	GLuint UniformRnd;
	GLuint UniformEntityCount;
	GLuint UniformChunkOffset;
	float DitherTime = 0;
	Shader vertex;
	Shader fragment;
	ShaderProgram program_render;
	GLuint TextureFrameBuffer;
	int CurrentTexture = 0;
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
public:
	RenderEngineGPU(GLFWwindow* window);
	virtual ~RenderEngineGPU();
	virtual void RenderWorld(World& world) override;
	void CreateQuadTree(World& world);
	void DisplayRender();
};

