/* Autor: Datcu Andrei Daniel - 331CC
 * ComplexObject2D
 * Clasa abstracta care defineste practic o multime de obiecte Object2D
 * aka o multime de poligoane convex neconectate intre ele
 */

#pragma once

#include <vector>
#include "Object2D.h"
#include "Point2D.h"

class ComplexObject2D : public std::vector<Object2D>{
public:

	ComplexObject2D(Point2D bottomLeftBoundary, Point2D topRightBoudnary);
	void moveUp();
	void rotateLeft();
	void rotateRight();
	void reset();

	void resetPosition();
	virtual void animateMe();
	
	void modifyLinearVelocity(float factor);
	void resetLinearVelocity();

	void modifyCircularVelocity(float factor);
	void resetCircularVelocity();

	const Point2D& getCurrentPosition();

	const float getRadius(); //raza cercului circumscris

	bool intersects(ComplexObject2D&, Point2D &result);

	std::vector<Point2D> convexHull, transf_convexHull;

protected:

	virtual void defineObject() = 0;

	virtual void animateCH();
	virtual void computeConvexHull();

	virtual bool checkBoundaries(Point2D newCenter, float newAngle);

	Point2D initialCenter, currentCenter, blBoundary, trBoundary;
	float currentAngle;
	
	float stdLinearVelocity, maxLinearVelocity;
	float stdCircularVelocity, maxCircularVelocity;
	float linearVelocity, circularVelocity;
	float colisionRadius;  //raza cercului circumscris
};

