#ifndef __OBJECTS_H__
#define __OBJECTS_H__

#include "Angel.h"
#include <vector>

typedef vec4  color4;
typedef vec4  point4;

// RGBA olors

const color4 vertex_colors[9] = {
	color4(1.0, 1.0, 1.0, 1.0),  // white	0
	color4(1.0, 0.0, 0.0, 1.0),  // red		1
	color4(1.0, 1.0, 0.0, 1.0),  // yellow	2
	color4(0.0, 1.0, 0.0, 1.0),  // green	3
	color4(0.0, 0.0, 1.0, 1.0),  // blue	4
	color4(1.0, 0.0, 1.0, 1.0),  // magenta	5
	color4(0.0, 0.0, 0.0, 1.0),  // black	6
	color4(0.0, 1.0, 1.0, 1.0),  // cyan	7
	color4(1.0, 0.5, 0.0, 0.0)   // orange  8
};

const float UNIT = 0.1;

class Cube {
	
	int index = 0;
	GLuint cubeBuff;

public:
	std::vector<point4> points;
	std::vector<point4> colors;
	float size = 0.05;
	float x = 0;
	float y = 0;
	float z = 0;

	float targetX = 0;
	float targetY = 0;
	float targetZ = 0;

	int wait = 1;
	int moving = 0;

	Cube();
	Cube(float size, float x, float y, float z);
	void quad(int a, int b, int c, int d, int color);
	void createBuffer();
	void refreshCubeBuffer();
	void tick();
	void draw(GLuint  ModelView, GLuint vPosition, GLuint vColor, vec3 origin, float rotateX, float rotateY, float rotateZ);
	void setTarget(float x, float y, float z);
};



class Ophido {
	float size = 0.2;

public:
	std::vector<Cube> body;
	float x = 0;
	float y = 0;
	float z = 0;
	int length = 1;

	Ophido();
	Ophido(float size, float x, float y, float z);
	void grow();
	void draw(GLuint  ModelView, GLuint vPosition, GLuint vColor, float rotateX, float rotateY, float rotateZ);
	void move(char c);
	void tick();
	void activate();
};

#endif