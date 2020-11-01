#pragma once

#include <glad/glad.h>
#include <glm/glm.hpp>



class GameObject
{

public:
	//Constructor 
	GameObject();
	GameObject(glm::vec2 pos, glm::vec2 size, glm::vec2 vel);

	
	
	struct ObjectProperties {
	

	//Vec2's to get the objects postion, size and velocity
	glm::vec2 postion, size, velocity;
	
	};
	

private:







};

