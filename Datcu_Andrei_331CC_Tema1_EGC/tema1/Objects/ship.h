/* Autor: Datcu Andrei Daniel - 331CC
 * Ship
 * Clasa pentru nava
 * Pe langa ComplexObject2D, prezinta metode pentru ascunderea/aratarea
 * armei, pentru verificarea daca un punct este "pe" arma, pentru 
 * setarea/resetarea invincibilitatii
 */

#pragma once
#include "../Framework/ComplexObject2D.h"
#include "../Framework/Point2D.h"
#include "../Framework/Polygon2D.h"
#include "../Framework/Visual2D.h"

class Ship: public ComplexObject2D{
public:
	Ship(Point2D blBoundary, Point2D trBoundary, 
		Point2D center, float radius, Color c = Color());

	void changeGunStatus();

	bool checkIfPointIsInGun(const Point2D &p);

	void makeInvinceable();
	void unmakeInvinceable();
	bool isInvinceable();

protected:
	void defineObject();

private:
	Point2D circleCenter;
	float radius;
	Color c;
	Polygon2D gun;
	bool isGunOut, invinceable;
	void pullGun();
	void retractGun();
};

