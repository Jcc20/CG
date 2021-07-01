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
	float tx = 0, ty = 0, tz =0;
	float ra=0, rx=0, ry=0, rz=0;
	float sx = 1, sy = 1, sz = 1;
	string nome;
	vector<Triangle> triangles;
	vector<Model> submodules;
};

float camX , camY , camZ, a = 0, b = 1.5, zoom = 150 ;
int frame = 0,  time = 0,   timebase = 0, fps = 0;
bool axis = false;
double tempo;
vector<Model> planetas;


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

void parseFiles(vector<Model> & planetas) {//itera os ficheiros de dados e guarda numa estrutura de dados
	for (int r=0; r< planetas.size(); r++) {
		FILE* file = fopen(planetas.at(r).nome.data(), "r");
		char* buffer = (char*) malloc(sizeof(char)*1024);
		while (fgets(buffer, 1024, file)) {
			Triangle tri;
			int shutup = sscanf(buffer, "%f%f%f%f%f%f%f%f%f", &tri.x1, &tri.y1, &tri.z1, &tri.x2, &tri.y2, &tri.z2, &tri.x3, &tri.y3, &tri.z3);
			planetas.at(r).triangles.push_back(tri);
		}
		fclose(file);
		free(buffer);
		if (planetas.at(r).submodules.size() > 0) { parseFiles(planetas.at(r).submodules);}
	}

};


void drawAxis() {

	if (axis) { //eixos
		glBegin(GL_LINES);
		// X axis in red
		glColor3f(1.0f, 0.0f, 0.0f);
		glVertex3f(-1000.0f, 0.0f, 0.0f);
		glVertex3f(1000.0f, 0.0f, 0.0f);
		// Y Axis in Green
		glColor3f(0.0f, 1.0f, 0.0f);
		glVertex3f(0.0f, -1000.0f, 0.0f);
		glVertex3f(0.0f, 1000.0f, 0.0f);
		// Z Axis in Blue
		glColor3f(0.0f, 0.0f, 1.0f);
		glVertex3f(0.0f, 0.0f, -1000.0f);
		glVertex3f(0.0f, 0.0f, 1000.0f);
		glEnd();
	}
}


void drawModel(vector<Model> & planetas) {
	


	for (int p = 0; p < planetas.size(); p++) {
		glPushMatrix();
		srand(p);
		float ran = (rand() % 10000 ) / 20.0f;
		glRotatef(tempo * ran, 0, 1, 0);
		glRotatef(planetas.at(p).ra, planetas.at(p).rx, planetas.at(p).ry, planetas.at(p).rz);
		glTranslatef(planetas.at(p).tx, planetas.at(p).ty, planetas.at(p).tz);
		glScalef(planetas.at(p).sx, planetas.at(p).sy, planetas.at(p).sz);

		glBegin(GL_TRIANGLES);
		glColor3f(1.0f, 1.0f, 1.0f);
		for (int l = 0; l < planetas.at(p).triangles.size(); l++) {
			glVertex3f(planetas.at(p).triangles.at(l).x1, planetas.at(p).triangles.at(l).y1, planetas.at(p).triangles.at(l).z1);
			glVertex3f(planetas.at(p).triangles.at(l).x2, planetas.at(p).triangles.at(l).y2, planetas.at(p).triangles.at(l).z2);
			glVertex3f(planetas.at(p).triangles.at(l).x3, planetas.at(p).triangles.at(l).y3, planetas.at(p).triangles.at(l).z3);
			}
		glEnd();

		if (planetas.at(p).submodules.size() > 0) { drawModel(planetas.at(p).submodules); }

		glPopMatrix();
	}




};

void renderScene(void) {

	// clear buffers
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	frame ++;
	time = glutGet(GLUT_ELAPSED_TIME);
	if (time - timebase > 1000) { 
		fps = frame * 1000.0 / (time - timebase) ; 
		timebase = time;
		frame = 0; 
	}
	string s = "d";
	sprintf(&s.front(), "%d", fps);
	glutSetWindowTitle(s.data());

	// set the camera
	glLoadIdentity();
	gluLookAt(camX, camY, camZ,
		0.0, 0.0, 0.0,
		0.0f, 1.0f, 0.0f);
	drawAxis();
	drawModel(planetas);
	// End of frame
	glutSwapBuffers();

}

void processKeys(unsigned char c, int xx, int yy) {

	switch (c) {
	case 'w':// rotate up
		if (b < 1.5) b += 0.1;
		break;
	case 's':// rotate down
		if (b > -1.5) b -= 0.1;
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
		axis= !axis;
		break;
	case 'f': //fps test
		tempo++;
		break;
	}
	camX =  zoom * cos(b) * sin(a);
	camY =  zoom * sin(b);
	camZ =  zoom * cos(b) * cos(a);
	glutPostRedisplay();
}

void processSpecialKeys(int key, int xx, int yy) {

	// put code to process special keys in here

}


void parseConfig(XMLElement* grupo, vector<Model> & planetass) {
	const char* nome, * tx, * ty, * tz, * ra, * rx, * ry, * rz, * sx, * sy, * sz;
	while (grupo) {   // itera o ficheiro config.xml e guarda os nomes dos ficheiros num arreio
		Model planet;


		XMLElement* trans = grupo->FirstChildElement("translate");
		if (trans) {
			tx = trans->Attribute("X"); ty = trans->Attribute("Y"); tz = trans->Attribute("Z");
			if (tx) { planet.tx = atof(tx); } if (ty) { planet.ty = atof(ty); } if (tz) { planet.tz = atof(tz); }
		}


		XMLElement* scale = grupo->FirstChildElement("scale");
		if (scale) {
			sx = scale->Attribute("X"); sy = scale->Attribute("Y"); sz = scale->Attribute("Z");
			if (sx) { planet.sx = atof(sx); }if (sy) { planet.sy = atof(sy); } if (sz) { planet.sz = atof(sz); }
		}


		XMLElement* rotate = grupo->FirstChildElement("rotate");
		if (rotate) {
			rx = rotate->Attribute("axisX"); ry = rotate->Attribute("axisY"); rz = rotate->Attribute("axisZ"); ra = rotate->Attribute("angle");
			if (rx) { planet.rx = atof(rx); }if (ry) { planet.ry = atof(ry); } if (rz) { planet.rz = atof(rz); } if (ra) { planet.ra = atof(ra); }
		}


		XMLElement* models = grupo->FirstChildElement("models");
		if (models) {
			XMLElement* model = models->FirstChildElement("model");
			nome = model->Attribute("file");
		}

		XMLElement* submodels = grupo->FirstChildElement("group");
		if (submodels) {
			parseConfig(submodels, planet.submodules);//recursividade, assim le os submodels como se fosse um model, vai dar ao mesmo
		}
		planet.nome = nome;
		planetass.push_back(planet);
		grupo = grupo->NextSiblingElement();
	}



}
void initcam() {
	camX = zoom * cos(b) * sin(a);
	camY = zoom * sin(b);
	camZ = zoom * cos(b) * cos(a);
}

int main(int argc, char** argv) {
	printf("Camara de teste: WASD para mover e Q|E para zoom in ou out\n\n");
	printf("F para os planetas se moverem\n");
	XMLDocument doc;
	doc.LoadFile("config.xml");
	XMLElement* cena = doc.FirstChildElement();
	XMLElement* grupo = cena->FirstChildElement("group");
	parseConfig(grupo, planetas);
	parseFiles(planetas);  //itera os ficheiros de dados e guarda numa estrutura de dados
	initcam();

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
