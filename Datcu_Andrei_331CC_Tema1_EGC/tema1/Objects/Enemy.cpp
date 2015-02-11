/* Autor: Datcu Andrei Daniel - 331CC
 * Enemy
 * Clasa abstracta care este mostenista de orice Inamic
 * Aduce in plus metoda planNextMove(), prin care Inamicul se misca
 */

#include "Enemy.h"
#include <math.h>


#define PI 3.14159265358979323846

Enemy::Enemy(Point2D bottomLeftBoundary, Point2D topRightBoudnary, int value):
	ComplexObject2D(bottomLeftBoundary, topRightBoudnary), value(value){

	gettingAwayFromTheEdge = false;
	justRun = 0;
}

void Enemy::planNextMove(){
	if (gettingAwayFromTheEdge){
		if (gettingAwayLeft)
			rotateLeft();
		else
			rotateRight();
		if (abs(targetAngle - currentAngle) < 2 * stdCircularVelocity){ 
			// am ajuns in directia opusa
			gettingAwayFromTheEdge = false; 
			justRun = 30;
		}
		return;
	}

	Point2D lastCenter = currentCenter;
	float lastAngle = currentAngle;

	if (justRun != 0){
		moveUp();
		--justRun;
		return;
	}
	
	bool goToLeft = rand() % 2;

	if (goToLeft)
		rotateLeft();
	else
		rotateRight();

	moveUp();

	if (extraCheckHittingTheWall() || lastCenter == currentCenter ||
		lastAngle == currentAngle){ //fundatura
			targetAngle = currentAngle + (goToLeft ? -PI : PI);
			gettingAwayLeft = !goToLeft;
			gettingAwayFromTheEdge = true;
	}
}

bool Enemy::extraCheckHittingTheWall(){
	return false;
}

void Enemy::animateMe(){
	planNextMove();
	ComplexObject2D::animateMe();
}