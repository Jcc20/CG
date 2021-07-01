#define _USE_MATH_DEFINES

#include <stdio.h>
#include <stdlib.h>
#include "tinyxml2/tinyxml2.cpp"
#include "tinyxml2/tinyxml2.h"
#include <GL/glut.h>
#include <math.h>
#include <vector>
#include <iostream> 

using namespace tinyxml2;
using namespace std;

 struct Triangle{
	float x1, y1, z1;
	float x2, y2, z2;
	float x3, y3, z3;
};

struct Model {
	string nome;
	vector<Triangle> triangles;
};

float camX = 10, camY = 0, camZ = 0, a = 0, b = 0, zoom = 10 ;
int frame = 0,  time = 0,   timebase = 0, fps = 0;

bool axis = false;

vector<string> modelos;

vector<Model> models;

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
gluPerspective(45.0f, ratio, 1.0f, 1000.0f);

// return to the model view matrix mode
glMatrixMode(GL_MODELVIEW);
}

void parseFiles() {//itera os ficheiros de dados e guarda numa estrutura de dados
	for (int r=0; r< modelos.size(); r++) {
		Model m;
		m.nome = modelos.at(r);
		printf("A carregar modelo numero %d chamado %s \n", r, m.nome.data());
		FILE* file = fopen(m.nome.data(), "r");
		char* buffer = (char*) malloc(sizeof(char)*1024);
		while (fgets(buffer, 1024, file)) {
			Triangle tri;
			int shutup = sscanf(buffer, "%f%f%f%f%f%f%f%f%f", &tri.x1, &tri.y1, &tri.z1, &tri.x2, &tri.y2, &tri.z2, &tri.x3, &tri.y3, &tri.z3);
			m.triangles.push_back(tri);
		}
		models.push_back(m);
		fclose(file);
		free(buffer);
		printf("Carregado o modelo %s com %d triangulos\n\n",m.nome.data(),m.triangles.size());
	}

};

void drawModel() {
	
	if (axis) { //eixos
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
	}

	for (int p = 0; p < models.size(); p++) {
		glBegin(GL_TRIANGLES);
		glColor3f(1.0f, 1.0f, 1.0f);
		for (int l = 0; l < models.at(p).triangles.size(); l++) {
			glVertex3f(models.at(p).triangles.at(l).x1, models.at(p).triangles.at(l).y1, models.at(p).triangles.at(l).z1);
			glVertex3f(models.at(p).triangles.at(l).x2, models.at(p).triangles.at(l).y2, models.at(p).triangles.at(l).z2);
			glVertex3f(models.at(p).triangles.at(l).x3, models.at(p).triangles.at(l).y3, models.at(p).triangles.at(l).z3);
			}
		glEnd();
	}




};

void renderScene(void) {

	// clear buffers
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	frame++;
	time = glutGet(GLUT_ELAPSED_TIME);
	if (time - timebase > 1000) { 
		fps = frame * 1000.0 / (time - timebase) ; 
		timebase = time;
		frame = 0; 
	}
	string s="s";
	sprintf(&s.front(), "%d", fps);
	glutSetWindowTitle(s.data());

	// set the camera
	glLoadIdentity();
	gluLookAt(camX, camY, camZ,
		0.0, 0.0, 0.0,
		0.0f, 1.0f, 0.0f);

	drawModel();
	// End of frame
	glutSwapBuffers();
}

void processKeys(unsigned char c, int xx, int yy) {

	switch (c) {
	case 'w':// rotate up
		if (b <= 1.5) b += 0.1;
		break;
	case 's':// rotate down
		if (b >= -1.5) b -= 0.1;
		break;
	case 'a': // rotate left
		a -= 0.1;
		break;
	case 'd':  
		a += 0.1;
		break;
	case 'q':   //zoom in
		if (zoom>=2)zoom-=2;
		break;
	case 'e':  //zoom in
		zoom+=2;
		break;
	case 'x': //eixos on/off
		axis=!axis;
		break;
	case 'f': //fps test
		break;
	}
	camX = zoom * cos(b) * sin(a);
	camY = zoom * sin(b);
	camZ = zoom * cos(b) * cos(a);
	glutPostRedisplay();
}

void processSpecialKeys(int key, int xx, int yy) {

	// put code to process special keys in here

}

int main(int argc, char** argv) {
	printf("Camara de teste: WASD para mover e Q|E para zoom in ou out\n\n");
	const char* nome;
	XMLDocument doc;
	doc.LoadFile("config.xml");
	XMLElement* cena = doc.FirstChildElement();
	XMLElement* modelo = cena->FirstChildElement("model");
	printf("Nomes dos ficheiros lidos do config.xml |\n");
	while (modelo) {   // itera o ficheiro config.xml e guarda os nomes dos ficheiros num arreio
		nome = modelo->Attribute("file");
		modelos.push_back(nome);
		printf("                                        |%s\n", modelos.at(modelos.size()-1).data()); 
		modelo = modelo->NextSiblingElement(); 
		}
	printf("\nIrao ser carregados %d modelos\n\n", modelos.size());
	parseFiles();  //itera os ficheiros de dados e guarda numa estrutura de dados

// init GLUT and the window
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DEPTH|GLUT_DOUBLE|GLUT_RGBA);
	glutInitWindowPosition(100,100);
	glutInitWindowSize(800,800);
	glutCreateWindow("CG@DI-UM");
		
// Required callback registry 
	glutDisplayFunc(renderScene);
	glutReshapeFunc(changeSize);
	
// Callback registration for keyboard processing
	glutKeyboardFunc(processKeys);
	glutSpecialFunc(processSpecialKeys);
	//glutIdleFunc(renderScene);
//  OpenGL settings
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);
	glPolygonMode(GL_FRONT, GL_LINE);

// enter GLUT's main cycle
	glutMainLoop();
	
	return 1;
}
