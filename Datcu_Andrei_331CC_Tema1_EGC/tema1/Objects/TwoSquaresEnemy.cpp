/* Autor: Datcu Andrei Daniel - 331CC
 * TwoSquaresEnemy
 * Dusman tip Patrat peste Patrat, asa cum este desenat in enuntul temei
 */

#include "TwoSquaresEnemy.h"
#include "../Framework/Rectangle2d.h"


TwoSquaresEnemy::TwoSquaresEnemy(Point2D bottomLeftBoudary,
								 Point2D topRightBoundary, 
								 Point2D topSquareBottomLeft, float length):	
			Enemy(bottomLeftBoudary, topRightBoundary, 10), 
			topSquareBottomLeft(topSquareBottomLeft), length(length){

	defineObject();
	computeConvexHull();
	reset();
}

void TwoSquaresEnemy::defineObject(){

	stdLinearVelocity = 1;
	stdCircularVelocity = 0.05;

	Color c(1, 0, 1);
	push_back(Rectangle2D(topSquareBottomLeft, length, length, c, false));
	push_back(Rectangle2D(Point2D(topSquareBottomLeft.x + length / 2,
		topSquareBottomLeft.y - length / 2), length, length, c, false));
	initialCenter.x = topSquareBottomLeft.x + length / 2;
	initialCenter.y = topSquareBottomLeft.y;
}

bool TwoSquaresEnemy::extraCheckHittingTheWall(){
	const float minDistance = 2.2 * length;
	return abs(blBoundary.x - initialCenter.x -currentCenter.x) < minDistance|| 
		 abs(blBoundary.y - initialCenter.y - currentCenter.y) < minDistance || 
		 abs(trBoundary.x - initialCenter.x - currentCenter.x) < minDistance ||
		 abs(trBoundary.y - initialCenter.y - currentCenter.y) < minDistance;
}