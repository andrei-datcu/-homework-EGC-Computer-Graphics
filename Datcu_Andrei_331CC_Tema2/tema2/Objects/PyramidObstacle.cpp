/* Autor: Datcu Andrei Daniel - 331CC
 * PyramidObstacle
 * Obstacolul tip piramida
 */

#include "PyramidObstacle.h"


PyramidObstacle::PyramidObstacle(Point3D bottomLeft, float width, bool mobile):
	Obstacle(Point3D(bottomLeft.x, bottomLeft.y, bottomLeft.z+width), mobile){

	vertices.push_back(new Point3D(bottomLeft));
	vertices.push_back(new Point3D(bottomLeft.x + width, bottomLeft.y,
		bottomLeft.z));
	vertices.push_back(new Point3D(bottomLeft.x + width, bottomLeft.y,
		bottomLeft.z + width));
	vertices.push_back(new Point3D(bottomLeft.x, bottomLeft.y,
		bottomLeft.z + width));
	vertices.push_back(new Point3D(bottomLeft.x + width/2,
		bottomLeft.y + width/2, bottomLeft.z + width/2));//motu

	aabb.first = bottomLeft;
	aabb.second = Point3D(bottomLeft.x + width, bottomLeft.y + width/2,
		bottomLeft.z + width);

	Face *jos, *stanga, *dreapta, *fata, *spate;

	jos = new Face();
	jos->contour.push_back(0);
	jos->contour.push_back(1);
	jos->contour.push_back(2);
	jos->contour.push_back(3);

	fata = new Face();
	fata->contour.push_back(0);
	fata->contour.push_back(1);
	fata->contour.push_back(4);

	dreapta = new Face();
	dreapta->contour.push_back(1);
	dreapta->contour.push_back(2);
	dreapta->contour.push_back(4);

	stanga = new Face();
	stanga->contour.push_back(0);
	stanga->contour.push_back(3);
	stanga->contour.push_back(4);

	spate = new Face();
	spate->contour.push_back(2);
	spate->contour.push_back(3);
	spate->contour.push_back(4);

	faces.push_back(jos);
	faces.push_back(fata);
	faces.push_back(spate);
	faces.push_back(stanga);
	faces.push_back(dreapta);

	for (Point3D *v :vertices)
		transf_vertices.push_back(new Point3D(*v));
}
