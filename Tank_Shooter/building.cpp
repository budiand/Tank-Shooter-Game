#include <vector>
#include <string>

#include "core/engine.h"
#include "utils/gl_utils.h"

#include "building.h"

using namespace std;

Building::Building() {

}

Building::~Building() {

}

Building::Building(float x, float y, float z, float angle, float sx, float sy, float sz)
{
    this->x = x;
    this->y = y;
    this->z = z;
	this->angle = angle;
    this->sx = sx;
    this->sy = sy;
    this->sz = sz;
}