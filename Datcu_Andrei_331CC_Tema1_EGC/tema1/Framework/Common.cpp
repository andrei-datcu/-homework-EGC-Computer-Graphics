/* Autor: Datcu Andrei Daniel - 331CC
 * Common
 * Colectii de functii explicate in README
 */


#include "Common.h"
#include "Point2D.h"
#include <math.h>
#include <float.h>
#include <random>

std::random_device rd; // obtain a random number from hardware

std::mt19937 generator(rd());

float eucDistance (const Point2D &p1, const Point2D &p2){
	//computes euclidian distance

	return sqrtf( (p1.x - p2.x) * (p1.x - p2.x) +
		(p1.y - p2.y) * (p1.y - p2.y));
}

bool getLineIntersection(Point2D p0, Point2D p1, Point2D p2, Point2D p3,
						 Point2D &intersection){

    float s1_x, s1_y, s2_x, s2_y;

    s1_x = p1.x - p0.x;
	s1_y = p1.y - p0.y;

    s2_x = p3.x - p2.x;
	s2_y = p3.y - p2.y;

    float s, t, det;

	det = -s2_x * s1_y + s1_x * s2_y;

	if (det < FLT_EPSILON) //paralele sau coliniare
		return false;
	
    s = (-s1_y * (p0.x - p2.x) + s1_x * (p0.y - p2.y)) / det;
    t = ( s2_x * (p0.y - p2.y) - s2_y * (p0.x - p2.x)) / det;

    if (s >= 0 && s <= 1 && t >= 0 && t <= 1){
        intersection.x = p0.x + (t * s1_x);
        intersection.y = p0.y + (t * s1_y);
        return true;
    }

    return false;
}

float det(const Point2D &p1, const Point2D &p2, const Point2D &p3){
	return p1.x*p2.y+p1.y*p3.x+p2.x*p3.y-p3.x*p2.y-p3.y*p1.x-p2.x*p1.y;
}

float truncf(float x){ //should be in cmath, but it isn't. Go Microsoft
	return x > 0 ? floorf(x) : ceilf(x);
}

Point2D generateRandomPointInRect(Point2D p1, Point2D p2){
	std::uniform_real_distribution<float> xd(p1.x, p2.x);
	std::uniform_real_distribution<float> yd(p1.y, p2.y);
	Point2D result(xd(generator), yd(generator));
	return result;
}

Point2D generateRandomPositionAtMinDistance(Point2D p1, Point2D p2, 
												Point2D target, float minDis){
	/*
		Functie care genereaza un punct in dreptunghiul definit de
		p1 (stanga jos) si p2 (dreapta sus) la o distanta > minDis fata de
		punctul target
	*/

	Point2D position;
	do
		position = generateRandomPointInRect(p1, p2);
	while (eucDistance(position,target) < minDis); 
	return position;
}