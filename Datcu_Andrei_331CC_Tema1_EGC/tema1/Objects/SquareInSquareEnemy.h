/* Autor: Datcu Andrei Daniel - 331CC
 * SquareInSquareEnemy
 * Dusman tip Patrat in Patrat, asa cum este desenat in enuntul temei
 */

#pragma once
#include "enemy.h"
class SquareInSquareEnemy : public Enemy{
public:
	SquareInSquareEnemy(Point2D bottomLeftBoudary, Point2D topRightBoundary, 
		Point2D bottomLeft, float length);

protected:
	bool extraCheckHittingTheWall();
	void defineObject();

private:
	Point2D bottomLeft;
	float length;
};

