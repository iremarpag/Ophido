#include "objects.h"

Ophido::Ophido() {
	body.push_back(Cube(size, 0, 0, 0));
	body.at(0).wait = 0;
}

Ophido::Ophido(float size, float x, float y, float z) {
	body.push_back(Cube(size, x, y, z));
	body.at(0).wait = 0;
}

void Ophido::activate() {
	body.at(0).createBuffer();
}

void Ophido::grow() {
	body.push_back(Cube(size, body.at(length - 1).x, body.at(length - 1).y, body.at(length - 1).z));
	body.at(length).setTarget(body.at(length-1).x, body.at(length-1).y, body.at(length-1).z);
	printf("target %f %f %f\n", body.at(length).targetX, body.at(length).targetY, body.at(length).targetZ);
	body.at(length).createBuffer();
	length++;
}

int Ophido::damage() {


	if (length == 1) {
		return 1;
	}
	
	body.pop_back();
	printf("Damage!! New Length: %d\n", length);
	length--;
}




void Ophido::draw(GLuint  ModelView, GLuint vPosition, GLuint vColor, GLuint vNormal, GLuint vTexCoord, GLuint LightPos, point4 light_position, vec3 viewer_pos, float rotateX, float rotateY, float rotateZ) {
	vec3 origin(0,0,0);
	for (int i = 0; i < length; i++) {
		body.at(i).draw(ModelView, vPosition, vColor, vNormal, vTexCoord, LightPos, light_position, viewer_pos, rotateX, rotateY, rotateZ);
	}
}

void Ophido::move(char c) {
	//if (!body.at(0).moving) {
		switch (c) {
		case 'w': case 'W':
			body.at(0).setTarget(body.at(0).x, body.at(0).y + UNIT, body.at(0).z);
			for (int i = 1; i < length; i++) {
				body.at(i).setTarget(body.at(i - 1).x, body.at(i - 1).y, body.at(i - 1).z);
			}
			break;
		case 'a': case 'A':
			body.at(0).setTarget(body.at(0).x - UNIT, body.at(0).y, body.at(0).z);
			for (int i = 1; i < length; i++) {
				body.at(i).setTarget(body.at(i - 1).x, body.at(i - 1).y, body.at(i - 1).z);
			}
			break;
		case 's': case 'S':
			body.at(0).setTarget(body.at(0).x, body.at(0).y - UNIT, body.at(0).z);
			for (int i = 1; i < length; i++) {
				body.at(i).setTarget(body.at(i - 1).x, body.at(i - 1).y, body.at(i - 1).z);
			}
			break;
		case 'd': case 'D':
			body.at(0).setTarget(body.at(0).x + UNIT, body.at(0).y, body.at(0).z);
			for (int i = 1; i < length; i++) {
				body.at(i).setTarget(body.at(i - 1).x , body.at(i - 1).y, body.at(i - 1).z);
			}
			break;
		}
		switch (c) {
		case 'w': case 'W':
		case 'a': case 'A':
		case 's': case 'S':
		case 'd': case 'D':
			for (int i = 0; i < length; i++) {
				body.at(i).moving = 1;
			}
			break;
		}
	//}
}

void Ophido::tick() {
	for (int i = 0; i < length; i++) {
		body.at(i).tick();
		//printf("%d: %f, %f, %f, %d, %f, %f, %f\n", i, body.at(i).x, body.at(i).y, body.at(i).z, body.at(i).moving, body.at(i).targetX, body.at(i).targetY, body.at(i).targetZ);
	}
}