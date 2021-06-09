#include "objects.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>  

Ophido ophido;
Sphere food;
Bunny enemy;
Powerup powerup;

// Model-view and projection matrices uniform location
GLuint  ModelView, Projection;

GLuint vColor;
GLuint vPosition;
GLuint vNormal;
GLuint vTexCoord;

enum { Xaxis = 0, Yaxis = 1, Zaxis = 2, NumAxes = 3 };
int Axis = Xaxis;
GLfloat  Theta[NumAxes] = { 0.0, 0.0, 0.0 };

int shading_mode = 0;

color4 ambient_product;
color4 diffuse_product;
color4 specular_product;

point4 light_position(-1.0, 1.0, -2.0, 1.0);
color4 light_ambient(0.2, 0.2, 0.2, 1.0);
color4 light_diffuse(1.0, 1.0, 1.0, 1.0);
color4 light_specular(1.0, 1.0, 1.0, 1.0);

color4 material_ambient(1.0, 0.0, 1.0, 1.0);
color4 material_diffuse(1.0, 0.8, 0.0, 1.0);
color4 material_specular(1.0, 0.8, 0.0, 1.0);

GLuint program;

GLuint LightPos;

int light_mode = 1;

GLuint textures[2];


GLubyte image[512][256][3];
GLubyte image2[1024][1024][3];

int textureFlag = 0; //enable texture mapping
GLuint  TextureFlagLoc;

// OpenGL initialization
void init() {

	ophido = Ophido();
	ophido.activate();
	food = Sphere(0.5, 0.5, 0);
	food.createBuffer();
	enemy = Bunny(-0.5, -0.5, 0);
	enemy.createBuffer();
	powerup = Powerup();
	powerup.activate();

	glGenTextures(2, textures);


	glBindTexture(GL_TEXTURE_2D, textures[0]);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, 512, 256, 0, GL_RGB, GL_UNSIGNED_BYTE, image);
	glGenerateMipmap(GL_TEXTURE_2D);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);

	glBindTexture(GL_TEXTURE_2D, textures[1]);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, 1024, 1024, 0, GL_RGB, GL_UNSIGNED_BYTE, image2);
	glGenerateMipmap(GL_TEXTURE_2D);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);

	glBindTexture(GL_TEXTURE_2D, textures[0]);

	// Load shaders and use the resulting shader program
	program = InitShader("vshader.glsl", "fshader.glsl");
	glUseProgram(program);

	// Create a vertex array object
	GLuint vao;
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);

	// set up vertex arrays
	vPosition = glGetAttribLocation(program, "vPosition");
	glEnableVertexAttribArray(vPosition);

	vColor = glGetAttribLocation(program, "vColor");
	glEnableVertexAttribArray(vColor);

	vNormal = glGetAttribLocation(program, "vNormal");
	glEnableVertexAttribArray(vNormal);

	vTexCoord = glGetAttribLocation(program, "vTexCoord");
	glEnableVertexAttribArray(vTexCoord);


	float  material_shininess = 100.0;

	ambient_product = light_ambient * material_ambient;
	diffuse_product = light_diffuse * material_diffuse;
	specular_product = light_specular * material_specular;

	glUniform4fv(glGetUniformLocation(program, "AmbientProduct"), 1, ambient_product);
	glUniform4fv(glGetUniformLocation(program, "DiffuseProduct"), 1, diffuse_product);
	glUniform4fv(glGetUniformLocation(program, "SpecularProduct"), 1, specular_product);

	LightPos = glGetUniformLocation(program, "LightPosition");
	glUniform4fv(LightPos, 1, light_position);

	glUniform1f(glGetUniformLocation(program, "Shininess"), material_shininess);
	glUniform1i(glGetUniformLocation(program, "ShadingType"), shading_mode);

	TextureFlagLoc = glGetUniformLocation(program, "TextureFlag");
	glUniform1i(TextureFlagLoc, textureFlag);

	// Retrieve transformation uniform variable locations
	ModelView = glGetUniformLocation(program, "ModelView");
	Projection = glGetUniformLocation(program, "Projection");

	glEnable(GL_CULL_FACE);
	glEnable(GL_DEPTH_TEST);
	glClearColor(1.0, 1.0, 1.0, 1.0);
}


