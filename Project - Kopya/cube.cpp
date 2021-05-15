#include "objects.h"
#include <time.h>
#include <stdlib.h>

Cube::Cube() {
	srand(time(NULL));
	int r = rand() % 8;
	r++;
	quad(1, 0, 3, 2, 3); //front
	quad(2, 3, 7, 6, 1); //right
	quad(3, 0, 4, 7, 4); //down
	quad(6, 5, 1, 2, 6); //up
	quad(4, 5, 6, 7, 2); //back
	quad(5, 4, 0, 1, 8); //left
}

Cube::Cube(float size, float x, float y, float z) {
	srand(time(NULL));
	int r = rand() % 8;
	r++;
	this->size = size/2;
	this->x = x;
	this->y = y;
	this->z = z;
	quad(1, 0, 3, 2, 3); //front
	quad(2, 3, 7, 6, 1); //right
	quad(3, 0, 4, 7, 4); //down
	quad(6, 5, 1, 2, 6); //up
	quad(4, 5, 6, 7, 2); //back
	quad(5, 4, 0, 1, 8); //left
}

const point4 cubeVertices[8] = {
	point4(-1, -1,  1, 1.0),
	point4(-1, 1,  1, 1.0),
	point4(1,  1,  1, 1.0),
	point4(1, -1,  1, 1.0),
	point4(-1, -1, -1, 1.0),
	point4(-1,  1, -1, 1.0),
	point4(1,  1, -1, 1.0),
	point4(1, -1, -1, 1.0)
};

void Cube::quad(int a, int b, int c, int d, int color) {
	points.push_back(point4(cubeVertices[a].x * size, cubeVertices[a].y * size, cubeVertices[a].z * size, cubeVertices[a].w));
	colors.push_back(vertex_colors[color]);
	points.push_back(point4(cubeVertices[b].x * size, cubeVertices[b].y * size, cubeVertices[b].z * size, cubeVertices[b].w));
	colors.push_back(vertex_colors[color]);
	points.push_back(point4(cubeVertices[c].x * size, cubeVertices[c].y * size, cubeVertices[c].z * size, cubeVertices[c].w));
	colors.push_back(vertex_colors[color]);
	points.push_back(point4(cubeVertices[a].x * size, cubeVertices[a].y * size, cubeVertices[a].z * size, cubeVertices[a].w));
	colors.push_back(vertex_colors[color]);
	points.push_back(point4(cubeVertices[c].x * size, cubeVertices[c].y * size, cubeVertices[c].z * size, cubeVertices[c].w));
	colors.push_back(vertex_colors[color]);
	points.push_back(point4(cubeVertices[d].x * size, cubeVertices[d].y * size, cubeVertices[d].z * size, cubeVertices[d].w));
	colors.push_back(vertex_colors[color]);
}

void Cube::createBuffer() {
	glGenBuffers(1, &cubeBuff);
	glBindBuffer(GL_ARRAY_BUFFER, cubeBuff);
	glBufferData(GL_ARRAY_BUFFER, sizeof(point4) * points.size() + sizeof(color4) * colors.size(), NULL, GL_STATIC_DRAW);
	refreshCubeBuffer();
}

void Cube::refreshCubeBuffer() {
	glBindBuffer(GL_ARRAY_BUFFER, cubeBuff);
	glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(point4) * points.size(), &points[0]);
	glBufferSubData(GL_ARRAY_BUFFER, sizeof(point4) * points.size(), sizeof(color4) * colors.size(), &colors[0]);
}

void Cube::draw(GLuint  ModelView, GLuint vPosition, GLuint vColor, vec3 origin, float rotateX, float rotateY, float rotateZ) {
	glBindBuffer(GL_ARRAY_BUFFER, cubeBuff);
	glVertexAttribPointer(vPosition, 4, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(0));
	glVertexAttribPointer(vColor, 4, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(sizeof(point4) * points.size()));

	vec3 displacement(this->x, this->y, this->z);
	mat4  model_view = (Translate(origin) * RotateX(rotateX) * RotateY(rotateY) * RotateZ(rotateZ) * Translate(displacement));
	glUniformMatrix4fv(ModelView, 1, GL_TRUE, model_view);

	glDrawArrays(GL_TRIANGLES, 0, 36);
}
int count = 0;
void Cube::tick() {
	if (this->x < targetX) {
		this->x += 0.01;
		count++;
	}
	if (this->x > targetX) {
		this->x -= 0.01;
		count++;
	}
	if (this->y < targetY) {
		this->y += 0.01;
		count++;
	}
	if (this->y > targetY) {
		this->y -= 0.01;
		count++;
	}
	/*if (moving) {

		/*if (count == 10) {
			printf("loo\n");
			moving = 0;
			count = 0;
		}
		else {
			
		}
		
	}*/

}


void Cube::setTarget(float x, float y, float z) {
	this->targetX = x;
	this->targetY = y;
	this->targetZ = z;
}

