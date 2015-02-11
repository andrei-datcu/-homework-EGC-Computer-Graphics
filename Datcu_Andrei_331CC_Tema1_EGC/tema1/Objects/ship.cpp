/* Autor: Datcu Andrei Daniel - 331CC
 * Ship
 * Clasa pentru nava
 * Pe langa ComplexObject2D, prezinta metode pentru ascunderea/aratarea
 * armei, pentru verificarea daca un punct este "pe" arma, pentru 
 * setarea/resetarea invincibilitatii
 */

#include "ship.h"
#include "../Framework/Polygon2D.h"
#include "../Framework/Circle2D.h"
#include "../Framework/Point2D.h"
#include "../Framework/Transform2D.h"
#include "../Framework/Rectangle2d.h"
#include "../Framework/Common.h"

Ship::Ship(Point2D blBoundary, Point2D trBoundary, 
		   Point2D center, float radius, Color c): 
			circleCenter(center), isGunOut(false), invinceable(false),
			radius(radius), c(c), ComplexObject2D(blBoundary, trBoundary){

	defineObject();
	computeConvexHull();
	reset();
}


void Ship::defineObject(){
	stdLinearVelocity = 4;
	stdCircularVelocity = 0.08;
	initialCenter = circleCenter;	
	const float rratio = 2.0 / 3,
		gunRatio = rratio * 0.98, gunSpaceRatio = 1.3, gunSizeRatio = 2;
	push_back(Circle2D(circleCenter, radius, c, false));

	Polygon2D inPoly(c, false);

	inPoly.addPoint(Point2D(circleCenter.x + rratio * radius, circleCenter.y));
	inPoly.addPoint(Point2D(circleCenter.x + 0.8 * rratio * radius,
		circleCenter.y + 0.9 * rratio * radius));

	inPoly.addPoint(Point2D(circleCenter.x + 0.75 * rratio * radius,
		circleCenter.y + 0.2 * rratio * radius));
	inPoly.addPoint(Point2D(circleCenter.x,
		circleCenter.y - 0.3 * rratio * radius));
	inPoly.addPoint(Point2D(circleCenter.x - 0.75 * rratio * radius,
		circleCenter.y + 0.2 * rratio * radius));
	inPoly.addPoint(Point2D(circleCenter.x - 0.8 * rratio * radius,
		circleCenter.y + 0.9 * rratio * radius));
	inPoly.addPoint(Point2D(circleCenter.x - rratio * radius, circleCenter.y));
	inPoly.addPoint(Point2D(circleCenter.x, circleCenter.y - rratio * radius));

	push_back(inPoly);

	gun.color = c;
	gun.fill = true;

	gun.addPoint(Point2D(circleCenter.x - gunRatio * radius,
		circleCenter.y + gunSpaceRatio * radius));
	gun.addPoint(Point2D(circleCenter.x + gunRatio * radius,
		circleCenter.y + gunSpaceRatio * radius));
	gun.addPoint(Point2D(circleCenter.x, circleCenter.y + 
		(gunSizeRatio + gunSpaceRatio) * radius));
}

void Ship::pullGun(){
	push_back(gun);
	computeConvexHull();
	linearVelocity = stdLinearVelocity / 2;
	maxLinearVelocity = 3 * stdLinearVelocity;
	animateMe();
	if (checkBoundaries(currentCenter, currentAngle))
		return;
	retractGun();
}

void Ship::retractGun(){
	pop_back();
	computeConvexHull();
	linearVelocity = stdLinearVelocity;
	maxLinearVelocity = 6 * stdLinearVelocity;
	isGunOut = false;
}

void Ship::changeGunStatus(){
	isGunOut = !isGunOut;
	if (isGunOut)
		pullGun();
	else
		retractGun();
}

bool Ship::checkIfPointIsInGun(const Point2D &p){
	
	if (isInvinceable())
		return true;
	
	if (!isGunOut)
		return false;

	Point2D cg; //centru de greutate arma

	for (Point2D *gp : at(size() - 1).transf_points){
		cg.x += gp->x;
		cg.y += gp->y;
	}

	cg.x /= at(size() - 1).transf_points.size();
	cg.y /= at(size() - 1).transf_points.size();

	Point2D *cp, *np;

	for (int i = 0; i < at(size() - 1).transf_points.size() - 1; ++i){
		cp = at(size() - 1).transf_points[i];
		np = at(size() - 1).transf_points[
			(i + 1) % at(size() - 1).transf_points.size()];
		
		if (det(*cp, *np, cg) * truncf(det(*cp, *np, p)) < 0 - FLT_EPSILON)
			return false;
	}
	return true;
}

bool Ship::isInvinceable(){
	return invinceable;
}

void Ship::makeInvinceable(){

	invinceable = true;
	for (Object2D &o : *this)
		o.color = Color(1, 0, 0);
	gun.color = Color(1, 0, 0);
}

void Ship::unmakeInvinceable(){

	invinceable = false;
	for (Object2D &o : *this)
		o.color = c;

	gun.color = c;
}
