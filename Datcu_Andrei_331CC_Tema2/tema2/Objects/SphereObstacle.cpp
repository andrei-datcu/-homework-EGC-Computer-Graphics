/* Autor: Datcu Andrei Daniel - 331CC
 * SpehereObstacle
 * Obstacolul tip sfera
 */

#include "SphereObstacle.h"
#include "../Framework/Circle2D.h"
#include "../Framework/Transform3D.h"

#define PI 3.14159265358979323846

SphereObstacle::SphereObstacle(Point3D center, bool mobile):
	Obstacle(Point3D(center.x, center.y, center.z + center.y), mobile){

	float radius = center.y;

	aabb.first = Point3D(center.x - radius, 0, center.z - radius);
	aabb.second = Point3D(center.x + radius, center.y + radius, 
		center.z + radius);

	float dr =  10;

	for (float height = 1; height <= radius; height += dr){
		Circle2D c(Point2D(center.x, center.z), height);
		Face *f = new Face();
		for (Point2D *p : c.points){
			f->contour.push_back(vertices.size());
			vertices.push_back(new Point3D(p->x, height, p->y));
			transf_vertices.push_back(new Point3D(*vertices.back()));
		}
		faces.push_back(f);

		Face *f2 = new Face();
		for (Point2D *p : c.points){
			f2->contour.push_back(vertices.size());
			vertices.push_back(new Point3D(p->x, 2 * radius - height, p->y));
			transf_vertices.push_back(new Point3D(*vertices.back()));
		}
		faces.push_back(f2);
	}

	Circle2D c2d (Point2D(center.x, center.y), radius);

	Object3D c3d;
	for (Point2D *p: c2d.points){
		c3d.vertices.push_back(new Point3D(p->x, p->y, center.z));
		c3d.transf_vertices.push_back(new Point3D(*c3d.vertices.back()));
	}

	float dangle = PI/10;

	Transform3D::loadIdentityProjectionMatrix();
	for (float angle = 0; angle < 2 * PI; angle += dangle){
		Transform3D::loadIdentityModelMatrix();
		Transform3D::translateMatrix(-center.x, -center.y, -center.z);
		Transform3D::rotateMatrixOy(angle);
		Transform3D::translateMatrix(center.x, center.y, center.z);
		Transform3D::applyTransform(&c3d);
		Face *f = new Face();
		for (Point3D *p: c3d.transf_vertices){
			f->contour.push_back(vertices.size());
			vertices.push_back(new Point3D(*p));
			transf_vertices.push_back(new Point3D(*p));
		}
		faces.push_back(f);
	}
}
