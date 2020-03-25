#pragma once
#include <glm/glm.hpp>
class Camera
{
public:
	glm::fvec3 Position = glm::fvec3(0,10,10);
	float yaw = 0;
	float tilt = -45;
	float FoV = 60;
};

