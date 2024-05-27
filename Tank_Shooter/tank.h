#pragma once

#include "utils/glm_utils.h"
#include "building.h"
#include "turret.h"

class Tank {
public:
    float x, y, z;
    float angle;
    float radius; // bot_radius
    float top_radius; // top turret
    int hp;
    GLboolean broken;

    Turret turret;
    
public:
    Tank();
    ~Tank();
    Tank(float x, float y, float z, float angle);
    void init(float x, float y, float z, float angle, float radius, float top_radius, int hp, GLboolean broken);
    void set_turret(Turret turret);
    void checkCollisionWithBuilding(Building& building);
};
