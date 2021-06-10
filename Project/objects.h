#ifndef __OBJECTS_H__
#define __OBJECTS_H__

#include "Angel.h"
#include <vector>
#include <string>
#include <fstream>
#include <iostream>

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

const int NumTimesToSubdivide = 8;
const int NumTriangles = 262144;  // (4 faces)^(NumTimesToSubdivide + 1)
const int SphereNumVertices = 3 * NumTriangles;
/*
const int NumTimesToSubdivide = 5;
const int NumTriangles = 4096;  // (4 faces)^(NumTimesToSubdivide + 1)
const int SphereNumVertices = 3 * NumTriangles;
*/

class Sphere {
private:
public:
	GLuint sphereBuff;
	std::vector<point4> points;
	std::vector<point4> colors;
	std::vector<vec3> normals;
	std::vector<vec2> tex_coords;


	float x = 0;
	float y = 0;
	float z = 0;

	Sphere();
	Sphere(float x, float y, float z);

	void triangle(const point4& a, const point4& b, const point4& c);
	point4 unit(const point4& p);
	void divide_triangle(const point4& a, const point4& b, const point4& c, int count);
	void tetrahedron(int count);
	void createBuffer();
	void refreshSphereBuffer();
	void draw(GLuint  ModelView, GLuint vPosition, GLuint vColor, GLuint vNormal, GLuint vTexCoord, GLuint LightPos, point4 light_position, vec3 viewer_pos, float rotateX, float rotateY, float rotateZ);
};





const float UNIT = 0.1;

class Cube {
	
	int index = 0;
	GLuint cubeBuff;

public:
	std::vector<point4> points;
	std::vector<point4> colors;
	std::vector<vec3> normals;
	std::vector<vec2> tex_coords;

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
	void setColor(int i);
	void draw(GLuint  ModelView, GLuint vPosition, GLuint vColor, GLuint vNormal, GLuint vTexCoord, GLuint LightPos, point4 light_position, vec3 viewer_pos, float rotateX, float rotateY, float rotateZ);
	void setTarget(float x, float y, float z);
};



class Ophido {
	float size = 0.1;

public:
	std::vector<Cube> body;
	float x = 0;
	float y = 0;
	float z = 0;
	int length = 1;

	Ophido();
	Ophido(float size, float x, float y, float z);
	void grow();
	int damage();
	void draw(GLuint  ModelView, GLuint vPosition, GLuint vColor, GLuint vNormal, GLuint vTexCoord, GLuint LightPos, point4 light_position, vec3 viewer_pos, float rotateX, float rotateY, float rotateZ);
	void move(char c);
	void tick();
	void activate();
};



class Powerup {
	float size = 0.1;

public:
	Cube model;
	float x = 0;
	float y = 0;
	float z = 0;
	int type = 0;

	Powerup();
	Powerup(float x, float y, float z);
	void draw(GLuint  ModelView, GLuint vPosition, GLuint vColor, GLuint vNormal, GLuint vTexCoord, GLuint LightPos, point4 light_position, vec3 viewer_pos, float rotateX, float rotateY, float rotateZ);
	void tick();
	void activate();
};






const std::string bunnyPath = "bunny.off";

class Bunny {
private:
	int bColor = 0;
	int bunnyNumVertices = 0;

public:
	GLuint bunnyBuff;
	std::vector<point4> points;
	std::vector<point4> colors;
	std::vector<vec3> normals;
	std::vector<vec2> tex_coords;

	float x = 0;
	float y = 0;
	float z = 0;
	int axis = 0;
	int faced = 0;


	Bunny();
	Bunny(float x, float y, float z);

	void setVertices();
	void createBuffer();
	void refreshBuffer();
	void tick();
	void draw(GLuint  ModelView, GLuint vPosition, GLuint vColor, GLuint vNormal, GLuint vTexCoord, GLuint LightPos, point4 light_position, vec3 viewer_pos, float rotateX, float rotateY, float rotateZ);
};







class Wall {
private:
public:
	GLuint wallBuff;
	std::vector<point4> points;
	std::vector<point4> colors;
	std::vector<vec3> normals;
	std::vector<vec2> tex_coords;

	Wall();
	Wall(int color);
	void quad(int a, int b, int c, int d, int color);
	void createBuffer();
	void refreshBuffer();
	void draw(GLuint  ModelView, GLuint vPosition, GLuint vColor, GLuint vNormal, GLuint vTexCoord, GLuint LightPos, point4 light_position, vec3 viewer_pos, float rotateX, float rotateY, float rotateZ);
};

#endif