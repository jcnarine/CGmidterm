#include "Puck.h"

Puck::Puck()
{
}

Puck::Puck(Transform::sptr& t, float m){

transform = t;
mass = 1.5;

}

Puck::~Puck()
{
}

void Puck::movePuck()
{
	//Calculation is done for acceleration
	glm::fvec3 initialAcceleration = force / mass;

	//Calculate maginitude
	glm::vec3 Magnitude = (sqrt(transform->GetLocalPosition()));

	//Calculate velocity
	newVelocity = initialVelocity + (initialAcceleration * deltaTime);

	//Friction Check Logic
	force.x *= friction;
	force.y *= friction;

	newVelocity.x *= friction;
	newVelocity.y *= friction;

	//Calculates the position
	glm::vec3 position = transform->GetLocalPosition() + (glm::vec3(newVelocity.x, newVelocity.y, newVelocity.z) * deltaTime) + (glm::vec3(initialAcceleration.x, initialAcceleration.y, initialAcceleration.z) * (0.5f) * (deltaTime * deltaTime));

	//set the local position of the second 
	transform->SetLocalPosition(position.x, position.y, position.z);
}

void Puck::respawnPuck(playerTag p)
{
	if (p== playerTag::PLAYER_ONE){
		transform->SetLocalPosition(1.0f, 0.0f, 4.45f);
	}
	else if (p == playerTag::PLAYER_TWO) {
		transform->SetLocalPosition(-1.0f, 0.0f, 4.45f);
	}
}