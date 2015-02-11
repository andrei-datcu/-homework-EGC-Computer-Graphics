/* Autor: Datcu Andrei Daniel - 331CC
 * ArrowEnemy
 * Dusman tip Polizor, asa cum este desenat in enuntul temei
 * Acest dusman are o animatie de rotatie in plus, prin care isi roteste
 * "lamele"
 */

#include "CircularSawEnemy.h"
#include "../Framework/Circle2D.h"
#include "../Framework/Polygon2D.h"
#include "../Framework/Transform2D.h"


CircularSawEnemy::CircularSawEnemy(Point2D bottomLeftBoudary,
		Point2D topRightBoundary, Point2D coreCenter, float coreRadius):
			Enemy(bottomLeftBoudary, topRightBoundary, 20), 
			coreCenter(coreCenter), coreRadius(coreRadius), bladesAngle(0){

	defineObject();
	computeConvexHull();
	reset();
}

void CircularSawEnemy::defineObject(){
	const float radiusFactor = 3;

	stdLinearVelocity = 1;
	stdCircularVelocity = 0.03;
	initialCenter = coreCenter;

	Color c(1, 0.5, 0.5);

	push_back(Circle2D(coreCenter, coreRadius, c, true));
	//cele 4 triunghiuri
	Polygon2D t1(c, false), t2(c, false), t3(c, false), t4(c, false);

	t1.addPoint(
		Point2D(coreCenter.x - 1.4 * coreRadius, coreCenter.y));
	t1.addPoint(
		Point2D(coreCenter.x - radiusFactor * 2 * coreRadius, coreCenter.y));
	t1.addPoint(Point2D(coreCenter.x - radiusFactor * 2 * coreRadius, 
		coreCenter.y + 2 * coreRadius * radiusFactor));

	t2.addPoint(
		Point2D(coreCenter.x, coreCenter.y - 1.4 * coreRadius));
	t2.addPoint(
		Point2D(coreCenter.x, coreCenter.y - radiusFactor * 2 * coreRadius));
	t2.addPoint(Point2D(coreCenter.x -  2 * radiusFactor * coreRadius,
		coreCenter.y - radiusFactor * 2 * coreRadius));
	
	t3.addPoint(
		Point2D(coreCenter.x + 1.4 * coreRadius, coreCenter.y));
	t3.addPoint(
		Point2D(coreCenter.x + radiusFactor * 2 * coreRadius, coreCenter.y));
	t3.addPoint(Point2D(coreCenter.x + radiusFactor * 2 * coreRadius, 
		coreCenter.y - 2 * coreRadius * radiusFactor));

	t4.addPoint(
		Point2D(coreCenter.x, coreCenter.y + coreRadius * 1.4));
	t4.addPoint(
		Point2D(coreCenter.x, coreCenter.y + radiusFactor * 2 * coreRadius));
	t4.addPoint(Point2D(coreCenter.x + 2 * radiusFactor * coreRadius,
		coreCenter.y + radiusFactor * 2 * coreRadius));

	push_back(t1);
	push_back(t2);
	push_back(t3);
	push_back(t4);

}

bool CircularSawEnemy::extraCheckHittingTheWall(){
	 const float minDistance = 9 * coreRadius;
	 return abs(blBoundary.x -initialCenter.x -currentCenter.x) < minDistance|| 
		 abs(blBoundary.y - initialCenter.y - currentCenter.y) < minDistance || 
		 abs(trBoundary.x - initialCenter.x - currentCenter.x) < minDistance ||
		 abs(trBoundary.y - initialCenter.y - currentCenter.y) < minDistance;

}

void CircularSawEnemy::animateMe(){

	planNextMove();
	bladesAngle += 5 * stdCircularVelocity;
	Transform2D::loadIdentityMatrix();
	Transform2D::translateMatrix(-initialCenter.x, -initialCenter.y);
	Transform2D::rotateMatrix(cos(bladesAngle), sin(bladesAngle));
	Transform2D::translateMatrix(initialCenter.x, initialCenter.y);
	Transform2D::translateMatrix(currentCenter.x, currentCenter.y);

	Transform2D::applyTransform(this);

	animateCH();
}
