/* Autor: Datcu Andrei Daniel - 331CC
 * LifeBonus
 * Obiectul tip bonus care ofera o viata in plus
 * Cerc alb cu o cruce verde in centru
 */

#include "LifeBonus.h"
#include "../Framework/Circle2D.h"
#include "../Framework/Line2D.h"


LifeBonus::LifeBonus(Point2D center, float radius):Bonus(1), 
	circleCenter(center), radius(radius){

	defineObject();
	computeConvexHull();
	reset(); 
}


void LifeBonus::defineObject(){
	
	stdCircularVelocity = 0;
	stdLinearVelocity = 0;
	initialCenter = circleCenter;

	push_back(Circle2D(circleCenter, radius, Color(1, 1, 1), false));

	float rfactor = 2.0 / 3;

	Color green(0, 0.8, 0);

	push_back(Line2D(Point2D(circleCenter.x, circleCenter.y + rfactor *radius),
		Point2D(circleCenter.x, circleCenter.y - rfactor * radius), green));

	push_back(Line2D(Point2D(circleCenter.x - rfactor *radius, circleCenter.y),
		Point2D(circleCenter.x + rfactor * radius, circleCenter.y), green));
}
