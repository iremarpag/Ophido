#include "objects.h"

//for bunny object since we read the model's information from .off file we don't know exact number of point
//so we cannot use array for hold the point information thus I used vectors as data holder

using namespace std;

Bunny::Bunny() {
	bColor = 1;
	this->x = 0;
	this->y = 0;
	this->z = 0;
	setVertices();
}

Bunny::Bunny(float x, float y, float z) {
	bColor = 1;
	this->x = x;
	this->y = y;
	this->z = z;
	setVertices();
}

void Bunny::createBuffer() {
	glGenBuffers(1, &bunnyBuff);
	glBindBuffer(GL_ARRAY_BUFFER, bunnyBuff);
	glBufferData(GL_ARRAY_BUFFER, sizeof(point4) * points.size() + sizeof(color4) * colors.size() + sizeof(vec3) * normals.size() + sizeof(vec2) * tex_coords.size(), NULL, GL_STATIC_DRAW);
	refreshBuffer();
}

void Bunny::refreshBuffer() {
	glBindBuffer(GL_ARRAY_BUFFER, bunnyBuff);
	glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(point4) * points.size(), &points[0]);
	glBufferSubData(GL_ARRAY_BUFFER, (sizeof(point4) * points.size()), sizeof(color4) * colors.size(), &colors[0]);
	glBufferSubData(GL_ARRAY_BUFFER, (sizeof(point4) * points.size()) + (sizeof(color4) * colors.size()), sizeof(vec3) * normals.size(), &normals[0]);
	glBufferSubData(GL_ARRAY_BUFFER, (sizeof(point4) * points.size()) + (sizeof(color4) * colors.size()) + (sizeof(vec3) * normals.size()), sizeof(vec2) * tex_coords.size(), &tex_coords[0]);
}




void Bunny::draw(GLuint  ModelView, GLuint vPosition, GLuint vColor, GLuint vNormal, GLuint vTexCoord, GLuint LightPos, point4 light_position, vec3 viewer_pos, float rotateX, float rotateY, float rotateZ) {
	refreshBuffer();
	glBindBuffer(GL_ARRAY_BUFFER, bunnyBuff);
	glVertexAttribPointer(vPosition, 4, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(0));
	glVertexAttribPointer(vColor, 4, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET((sizeof(point4) * points.size())));
	glVertexAttribPointer(vNormal, 3, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET((sizeof(point4) * points.size()) + (sizeof(color4) * colors.size())));
	glVertexAttribPointer(vTexCoord, 2, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET((sizeof(point4) * points.size()) + (sizeof(color4) * colors.size()) + (sizeof(vec3) * normals.size())));

	point4 lightPos(light_position.x - this->x, light_position.y - this->y, light_position.z - this->z, light_position.w);
	glUniform4fv(LightPos, 1, lightPos);

	float Xrot = 0;
	float Yrot = 0;
	float Zrot = 0;

	if (axis) {
		Zrot = 90;
		if (faced) {
			Zrot = 270;
		}
	}
	else {
		if (!faced) {
			Zrot = 180;
		}
	}


	mat4  model_view = (RotateX(rotateX) * RotateY(rotateY) * RotateZ(rotateZ) * Translate(-viewer_pos) * Translate(vec3(this->x, this->y, this->z)) * RotateX(Xrot) * RotateY(Yrot) * RotateZ(Zrot));
	glUniformMatrix4fv(ModelView, 1, GL_TRUE, model_view);

	glDrawArrays(GL_TRIANGLES, 0, bunnyNumVertices);
}



void Bunny::tick() {
	if (axis) {//x axis
		if (this->x > 0.9 && faced == 0) {
			faced = 1;
		}
		else if (this->x < -0.9 && faced == 1) {
			faced = 0;
		}

		if (faced) {
			this->x = this->x - 0.01;
		}
		else {
			this->x = this->x + 0.01;
		}
	}
	else {//y axis
		if (this->y > 0.9 && faced == 0) {
			faced = 1;
		}
		else if (this->y < -0.9 && faced == 1) {
			faced = 0;
		}

		if (faced) {
			this->y = this->y - 0.01;
		}
		else {
			this->y = this->y + 0.01;
		}
	}
}



void Bunny::setVertices() {
	std::ifstream infile(bunnyPath);
	string line;
	infile >> line;
	if (line == "OFF") {//checks if it is an off file
		std::vector<point4> vertices;

		infile >> line;
		int verticesNum = stoi(line);
		infile >> line;
		int nor = stoi(line);
		infile >> line;

		for (int i = 0; i < verticesNum; i++) {
			infile >> line;
			float x = std::stof(line);

			infile >> line;
			float y = std::stof(line);

			infile >> line;
			float z = std::stof(line);

			//before setting read point's x, y and z I divide all the points' x value with half size on x axis, 
			//y value with half size on y axis and z value with half size on z axis to make it fit on the screen
			//screen size is 2, 2, 2 thats why I divide half of the size on the related axis
			//then I multiply each value with 0.1 to make its size divided by ten (it is the default size of the all objects)
			//after that I move the points to set bunny's position correct.
			point4 p = point4(x / (17.5 / 2) * 0.1, y / (29.7 / 2) * 0.1 - 0.1, z / (33.1 / 2) * 0.1, 1.0);
			vertices.push_back(p);

			float u = (0.5 + ((atan2(z, x)) / (2 * M_PI)));
			float v = (0.5 + ((asin(y)) / M_PI));
			vec2 coor(u, v);
			tex_coords.push_back(coor);
		}


		//since I fixed all points' values while reading them I don't need to modfy them while creating triangles.
		for (int i = 0; i < nor; i++) {
			infile >> line; //always 3

			infile >> line;
			int a = std::stof(line);

			infile >> line;
			int b = std::stof(line);

			infile >> line;
			int c = std::stof(line);

			vec3  normal = normalize(cross(vertices.at(b) - vertices.at(a), vertices.at(c) - vertices.at(b)));
			points.push_back(vertices.at(a));
			colors.push_back(vertex_colors[bColor]);
			normals.push_back(normal);
			bunnyNumVertices++;

			points.push_back(vertices.at(b));
			colors.push_back(vertex_colors[bColor]);
			normals.push_back(normal);
			bunnyNumVertices++;

			points.push_back(vertices.at(c));
			colors.push_back(vertex_colors[bColor]);
			normals.push_back(normal);
			bunnyNumVertices++;


		}
	}
}