/* Autor: Datcu Andrei Daniel - 331CC
 * Obstacle
 * Clasa mostetina de toate obstacolele
 */

#include "obstacle.h"
#include "../Framework/Transform3D.h"

Obstacle::Obstacle(Point3D highestPoint, bool mobile): 
	highestPoint(highestPoint), initialHighestPoint(0, 0, 0), speed(0),
	acceleration(10), maxSpeed(120), decceleration(1), mobile(mobile){
}

void Obstacle::moveDown(){

	initialHighestPoint.z -= speed;
	aabb.first.z -= speed;
	aabb.second.z -= speed;
}

void Obstacle::moveUp(){
	initialHighestPoint.z += 0.7  * maxSpeed;
	aabb.first.z += 0.7  * maxSpeed;
	aabb.second.z += 0.7  * maxSpeed;
}

void Obstacle::animate(){
	Transform3D::loadIdentityModelMatrix();
	Transform3D::translateMatrix(0, 0, initialHighestPoint.z);
	Transform3D::applyTransform(this);
}

float Obstacle::getTopZ(){
	return highestPoint.z + initialHighestPoint.z;
}

void Obstacle::accelerate(){
	if (speed < maxSpeed)
		speed += acceleration;
}

void Obstacle::deccelerate(){
	if (speed > 0)
		speed -= decceleration;
	if (speed < 0)
		speed = 0;
}
