#include "Entity.h"

void Entity::Intergrate()
{
	auto temppos = Position;
	Position += Position - PositionOld;
	PositionOld = temppos;
}
void Entity::Update()
{
	Intergrate();
}
