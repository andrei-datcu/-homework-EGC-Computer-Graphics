/* Autor: Datcu Andrei Daniel - 331CC
 * ShieldBonus
 * Obiectul tip bonus care ofera invincibilitate
 * Cerc gri cu scut gri in mijloc
 */

#pragma once
#include "bonus.h"
class ShieldBonus :
	public Bonus
{
public:
	ShieldBonus(Point2D center, float radius);

protected:
	void defineObject();

private:
	Point2D circleCenter;
	float radius;
};

