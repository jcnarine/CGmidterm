#pragma once
#pragma once

#include <memory>
#include <GLM/glm.hpp>
#include <string>
using namespace std;




class CollisionDetection
{

public:

	//Struct created of the class that will apply tag's to each object
	struct collisionTags
	{
		//String variable declared that can be appplied to each of the game objects
		string collTag;

	};

	CollisionDetection();
	~CollisionDetection();

	bool CheckSphereCollision(); //Sphere - Sphere collision
	
	bool CheckWallCollision();// Sphere - AABB collision


	
	


private:

	//Member Variables
	int radius;
	int size;

	//Vectors
	glm::vec2 center;
	glm::vec2 difference;


	

	


};





