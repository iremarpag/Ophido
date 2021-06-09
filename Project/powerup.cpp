#include "objects.h"

Powerup::Powerup() {
	model = Cube(size, 0, 0, 0);
}

Powerup::Powerup(float size, float x, float y, float z) {
	model = Cube(size, x, y, z);
}

void Powerup::activate() {
	model.createBuffer();
}

void Powerup::draw(GLuint  ModelView, GLuint vPosition, GLuint vColor, GLuint vNormal, GLuint vTexCoord, GLuint LightPos, point4 light_position, vec3 viewer_pos, float rotateX, float rotateY, float rotateZ) {
	model.draw(ModelView, vPosition, vColor, vNormal, vTexCoord, LightPos, light_position, viewer_pos, rotateX, rotateY, rotateZ);
}


void Powerup::tick() {
	model.tick();
}