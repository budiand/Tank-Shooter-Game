#pragma once

#include "utils/glm_utils.h"

class Building {
public:
    float x, y, z;
    float angle, sx, sy, sz;

public:
    Building();
    ~Building();
    Building(float x, float y, float z, float angle, float sx, float sy, float sz);
};
