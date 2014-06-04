#pragma once
#include "tuple.h"

class Vector :
	public Tuple
{
public:

	Vector(double x = 0.0, double y = 0.0, double z = 0.0);

	double	getNorm(void);
	void	normalize(void);
	void	scale(double s);
	Vector	cross(Vector);
	double	dot(Vector);
	double	angle(Vector);
	void	rotate(double angle);

};
