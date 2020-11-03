#pragma once
#include "Movement.h"

class Puck :public Movement
{
	public:

	Puck();
	Puck(Transform::sptr& t, float m);
	~Puck();

	void movePuck();
	void respawnPuck(playerTag p);
	glm::vec3 getVelocity() { return initialVelocity; };
	void setVelocity(glm::vec3 v){ initialVelocity = v; };
};

