#include "CollisionDetection.h"

//Referenced from https://learnopengl.com/In-Practice/2D-Game/Collisions/Collision-detection
//Referenced from https://stackoverflow.com/questions/27342362/how-to-create-a-circle-collision-detection-c/27342421


   bool CollisionDetection::CheckSphereCollision(GameObject& one, GameObject& two)
{
     float distance = sqrt((two.position.x - one.position.x) * (two.position.x - one.position.x) + (two.position.y - one.position.y) * (two.position.y - one.position.y));

     if (distance > one.radius + two.radius)
     {
         return true;
     }else {
        return false;
     }
}

//Circle-One, Box-Two
  bool CollisionDetection::CheckWallCollision(GameObject& one, GameObject& two)
 {
     // get center point circle first 
     glm::vec2 center(one.position + one.radius);
     // calculate AABB info (center, half-extents)
     glm::vec2 aabb_half_extents(two.size.x / 2.0f, two.size.y / 2.0f);
     glm::vec2 aabb_center(
         two.position.x + aabb_half_extents.x,
         two.position.y + aabb_half_extents.y
     );
     // get difference vector between both centers
     glm::vec2 difference = center - aabb_center;
     glm::vec2 clamped = glm::clamp(difference, -aabb_half_extents, aabb_half_extents);
     // add clamped value to AABB_center and we get the value of box closest to circle
     glm::vec2 closest = aabb_center + clamped;
     // retrieve vector between center circle and closest point AABB and check if length <= radius
     difference = closest - center;
     return glm::length(difference) < one.radius;
 }

 void CollisionDetection::CheckCollision(GameObject& one, GameObject& two){

 if (two.tag=="wall"){

     bool collide = CheckWallCollision(one, two);
     const Transform::sptr& temp = *one.transform;
     if (one.tag == "player" && collide == true){
     temp->SetLocalPosition(glm::vec3(0,0,0));
     }
     if (one.tag == "puck" && collide == true) {
     temp->SetLocalPosition(glm::vec3(0, 0, 0));
     }
 }

 if (two.tag=="puck"){
     bool collide= CheckSphereCollision(one, two);
     const Transform::sptr& temp = *one.transform;
     const Transform::sptr& temp2 = *two.transform;
     if (collide){
         temp->SetLocalPosition(glm::vec3(0, 0, 0));
         temp2->SetLocalPosition(glm::vec3(0, 0, 0));
	     }
    }
 }

 float clamp(float value, float min, float max) {
	 return std::max(min, std::min(max, value));
 }

