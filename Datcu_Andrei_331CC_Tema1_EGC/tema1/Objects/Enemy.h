/* Autor: Datcu Andrei Daniel - 331CC
 * Enemy
 * Clasa abstracta care este mostenista de orice Inamic
 * Aduce in plus metoda planNextMove(), prin care Inamicul se misca
 */

#pragma once
#include "../Framework/complexobject2d.h"

class Enemy : public ComplexObject2D{

public:
	Enemy(Point2D bottomLeftBoundary, Point2D topRightBoudnary, int value);
	virtual void animateMe();
	const int value;

protected:
	virtual void planNextMove();
	virtual bool extraCheckHittingTheWall();
	bool gettingAwayFromTheEdge, gettingAwayLeft;
	int justRun;
	float targetAngle;

};

