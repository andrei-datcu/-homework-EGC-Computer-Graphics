/* Autor: Datcu Andrei Daniel - 331CC
 * Car
 * Obiect3D tip masina
 */

#include "car.h"
#include "../Framework/Circle2D.h"
#include "../Framework/Transform3D.h"
#include "obstacle.h"
#include <cmath>

#define PI 3.14159265358979323846

Car::Car(Point3D leftBackWheelCenter, float track, float wheelbase,
		 float minx, float maxx):
	
		Object3D(std::vector<Point3D*>(), std::vector<Face*>()), minx(minx),
			maxx(maxx), speed(0){

	float wheelRadius = leftBackWheelCenter.y;
	//astfel incat sa cada pe planul xoz;

	aabb.first = Point3D(leftBackWheelCenter.x, 0,
		leftBackWheelCenter.z - wheelbase);
	aabb.second = Point3D(leftBackWheelCenter.x + track,
		leftBackWheelCenter.y + 2 * wheelRadius, leftBackWheelCenter.z);

	leftx = leftBackWheelCenter.x;
	rightx = leftx + track;
	Circle2D leftBackWheel(Point2D(leftBackWheelCenter.y,
		leftBackWheelCenter.z), wheelRadius);

	//Roata stanga spate
	
	Face *leftBackWheelOutFace = new Face(), *leftBackWheelInFace = new Face();

	for (int i = 0; i < leftBackWheel.points.size(); ++i){
		vertices.push_back(new Point3D(leftBackWheelCenter.x,
			leftBackWheel.points[i]->x, leftBackWheel.points[i]->y));
		transf_vertices.push_back(new Point3D(*vertices.back()));
		bl.push_back(new Point3D(*vertices.back()));
		leftBackWheelOutFace->contour.push_back(vertices.size() - 1);
	}

	for (int i = 0; i < leftBackWheel.points.size(); ++i){
		vertices.push_back(new Point3D(leftBackWheelCenter.x + wheelRadius/2,
			leftBackWheel.points[i]->x, leftBackWheel.points[i]->y));
		transf_vertices.push_back(new Point3D(*vertices.back()));
		leftBackWheelInFace->contour.push_back(vertices.size() - 1);
	}

	faces.push_back(leftBackWheelOutFace);
	faces.push_back(leftBackWheelInFace);

	//Roata dreapta spate
	
	Circle2D rightBackWheel(Point2D(leftBackWheelCenter.y,
		leftBackWheelCenter.z), wheelRadius);
	Face *rightBackWheelOutFace = new Face(),
		*rightBackWheelInFace = new Face();

	for (int i = 0; i < rightBackWheel.points.size(); ++i){
		vertices.push_back(new Point3D(leftBackWheelCenter.x + track,
			rightBackWheel.points[i]->x, rightBackWheel.points[i]->y));
		transf_vertices.push_back(new Point3D(*vertices.back()));
		br.push_back(new Point3D(*vertices.back()));
		rightBackWheelOutFace->contour.push_back(vertices.size() - 1);
	}

	for (int i = 0; i < rightBackWheel.points.size(); ++i){
		vertices.push_back(new Point3D
			(leftBackWheelCenter.x + track - wheelRadius/2,
			rightBackWheel.points[i]->x, rightBackWheel.points[i]->y));
		transf_vertices.push_back(new Point3D(*vertices.back()));
		rightBackWheelInFace->contour.push_back(vertices.size() - 1);
	}

	faces.push_back(rightBackWheelOutFace);
	faces.push_back(rightBackWheelInFace);

	//Roata stanga fata

	Circle2D leftFrontWheel(Point2D(leftBackWheelCenter.y, 
		leftBackWheelCenter.z - wheelbase), wheelRadius);
	Face *leftFrontWheelOutFace = new Face(),
		*leftFrontWheelInFace = new Face();

	for (int i = 0; i < leftFrontWheel.points.size(); ++i){
		vertices.push_back(new Point3D(leftBackWheelCenter.x, 
			leftFrontWheel.points[i]->x, leftFrontWheel.points[i]->y));
		transf_vertices.push_back(new Point3D(*vertices.back()));
		br.push_back(new Point3D(*vertices.back()));
		leftFrontWheelOutFace->contour.push_back(vertices.size() - 1);
	}

	for (int i = 0; i < leftFrontWheel.points.size(); ++i){
		vertices.push_back(new Point3D(leftBackWheelCenter.x + wheelRadius/2, 
			leftFrontWheel.points[i]->x, leftFrontWheel.points[i]->y));
		transf_vertices.push_back(new Point3D(*vertices.back()));
		leftFrontWheelInFace->contour.push_back(vertices.size() - 1);
	}

	faces.push_back(leftFrontWheelOutFace);
	faces.push_back(leftFrontWheelInFace);

	//Roata dreapta fata

	Circle2D rightFrontWheel(Point2D(leftBackWheelCenter.y, 
		leftBackWheelCenter.z - wheelbase), wheelRadius);
	Face *rightFrontWheelOutFace = new Face(),
		*rightFrontWheelInFace = new Face();

	for (int i = 0; i < rightFrontWheel.points.size(); ++i){
		vertices.push_back(new Point3D(leftBackWheelCenter.x + track, 
			rightFrontWheel.points[i]->x, rightFrontWheel.points[i]->y));
		transf_vertices.push_back(new Point3D(*vertices.back()));
		br.push_back(new Point3D(*vertices.back()));
		rightFrontWheelOutFace->contour.push_back(vertices.size() - 1);
	}

	for (int i = 0; i < rightFrontWheel.points.size(); ++i){
		vertices.push_back(new Point3D
			(leftBackWheelCenter.x + track - wheelRadius/2, 
			rightFrontWheel.points[i]->x, rightFrontWheel.points[i]->y));
		transf_vertices.push_back(new Point3D(*vertices.back()));
		rightFrontWheelInFace->contour.push_back(vertices.size() - 1);
	}

	faces.push_back(rightFrontWheelOutFace);
	faces.push_back(rightFrontWheelInFace);

	//Caroseria

	int bodyStartIndex = vertices.size();

	vertices.push_back(new Point3D(leftBackWheelCenter.x + wheelRadius / 2, 
		leftBackWheelCenter.y, leftBackWheelCenter.z + 0.5 * wheelRadius));
	vertices.push_back(new Point3D(
		leftBackWheelCenter.x + track - wheelRadius/2, leftBackWheelCenter.y,
		leftBackWheelCenter.z + 0.5 * wheelRadius));
	vertices.push_back(new Point3D(
		leftBackWheelCenter.x + track - wheelRadius/2, leftBackWheelCenter.y, 
		leftBackWheelCenter.z - wheelbase - 0.5 * wheelRadius));
	vertices.push_back(new Point3D(
		leftBackWheelCenter.x + wheelRadius / 2, leftBackWheelCenter.y, 
		leftBackWheelCenter.z - wheelbase - 0.5 * wheelRadius));
	vertices.push_back(new Point3D(leftBackWheelCenter.x + wheelRadius / 2, 
		leftBackWheelCenter.y + 2 * wheelRadius, 
		leftBackWheelCenter.z + 0.5 * wheelRadius));
	vertices.push_back(new Point3D(
		leftBackWheelCenter.x + track - wheelRadius/2, 
		leftBackWheelCenter.y + 2 * wheelRadius, 
		leftBackWheelCenter.z + 0.5 * wheelRadius));
	vertices.push_back(new Point3D(
		leftBackWheelCenter.x + track - wheelRadius/2, 
		leftBackWheelCenter.y + 2 * wheelRadius, 
		leftBackWheelCenter.z - wheelbase - 0.5 * wheelRadius));
	vertices.push_back(new Point3D(leftBackWheelCenter.x + wheelRadius/2, 
		leftBackWheelCenter.y + 2 * wheelRadius, 
		leftBackWheelCenter.z - wheelbase - 0.5 * wheelRadius));

	Face *jos, *sus, *stanga, *dreapta, *fata, *spate;

	jos = new Face();
	jos->contour.push_back(bodyStartIndex + 0);
	jos->contour.push_back(bodyStartIndex + 1);
	jos->contour.push_back(bodyStartIndex + 2);
	jos->contour.push_back(bodyStartIndex + 3);

	sus = new Face();
	sus->contour.push_back(bodyStartIndex + 4);
	sus->contour.push_back(bodyStartIndex + 5);
	sus->contour.push_back(bodyStartIndex + 6);
	sus->contour.push_back(bodyStartIndex + 7);

	stanga = new Face();
	stanga->contour.push_back(bodyStartIndex + 0);
	stanga->contour.push_back(bodyStartIndex + 3);
	stanga->contour.push_back(bodyStartIndex + 7);
	stanga->contour.push_back(bodyStartIndex + 4);

	dreapta = new Face();
	dreapta->contour.push_back(bodyStartIndex + 1);
	dreapta->contour.push_back(bodyStartIndex + 2);
	dreapta->contour.push_back(bodyStartIndex + 6);
	dreapta->contour.push_back(bodyStartIndex + 5);

	fata = new Face();
	fata->contour.push_back(bodyStartIndex + 0);
	fata->contour.push_back(bodyStartIndex + 1);
	fata->contour.push_back(bodyStartIndex + 5);
	fata->contour.push_back(bodyStartIndex + 4);

	spate = new Face();
	spate->contour.push_back(bodyStartIndex + 2);
	spate->contour.push_back(bodyStartIndex + 3);
	spate->contour.push_back(bodyStartIndex + 7);
	spate->contour.push_back(bodyStartIndex + 6);

	for (int i = bodyStartIndex; i < vertices.size(); ++i)
		transf_vertices.push_back(new Point3D(*vertices[i]));

	faces.push_back(sus);
	faces.push_back(jos);
	faces.push_back(stanga);
	faces.push_back(dreapta);
	faces.push_back(fata);
	faces.push_back(spate);
	center = Point3D(leftBackWheelCenter.x + track / 2, leftBackWheelCenter.y,
		leftBackWheelCenter.z + wheelbase / 2);
	currentCenter = Point3D(0, 0, 0);
	currentAngle = 0;
}

