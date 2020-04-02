#pragma once
#include <glm/glm.hpp>
class World;
class Entity
{
private:
	World& world;
public:
	float DirectionCounter = 0;
	float Damping = 0.01;
	glm::vec3 Direction = glm::vec3(0);
	glm::vec3 Acceleration = glm::vec3(0);
	glm::vec3 Position = glm::vec3(0);
	glm::vec3 PositionOld = glm::vec3(0);
	glm::vec3 Size = glm::vec3(0);
	glm::vec3 Rotation = glm::vec3(0);
	glm::vec3 RotationOld = glm::vec3(0);
	glm::vec3 Colour = glm::vec3(0.5,0.4,0.5);
	Entity(World & world);
	virtual ~Entity();
	void UpdateCollisions();
	void Intergrate();
	virtual void Update();
};

