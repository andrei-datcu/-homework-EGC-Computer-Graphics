/* Autor: Datcu Andrei Daniel - 331CC
 * Common
 * Colectii de functii explicate in README
 */


#pragma once
#ifndef __COMMMON_H__
#define __COMMMON_H__

#include "Point2D.h"
#include <random>



float eucDistance (const Point2D &p1, const Point2D &p2);

bool getLineIntersection(Point2D p0, Point2D p1, Point2D p2, Point2D p3,
						 Point2D &intersection);

float det(const Point2D &p1, const Point2D &p2, const Point2D &p3);

float truncf(float x);

Point2D generateRandomPointInRect(Point2D p1, Point2D p2);
Point2D generateRandomPositionAtMinDistance(Point2D p1, Point2D p2, 
												Point2D target, float minDis);

#endif
