/* Autor: Datcu Andrei Daniel - 331CC
 * Car
 * Obiect3D tip masina
 */

#pragma once
#include "..\framework\object3d.h"
#include "obstacle.h"
class Car :
	public Object3D
{
public:
	Car(Point3D leftBackWheelCenter, float track, float wheelbase, 
		float minx, float maxx);
	void rotateLeft();
	void rotateRight();
	void straightenUp();
	void animate();
	void accelerate();
	bool move();
	void deccelerate();
	bool intersects(Obstacle &o);

private:
	Point3D center, currentCenter;
	float speed;
	float leftx, rightx, maxx, minx;
	std::vector<Point3D*> bl, br, tl, tr; //cele 4 roti transformate
	float currentAngle;
	std::pair<Point3D, Point3D> aabb; //axis aligned bounding boxes
};

