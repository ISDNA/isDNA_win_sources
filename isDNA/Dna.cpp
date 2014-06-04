#include "Dna.h"

//--------------------------------------------------------------
Dna::Dna(char* gbkFile, char* conformationFile) {

	PI			= 3.141592653589793;
	BP_LEN		= 3.3;
	BP_PER_TURN	= 10.5;

	loadSequence(gbkFile);

	if (conformationFile == NULL) {
		circularConformation();
	} else {
		loadConformation(conformationFile);
	}

	_bbA = new BuildingBlock('A');
	_bbC = new BuildingBlock('C');
	_bbG = new BuildingBlock('G');
	_bbT = new BuildingBlock('T');

}

//--------------------------------------------------------------
void Dna::draw(void) {

	float	ALPHA	= (360/BP_PER_TURN);
	float	W		= 20.0f/SCALE;	
	float	R		= (sqrt(3.0f)/3.0f)*W;

	Vector u(0,0,1);

	for (int i=0; i < _size; i++) {

		Vector v(_axis[i+1]._x - _axis[i]._x, 
				 _axis[i+1]._y - _axis[i]._y, 
				 _axis[i+1]._z - _axis[i]._z);

		Vector crossProd = u.cross(v);
		float angle = u.angle(v);

		Point middle((_axis[i]._x + _axis[i+1]._x) / 2,
					 (_axis[i]._y + _axis[i+1]._y) / 2,
					 (_axis[i]._z + _axis[i+1]._z) / 2);

		// chain A
		glPushMatrix();
		glTranslatef(middle._x/SCALE, middle._y/SCALE, middle._z/SCALE);
		glRotatef(angle, crossProd._x, crossProd._y, crossProd._z);
		glRotatef(ALPHA*i, 0.0f, 0.0f, 1.0f);
		glTranslatef(-R/SCALE, 0.0f, 0.0f);
		glRotatef(30.0f, 0.0f, 0.0f, 1.0f);
		switch (_sequence[i]) {
			case 'a': _bbA->draw(); break;
			case 'c': _bbC->draw(); break;
			case 'g': _bbG->draw(); break;
			case 't': _bbT->draw(); break;
		}
		glPopMatrix();

		// chain B
		glPushMatrix();
		glTranslatef(middle._x/SCALE, middle._y/SCALE, middle._z/SCALE);
		glRotatef(angle, crossProd._x, crossProd._y, crossProd._z);
		glRotatef(-120.0f, 0.0f, 0.0f, 1.0f);
		glRotatef(ALPHA*i, 0.0f, 0.0f, 1.0f);
		glTranslatef(-R/SCALE, 0.0f, 0.0f);
		glRotatef(-30.0f, 0.0f, 0.0f, 1.0f);
		switch (_sequence[i]) {
			case 'a': _bbT->draw(); break;
			case 'c': _bbG->draw(); break;
			case 'g': _bbC->draw(); break;
			case 't': _bbA->draw(); break;
		}
		glPopMatrix();
	}
}

//--------------------------------------------------------------
void Dna::drawSkeleton(void) {
	
	// circular conformation
	glBegin(GL_LINE_LOOP);
		Point p(0,0,0);
		for (int i=0; i<_segments; i++) {
			p._x = _skeleton[i]._x/SCALE;
			p._y = _skeleton[i]._y/SCALE;
			p._z = _skeleton[i]._z/SCALE;
			glVertex3f(p._x, p._y, p._z);
		}
	glEnd();

	// random conformation
	/*glBegin(GL_LINE_LOOP);
		Point p(0,0,0);
		for (int i=0; i<_segments; i++) {
			glVertex3f(p._x, p._y, p._z);
			p._x += _skeleton[i]._x/SCALE;
			p._y += _skeleton[i]._y/SCALE;
			p._z += _skeleton[i]._z/SCALE;
		}
	glEnd();*/
}