void Car::rotateLeft(){
	if (currentAngle  < 0 + FLT_EPSILON)
		currentAngle += PI/4;
}

void Car::rotateRight(){
	if (abs(currentAngle + PI/4)> FLT_EPSILON)
		currentAngle -= PI/4;
}

void Car::straightenUp(){
	currentAngle = 0;
}

void Car::accelerate(){
 if (speed < 60) speed+=2;
}

void Car::deccelerate(){
	
	if (speed > 0)
		speed--;
}

bool Car::move(){

	if (speed == 0)
		return true;

	if (abs(currentAngle) > FLT_EPSILON){ // daca nu sunt drept
		if (currentAngle < 0){
			if (rightx + 20 < maxx){
				currentCenter.x += 20;
				aabb.first.x += 20;
				aabb.second.x += 20;
				rightx += 20;
				leftx += 20;
				return true;
			}
		}
		else{
			if (leftx - 20 > minx){
				currentCenter.x -= 20;
				aabb.first.x -= 20;
				aabb.second.x -= 20;
				leftx -= 20;
				rightx -= 20;
				return true;
			}
		}
	}
	else
		return true;
	return false;
}

void Car::animate(){
	Transform3D::loadIdentityModelMatrix();
	Transform3D::loadIdentityProjectionMatrix();
	Transform3D::perspectiveProjectionMatrix(center.x + currentCenter.x + 30, 
		550, -650);
	Transform3D::translateMatrix(-center.x, -center.y, -center.z);
	Transform3D::rotateMatrixOy(currentAngle);
	Transform3D::translateMatrix(center.x, center.y, center.z);
	Transform3D::translateMatrix(currentCenter.x, currentCenter.y,
		currentCenter.z);
	Transform3D::applyTransform(this);
}

bool Car::intersects(Obstacle &o){
	return !(aabb.first.x > o.aabb.second.x || aabb.second.x < o.aabb.first.x||
		aabb.first.y > o.aabb.second.y || aabb.second.y < o.aabb.first.y ||
		aabb.first.z > o.aabb.second.z || aabb.second.z < o.aabb.first.z);
}