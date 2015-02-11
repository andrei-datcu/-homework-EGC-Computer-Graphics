/* Autor: Datcu Andrei Daniel - 331CC
 * ArrowEnemy
 * Dusman tip Polizor, asa cum este desenat in enuntul temei
 * Acest dusman are o animatie de rotatie in plus, prin care isi roteste
 * "lamele"
 */

#pragma once
#include "enemy.h"

class CircularSawEnemy : public Enemy{
public:
	CircularSawEnemy(Point2D bottomLeftBoudary, Point2D topRightBoundary, 
		Point2D coreCenter, float coreRadius);

	void animateMe();

protected:
	bool extraCheckHittingTheWall();
	void defineObject();

private:
	Point2D coreCenter;
	float coreRadius;
	float bladesAngle;
};

