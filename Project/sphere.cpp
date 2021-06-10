#include "objects.h"
#include <math.h>

Sphere::Sphere() {
	this->x = 0;
	this->y = 0;
	this->z = 0;
	tetrahedron(NumTimesToSubdivide);
}

Sphere::Sphere(float x, float y, float z) {
	this->x = x;
	this->y = y;
	this->z = z;
	tetrahedron(NumTimesToSubdivide);
}

void Sphere::triangle(const point4& a, const point4& b, const point4& c) {
	vec3  normal = normalize(cross(b - a, c - b));
	normals.push_back(normal); points.push_back(a); colors.push_back(vertex_colors[3]);
	normals.push_back(normal); points.push_back(b); colors.push_back(vertex_colors[3]);
	normals.push_back(normal); points.push_back(c); colors.push_back(vertex_colors[3]);
}

point4 Sphere::unit(const point4& p) {
	float len = p.x * p.x + p.y * p.y + p.z * p.z;

	point4 t;
	if (len > DivideByZeroTolerance) {
		t = p / sqrt(len);
		t.w = 1.0;
	}

	return t;
}

void Sphere::divide_triangle(const point4& a, const point4& b, const point4& c, int count) {
	if (count > 0) {
		point4 v1 = unit(a + b);
		point4 v2 = unit(a + c);
		point4 v3 = unit(b + c);
		divide_triangle(a, v1, v2, count - 1);
		divide_triangle(c, v2, v3, count - 1);
		divide_triangle(b, v3, v1, count - 1);
		divide_triangle(v1, v3, v2, count - 1);
	}
	else {
		triangle(a, b, c);
	}
}

void Sphere::tetrahedron(int count) {
	point4 v[4] = {
	vec4(0.0, 0.0, 1.0, 1.0),
	vec4(0.0, 0.942809, -0.333333, 1.0),
	vec4(-0.816497, -0.471405, -0.333333, 1.0),
	vec4(0.816497, -0.471405, -0.333333, 1.0)
	};

	divide_triangle(v[0], v[1], v[2], count);
	divide_triangle(v[3], v[2], v[1], count);
	divide_triangle(v[0], v[3], v[1], count);
	divide_triangle(v[0], v[2], v[3], count);



	for (int i = 0; i < SphereNumVertices; i++) {
		float u = (0.5 + ((atan2(points[i].z, points[i].x)) / (2 * M_PI)));
		float v = (0.5 + ((asin(points[i].y)) / M_PI));
		vec2 coor(u, v);
		tex_coords.push_back(coor);
	}

	//since our sphere is too big I multiply all point's varibles with 0.1 to set its size 1/10 of its original size
	for (int i = 0; i < SphereNumVertices; i++) {
		points[i].x = points[i].x * 0.1;
		points[i].y = points[i].y * 0.1;
		points[i].z = points[i].z * 0.1;
	}

}

void Sphere::createBuffer() {
	glGenBuffers(1, &sphereBuff);
	glBindBuffer(GL_ARRAY_BUFFER, sphereBuff);
	glBufferData(GL_ARRAY_BUFFER, sizeof(point4) * points.size() + sizeof(color4) * colors.size() + sizeof(vec3) * normals.size() + sizeof(vec2) * tex_coords.size(), NULL, GL_STATIC_DRAW);
	refreshSphereBuffer();
}

void Sphere::refreshSphereBuffer() {
	glBindBuffer(GL_ARRAY_BUFFER, sphereBuff);
	glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(point4) * points.size(), &points[0]);
	glBufferSubData(GL_ARRAY_BUFFER, (sizeof(point4) * points.size()), sizeof(color4) * colors.size(), &colors[0]);
	glBufferSubData(GL_ARRAY_BUFFER, (sizeof(point4) * points.size()) + (sizeof(color4) * colors.size()), sizeof(vec3) * normals.size(), &normals[0]);
	glBufferSubData(GL_ARRAY_BUFFER, (sizeof(point4) * points.size()) + (sizeof(color4) * colors.size()) + (sizeof(vec3) * normals.size()), sizeof(vec2) * tex_coords.size(), &tex_coords[0]);
}

void Sphere::draw(GLuint  ModelView, GLuint vPosition, GLuint vColor, GLuint vNormal, GLuint vTexCoord, GLuint LightPos, point4 light_position, vec3 viewer_pos, float rotateX, float rotateY, float rotateZ) {
	glBindBuffer(GL_ARRAY_BUFFER, sphereBuff);
	glVertexAttribPointer(vPosition, 4, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(0));
	glVertexAttribPointer(vColor, 4, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET((sizeof(point4) * points.size())));
	glVertexAttribPointer(vNormal, 3, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET((sizeof(point4) * points.size()) + (sizeof(color4) * colors.size())));
	glVertexAttribPointer(vTexCoord, 2, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET((sizeof(point4) * points.size()) + (sizeof(color4) * colors.size()) + (sizeof(vec3) * normals.size())));

	point4 lightPos(light_position.x - this->x, light_position.y - this->y, light_position.z - this->z, light_position.w);
	glUniform4fv(LightPos, 1, lightPos);

	mat4  model_view = (RotateX(rotateX) * RotateY(rotateY) * RotateZ(rotateZ) * Translate(-viewer_pos) * Translate(vec3(this->x, this->y, this->z)));
	glUniformMatrix4fv(ModelView, 1, GL_TRUE, model_view);

	glDrawArrays(GL_TRIANGLES, 0, SphereNumVertices);
}