struct Point {
	float x, y;
};


int isOverlapping(Point l1, Point r1, Point l2, Point r2) {
	if (l1.x > r2.x || l2.x > r1.x)
		return 0;
	if (l1.y < r2.y || l2.y < r1.y)
		return 0;
	return 1;
}

int end = 0;
int foodVis = 1;
int foodCounter;
void ophidoFoodInteraction() {
	Point o1 = Point();
	o1.x = ophido.body.at(0).x - ophido.body.at(0).size;
	o1.y = ophido.body.at(0).y + ophido.body.at(0).size;

	Point o2 = Point();
	o2.x = ophido.body.at(0).x + ophido.body.at(0).size;
	o2.y = ophido.body.at(0).y - ophido.body.at(0).size;


	Point s1 = Point();
	s1.x = food.x - 0.1;
	s1.y = food.y + 0.1;

	Point s2 = Point();
	s2.x = food.x + 0.1;
	s2.y = food.y - 0.1;


	//printf("%f, %f\n", ophido.body.at(0).x, ophido.body.at(0).y);
	//printf("o: %f, %f, %f, %f\n", o1.x, o1.y, o2.x, o2.y);
	//printf("%f, %f\n", food.x, food.y);
	//printf("s: %f, %f, %f, %f\n", s1.x, s1.y, s2.x, s2.y);
	//printf("overlap: %d", isOverlapping(o1, o2, s1, s2));

	if (isOverlapping(o1, o2, s1, s2)) {
		printf("NumNum\n");
		foodVis = 0;
		foodCounter = 40;
		ophido.grow();
	}
}

int enemyVis = 1;
int enemyCounter;
void ophidoEnemyInteraction() {

	for (int i = 0; i < ophido.length; i++) {

		Point o1 = Point();
		o1.x = ophido.body.at(i).x - ophido.body.at(i).size;
		o1.y = ophido.body.at(i).y + ophido.body.at(i).size;

		Point o2 = Point();
		o2.x = ophido.body.at(i).x + ophido.body.at(i).size;
		o2.y = ophido.body.at(i).y - ophido.body.at(i).size;


		Point s1 = Point();
		s1.x = enemy.x - (0.1);
		s1.y = enemy.y + (0.1);

		Point s2 = Point();
		s2.x = enemy.x + (0.1);
		s2.y = enemy.y - (0.1);


		//printf("%f, %f\n", ophido.body.at(0).x, ophido.body.at(0).y);
		//printf("o: %f, %f, %f, %f\n", o1.x, o1.y, o2.x, o2.y);
		//printf("%f, %f\n", food.x, food.y);
		//printf("s: %f, %f, %f, %f\n", s1.x, s1.y, s2.x, s2.y);
		//printf("overlap: %d\n", isOverlapping(o1, o2, s1, s2));

		if (isOverlapping(o1, o2, s1, s2)) {
			printf("Damage\n");
			enemyVis = 0;
			enemyCounter = 40;
			if (ophido.damage() == 1) {
				end = 1;
			}
			return;
		}
	}	
}











void idle(void) {
	glutPostRedisplay();
}


void timer(int p) {
	glutPostRedisplay();

	if (!end) {
		ophido.tick();

		if (foodVis) {
			ophidoFoodInteraction();
		}
		else {
			if (!foodCounter) {
				printf("girdim\n");
				int i = (rand() % 201);
				float x = ((float)i) / 100;
				i = (rand() % 201);
				float y = ((float)i) / 100;
				food.x = (x - 1.0);
				food.y = (y - 1.0);

				foodVis = 1;
			}
			else {
				foodCounter--;
			}
		}

		if (enemyVis) {
			enemy.tick();
			ophidoEnemyInteraction();
		}
		else {
			if (!enemyCounter) {
				printf("girdim\n");
				int i = (rand() % 201);
				float x = ((float)i) / 100;
				i = (rand() % 201);
				float y = ((float)i) / 100;
				enemy.x = (x - 1.0);
				enemy.y = (y - 1.0);
				i = rand() % 2;
				enemy.axis = i;
				enemyVis = 1;
			}
			else {
				enemyCounter--;
			}
		}
	}
	glutTimerFunc(2, timer, 0);
}

