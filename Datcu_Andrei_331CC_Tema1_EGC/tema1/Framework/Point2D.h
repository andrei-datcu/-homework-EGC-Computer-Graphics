#pragma once
#include "Color.h"


class Point2D
{
public:
	float x,y;
	//Color color;

public:
	Point2D()
	{
		x = 0;
		y = 0;
		//color.r = color.g = color.b = 0;
	}

	Point2D(float _x, float _y)
	{
		x = _x;
		y = _y;
		//color.r = color.g = color.b = 0;
	}

	/* Point2D(float _x, float _y, Color _color)
	{
		x = _x;
		y = _y;
		color.r = _color.r;
		color.g = _color.g;
		color.b = _color.b;
	}*/

	~Point2D()
	{}

	bool operator < (const Point2D& p){
		
		if (this->x < p.x)
			return true;
		if (this->x == p.x)
			return this->y < p.y;
		return false;
	}

	bool operator == (const Point2D&p){
		return this->x == p.x && this->y == p.y;
	}

	const Point2D& operator + (const Point2D &p){
		return Point2D(this->x + p.x, this->y + p.y);
	}
};