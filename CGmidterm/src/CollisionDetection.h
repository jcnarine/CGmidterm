#pragma once
#pragma once

#include <memory>
#include "GameObject.h"
#include <GLM/glm.hpp>
#include <string>
using namespace std;

class CollisionDetection
{

public:

	static void CheckCollision(GameObject& one, GameObject& two);
	static bool CheckSphereCollision(GameObject& one, GameObject& two);
	//Sphere - Sphere collision
	static bool CheckWallCollision(GameObject& one, GameObject& two); // Sphere - AABB collision
};





