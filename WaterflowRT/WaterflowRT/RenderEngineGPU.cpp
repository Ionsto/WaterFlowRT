#include "RenderEngineGPU.h"
#include "RenderEngineSWStepped.h"
#include <algorithm>
#include <glm/gtx/rotate_vector.hpp>
#include <glm/gtx/component_wise.hpp>
RenderEngineGPU::RenderEngineGPU(GLFWwindow* window) : RenderEngine(window)
{
	//Create shaders
	std::cout << "Init shaders" << std::endl;
	vertex.Init("quad.vert", GL_VERTEX_SHADER);
	fragment.Init("quad.frag", GL_FRAGMENT_SHADER);
	compute.Init("raytrace.comp", GL_COMPUTE_SHADER);
	std::cout << "Creating program" << std::endl;
	program_render.CreateProgram();
	program_render.AddShader(vertex);
	program_render.AddShader(fragment);
	program_render.LinkProgram();
	program_render.UseProgram();
	program_compute.CreateProgram();
	program_compute.AddShader(compute);
	program_compute.LinkProgram();
	//Create textures
//Gen
	glGenBuffers(1, &FB_VAO);
	glGenBuffers(1, &FB_EBO);
	glGenBuffers(1, &FB_VBO);
	//init
	glBindVertexArray(FB_VAO);
	glBindBuffer(GL_ARRAY_BUFFER, FB_VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(fb_verts), fb_verts, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), 0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)(2 * sizeof(float)));
	glEnableVertexAttribArray(1);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, FB_EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(fb_ebo), fb_ebo, GL_STATIC_DRAW);
	program_compute.UseProgram();
	GLuint texdest = glGetUniformLocation(program_compute.GetProgram(), "destTex");
	if (texdest == -1)
	{
		std::cout << "Error finding texture destination" << std::endl;
	}
	UniformYaw = glGetUniformLocation(program_compute.GetProgram(), "CameraYaw");
	UniformTilt = glGetUniformLocation(program_compute.GetProgram(), "CameraTilt");
	UniformChunkOffset = glGetUniformLocation(program_compute.GetProgram(), "ChunkOffsets");
	UniformSunTime = glGetUniformLocation(program_compute.GetProgram(), "SunTime");
	glUniform1i(texdest,0);
	glUniform1f(UniformTilt,0);
	glUniform1f(UniformYaw,0);
	glUniform1f(UniformSunTime,0);
	glUniform3f(UniformChunkOffset,0,0,0);
	glGenTextures(1, &TextureFrameBuffer);
	glBindTexture(GL_TEXTURE_2D, TextureFrameBuffer);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	std::vector<float>  data(FrameSizeX * FrameSizeY * 4);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA32F, FrameSizeX, FrameSizeY, 0, GL_RGBA, GL_FLOAT, data.data());
	glBindImageTexture(0,TextureFrameBuffer,0,GL_FALSE,0,GL_WRITE_ONLY,GL_RGBA);
	glGenBuffers(1, &BlockBufferSSBO);
	glBindBuffer(GL_SHADER_STORAGE_BUFFER,BlockBufferSSBO);
	glBufferStorage(GL_SHADER_STORAGE_BUFFER, sizeof(GPUBlock) * GPUBlockBuffer.size(), GPUBlockBuffer.data(), GL_DYNAMIC_STORAGE_BIT);
	glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 0, BlockBufferSSBO);
}
RenderEngineGPU::~RenderEngineGPU()
{

}
void RenderEngineGPU::CreateQuadTree(World& world)
{
	for (int x = 0; x < world.TestChonk.Size; ++x)
	{
		for (int y = 0; y < world.TestChonk.Size; ++y)
		{
			for (int z = 0; z < world.TestChonk.Size; ++z)
			{
				auto & gpublock = GPUBlockBuffer[z + (RenderGridSize * y) + (RenderGridSize * RenderGridSize * x)];
				auto block = world.TestChonk.GetBlockLocal(x, y, z);
				gpublock.Solid = block.Solid;
				gpublock.x = block.Colour.r;
				gpublock.y = block.Colour.g;
				gpublock.z = block.Colour.b;
				//gpublock.z = block.Colour.z;
				//gpublock.y = 0;
			}
		}
	}
}
void RenderEngineGPU::RenderWorld(World& world)
{
	CreateQuadTree(world);
 	program_compute.UseProgram();
//	glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 0, BlockBufferSSBO);
 //	glBufferStorage(GL_SHADER_STORAGE_BUFFER, sizeof(GPUBlock) * GPUBlockBuffer.size(), GPUBlockBuffer.data(), GL_DYNAMIC_STORAGE_BIT);
 //	glMemoryBarrier(GL_SHADER_STORAGE_BARRIER_BIT);
	glBindBuffer(GL_SHADER_STORAGE_BUFFER, BlockBufferSSBO);
	glBufferSubData(GL_SHADER_STORAGE_BUFFER, 0, sizeof(GPUBlock) * GPUBlockBuffer.size(), GPUBlockBuffer.data());
	glMemoryBarrier(GL_SHADER_STORAGE_BARRIER_BIT);
	glUniform1f(UniformTilt,cam.tilt);
	glUniform1f(UniformYaw,cam.yaw);
	glUniform1f(UniformSunTime,world.SunCounter);
	glUniform3f(UniformChunkOffset,cam.Position.x,cam.Position.y,cam.Position.z);
 	glActiveTexture(GL_TEXTURE0);
 	glBindImageTexture(0, TextureFrameBuffer, 0, GL_FALSE, 0, GL_WRITE_ONLY,GL_RGBA);
	const int LocalSize = 1;
 	glDispatchCompute(FrameSizeX/LocalSize, (GLuint)FrameSizeY/LocalSize, 1);
//	glMemoryBarrier(GL_SHADER_STORAGE_BARRIER_BIT);
	glMemoryBarrier(GL_SHADER_IMAGE_ACCESS_BARRIER_BIT);
	//display render
	DisplayRender();
	//kick off compute shader
}
void RenderEngineGPU::DisplayRender() {
//	glBindTexture(GL_TEXTURE_2D, TextureFrameBuffer);
	program_render.UseProgram();
	glActiveTexture(GL_TEXTURE0);
	glBindImageTexture(0, TextureFrameBuffer, 0, GL_FALSE, 0, GL_WRITE_ONLY,GL_RGBA32F);
	glBindVertexArray(FB_VAO);
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);
}
