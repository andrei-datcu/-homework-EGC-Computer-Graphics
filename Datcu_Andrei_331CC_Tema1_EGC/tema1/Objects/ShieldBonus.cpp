/* Autor: Datcu Andrei Daniel - 331CC
 * ShieldBonus
 * Obiectul tip bonus care ofera invincibilitate
 * Cerc gri cu scut gri in mijloc
 */

#include "ShieldBonus.h"
#include "../Framework/Circle2D.h"
#include "../Framework/Polygon2D.h"


ShieldBonus::ShieldBonus(Point2D center, float radius):Bonus(2), 
	circleCenter(center), radius(radius){

	defineObject();
	computeConvexHull();
	reset(); 
}


void ShieldBonus::defineObject(){
	
	stdCircularVelocity = 0;
	stdLinearVelocity = 0;
	initialCenter = circleCenter;

	Color grey(0.75, 0.75, 0.75);

	push_back(Circle2D(circleCenter, radius, grey, false));

	float rfactor = 1.6 / 3;

	Polygon2D shield(grey, false);

	shield.addPoint(Point2D(circleCenter.x + rfactor *radius, circleCenter.y));
	shield.addPoint(Point2D(circleCenter.x + rfactor * radius,
		circleCenter.y + 1.2 * rfactor * radius));
	shield.addPoint(Point2D(circleCenter.x - rfactor * radius,
		circleCenter.y + 1.2 * rfactor * radius));
	shield.addPoint(Point2D(circleCenter.x - rfactor *radius, circleCenter.y));
	shield.addPoint(Point2D(circleCenter.x,
		circleCenter.y - 1.2 * rfactor * radius));

	push_back(shield);
}
