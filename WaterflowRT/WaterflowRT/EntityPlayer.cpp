#include "EntityPlayer.h"
void EntityPlayer::UpdateInputs()
{
	if (PlayerInput.W)
	{
		PositionOld -= MoveSpeed * glm::fvec3(cos(Rotation.z),sin(Rotation.z),0);
	}
	if (PlayerInput.S)
	{
		PositionOld -= MoveSpeed * glm::fvec3(cos(Rotation.z),sin(Rotation.z),0);
	}
	RotationOld.z -= LookSpeed * PlayerInput.dx;
	RotationOld.y -= LookSpeed * PlayerInput.dy;
}
void EntityPlayer::Update()
{
	Entity::Update();
	UpdateInputs();
	RotationOld += 0.9f * (Rotation - RotationOld);
}