//handle keyboard inputs
void keyboard(unsigned char key, int x, int y) {
	switch (key) {
	case 033:  // Escape key
	case 'q': case 'Q':
		exit(EXIT_SUCCESS);
		break;
	case 'w': case 'W':
	case 'a': case 'A':
	case 's': case 'S':
	case 'd': case 'D':
		ophido.move(key);
		break;
	case '4':
		Theta[Yaxis] -= 2;
		break;
	case '6':
		Theta[Yaxis] += 2;
		break;
	case '8':
		Theta[Xaxis] -= 2;
		break;
	case '5':
		Theta[Xaxis] += 2;
		break;
	}
}

void mouse(int button, int state, int x, int y) {
	
}

void reshape(int w, int h) {
	glViewport(0, 0, w, h);
	GLfloat aspect = GLfloat(w) / h;
	mat4  projection = Perspective(45.0, aspect, 0.5, 3.0);
	glUniformMatrix4fv(Projection, 1, GL_TRUE, projection);
}



void display(void) {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	const vec3 viewer_pos(0.0, 0.0, 3.0);

	glUniform1i(glGetUniformLocation(program, "ShadingType"), shading_mode);
	glUniform1i(TextureFlagLoc, textureFlag);

	glUniform1i(TextureFlagLoc, textureFlag);
	glBindTexture(GL_TEXTURE_2D, textures[0]);
	ophido.draw(ModelView, vPosition, vColor, vNormal, vTexCoord, LightPos, light_position, viewer_pos, Theta[Xaxis], Theta[Yaxis], Theta[Zaxis]);
	if (foodVis) {
		glBindTexture(GL_TEXTURE_2D, textures[1]);
		food.draw(ModelView, vPosition, vColor, vNormal, vTexCoord, LightPos, light_position, viewer_pos, Theta[Xaxis], Theta[Yaxis], Theta[Zaxis]);
	}
	if (enemyVis) {
		glUniform1i(TextureFlagLoc, 0);
		enemy.draw(ModelView, vPosition, vColor, vNormal, vTexCoord, LightPos, light_position, viewer_pos, Theta[Xaxis], Theta[Yaxis], Theta[Zaxis]);
	}
	//powerup.draw(ModelView, vPosition, vColor, vNormal, vTexCoord, LightPos, light_position, viewer_pos, Theta[Xaxis], Theta[Yaxis], Theta[Zaxis]);
	glutSwapBuffers();
}



//menu for drawing mode
void DrawingModeMenu(int value) {
	switch (value) {
	case 1:
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		break;
	case 2:
		glPolygonMode(GL_FRONT_AND_BACK, GL_POINT);
		break;
	case 3:
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		break;
	}
}

//menu for background color
void BackgroundColorMenu(int value) {
	glClearColor(vertex_colors[value].x, vertex_colors[value].y, vertex_colors[value].z, vertex_colors[value].w);
}

void ShadingMenu(int value) {
	shading_mode = value;
	glUniform1i(glGetUniformLocation(program, "ShadingType"), shading_mode);
}

void TextureMenu(int value) {
	if (value == 1) {
		textureFlag = 1;
		glUniform1i(TextureFlagLoc, textureFlag);
	}
	else if (value == 2) {
		textureFlag = 0;
		glUniform1i(TextureFlagLoc, textureFlag);
	}
}

FILE* fd;
int e;
void readPPM() {
	printf("Reading basketball.ppm\n");
	char* fileName = "basketball.ppm";
	int k, n, m;
	char c;
	char b[100];
	float s;
	int red, green, blue;
	fd = fopen(fileName, "r");
	e = fscanf(fd, "%[^\n] ", b);
	if (b[0] != 'P' || b[1] != '3') {
		printf("%s is not a PPM file!\n", b);
		exit(0);
	}

	e = fscanf(fd, "%c", &c);
	while (c == '#')
	{
		e = fscanf(fd, "%[^\n] ", b);
		printf("%s\n", b);
		e = fscanf(fd, "%c", &c);
	}
	ungetc(c, fd);
	e = fscanf(fd, "%d %d %d", &n, &m, &k);
	for (int i = n - 1; i > -1; i--) {
		for (int j = 0; j < m; j++) {
			e = fscanf(fd, "%d %d %d", &red, &green, &blue);
			image[i][j][0] = (GLubyte)red;
			image[i][j][1] = (GLubyte)green;
			image[i][j][2] = (GLubyte)blue;
		}
	}
	printf("Reading basketball.ppm Done!\n");
}

