/* Autor: Datcu Andrei Daniel - 331CC
 * SquareInSquareEnemy
 * Dusman tip Patrat in Patrat, asa cum este desenat in enuntul temei
 */

#include "SquareInSquareEnemy.h"
#include "../Framework/Rectangle2d.h"
#include "../Framework/Polygon2D.h"

SquareInSquareEnemy::SquareInSquareEnemy
	(Point2D bottomLeftBoudary, Point2D topRightBoundary, 
		Point2D bottomLeft, float length):
	Enemy(bottomLeftBoudary, topRightBoundary, 5), bottomLeft(bottomLeft){

	this->length = length;
	defineObject();
	computeConvexHull();
	reset();
}

void SquareInSquareEnemy::defineObject(){
	stdLinearVelocity = 1;
	stdCircularVelocity = 0.05;

	Color c(0, 1, 0);
	push_back(Rectangle2D(bottomLeft, length, length, c, false));
	initialCenter.x = bottomLeft.x + length / 2;
	initialCenter.y = bottomLeft.y + length / 2;

	Polygon2D inPoly(c, false);
	inPoly.addPoint(Point2D(initialCenter.x, bottomLeft.y + length));
	inPoly.addPoint(Point2D(bottomLeft.x,  initialCenter.y));
	inPoly.addPoint(Point2D(initialCenter.x, bottomLeft.y));
	inPoly.addPoint(Point2D(bottomLeft.x + length, initialCenter.y));
	push_back(inPoly);
}

bool SquareInSquareEnemy::extraCheckHittingTheWall(){
	const float minDistance = 1.5 * length;
	 return abs(blBoundary.x -initialCenter.x -currentCenter.x) < minDistance|| 
		 abs(blBoundary.y - initialCenter.y - currentCenter.y) < minDistance || 
		 abs(trBoundary.x - initialCenter.x - currentCenter.x) < minDistance ||
		 abs(trBoundary.y - initialCenter.y - currentCenter.y) < minDistance;
}

