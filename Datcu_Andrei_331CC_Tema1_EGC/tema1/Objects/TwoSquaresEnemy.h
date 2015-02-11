/* Autor: Datcu Andrei Daniel - 331CC
 * TwoSquaresEnemy
 * Dusman tip Patrat peste Patrat, asa cum este desenat in enuntul temei
 */

#pragma once
#include "enemy.h"
class TwoSquaresEnemy :
	public Enemy
{
public:
	TwoSquaresEnemy(Point2D bottomLeftBoudary, Point2D topRightBoundary, 
		Point2D topSquareBottomLeft, float length);


protected:
	bool extraCheckHittingTheWall();
	void defineObject();

private:
	Point2D topSquareBottomLeft;
	float length;
};

