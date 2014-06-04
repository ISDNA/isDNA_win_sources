#pragma once
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <time.h>
#include <math.h>
#include <windows.h>
#include <GL/glui.h>
#include "Point.h"
#include "Vector.h"
#include "BuildingBlock.h"

class Dna
{

private :

	// constants
	static const int	MIN_BP_SEG  = 25;
	static const int	MAX_BP_SEG  = 30;
	static const int	SCALE		= 1;

	// pseudo constants
	double		PI;
	double		BP_LEN;
	double		BP_PER_TURN;

	// variables
	char		_sequence[10100];
	int			_size;
	Point		_axis[10100];
	Point		_skeleton[1000];
	int			_segmentPairs;	
	int			_segments;			// skeleton segments

	// methods
	double	unifRand(void);
	char	segmentsIntersect(Point p0, Point p1, Point p2, Point p3);

	// nucleotides building blocks
	BuildingBlock* _bbA;
	BuildingBlock* _bbC;
	BuildingBlock* _bbG;
	BuildingBlock* _bbT;

public :
	Dna(char*, char*);
	int loadSequence(char*);
	void draw(void);
	void drawSkeleton(void);
	void drawAxis(void);
	void circularConformation(void);
	void buildAxis(void);
	void printSegmentsLengths(void);
	int  calcSegmentPairs(void);
	double twist(void);
	double writhe(void);
	double averageAngle(void);
	void saveConformation(char* file);
	int loadConformation(char* file);
	void translate(double x, double y, double z);
	void rotate(double angle, Point p, Point q);
};
