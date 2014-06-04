#include "Vector.h"

Vector::Vector(double x, double y, double z) : Tuple(x, y, z) {
}

double Vector::getNorm(void) {

	return sqrt(_x*_x + _y*_y + _z*_z);
}

void Vector::normalize(void) {

	double x, y, z, norm = getNorm();

	x = _x/norm;
    y = _y/norm;
    z = _z/norm;

	_x = x;
    _y = y;
    _z = z;
}

void Vector::scale(double s) {

	normalize();

	_x = _x * s;
    _y = _y * s;
    _z = _z * s;
}

Vector Vector::cross(Vector v) {

	Vector r(0,0,0);

    r._x =  (_y*v._z)-(v._y*_z);
    r._y = -(_x*v._z)+(v._x*_z);
    r._z =  (_x*v._y)-(_y*v._x);

    return r;
}

double Vector::dot(Vector v) {

    return (_x * v._x + _y * v._y + _z * v._z);
}

double Vector::angle(Vector v) {
	
	Vector v1(_x, _y, _z);
	Vector v2(v._x, v._y, v._z);

	v1.normalize();		 
	v2.normalize();

	double a = acos(v1.dot(v2));
	double PI = 3.141592653589793;

	return (a*(180/PI));

}

void Vector::rotate(double angle) {

	double x;
	double y;
	double z;

	// rotation X
	x = _x;
	y = _y*cos(angle) + _z*sin(angle);
	z = _z*cos(angle) - _y*sin(angle);
	_x = x;
	_y = y;
	_z = z;

	// rotation Y
	x = _x*cos(angle) - _z*sin(angle);
	y = _y;
	z = _x*sin(angle) + _z*cos(angle);
	_x = x;
	_y = y;
	_z = z;

	// rotation Z
	x = _x*cos(angle) + _y*sin(angle);
	y = _y*cos(angle) - _x*sin(angle);
	z = _z;
	_x = x;
	_y = y;
	_z = z;
}
