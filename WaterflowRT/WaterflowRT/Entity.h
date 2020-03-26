#pragma once
#include <glm/glm.hpp>
class World;
class Entity
{
private:
	World& world;
public:
	glm::vec3 Position;
	glm::vec3 PositionOld;
	glm::vec3 Size;
	glm::vec3 Rotation;
	glm::vec3 RotationOld;
	void Intergrate();
	virtual void Update();
};