void readPPM2() {
	printf("Reading cheese.ppm\n");
	int k, n, m;
	char c;
	char* fileName = "cheese.ppm";
	char b[100];
	float s;
	int red, green, blue;
	fd = fopen(fileName, "r");
	e = fscanf(fd, "%[^\n] ", b);
	if (b[0] != 'P' || b[1] != '3') {
		printf("%s is not a PPM file!\n", b);
		exit(0);
	}
	e = fscanf(fd, "%c", &c);
	while (c == '#')
	{
		e = fscanf(fd, "%[^\n] ", b);
		printf("%s\n", b);
		e = fscanf(fd, "%c", &c);
	}
	ungetc(c, fd);
	e = fscanf(fd, "%d %d %d", &n, &m, &k);
	for (int i = n - 1; i > -1; i--) {
		for (int j = 0; j < m; j++) {
			e = fscanf(fd, "%d %d %d", &red, &green, &blue);
			image2[i][j][0] = (GLubyte)red;
			image2[i][j][1] = (GLubyte)green;
			image2[i][j][2] = (GLubyte)blue;
		}
	}
	printf("Reading cheese.ppm Done!\n");
}


int main(int argc, char** argv) {

	readPPM();
	readPPM2();
	srand(time(NULL));

	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH);
	glutInitWindowSize(512, 512);
	glutInitWindowPosition(50, 50);
	glutCreateWindow("Ophido");

	int drawingModeMenu = glutCreateMenu(DrawingModeMenu);
	glutAddMenuEntry("Line", 1);
	glutAddMenuEntry("Point", 2);
	glutAddMenuEntry("Solid", 3);

	int bgcolorMenu = glutCreateMenu(BackgroundColorMenu);
	glutAddMenuEntry("White", 0);
	glutAddMenuEntry("Red", 1);
	glutAddMenuEntry("Yellow", 2);
	glutAddMenuEntry("Green", 3);
	glutAddMenuEntry("Blue", 4);
	glutAddMenuEntry("Magneta", 5);
	glutAddMenuEntry("Black", 6);
	glutAddMenuEntry("Cyan", 7);
	glutCreateMenu(BackgroundColorMenu);

	int shadingMenu = glutCreateMenu(ShadingMenu);
	glutAddMenuEntry("Phong", 0);
	glutAddMenuEntry("Gouraud", 1);
	glutAddMenuEntry("Modified Phong", 3);
	glutAddMenuEntry("Off", 2);
	glutCreateMenu(ShadingMenu);

	int textureMenu = glutCreateMenu(TextureMenu);
	glutAddMenuEntry("On", 1);
	glutAddMenuEntry("Off", 2);
	glutCreateMenu(TextureMenu);

	glutAddSubMenu("Drawing Mode", drawingModeMenu);
	glutAddSubMenu("BackgroundColor", bgcolorMenu);
	glutAddSubMenu("ShadingMode", shadingMenu);
	glutAddSubMenu("TextureMode", textureMenu);

	glutAttachMenu(GLUT_RIGHT_BUTTON);

	printf("Supported GLSL version is %s.\n", glGetString(GL_SHADING_LANGUAGE_VERSION));
	printf("%s\n%s\n%s\n",
		glGetString(GL_RENDERER),  // e.g. Intel HD Graphics 3000 OpenGL Engine
		glGetString(GL_VERSION),    // e.g. 3.2 INTEL-8.0.61
		glGetString(GL_SHADING_LANGUAGE_VERSION));

	glewExperimental = GL_TRUE;
	glewInit();

	init();

	glutDisplayFunc(display);
	glutKeyboardFunc(keyboard);
	glutMouseFunc(mouse);
	glutIdleFunc(idle);
	glutReshapeFunc(reshape);
	glutTimerFunc(2, timer, 0);
	glutMainLoop();
	return 0;
}