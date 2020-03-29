#pragma once
#include <glm/glm.hpp>
class Camera
{
public:
	glm::fvec3 Position = glm::fvec3(10,10,12);
	float yaw = 0;
	float tilt = 0;
	float FoV = 60;
};

