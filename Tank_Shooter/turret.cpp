#include <vector>
#include <string>

#include "core/engine.h"
#include "utils/gl_utils.h"

#include "turret.h"

using namespace std;


Turret::Turret() {

}

Turret::~Turret() {

}

Turret::Turret(float angle)
{
	angleX = angle;
}

void Turret::init(float angle)
{
	angleX = angle;
}