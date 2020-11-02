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
	static bool CheckSphereCollision(GameObject& one, GameObject& two); //Sphere - Sphere collision
	static objectTag CheckWallCollision(GameObject& puck);
};





