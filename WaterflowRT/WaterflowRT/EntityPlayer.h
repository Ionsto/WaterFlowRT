#pragma once
#include "Entity.h"
struct Inputs {
	bool W, A, S, D;
	float dx, dy;
};
class EntityPlayer :
	public Entity
{
private:
	float MoveSpeed = 1;
	float LookSpeed = 1;
public:
	Inputs PlayerInput;
	virtual void Update() override;
	void UpdateInputs();
};

