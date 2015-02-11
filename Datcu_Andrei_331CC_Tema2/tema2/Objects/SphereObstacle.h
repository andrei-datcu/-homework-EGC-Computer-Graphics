/* Autor: Datcu Andrei Daniel - 331CC
 * SpehereObstacle
 * Obstacolul tip sfera
 */

#pragma once
#include "obstacle.h"
class SphereObstacle : public Obstacle{
public:
	SphereObstacle(Point3D center, bool mobile);
};

