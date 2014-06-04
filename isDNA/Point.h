#pragma once
#include <math.h>
#include "tuple.h"
#include "vector.h"

class Point : public Tuple
{
public:

	Point(double x = 0.0, double y = 0.0, double z = 0.0);

	double distance(Point p);
	void rotate(double angle);
	void rotate(double angle, Point p1, Point p2);

};