//--------------------------------------------------------------
void Dna::saveConformation(char* file) {

	FILE* f = fopen(file, "w");

	for (int i=0; i < _segments; i++) {

		fprintf(f, "%f %f %f\n", _skeleton[i]._x, _skeleton[i]._y, _skeleton[i]._z);
	}

	fclose(f);

	printf("Conformation saved to: %s \n", file);
}

//--------------------------------------------------------------
int Dna::loadConformation(char* file) {

	FILE* f;
	char l[80];
	char* value;
	char n;
	int i;

	f = fopen(file, "r");

	if (f == NULL) {
		printf("Error loading conformation");
		return (1);
	}
	i = 0;
	while (fgets(l,sizeof(l),f) != NULL) {
		value = strtok (l," ");
		_skeleton[i]._x = atof(value);
		value = strtok (NULL, " ");
		_skeleton[i]._y = atof(value);
		value = strtok (NULL, " ");
		_skeleton[i]._z = atof(value);
		i++;
	}
	_segments = i;
	fclose(f);
}

//--------------------------------------------------------------
int Dna::loadSequence(char* gbkFile) {

	FILE* f;
	char l[80];
	char* ch;
	char n;
	int i;

	f = fopen(gbkFile, "r");

	if (f == NULL) {
		printf("Error loading sequence");
		return (1);
	}
	i = 0;
	while (fgets(l,sizeof(l),f) != NULL) {
		ch = strtok (l," ");
		if (strstr(ch,"ORIGIN") != NULL) {
			while ((n=fgetc(f)) != EOF) {
				if ((n=='a') || (n=='c') || (n=='g') || (n=='t')) {
					_sequence[i] = n;
					i++;
				}
			}
		}
	}
	_size = i;
	fclose(f);

	_segmentPairs = calcSegmentPairs();
	//printf("%d pairs per segment \n", _segmentPairs);

	// output sequence
	/*for (int i=0; i<_size; i++) {
		printf("%c", _sequence[i]);
	}
	printf("\n");*/
}

//--------------------------------------------------------------
int Dna::calcSegmentPairs(void) {
	
	int gap = _size;
	int basePairs = MIN_BP_SEG;
	
	for (int bp = MIN_BP_SEG; bp <= MAX_BP_SEG; bp++) {

		int segments = floor(((double) _size / bp) + 0.5); // round

		if (abs(_size - (bp * segments)) < gap) {
			gap = abs(_size - (bp * segments));
			basePairs = bp;
		}
	}

	return basePairs;
}

//--------------------------------------------------------------
void Dna::circularConformation(void) {

	double P  = _size * BP_LEN;
	double R  = (P /(2*PI));

	_segments = ((int) (_size / _segmentPairs)) + 1;
	double angle = (2*PI) / _segments;

	//printf("size = %d\n", _size);
	//printf("segs = %d\n", _segments);

	// first point
	Point o(R,0,0);
	_skeleton[0]._x = o._x;
	_skeleton[0]._y = o._y;
	_skeleton[0]._z = o._z;

	for (int i=1; i<_segments; i++) {
		Point p(R,0,0);
		p.rotate(i*angle);
		_skeleton[i]._x = p._x;
		_skeleton[i]._y = p._y;
		_skeleton[i]._z = p._z;
	}

	//printSegmentsLengths();
}

//--------------------------------------------------------------
void Dna::buildAxis(void) {

	int axisIndex = 0;
	Point p(_skeleton[0]._x, _skeleton[0]._y, _skeleton[0]._z);

	for (int i=1; i<_segments; i++) {

		Vector v(_skeleton[i]._x - _skeleton[i-1]._x, _skeleton[i]._y - _skeleton[i-1]._y, _skeleton[i]._z - _skeleton[i-1]._z);

		v.normalize();
		v.scale(BP_LEN);

		for (int j=axisIndex; j<axisIndex+_segmentPairs; j++) {
			p.add(v);
			_axis[j] = p; 
		}

		axisIndex += _segmentPairs;
	}

	// last shorter segment
	Vector v(_skeleton[0]._x - _skeleton[_segments-1]._x, _skeleton[0]._y - _skeleton[_segments-1]._y, _skeleton[0]._z - _skeleton[_segments-1]._z);
	v.normalize();
	v.scale(BP_LEN);
	for (int j=axisIndex; j<=_size; j++) {
		p.add(v);
		_axis[j] = p; 
	}
}

