/* Autor: Datcu Andrei Daniel - 331CC
 * PyramidObstacle
 * Obstacolul tip piramida
 */

#pragma once
#include "obstacle.h"
class PyramidObstacle : public Obstacle{
public:
	PyramidObstacle(Point3D bottomLeft, float width, bool mobile);
};

