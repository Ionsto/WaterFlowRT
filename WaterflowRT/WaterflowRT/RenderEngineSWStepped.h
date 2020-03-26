#pragma once
#include <glm/glm.hpp>
#include "RenderEngine.h"
#include "Camera.h"
#include "Ray.h"
#include "Shader.h"
#include "ShaderProgram.h"
class RenderEngineSWStepped : public RenderEngine
{
	static constexpr int MaxSteps = 500;
	static constexpr float StepSize = 0.1;
	static constexpr int FrameSizeX = 30;
	static constexpr int FrameSizeY = 30;
	std::array<glm::fvec3,FrameSizeX * FrameSizeY> FrameBuffer;
	Shader vertex;
	Shader fragment;
	ShaderProgram program_sw;
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
public:
	RenderEngineSWStepped(GLFWwindow* window);
	virtual ~RenderEngineSWStepped();
	virtual void RenderWorld(World& world) override;
	void StepRay(Ray& ray, World& grid,int Reflections = 0);
	glm::fvec3 SunRay(glm::fvec3 pos, World& grid);
	glm::fvec3 & GetPixel(int x, int y);
};

