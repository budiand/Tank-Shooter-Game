#include <vector>
#include <string>
#include <random>

#include "core/engine.h"
#include "utils/gl_utils.h"

#include "enemy.h"
#include "turret.h"
#include "building.h"
#include "projectile.h"

using namespace std;

Enemy::Enemy() {
}

Enemy::~Enemy() {

}
// basic stuff
Enemy::Enemy(float x, float y, float z, float angle, GLboolean exists, GLboolean broken)
{
    this->x = x;
    this->y = y;
    this->z = z;
    this->angle = angle;
    this->exists = exists;
    this->broken = broken;  
}

// collision + movement stuff
void Enemy::init(float top_radius, float bot_radius, float state_timer, int hp) {
    this->top_radius = top_radius;
    this->bot_radius = bot_radius;
    this->state_timer = state_timer;
    this->hp = hp;
}

void Enemy::set_turret(Turret turret) {
    this->turret = turret;
}

void Enemy::checkCollisionWithBuilding(Building& building) {
    glm::vec3 minBound = glm::vec3(building.x - building.sx / 2, building.y - building.sy / 2, building.z - building.sz / 2);
    glm::vec3 maxBound = glm::vec3(building.x + building.sx / 2, building.y + building.sy / 2, building.z + building.sz / 2);

    // Calculate closest point in AABB to tank
    glm::vec3 closestPoint = glm::clamp(glm::vec3(x, y, z), minBound, maxBound);

    // Calculate distance between closest point and tank center
    float distance = glm::distance(glm::vec3(x, y, z), closestPoint);

    if (distance < bot_radius) {
        float overlapDistance = bot_radius - distance;
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