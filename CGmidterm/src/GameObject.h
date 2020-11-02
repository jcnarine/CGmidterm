#pragma once

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <Graphics\VertexArrayObject.h>
#include <Gameplay\Transform.h>

class GameObject
{

public:
	GameObject(glm::vec3 s, const Transform::sptr& t, std::string name);
	GameObject(glm::vec3 s, glm::vec3 p, std::string t);
	void UpdatePosition();

private:
	//Vec2's to get the objects postion, size and velocity
	glm::vec3 position, size, velocity;
	const Transform::sptr* transform;
	std::string tag;
	float radius=0;

	friend class CollisionDetection;
};