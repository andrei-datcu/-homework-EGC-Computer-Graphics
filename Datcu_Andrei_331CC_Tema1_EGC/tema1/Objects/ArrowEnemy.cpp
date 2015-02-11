/* Autor: Datcu Andrei Daniel - 331CC
 * ArrowEnemy
 * Dusman tip Sageata, asa cum este desenat in enuntul temei
 */

#include "ArrowEnemy.h"
#include "../Framework/Polygon2D.h"


ArrowEnemy::ArrowEnemy(Point2D bottomLeftBoudary,
		Point2D topRightBoundary, Point2D bottomLeft, float length):
			Enemy(bottomLeftBoudary, topRightBoundary, 15), 
			length(length), bottomLeft(bottomLeft){

	defineObject();
	computeConvexHull();
	reset();
}

void ArrowEnemy::defineObject(){

	stdLinearVelocity = 1;
	stdCircularVelocity = 0.05;

	float width = 2 * length / 3;

	Polygon2D poly1(Color(0, 0, 1), true), poly2(Color(1, 1, 0), true);

	poly1.addPoint(bottomLeft);
	poly1.addPoint(Point2D(bottomLeft.x + width/2, bottomLeft.y + length/5));
	poly1.addPoint(Point2D(bottomLeft.x + width/2, bottomLeft.y + length));

	poly2.addPoint(Point2D(bottomLeft.x + width/2, bottomLeft.y + length));
	poly2.addPoint(Point2D(bottomLeft.x + width, bottomLeft.y));
	poly2.addPoint(Point2D(bottomLeft.x + width/2, bottomLeft.y + length/5));
	
	push_back(poly1);
	push_back(poly2);

	initialCenter.x = (3 * bottomLeft.x + 2 * width) / 3;
	initialCenter.y = (3 * bottomLeft.y + 6 * length/5) / 3;
}

bool ArrowEnemy::extraCheckHittingTheWall(){
	 const float minDistance = 1.2 * length;
	 return abs(blBoundary.x - initialCenter.x -currentCenter.x) <minDistance|| 
		 abs(blBoundary.y - initialCenter.y - currentCenter.y) < minDistance || 
		 abs(trBoundary.x - initialCenter.x - currentCenter.x) < minDistance ||
		 abs(trBoundary.y - initialCenter.y - currentCenter.y) < minDistance;

}
