/* Autor: Datcu Andrei Daniel - 331CC
 * ComplexObject2D
 * Clasa abstracta care defineste practic o multime de obiecte Object2D
 * aka o multime de poligoane convex neconectate intre ele
 */

#include "ComplexObject2D.h"
#include "Common.h"
#include <tuple>
#include <vector>
#include <algorithm>
#include "Transform2D.h"
#include <iostream>

ComplexObject2D::ComplexObject2D(Point2D bottomLeftBoundary, 
								 Point2D topRightBoudnary):
	std::vector<Object2D>(), convexHull(), transf_convexHull(),
	blBoundary(bottomLeftBoundary), trBoundary(topRightBoudnary){
}

bool ComplexObject2D::intersects(ComplexObject2D &obj, Point2D &result){

	/**
	  * Metoda care verifica daca obiectul curent se intersecteaza cu un alt
	  * obiect. Daca intoarce true, atunci punctul intersectie este salvat
	  * in result
	**/

	Point2D cc = initialCenter + currentCenter;
	Point2D oc = obj.initialCenter + obj.currentCenter;

	if (eucDistance(cc, oc) > (colisionRadius + obj.colisionRadius))
		return false;

	for (int i = 0; i < obj.transf_convexHull.size(); ++i)
		for (int j = 0; j < transf_convexHull.size(); ++j)
			if (getLineIntersection(obj.transf_convexHull[i],
					obj.transf_convexHull[(i + 1) %
						obj.transf_convexHull.size()], transf_convexHull[j],
					transf_convexHull[(j + 1) % transf_convexHull.size()],
					result))
				
				return true;
	
	return false;
}

void ComplexObject2D::computeConvexHull(){

	/**
	  * Metoda care calculeaza infasuratoarea convexa folosind scanrea 
	  * graham.
	**/

	std::vector <std::tuple<Point2D, float, float>> temp;

	int certainPos = 0;

	for (Object2D o: *this)
		for (Point2D *p : o.points){
			temp.push_back(std::make_tuple(*p, 0, 0));
			if (*p < std::get<0>(temp[certainPos]))
				 certainPos = temp.size() - 1;
		}

	auto aux = temp[certainPos]; 
	temp[certainPos] = temp[0];
	temp[0] = aux;

	Point2D certain = std::get<0>(temp[0]), pct;

	for (auto &e : temp){
		pct = std::get<0>(e);
		if (abs(pct.x - certain.x) <FLT_EPSILON)
			std::get<2>(e) = FLT_MAX;
		else
			std::get<2>(e) = (pct.y - certain.y) / (pct.x - certain.x);
		
		std::get<1>(e) = eucDistance(pct, certain);
	}

	std::get<2>(temp[0]) = -FLT_MAX;

	std::sort(temp.begin(), temp.end(),
		[](std::tuple<Point2D, float, float> e1,
			std::tuple<Point2D, float, float> e2){

				float p1 = std::get<2>(e1), p2 = std::get<2>(e2);
				float d1 = std::get<1>(e1), d2 = std::get<1>(e2);

				return p1 < p2 || (p1 == p2 && d1 < d2);
	});

	convexHull.clear();
	
	Point2D p = std::get<0>(temp[0]);

	convexHull.push_back(p);
	convexHull.push_back(std::get<0>(temp[1]));

	for (int i = 2; i < temp.size();)
		if (truncf(det(convexHull[convexHull.size() - 2],
				convexHull[convexHull.size() - 1], std::get<0>(temp[i]))) < 0)

			convexHull.pop_back();
		else
			convexHull.push_back(std::get<0>(temp[i++]));
	
	transf_convexHull = convexHull;

		float cx = 0, cy = 0;

	for (Point2D p : convexHull){
		cx += p.x;
		cy += p.y;
	}

	cx /= convexHull.size();
	cy /= convexHull.size();

	Point2D chCenter(cx, cy);

	float maxRad = 0, dis;

	for (Point2D p : convexHull){
		dis = eucDistance(p, chCenter);
		maxRad = std::max(maxRad, dis);
	}

	colisionRadius = maxRad;
}

void ComplexObject2D::resetLinearVelocity(){
	linearVelocity = stdLinearVelocity;
}

void ComplexObject2D::modifyLinearVelocity(float factor){
	if (linearVelocity * factor < maxLinearVelocity)
		linearVelocity *= factor;
}

void ComplexObject2D::resetCircularVelocity(){
	circularVelocity = stdCircularVelocity;
}

void ComplexObject2D::modifyCircularVelocity(float factor){
	if (circularVelocity * factor < maxCircularVelocity)
		circularVelocity *= factor;
}

void ComplexObject2D::moveUp(){
	Point2D newCenter;

	newCenter.x = currentCenter.x + linearVelocity * sin(-currentAngle);
	newCenter.y = currentCenter.y + linearVelocity * cos(-currentAngle);

	if (checkBoundaries(newCenter, currentAngle))
		currentCenter = newCenter;
}

void ComplexObject2D::rotateLeft(){
	float newAngle = currentAngle + circularVelocity;
	if (checkBoundaries(currentCenter, newAngle))
		currentAngle = newAngle;
}

void ComplexObject2D::rotateRight(){
	float newAngle = currentAngle - circularVelocity;
	if (checkBoundaries(currentCenter, newAngle))
		currentAngle = newAngle;
}

void ComplexObject2D::animateMe(){

	Transform2D::loadIdentityMatrix();
	Transform2D::translateMatrix(-initialCenter.x, -initialCenter.y);
	Transform2D::rotateMatrix(cos(currentAngle), sin(currentAngle));
	Transform2D::translateMatrix(initialCenter.x, initialCenter.y);
	Transform2D::translateMatrix(currentCenter.x, currentCenter.y);

	Transform2D::applyTransform(this);
	animateCH();
}

void ComplexObject2D::animateCH(){
	for (int i = 0; i < convexHull.size(); ++i)
		Transform2D::applyTransform(&convexHull[i], &transf_convexHull[i]);
}


bool ComplexObject2D::checkBoundaries(Point2D newCenter, float newAngle){

	/**
	  * Metoda care verifica daca obiectul curent ar putea face urmatoarea
	  * miscare si NU ar iesi din cadru
	**/

	std::vector<Point2D> tempCH = convexHull;

	Transform2D::loadIdentityMatrix();
	Transform2D::translateMatrix(-initialCenter.x, -initialCenter.y);
	Transform2D::rotateMatrix(cos(newAngle), sin(newAngle));
	Transform2D::translateMatrix(initialCenter.x, initialCenter.y);
	Transform2D::translateMatrix(newCenter.x, newCenter.y);

	for (int i = 0; i < convexHull.size(); ++i){
		Transform2D::applyTransform(&convexHull[i], &tempCH[i]);
		if (tempCH[i].x < blBoundary.x || tempCH[i].y < blBoundary.y ||
			tempCH[i].x > trBoundary.x + FLT_EPSILON || tempCH[i].y > 
			trBoundary.y + FLT_EPSILON)
				 return false;
	}

	return true;
}

void ComplexObject2D::reset(){
	resetPosition();
	resetLinearVelocity();
	resetCircularVelocity();
	maxLinearVelocity = 6 * stdLinearVelocity;
	maxCircularVelocity = 6 * stdCircularVelocity;
}

void ComplexObject2D::resetPosition(){
	currentAngle = 0;
	currentCenter.x = 0;
	currentCenter.y = 0;
}

const Point2D& ComplexObject2D::getCurrentPosition(){
	return currentCenter + initialCenter;
}