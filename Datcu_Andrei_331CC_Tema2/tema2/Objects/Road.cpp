/* Autor: Datcu Andrei Daniel - 331CC
 * Road
 * Clasa care implementeaza soseaua, un dreptunghi lung, in planul xoz
 */

#include "Road.h"


Road::Road(Point3D lowestLeft, float width, float height, int lanes):
	Object3D(std::vector<Point3D*>(), std::vector<Face*>()), width(width),
	laneCount(lanes), height(height), lowestLeft(lowestLeft){

	float laneSize = width / lanes;

	vertices.push_back(new Point3D(lowestLeft));
	vertices.push_back(new Point3D(lowestLeft.x + width, 
		lowestLeft.y, lowestLeft.z));
	vertices.push_back(new Point3D(lowestLeft.x + width,
		lowestLeft.y, lowestLeft.z + height));
	vertices.push_back(new Point3D(lowestLeft.x, lowestLeft.y, 
		lowestLeft.z + height));

	for (int i = 1; i < lanes; ++i)
		if (i % 2 == 0){
			vertices.push_back(new Point3D(lowestLeft.x + i * laneSize, 
				lowestLeft.y, lowestLeft.z));
			vertices.push_back(new Point3D(lowestLeft.x + i * laneSize, 
				lowestLeft.y, lowestLeft.z + height));
		}
		else{
			vertices.push_back(new Point3D(lowestLeft.x + i * laneSize, 
				lowestLeft.y, lowestLeft.z + height));
			vertices.push_back(new Point3D(lowestLeft.x + i * laneSize, 
				lowestLeft.y, lowestLeft.z));
		}

	if (lanes % 2)
		vertices.push_back(new Point3D(lowestLeft.x, lowestLeft.y, 
		lowestLeft.z + height));
	else
		vertices.push_back(new Point3D(lowestLeft));
	
	
	faces.push_back(new Face(std::vector<int>()));

	for (int i = 0; i < vertices.size(); ++i){
		transf_vertices.push_back(new Point3D(*vertices[i]));
		faces[0]->contour.push_back(i);
	}

}

Point3D Road::getBottomLeftForObject(int lane, float objectWidth,
									 float objectDepth){

	Point3D result(0, 0, 0);

	result.x = (lane - 1) * 
		(width / laneCount) + ((width / laneCount) - objectWidth)/2 +
		lowestLeft.x;
	result.z = lowestLeft.z + height - objectDepth;
	return result;
}