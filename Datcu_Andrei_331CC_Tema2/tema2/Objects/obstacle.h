/* Autor: Datcu Andrei Daniel - 331CC
 * Obstacle
 * Clasa mostetina de toate obstacolele
 */

#pragma once
#include "../framework/object3d.h"
class Obstacle : public Object3D{
public:
	Obstacle(Point3D highestPoint, bool mobile);
	void moveDown();
	void moveUp();
	void animate();
	float getTopZ();
	void accelerate();
	void deccelerate();

	const bool mobile;

	std::pair<Point3D, Point3D> aabb;
private:
	float speed, maxSpeed, acceleration, decceleration;
	Point3D initialHighestPoint, highestPoint;
};