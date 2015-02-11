/* Autor: Datcu Andrei Daniel - 331CC
 * Road
 * Clasa care implementeaza soseaua, un dreptunghi lung, in planul xoz
 */

#pragma once
#include "..\framework\object3d.h"
#include "../Framework/Point3D.h"


class Road : public Object3D{
public:
	Road(Point3D lowestLeft, float width, float height, int lanes = 3);

	Point3D getBottomLeftForObject(int lane, float objectWidth,
		float objectDepth);

	const int laneCount;
	const float width, height;
private:
	Point3D lowestLeft;
};
