/* Autor: Datcu Andrei Daniel - 331CC
 * ArrowEnemy
 * Dusman tip Sageata, asa cum este desenat in enuntul temei
 */

#pragma once
#include "enemy.h"

class ArrowEnemy : public Enemy{

public:

	ArrowEnemy(Point2D bottomLeftBoudary, Point2D topRightBoundary, 
		Point2D bottomLeft, float length);

protected:
	bool extraCheckHittingTheWall();
	void defineObject();

private:
	float length; //lungimea laturii triunghiului
	Point2D bottomLeft;

};

