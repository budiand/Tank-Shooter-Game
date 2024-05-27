#include <vector>
#include <string>

#include "core/engine.h"
#include "utils/gl_utils.h"

#include "projectile.h"
#include "building.h"

using namespace std;

Projectile::Projectile() {

}

Projectile::~Projectile() {

}

Projectile::Projectile(float x, float y, float z, float angle, float radius, float ttl, GLboolean exists)
{
    this->x = x;
    this->y = y;
    this->z = z;
    this->angle = angle;
    this->radius = radius;
    this->ttl = ttl;
    this->exists = exists;
}

void Projectile::init(float x, float y, float z) {
    this->x = x;
    this->y = y;
    this->z = z;
}

void Projectile::checkCollisionWithBuilding(Building& building) {
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
            exists = false;
        }
    }
}
