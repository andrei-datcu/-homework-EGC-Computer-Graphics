/* Autor: Datcu Andrei Daniel - 331CC
 * LifeBonus
 * Obiectul tip bonus care ofera o viata in plus
 * Cerc alb cu o cruce verde in centru
 */

#pragma once
#include "bonus.h"
class LifeBonus : public Bonus
{
public:
	LifeBonus(Point2D center, float radius);

protected:
	void defineObject();

private:
	Point2D circleCenter;
	float radius;
};

