#include "objects.h"
#include <math.h>


const point4 wallVer[8] = {
	point4(-1, 1, 0, 1.0),
	point4(-1, 1, 0, 1.0),
	point4(-1, -1, 0, 1.0),
	point4(-1, -1, 0, 1.0),

	point4(1, 1, 0, 1.0),
	point4(1, 1, 0, 1.0),
	point4(1, -1, 0, 1.0),
	point4(1, -1, 0, 1.0)
};


Wall::Wall() {
	//quad(0, 1, 2, 3, 1);
	//quad(4, 5, 6, 7, 1);
	quad(1, 2, 6, 5, 1);
}
Wall::Wall(int color) {
	quad(0, 1, 2, 3, color);
	quad(4, 5, 6, 7, color);
}


void Wall::quad(int a, int b, int c, int d, int color) {
	vec4 u = wallVer[b] - wallVer[a];
	vec4 v = wallVer[c] - wallVer[b];

	vec3 normal = normalize(cross(u, v));


	points.push_back(point4(wallVer[a].x, wallVer[a].y, wallVer[a].z, wallVer[a].w));
	colors.push_back(vertex_colors[color]);
	normals.push_back(normal);
	tex_coords.push_back(vec2(0.0, 0.0));

	points.push_back(point4(wallVer[b].x, wallVer[b].y, wallVer[b].z, wallVer[b].w));
	colors.push_back(vertex_colors[color]);
	normals.push_back(normal);
	tex_coords.push_back(vec2(0.0, 1.0));

	points.push_back(point4(wallVer[c].x, wallVer[c].y, wallVer[c].z, wallVer[c].w));
	colors.push_back(vertex_colors[color]);
	normals.push_back(normal);
	tex_coords.push_back(vec2(1.0, 1.0));

	points.push_back(point4(wallVer[a].x, wallVer[a].y, wallVer[a].z, wallVer[a].w));
	colors.push_back(vertex_colors[color]);
	normals.push_back(normal);
	tex_coords.push_back(vec2(0.0, 0.0));

	points.push_back(point4(wallVer[c].x, wallVer[c].y, wallVer[c].z, wallVer[c].w));
	colors.push_back(vertex_colors[color]);
	normals.push_back(normal);
	tex_coords.push_back(vec2(1.0, 1.0));

	points.push_back(point4(wallVer[d].x, wallVer[d].y, wallVer[d].z, wallVer[d].w));
	colors.push_back(vertex_colors[color]);
	normals.push_back(normal);
	tex_coords.push_back(vec2(1.0, 0.0));
}

void Wall::createBuffer() {
	glGenBuffers(1, &wallBuff);
	glBindBuffer(GL_ARRAY_BUFFER, wallBuff);
	glBufferData(GL_ARRAY_BUFFER, sizeof(point4) * points.size() + sizeof(color4) * colors.size() + sizeof(vec3) * normals.size() + sizeof(vec2) * tex_coords.size(), NULL, GL_STATIC_DRAW);
	refreshBuffer();
}

void Wall::refreshBuffer() {
	glBindBuffer(GL_ARRAY_BUFFER, wallBuff);
	glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(point4) * points.size(), &points[0]);
	glBufferSubData(GL_ARRAY_BUFFER, sizeof(point4) * points.size(), sizeof(color4) * colors.size(), &colors[0]);
	glBufferSubData(GL_ARRAY_BUFFER, (sizeof(point4) * points.size()) + (sizeof(color4) * colors.size()), sizeof(vec3) * normals.size(), &normals[0]);
	glBufferSubData(GL_ARRAY_BUFFER, (sizeof(point4) * points.size()) + (sizeof(color4) * colors.size()) + (sizeof(vec3) * normals.size()), sizeof(vec2) * tex_coords.size(), &tex_coords[0]);

}

void Wall::draw(GLuint  ModelView, GLuint vPosition, GLuint vColor, GLuint vNormal, GLuint vTexCoord, GLuint LightPos, point4 light_position, vec3 viewer_pos, float rotateX, float rotateY, float rotateZ) {
	glBindBuffer(GL_ARRAY_BUFFER, wallBuff);
	glVertexAttribPointer(vPosition, 4, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(0));
	glVertexAttribPointer(vColor, 4, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET((sizeof(point4) * points.size())));
	glVertexAttribPointer(vNormal, 3, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET((sizeof(point4) * points.size()) + (sizeof(color4) * colors.size())));
	glVertexAttribPointer(vTexCoord, 2, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET((sizeof(point4) * points.size()) + (sizeof(color4) * colors.size()) + (sizeof(vec3) * normals.size())));

	//point4 lightPos(light_position.x - (-1.0), light_position.y - 0, light_position.z - 0, light_position.w);
	glUniform4fv(LightPos, 1, light_position);

	mat4  model_view = (RotateX(rotateX) * RotateY(rotateY) * RotateZ(rotateZ) * Translate(-viewer_pos));
	glUniformMatrix4fv(ModelView, 1, GL_TRUE, model_view);

	glDrawArrays(GL_POLYGON, 0, 6);
}