#include "Movement.h"

//https://gamedev.stackexchange.com/questions/4673/how-do-i-implement-deceleration

Movement::Movement()
{
}

Movement::~Movement()
{
}

void Movement::movePlayer() {
	//Calculation is done for acceleration
	glm::fvec3 initialAcceleration = force / mass;

	//Calculate maginitude
	glm::vec3 Magnitude = (sqrt(transform->GetLocalPosition()));

	//Calculate velocity
	newVelocity = initialVelocity + (initialAcceleration * deltaTime);

	//Deceleration Check Logic
	if (isKeyPressed == false)
	{
		//If the user let's go of the keys, a forc of friction is applied 
		//On the paddles and is multiplied by the current force.
		//By doing this, the force slowly decreases to zero, and thus causes the paddles to decelerate
		force.x *= friction;
		force.y *= friction;

		//neVelocity is also multiplied by the friction. By doing this, velocity changes and causes the velocity
		//to be reset to zero (i.e the padlles have no change in position and therefore are not moving)
		newVelocity.x *= friction;
		newVelocity.y *= friction;
	}

	//Calculates the position
	glm::vec3 position = transform->GetLocalPosition() + (glm::vec3(newVelocity.x, newVelocity.y, newVelocity.z) * deltaTime) + (glm::vec3(initialAcceleration.x, initialAcceleration.y, initialAcceleration.z) * (0.5f) * (deltaTime * deltaTime));

	//TODO: Put if loops for location of position, make clamp of x, y and z

	if (player == playerTag::PLAYER_ONE) {

		position = glm::vec3(std::clamp(position.x, 1.0f, 12.63f), std::clamp(position.y, -4.710f, 4.710f), position.z);

	}
	else if (player == playerTag::PLAYER_TWO) {

		position = glm::vec3(std::clamp(position.x, -12.623f, -1.0f), std::clamp(position.y, -4.710f, 4.710f), position.z);
	}

	//set the local position of the second 
	transform->SetLocalPosition(position.x, position.y, position.z);

}

