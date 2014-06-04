#pragma once
#include <math.h>

class Tuple
{

public:

	Tuple(double x = 0.0, double y = 0.0, double z = 0.0);

	double _x;
	double _y;
	double _z;

	virtual void add(Tuple t);
	virtual void add(double x, double y, double z);
	virtual void sub(Tuple t);
	virtual void sub(double x, double y, double z);
	virtual void print(void);

};
