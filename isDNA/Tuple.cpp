#include <stdio.h>
#include "Tuple.h"

Tuple::Tuple(double x, double y, double z) {
	_x = x;
	_y = y;
	_z = z;
}

void Tuple::add(Tuple t) {
	_x += t._x;
	_y += t._y;
	_z += t._z;
}

void Tuple::add(double x, double y, double z) {
	_x += x;
	_y += y;
	_z += z;
}

void Tuple::sub(Tuple t) {
	_x -= t._x;
	_y -= t._y;
	_z -= t._z;
}

void Tuple::sub(double x, double y, double z) {
	_x -= x;
	_y -= y;
	_z -= z;
}

void Tuple::print(void) {
	printf("(%f,%f,%f)\n", _x, _y, _z);
}
