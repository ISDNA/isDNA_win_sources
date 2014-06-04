#include "BuildingBlock.h"

//--------------------------------------------------------------
BuildingBlock::BuildingBlock(char nucleotide) {

	_nucleotide = nucleotide;
	_vertexCount = 0;
	initColor(nucleotide);
	load(nucleotide);
}

//--------------------------------------------------------------
char BuildingBlock::getNucleotide(void) {
	return _nucleotide;
}

//--------------------------------------------------------------
GLfloat* BuildingBlock::getVertex(void) {
	return _vertex;
}

//--------------------------------------------------------------
GLfloat* BuildingBlock::getNormals(void) {
	return _normals;
}

//--------------------------------------------------------------
GLint BuildingBlock::getVertexCount(void) {
	return _vertexCount;
}

//--------------------------------------------------------------
GLfloat* BuildingBlock::getColor(void) {
	return _color;
}

//--------------------------------------------------------------
void BuildingBlock::initColor(char nucleotide) {

	switch (nucleotide) {
		case 'A': _color[0] = 0.0f; _color[1] = 0.0f; _color[2] = 1.0f; break;
		case 'C': _color[0] = 1.0f; _color[1] = 1.0f; _color[2] = 1.0f; break;
		case 'G': _color[0] = 1.0f; _color[1] = 0.0f; _color[2] = 0.0f; break;
		case 'T': _color[0] = 0.0f; _color[1] = 1.0f; _color[2] = 0.0f; break;
	}
}

//--------------------------------------------------------------
void BuildingBlock::draw(void) {

	glMaterialfv(GL_FRONT, GL_AMBIENT, _color);
	glEnableClientState(GL_VERTEX_ARRAY);
	glEnableClientState(GL_NORMAL_ARRAY);
	glVertexPointer(3, GL_FLOAT, 0, _vertex);
	glNormalPointer(GL_FLOAT, 0, _normals);
	glDrawArrays(GL_TRIANGLES, 0, _vertexCount);
	glDisableClientState(GL_TEXTURE_COORD_ARRAY);
	glDisableClientState(GL_NORMAL_ARRAY);
	glDisableClientState(GL_VERTEX_ARRAY);
}

//--------------------------------------------------------------
int BuildingBlock::load(char nucleotide) {

	FILE* f;
	char l[1024];
	char *ch;
	int i;
	char *filename = "a.ism";

	switch (nucleotide) {
		case 'A': filename = "a.ism"; break;
		case 'C': filename = "c.ism"; break;
		case 'G': filename = "g.ism"; break;
		case 'T': filename = "t.ism"; break;
	}

	f = fopen(filename, "r");

	if (f == NULL) {
		printf("Error loading nucleotide " + nucleotide);
		return (1);
	}
	i = 0;
	while (fgets(l,sizeof(l),f) != NULL) {
		if (l[0] == 'v') {
			ch = strtok (l," ");
			ch = strtok (NULL, " "); // ignore 'v'
			int j = 0;
			while (ch != NULL) {
				_vertex[i]    = atof(ch)/SCALE;	ch = strtok (NULL, " ");
				_vertex[i+1]  = atof(ch)/SCALE;	ch = strtok (NULL, " ");
				_vertex[i+2]  = atof(ch)/SCALE;	ch = strtok (NULL, " ");
				_normals[i]   = atof(ch);	ch = strtok (NULL, " ");
				_normals[i+1] = atof(ch);	ch = strtok (NULL, " ");
				_normals[i+2] = atof(ch);	ch = strtok (NULL, " ");
				ch = strtok (NULL, " ");
			}
			i+=3;
		}
	}
	_vertexCount = i/3;
	fclose(f); 
}
