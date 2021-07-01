#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif

#include <math.h>

int tx = 0, ty = 0, tz = 0;
int angle = 0, rx = 0, ry = 1, rz = 0;
int sx = 1, sy = 1, sz = 1;

void changeSize(int w, int h) {

	// Prevent a divide by zero, when window is too short
	// (you cant make a window with zero width).
	if(h == 0)
		h = 1;

	// compute window's aspect ratio 
	float ratio = w * 1.0 / h;

	// Set the projection matrix as current
	glMatrixMode(GL_PROJECTION);
	// Load Identity Matrix
	glLoadIdentity();
	
	// Set the viewport to be the entire window
    glViewport(0, 0, w, h);

	// Set perspective
	gluPerspective(45.0f ,ratio, 1.0f ,1000.0f);

	// return to the model view matrix mode
	glMatrixMode(GL_MODELVIEW);
}


void renderScene(void) {

	// clear buffers
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// set the camera
	glLoadIdentity();
	gluLookAt(5.0,5.0,5.0, 
		      0.0,0.0,0.0,
			  0.0f,1.0f,0.0f);

// put the geometric transformations here
	glTranslatef(tx,ty,tz);
	glRotatef(angle,rx,ry,rz);
	glScalef(sx, sy, sz);

// put drawing instructions here
	glBegin(GL_LINES);
		// X axis in red
		glColor3f(1.0f, 0.0f, 0.0f);
		glVertex3f(-100.0f, 0.0f, 0.0f);
		glVertex3f(100.0f, 0.0f, 0.0f);
		// Y Axis in Green
		glColor3f(0.0f, 1.0f, 0.0f);
		glVertex3f(0.0f, -100.0f, 0.0f);
		glVertex3f(0.0f, 100.0f, 0.0f);
		// Z Axis in Blue
		glColor3f(0.0f, 0.0f, 1.0f);
		glVertex3f(0.0f, 0.0f, -100.0f);
		glVertex3f(0.0f, 0.0f, 100.0f);
	glEnd();

	glBegin(GL_TRIANGLES);
		glColor3f(1.0f, 1.0f, 1.0f);
		glVertex3f(1.0f, 0.0f, 1.0f);
		glVertex3f(1.0f, 0.0f, -1.0f);
		glVertex3f(-1.0f, 0.0f, -1.0f);

		glVertex3f(-1.0f, 0.0f, -1.0f);
		glVertex3f(-1.0f, 0.0f, 1.0f);
		glVertex3f(1.0f, 0.0f, 1.0f);

		glColor3f(1.0f, 0.0f, 0.0f);
		glVertex3f(1.0f, 0.0f, 1.0f);
		glVertex3f(0.0f, 2.0f, 0.0f);
		glVertex3f(-1.0f, 0.0f, 1.0f);

		glColor3f(0.0f, 1.0f, 0.0f);
		glVertex3f(1.0f, 0.0f, -1.0f);
		glVertex3f(0.0f, 2.0f, 0.0f);
		glVertex3f(1.0f, 0.0f, 1.0f);
		
		glColor3f(0.0f, 0.0f, 1.0f);
		glVertex3f(-1.0f, 0.0f, -1.0f);
		glVertex3f(0.0f, 2.0f, 0.0f);
		glVertex3f(1.0f, 0.0f, -1.0f);
		
		glColor3f(1.0f, 1.0f, 1.0f);
		glVertex3f(-1.0f, 0.0f, 1.0f);
		glVertex3f(0.0f, 2.0f, 0.0f);
		glVertex3f(-1.0f, 0.0f, -1.0f);
	glEnd();

	// End of frame
	glutSwapBuffers();
}



// write function to process keyboard events
void translacao(int key_code, int x, int y) {
	if (key_code == GLUT_KEY_UP) { tx -= 1; tz -= 1; }
	if (key_code == GLUT_KEY_DOWN) { tx += 1; tz += 1; }
	if (key_code == GLUT_KEY_LEFT) { tx -= 1; tz += 1; }
	if (key_code == GLUT_KEY_RIGHT) { tx += 1; tz -= 1; }
	glutPostRedisplay();
}

void rotacao(int button, int state, int x, int y) {
	if (state == GLUT_UP) {
		if (button == GLUT_LEFT_BUTTON) angle += 25;
		if (button == GLUT_RIGHT_BUTTON) angle -= 25;
	}
	glutPostRedisplay();
}

void modoDesenhoOUEscalamento(unsigned char key, int x, int y) {
	if (key == 'q') glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	if (key == 'w') glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	if (key == 'e') glPolygonMode(GL_FRONT_AND_BACK, GL_POINT);
	if (key == 'x') sy += 1;
	if (key == 'z') sy -= 1;
	glutPostRedisplay();
}


int main(int argc, char **argv) {

// init GLUT and the window
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DEPTH|GLUT_DOUBLE|GLUT_RGBA);
	glutInitWindowPosition(100,100);
	glutInitWindowSize(800,800);
	glutCreateWindow("CG@DI-UM");
		
// Required callback registry 
	glutDisplayFunc(renderScene);
	glutReshapeFunc(changeSize);
	
// put here the registration of the keyboard callbacks
	glutSpecialFunc(translacao);
	glutKeyboardFunc(modoDesenhoOUEscalamento);
	glutMouseFunc(rotacao);

//  OpenGL settings
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);
	
// enter GLUT's main cycle
	glutMainLoop();
	
	return 1;
}
