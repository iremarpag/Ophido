#include "objects.h"

Powerup::Powerup() {
	model = Cube(size, 0, 0, 0);
}

Powerup::Powerup(float x, float y, float z) {
	model = Cube(size, x, y, z);
	this->x = x;
	this->y = y;
	this->z = z;
}

void Powerup::activate() {
	model.createBuffer();
}

void Powerup::draw(GLuint  ModelView, GLuint vPosition, GLuint vColor, GLuint vNormal, GLuint vTexCoord, GLuint LightPos, point4 light_position, vec3 viewer_pos, float rotateX, float rotateY, float rotateZ) {
	if (type == 0) {
		model.setColor(1);
	}
	else if (type == 1) {
		model.setColor(4);
	}
	else if (type == 2) {
		model.setColor(8);
	}
	model.x = this->x;
	model.y = this->y;
	model.draw(ModelView, vPosition, vColor, vNormal, vTexCoord, LightPos, light_position, viewer_pos, rotateX, rotateY, rotateZ);
}


void Powerup::tick() {
	model.tick();
}