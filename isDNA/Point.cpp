#include <stdio.h>
#include <math.h>
#include "Point.h"

Point::Point(double x, double y, double z) : Tuple(x, y, z) {
}

// simple rotation around the origin in z=0
void Point::rotate(double angle) {

	double x = _x*cos(angle) - _y*sin(angle);
	double y = _x*sin(angle) + _y*cos(angle);

	_x = x;
	_y = y;
}

// rotation around an arbitrary axis defined by points p and q
void Point::rotate(double angle, Point p1, Point p2) {

	Point q = Point(0.0,0.0,0.0);
	double costheta,sintheta;
	Vector r = Vector(p2._x - p1._x, p2._y - p1._y, p2._z - p1._z);

	_x -= p1._x;
	_y -= p1._y;
	_z -= p1._z;
	r.normalize();

	costheta = cos(angle);
	sintheta = sin(angle);

	q._x += (costheta + (1 - costheta) * r._x * r._x) * _x;
	q._x += ((1 - costheta) * r._x * r._y - r._z * sintheta) * _y;
	q._x += ((1 - costheta) * r._x * r._z + r._y * sintheta) * _z;

	q._y += ((1 - costheta) * r._x * r._y + r._z * sintheta) * _x;
	q._y += (costheta + (1 - costheta) * r._y * r._y) * _y;
	q._y += ((1 - costheta) * r._y * r._z - r._x * sintheta) * _z;

	q._z += ((1 - costheta) * r._x * r._z - r._y * sintheta) * _x;
	q._z += ((1 - costheta) * r._y * r._z + r._x * sintheta) * _y;
	q._z += (costheta + (1 - costheta) * r._z * r._z) * _z;

	q._x += p1._x;
	q._y += p1._y;
	q._z += p1._z;

	_x = q._x;
	_y = q._y;
	_z = q._z;
}

// distance to another point
double Point::distance(Point p) {

	return sqrt((_x - p._x)*(_x - p._x) + (_y - p._y)*(_y - p._y) + (_z - p._z)*(_z - p._z));
}
