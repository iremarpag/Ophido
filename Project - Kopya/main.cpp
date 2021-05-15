#include "objects.h"

Ophido ophido;

// Model-view and projection matrices uniform location
GLuint  ModelView, Projection;

GLuint vColor;
GLuint vPosition;
enum { Xaxis = 0, Yaxis = 1, Zaxis = 2, NumAxes = 3 };
int Axis = Xaxis;
GLfloat  Theta[NumAxes] = { 0.0, 0.0, 0.0 };

// OpenGL initialization
void init() {

	ophido = Ophido();
	ophido.activate();

	// Load shaders and use the resulting shader program
	GLuint program = InitShader("vshader.glsl", "fshader.glsl");
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

	// Retrieve transformation uniform variable locations
	ModelView = glGetUniformLocation(program, "ModelView");
	Projection = glGetUniformLocation(program, "Projection");

	glEnable(GL_DEPTH_TEST);
	glClearColor(1.0, 1.0, 1.0, 1.0);
}

void display(void) {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	ophido.draw(ModelView, vPosition, vColor, Theta[Xaxis], Theta[Yaxis], Theta[Zaxis]);

	glutSwapBuffers();
}

void idle(void) {
	glutPostRedisplay();
}


void timer(int p) {
	glutPostRedisplay();
	ophido.tick();
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
	case 'g': case 'G':
		ophido.grow();
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
	ophido.grow();
}

void reshape(int w, int h) {
	glViewport(0, 0, w, h);
	mat4  projection;
	if (w <= h) {
		projection = Ortho(-1.0, 1.0, -1.0 * (GLfloat)h / (GLfloat)w, 1.0 * (GLfloat)h / (GLfloat)w, -1.0, 1.0);
	}
	else {
		projection = Ortho(-1.0 * (GLfloat)w / (GLfloat)h, 1.0 * (GLfloat)w / (GLfloat)h, -1.0, 1.0, -1.0, 1.0);
	}
	glUniformMatrix4fv(Projection, 1, GL_TRUE, projection);
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

int main(int argc, char** argv) {
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH);
	glutInitWindowSize(512, 512);
	glutInitWindowPosition(50, 50);
	glutCreateWindow("Bouncing Ball");

	int drawingModeMenu = glutCreateMenu(DrawingModeMenu);
	glutAddMenuEntry("Line", 1);
	glutAddMenuEntry("Point", 2);
	glutAddMenuEntry("Solid", 3);

	int bgcolorMenu = glutCreateMenu(BackgroundColorMenu);
	glutAddMenuEntry("Black", 0);
	glutAddMenuEntry("Red", 1);
	glutAddMenuEntry("Yellow", 2);
	glutAddMenuEntry("Green", 3);
	glutAddMenuEntry("Blue", 4);
	glutAddMenuEntry("Magneta", 5);
	glutAddMenuEntry("White", 6);
	glutAddMenuEntry("Cyan", 7);
	glutCreateMenu(BackgroundColorMenu);

	glutAddSubMenu("Drawing Mode", drawingModeMenu);
	glutAddSubMenu("BackgroundColor", bgcolorMenu);

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