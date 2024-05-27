#pragma once

#include "utils/glm_utils.h"
#include "turret.h"
#include "building.h"
#include "projectile.h"
#include <random>

class Enemy {
public:
    float x, y, z;
    float angle;
    float top_radius, bot_radius;

    int state;
    float state_timer;

    GLboolean exists, broken;

    std::vector<Projectile> projectiles;
    float projectile_time;

    Turret turret;
    int hp;

public:
    Enemy();
    ~Enemy();
    Enemy(float x, float y, float z, float angle, GLboolean exists, GLboolean broken);

    void init(float top_radius, float bot_radius, float state_timer, int hp);
    void set_turret(Turret turret);
    
    void checkCollisionWithBuilding(Building& building);
};
