#pragma once

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <Graphics\VertexArrayObject.h>
#include <Gameplay\Transform.h>

enum class  objectTag {
		LS_WALL,
		RS_WALL,
		BM_WALL,
		T_WALL,
		P1,
		P2,
		PUCK,
		NONE
};

class GameObject
{

public:
	GameObject();
	GameObject(glm::vec3 p, objectTag n);
	void UpdatePosition(glm::vec3 p) { position = p; }
	glm::vec3 GetPosition() { return position; }
	glm::vec2 GetSize() { return size; }
	float GetRadius() { return radius; }
	objectTag GetTag() { return item; }

private:
	//Vec2's to get the objects postion, size and velocity
	glm::vec3 position;
	glm:: vec2 size;
	float radius;
	objectTag item;

	friend class CollisionDetection;
};