//--------------------------------------------------------------
void Dna::drawAxis(void) {

	glBegin(GL_LINE_LOOP);
		Point p(_axis[0]._x, _axis[0]._y, _axis[0]._z);
		for (int i=0; i<_size; i++) {
			p._x = _axis[i]._x/SCALE;
			p._y = _axis[i]._y/SCALE;
			p._z = _axis[i]._z/SCALE;
			glVertex3f(p._x, p._y, p._z);
		}
	glEnd();
}


//--------------------------------------------------------------
double Dna::unifRand(void) {

    return rand() / double(RAND_MAX);
}

//--------------------------------------------------------------
void Dna::printSegmentsLengths(void) {

	for (int i=0; i<_segments-1; i++) {
		printf("segment(%d,%d) = %f \n", i, i+1, _skeleton[i].distance(_skeleton[i+1]));
	}
	printf("segment(%d,0) = %f \n", _segments-1, _skeleton[_segments-1].distance(_skeleton[0]));
}

//--------------------------------------------------------------
double Dna::twist(void) {

	double tw = 0;

	for (int i=0; i<_segments-3; i++) {

		Vector s1 = Vector(_skeleton[i+1]._x - _skeleton[i]._x, _skeleton[i+1]._y - _skeleton[i]._y, _skeleton[i+1]._z - _skeleton[i]._z);
		Vector s2 = Vector(_skeleton[i+2]._x - _skeleton[i+1]._x, _skeleton[i+2]._y - _skeleton[i+1]._y, _skeleton[i+2]._z - _skeleton[i+1]._z);
		Vector s3 = Vector(_skeleton[i+3]._x - _skeleton[i+2]._x, _skeleton[i+3]._y - _skeleton[i+2]._y, _skeleton[i+3]._z - _skeleton[i+2]._z);

		Vector p1 = s1.cross(s2);
		Vector p2 = s2.cross(s3);
		p1.normalize();
		p2.normalize();

		double p1_dot_p2 = p1.dot(p2);
		double p1_dot_s3 = p1.dot(s3);

		tw += acos(p1_dot_p2) * ((p1_dot_s3 > 0) ? 1 : ((p1_dot_s3 < 0) ? -1 : 0));
	}

	tw /= 2 * PI;

	return tw;
}

