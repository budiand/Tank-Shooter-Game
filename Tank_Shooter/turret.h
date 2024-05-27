#pragma once

#include "utils/glm_utils.h"

class Turret {
public:
    float angleX;

public:
    Turret();
    ~Turret();
    Turret(float angle);
    void init(float angle);
};
