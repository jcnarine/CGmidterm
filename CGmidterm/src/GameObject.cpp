#include "GameObject.h"

GameObject::GameObject()
{
	//Default Constructor
}

GameObject::GameObject(glm::vec2 pos, glm::vec2 size, glm::vec2 vel)
{
	//Instance of struct created. 'puck variable will get the data of the pucks position, size and velocity
	ObjectProperties puck{ pos, size, vel };


	
}
