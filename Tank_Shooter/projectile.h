#pragma once

#include "utils/glm_utils.h"
#include "building.h"

class Projectile {
public:
    float x, y, z;
    float angle, ttl;
    float radius;

    GLboolean exists;

public:
    Projectile();
    ~Projectile();
    Projectile(float x, float y, float z, float angle, float radius, float ttl, GLboolean exists);
    void init(float x, float y, float z);
    void checkCollisionWithBuilding(Building& building);
};
