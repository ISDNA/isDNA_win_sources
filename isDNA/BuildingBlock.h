#pragma once
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <GL/glui.h>

class BuildingBlock
{

private :

	// constants
	static const int SCALE = 1;

	// variables
	char		_nucleotide;
	GLfloat		_vertex[2500];
	GLfloat		_normals[2500];
	GLint		_vertexCount;
	GLfloat		_color[3];

	// methods
	void		initColor(char);
	int			load(char);

public:
	BuildingBlock(char);
	char		getNucleotide(void);
	GLfloat*	getVertex(void);
	GLfloat*	getNormals(void);
	GLint		getVertexCount(void);
	GLfloat*	getColor(void);
	void		draw(void);

};
