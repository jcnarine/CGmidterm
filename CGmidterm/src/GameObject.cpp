#include "GameObject.h"

GameObject::GameObject(glm::vec3 s, const Transform::sptr& t, std::string name)
{
	position = t->GetLocalPosition();
	transform = &t;
	tag = name;

	if (name == "paddle")
	{
		size = s;
		radius = 10;
	}
	else if (name == "puck")
	{
		size = s;
		radius = 5;
	}
}

GameObject::GameObject(glm::vec3 s, glm::vec3 p, std::string t){

	position = p;
	tag = t;

	 if (tag == "wall")
	 {
		size = s;
	 }
}

void GameObject::UpdatePosition()
{
    Transform::sptr temp = *transform;
	position = temp->GetLocalPosition();
}