//--------------------------------------------------------------
double Dna::writhe(void) {

	double twz = 0;
	double wrz = 0;
	double tw  = twist();
	double wr  = 0;

	// calculate directional twist
	for (int i=0; i<_segments-3; i++) {

		Vector s1 = Vector(_skeleton[i+1]._x - _skeleton[i]._x, _skeleton[i+1]._y - _skeleton[i]._y, _skeleton[i+1]._z - _skeleton[i]._z);
		Vector s2 = Vector(_skeleton[i+2]._x - _skeleton[i+1]._x, _skeleton[i+2]._y - _skeleton[i+1]._y, _skeleton[i+2]._z - _skeleton[i+1]._z);
		Vector s3 = Vector(_skeleton[i+3]._x - _skeleton[i+2]._x, _skeleton[i+3]._y - _skeleton[i+2]._y, _skeleton[i+3]._z - _skeleton[i+2]._z);

		Vector p1 = s1.cross(s2);
		Vector p2 = s2.cross(s3);
		p1.normalize();
		p2.normalize();

		if ((p1._z*p2._z) < 0) {
			double p1_dot_s3 = p1.dot(s3);
			twz += (p1_dot_s3 > 0) ? 1 : ((p1_dot_s3 < 0) ? -1 : 0);
		}
	}
	twz /= 2;

	// calculate directional writhe
	for (int i=1; i<_segments-3; i++) {

		for (int j=0; j<i; j++) {

			// segments projections plane XY
			Point pi1 = Point(_skeleton[i]._x, _skeleton[i]._y, 0);
			Point pi2 = Point(_skeleton[i+1]._x, _skeleton[i+1]._y, 0);
			Point pj1 = Point(_skeleton[j]._x, _skeleton[j]._y, 0);
			Point pj2 = Point(_skeleton[j+1]._x, _skeleton[j+1]._y, 0);

			// apparent crossing
			if (segmentsIntersect(pi1, pi2, pj1, pj2)) {

				Vector si = Vector(_skeleton[i+1]._x - _skeleton[i]._x, _skeleton[i+1]._y - _skeleton[i]._y, _skeleton[i+1]._z - _skeleton[i]._z);
				Point  ri = Point(_skeleton[i]._x, _skeleton[i]._y, _skeleton[i]._z);
				Vector sj = Vector(_skeleton[j+1]._x - _skeleton[j]._x, _skeleton[j+1]._y - _skeleton[j]._y, _skeleton[j+1]._z - _skeleton[j]._z);
				Point  rj = Point(_skeleton[j]._x, _skeleton[j]._y, _skeleton[j]._z);

				Vector sj_cross_si = sj.cross(si);
				rj.sub(ri);
				Vector v = Vector(rj._x, rj._y, rj._z);
				double sign = sj_cross_si.dot(v);

				wrz +=  (sign > 0) ? 1 : ((sign < 0) ? -1 : 0);
			}
		}
	}

	// calculate writhe
	wr = twz + wrz - tw;

	return wr;
}

//--------------------------------------------------------------
// Adapted from: Andre LeMothe's "Tricks of the Windows Game Programming Gurus"
//--------------------------------------------------------------
char Dna::segmentsIntersect(Point p0, Point p1, Point p2, Point p3) {

	float s1_x, s1_y, s2_x, s2_y;
    s1_x = p1._x - p0._x;     s1_y = p1._y - p0._y;
    s2_x = p3._x - p2._x;     s2_y = p3._y - p2._y;

    float s, t;
    s = (-s1_y * (p0._x - p2._x) + s1_x * (p0._y - p2._y)) / (-s2_x * s1_y + s1_x * s2_y);
    t = ( s2_x * (p0._y - p2._y) - s2_y * (p0._x - p2._x)) / (-s2_x * s1_y + s1_x * s2_y);

    if (s >= 0 && s <= 1 && t >= 0 && t <= 1)
    {
        // Collision detected
        return 1;
    }

    return 0; // No collision
}

//--------------------------------------------------------------
double Dna::averageAngle(void) {

	double angle = 0;

	for (int i=0; i < _segments-1; i++) {
		Vector v1 = Vector(_skeleton[i]._x - _skeleton[i+1]._x, 
						   _skeleton[i]._y - _skeleton[i+1]._y, 
						   _skeleton[i]._z - _skeleton[i+1]._z);
		Vector v2 = Vector(_skeleton[i+2]._x - _skeleton[i]._x, 
						   _skeleton[i+2]._y - _skeleton[i]._y, 
						   _skeleton[i+2]._z - _skeleton[i]._z);
		angle += v1.angle(v2);
	}

	angle /= _segments;

	return angle;
}

//--------------------------------------------------------------
void Dna::translate(double x, double y, double z) {

	for (int i=0; i < _segments; i++) {
		_skeleton[i]._x += x;
		_skeleton[i]._y += y;
		_skeleton[i]._z += z;
	}
}

//--------------------------------------------------------------
void Dna::rotate(double angle, Point p, Point q) {

	for (int i=0; i < _segments; i++) {
		_skeleton[i].rotate(angle, p, q);
	}
}

