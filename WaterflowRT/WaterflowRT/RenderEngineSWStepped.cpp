#include "RenderEngineSWStepped.h"
#include <algorithm>
#include <glm/gtx/rotate_vector.hpp>
#include <glm/gtx/component_wise.hpp>
RenderEngineSWStepped::RenderEngineSWStepped(GLFWwindow* window) : RenderEngine(window)
{
	//Create shaders
	std::cout << "Init shaders" << std::endl;
	vertex.Init("quad.vert", GL_VERTEX_SHADER);
	fragment.Init("quad.frag", GL_FRAGMENT_SHADER);
	std::cout << "Creating program" << std::endl;
	program_sw.CreateProgram();
	program_sw.AddShader(vertex);
	program_sw.AddShader(fragment);
	program_sw.LinkProgram();
	program_sw.UseProgram();
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

	glGenTextures(3, TextureFrameBuffer);
	for (int b = 0; b < 3; ++b)
	{
		glBindTexture(GL_TEXTURE_2D, TextureFrameBuffer[b]);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, FrameSizeX, FrameSizeY, 0, GL_RGB, GL_FLOAT, &FrameBuffer[0]);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	}
	glUniform1i(glGetUniformLocation(program_sw.GetProgram(), "Texture0"), 0);
	glUniform1i(glGetUniformLocation(program_sw.GetProgram(), "Texture1"), 1);
	glUniform1i(glGetUniformLocation(program_sw.GetProgram(), "Texture2"), 2);

}
RenderEngineSWStepped::~RenderEngineSWStepped()
{

}
void RenderEngineSWStepped::RenderWorld(World& world)
{
#pragma omp parallel for
	for (int vx = 0; vx < FrameSizeX; ++vx)
	{
		for (int vy = 0; vy < FrameSizeY; ++vy)
		{
			float dyaw = ((static_cast<float>(vx) / static_cast<float>(FrameSizeX)) - 0.5) * cam.FoV;
			float dtilt = ((static_cast<float>(vy) / static_cast<float>(FrameSizeY)) - 0.5) * cam.FoV * static_cast<float>(FrameSizeY)/static_cast<float>(FrameSizeX);
			Ray ray;
			ray.Origin = cam.Position;
			ray.Direction = glm::rotateZ(glm::rotateY(glm::fvec3(1, 0, 0), glm::radians(static_cast<float>(-cam.tilt + dtilt))), glm::radians(static_cast<float>(cam.yaw + dyaw)));
			//Step the ray	
			StepRay(ray, world);
			//idk
			GetPixel(vx, vy).x = ray.Colour.x;
			GetPixel(vx, vy).y = ray.Colour.y;
			GetPixel(vx, vy).z = ray.Colour.z;
		}
	}
	CurrentBuffer = 0;
	glBindTexture(GL_TEXTURE_2D, TextureFrameBuffer[CurrentBuffer]);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, FrameSizeX, FrameSizeY, 0, GL_RGB, GL_FLOAT, &FrameBuffer[0]);
	//CurrentBuffer = (CurrentBuffer + 1) % 3;

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, TextureFrameBuffer[0]);
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, TextureFrameBuffer[1]);
	glActiveTexture(GL_TEXTURE2);
	glBindTexture(GL_TEXTURE_2D, TextureFrameBuffer[2]);

	program_sw.UseProgram();
	glBindVertexArray(FB_VAO);
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);
}
glm::fvec3 RenderEngineSWStepped::SunRay(glm::fvec3 pos, World& world)
{
	glm::fvec3 SunDirection = world.SunPosition - pos;
	int SunSteps = floor(glm::length(SunDirection) / StepSize);
	//float AjustedStepSize = glm::length(SunDirection) / SunSteps;
	auto Step = (glm::normalize(SunDirection)) * (float)StepSize;
	auto currentpos = world.SunPosition;
	glm::fvec3 Colour = world.SunColour;
	for (int i = 0; i < SunSteps; ++i)
	{
		if(world.grid.InBounds(currentpos.x, currentpos.y, currentpos.z))
		{
			auto vox = world.grid.Get(currentpos.x, currentpos.y, currentpos.z);
			if (vox.Solid)
			{
				Colour.r = 0;
				Colour.g = 0;
				Colour.b = 0;
			}
			else {
				Colour -= (vox.Colour * vox.Density * StepSize);
				Colour -= (glm::fvec3(0.9,0.1,0) * vox.WaterContent * StepSize);
			} 
			if (glm::dot(Colour, Colour) < 0.0001)
			{
				//We're adding 0 colour to the world
				break;
			}
		}
		//perposly skip first intersectiron
		currentpos -= Step;
	}
	Colour.x = std::min(std::max(Colour.x,0.0f),1.0f);
	Colour.y = std::min(std::max(Colour.y,0.0f),1.0f);
	Colour.z = std::min(std::max(Colour.z,0.0f),1.0f);
	return Colour;
}
void RenderEngineSWStepped::StepRay(Ray& ray, World& world,int Reflections)
{
	auto& grid = world.grid;
	int i;
	for (i = 0; i < MaxSteps; ++i)
	{
		if (!grid.InBounds(ray.Origin.x,ray.Origin.y,ray.Origin.z))
		{
			ray.Colour.b += 1;
			ray.Colour += world.SunColour * std::powf(std::min(std::max(glm::dot(ray.Direction,glm::normalize(world.SunPosition - ray.Origin)),0.0f),1.0f),6);
			break;
		}
		Voxel & vox = grid.Get(ray.Origin.x, ray.Origin.y, ray.Origin.z);
		//Sample to light source 
		if (!vox.Solid)
		{
			ray.Colour -= (vox.Colour * vox.Density * StepSize);// *lighting;
			ray.Colour += (glm::fvec3(0,0,0.1) * vox.WaterContent * StepSize);
			//ray.Colour += vox.ColourScatter * vox.Density * StepSize;
			//Refraction time
			auto nextorigin = ray.Origin + ray.Direction * StepSize;
			if (grid.InBounds(nextorigin.x, nextorigin.y, nextorigin.z))
			{
				float rnda = ((rand() % 100) / 50.0);
				float rndb = ((rand() % 100) / 50.0);
				rnda = 1;
				rndb = 1;
				float dwater = ((vox.WaterContent * rnda - grid.Get(nextorigin.x, nextorigin.y, nextorigin.z).WaterContent) * rndb);// * ((rand()%100)/50.0)-1)*0.01;
				glm::fvec3 normal = glm::floor(nextorigin) - glm::floor(ray.Origin);
				if (RenderConfig.Refraction)
				{
					float effect = dwater * 0.1;
					ray.Direction = ((1.0f - effect) * ray.Direction) + (effect * normal);
				}
				if(abs(dwater) > 0.5 && RenderConfig.Reflections && Reflections == 0)
				{
					Ray refl;
				refl.Origin = ray.Origin;
					refl.Direction = ray.Direction - (normal * glm::dot(normal,ray.Direction) * 2.0f);
					StepRay(refl, world,Reflections+1);
					auto lighting = refl.Colour;
					//auto lighting = SunRay(ray.Origin, world);
					ray.Colour += lighting * 0.5f;
				}
			}
			ray.Origin += ray.Direction * StepSize;
		}
		else
		{
			auto lighting = SunRay(ray.Origin, world);
			ray.Colour += vox.Colour * lighting;
			break;
		}
	}
	if (i == MaxSteps)
	{
			ray.Colour.b += 1;
	}
	ray.Colour.x = std::min(std::max(ray.Colour.x,0.0f),1.0f);
	ray.Colour.y = std::min(std::max(ray.Colour.y,0.0f),1.0f);
	ray.Colour.z = std::min(std::max(ray.Colour.z,0.0f),1.0f);
}

glm::fvec3 & RenderEngineSWStepped::GetPixel(int x, int y)
{
	if (x >= 0 && x < FrameSizeX)
	{
		if (y >= 0 && y < FrameSizeY)
		{
			return FrameBuffer[x + (y * FrameSizeX)];
		}
	}
}
