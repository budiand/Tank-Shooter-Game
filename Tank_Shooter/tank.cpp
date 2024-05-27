#include <vector>
#include <string>

#include "core/engine.h"
#include "utils/gl_utils.h"

#include "tank.h"
#include "building.h"
#include "turret.h"

using namespace std;

Tank::Tank() {
    
}

Tank::~Tank() {

}

Tank::Tank(float x, float y, float z, float angle) 
{
    this->x = x;
    this->y = y;
    this->z = z;
    this->angle = angle;
}

void Tank::init(float x, float y, float z, float angle, float radius, float top_radius, int hp, GLboolean broken) {
    this->x = x;
    this->y = y;
    this->z = z;
    this->angle = angle;
    this->radius = radius;
    this->top_radius = top_radius;
    this->hp = hp;
    this->broken = broken;
}

void Tank::set_turret(Turret turret) {
    this->turret = turret;
}

void Tank::checkCollisionWithBuilding(Building& building) {
    glm::vec3 minBound = glm::vec3(building.x - building.sx / 2, building.y - building.sy / 2, building.z - building.sz / 2);
    glm::vec3 maxBound = glm::vec3(building.x + building.sx / 2, building.y + building.sy / 2, building.z + building.sz / 2);

    // Calculate closest point in AABB to tank
    glm::vec3 closestPoint = glm::clamp(glm::vec3(x, y, z), minBound, maxBound);

    // Calculate distance between closest point and tank center
    float distance = glm::distance(glm::vec3(x, y, z), closestPoint);

    if (distance < radius) {
        float overlapDistance = radius - distance;
        if (overlapDistance > 0) {
            // Calculate penetration direction
            glm::vec3 direction = glm::normalize(glm::vec3(x, y, z) - closestPoint);

            // Calculate adjustment vector
            glm::vec3 adjustment = overlapDistance * direction;

            // Move the tank away from the building
            x += adjustment.x;
            y += adjustment.y;
            z += adjustment.z;
        }
    }
}