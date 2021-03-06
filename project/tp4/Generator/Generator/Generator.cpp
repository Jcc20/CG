#define _USE_MATH_DEFINES
#define _CRT_SECURE_NO_WARNINGS

#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <fstream>
#include <cmath>
#include <math.h>
#include <vector>

using namespace std;


struct Point {
	float x;
	float y;
	float z;
};

struct PatchData {
	int nrPatches;
	vector<vector<int>> indices;
	int nrPontos;
	vector<Point> pontos;
};

PatchData patchDados;



void transpose(float A[][4], float B[][4]) {
	int i, j;
	for (i = 0; i < 4; i++)
		for (j = 0; j < 4; j++)
			B[i][j] = A[j][i];
	int asd = 3;
}
void normalize(double* a) {
	float l = sqrt(a[0] * a[0] + a[1] * a[1] + a[2] * a[2]);
	a[0] = a[0] / l;
	a[1] = a[1] / l;
	a[2] = a[2] / l;
}
void multMatrix(double a[1][4], double b[4][4], double res[1][4]) {

	res[0][0] = (double)(a[0][0] * b[0][0] + (a[0][1] * b[1][0]) + a[0][2] * b[2][0] + a[0][3] * b[3][0]);
	res[0][1] = (double)(a[0][0] * b[0][1] + (a[0][1] * b[1][1]) + a[0][2] * b[2][1] + a[0][3] * b[3][1]);
	res[0][2] = (double)(a[0][0] * b[0][2] + (a[0][1] * b[1][2]) + a[0][2] * b[2][2] + a[0][3] * b[3][2]);
	res[0][3] = (double)(a[0][0] * b[0][3] + (a[0][1] * b[1][3]) + a[0][2] * b[2][3] + a[0][3] * b[3][3]);
}
void multMatrix2(double t[1][4], double v[4][1], double* res)
{
	*res = t[0][0] * v[0][0] + t[0][1] * v[1][0] + t[0][2] * v[2][0] + t[0][3] * v[3][0];
}
void cross(double* a, double* b, double* res) {
	res[0] = a[1] * b[2] - a[2] * b[1];
	res[1] = a[2] * b[0] - a[0] * b[2];
	res[2] = a[0] * b[1] - a[1] * b[0];
}




//le o ficheiro .patch e guarda tudo no patchDados
void loadPatch(char* patch) { 
	char buffer[4096];   //talvez devessemos usar ifstream, mas nao sei muito bem, e ca estamos...
	char* temp;
	FILE* file = fopen(patch, "r");

	if (file == NULL) {
		printf("Fich n encontrado\n"); return;
	}

	fgets(buffer, 4096, file);//ler nr de patches
	temp = strtok(buffer, "\n");
	patchDados.nrPatches = atoi(temp);

	for (int a = 0; a < patchDados.nrPatches; a++) {
		patchDados.indices.push_back({});//needed para inicializar?!
		fgets(buffer, 4096, file);//ler uma linha dos indices

		temp = strtok(buffer, " ");
		while (temp[strlen(temp) - 1] != '\n') {
			patchDados.indices.at(a).push_back(atoi(temp));
			temp = strtok(NULL, " ");
		}
		temp[strlen(temp) - 1] == '\0';
		patchDados.indices.at(a).push_back(atoi(temp));
	}



	fgets(buffer, 4096, file);//ler nr de patches
	temp = strtok(buffer, "\n");
	patchDados.nrPontos = atoi(temp);

	for (int b = 0; b < patchDados.nrPontos; b++) {
		Point p;
		fgets(buffer, 4096, file);//ler uma linha dos pontos
		temp = strtok(buffer, " ");
		p.x = atof(temp);
		temp = strtok(NULL, " ");
		p.y = atof(temp);
		temp = strtok(NULL, "\n");
		p.z = atof(temp);
		patchDados.pontos.push_back(p);
	}

}
//calcula  um ponto bezier  https://i.imgur.com/vvaWBCG.png
void gerarPontoBezier(double u, double v, int a, double r[3])
{
	double up[1][4] = { {powf(u,3), powf(u,2), u, 1} };


	double m[4][4] = { {-1.0f,  3.0f, -3.0f,  1.0f},
					   { 3.0f, -6.0f,  3.0f,  0.0f},
					   {-3.0f,  3.0f,  0.0f,  0.0f},
					   { 1.0f,  0.0f,  0.0f,  0.0f} };

	double mT[4][4] = { {-1.0f,  3.0f, -3.0f, 1.0f},
						{ 3.0f, -6.0f,  3.0f, 0.0f},
						{-3.0f,  3.0f,  0.0f, 0.0f},
						{ 1.0f,  0.0f,  0.0f, 0.0f} };

	double vH[4][1] = { {powf(v,3)},
						{powf(v,2)},
						{powf(v,1)},
						{powf(v,0)} };

	double pX[4][4];
	double pY[4][4];
	double pZ[4][4];

	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < 4; j++) {
			pX[i][j] = patchDados.pontos.at(patchDados.indices.at(a).at(i * 4 + j)).x;
			pY[i][j] = patchDados.pontos.at(patchDados.indices.at(a).at(i * 4 + j)).y;
			pZ[i][j] = patchDados.pontos.at(patchDados.indices.at(a).at(i * 4 + j)).z;
		}
	}

	double res[1][4];

	double resX[1][4];
	double resY[1][4];
	double resZ[1][4];

	double tX[1][4];
	double tY[1][4];
	double tZ[1][4];

	double x;
	double y;
	double z;

	/*   res = u * m    */
	multMatrix(up, m, res);

	/* resx = res * px,   y e z  */
	multMatrix(res, pX, resX);
	multMatrix(res, pY, resY);
	multMatrix(res, pZ, resZ);

	/* tx = resx * mt, y e z */
	multMatrix(resX, mT, tX);
	multMatrix(resY, mT, tY);
	multMatrix(resZ, mT, tZ);

	/*  x = transX * V       */
	multMatrix2(tX, vH, &x);
	multMatrix2(tY, vH, &y);
	multMatrix2(tZ, vH, &z);

	/*  Colocar os resultados no vetor a dar "return"   */
	r[0] = x;
	r[1] = y;
	r[2] = z;
}


//calcula  vector u https://i.imgur.com/WZCN7By.png
void gerarVBezier(double u, double v, int a, double r[3])
{
	double up[1][4] = { {powf(u,3), powf(u,2), u, 1} };


	double m[4][4] = { {-1.0f,  3.0f, -3.0f,  1.0f},
					   { 3.0f, -6.0f,  3.0f,  0.0f},
					   {-3.0f,  3.0f,  0.0f,  0.0f},
					   { 1.0f,  0.0f,  0.0f,  0.0f} };

	double mT[4][4] = { {-1.0f,  3.0f, -3.0f, 1.0f},
						{ 3.0f, -6.0f,  3.0f, 0.0f},
						{-3.0f,  3.0f,  0.0f, 0.0f},
						{ 1.0f,  0.0f,  0.0f, 0.0f} };

	double vH[4][1] = { {powf(v,3)},
						{powf(v,2)},
						{powf(v,1)},
						{powf(v,0)} };



	double vHd[4][1] = { { 3* powf(v,2)},
						{2*v},
						{1},
						{0} };


	double pX[4][4];
	double pY[4][4];
	double pZ[4][4];

	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < 4; j++) {
			pX[i][j] = patchDados.pontos.at(patchDados.indices.at(a).at(i * 4 + j)).x;
			pY[i][j] = patchDados.pontos.at(patchDados.indices.at(a).at(i * 4 + j)).y;
			pZ[i][j] = patchDados.pontos.at(patchDados.indices.at(a).at(i * 4 + j)).z;
		}
	}

	double res[1][4];

	double resX[1][4];
	double resY[1][4];
	double resZ[1][4];

	double tX[1][4];
	double tY[1][4];
	double tZ[1][4];

	double x;
	double y;
	double z;

	/*   res = u * m    */
	multMatrix(up, m, res);

	/* resx = res * px,   y e z  */
	multMatrix(res, pX, resX);
	multMatrix(res, pY, resY);
	multMatrix(res, pZ, resZ);

	/* tx = resx * mt, y e z */
	multMatrix(resX, mT, tX);
	multMatrix(resY, mT, tY);
	multMatrix(resZ, mT, tZ);

	/*  x = transX * V       */
	multMatrix2(tX, vHd, &x);
	multMatrix2(tY, vHd, &y);
	multMatrix2(tZ, vHd, &z);

	/*  Colocar os resultados no vetor a dar "return"   */
	r[0] = x;
	r[1] = y;
	r[2] = z;
}
//calcula  vector u https://i.imgur.com/LBIOsuF.png
void gerarUBezier(double u, double v, int a, double r[3])
{
	double up[1][4] = { {powf(u,3), powf(u,2), u, 1} };

	double upd[1][4] = { {3 * powf(u,2), 2 * u, 1, 0} };

	double m[4][4] = { {-1.0f,  3.0f, -3.0f,  1.0f},
					   { 3.0f, -6.0f,  3.0f,  0.0f},
					   {-3.0f,  3.0f,  0.0f,  0.0f},
					   { 1.0f,  0.0f,  0.0f,  0.0f} };

	double mT[4][4] = { {-1.0f,  3.0f, -3.0f, 1.0f},
						{ 3.0f, -6.0f,  3.0f, 0.0f},
						{-3.0f,  3.0f,  0.0f, 0.0f},
						{ 1.0f,  0.0f,  0.0f, 0.0f} };

	double vH[4][1] = { {powf(v,3)},
						{powf(v,2)},
						{powf(v,1)},
						{powf(v,0)} };

	double vHT[1][4] = { {powf(v,3) , powf(v,2) , powf(v,1) , powf(v,0)}  };

	double pX[4][4];
	double pY[4][4];
	double pZ[4][4];

	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < 4; j++) {
			pX[i][j] = patchDados.pontos.at(patchDados.indices.at(a).at(i * 4 + j)).x;
			pY[i][j] = patchDados.pontos.at(patchDados.indices.at(a).at(i * 4 + j)).y;
			pZ[i][j] = patchDados.pontos.at(patchDados.indices.at(a).at(i * 4 + j)).z;
		}
	}

	double res[1][4];

	double resX[1][4];
	double resY[1][4];
	double resZ[1][4];

	double tX[1][4];
	double tY[1][4];
	double tZ[1][4];

	double x;
	double y;
	double z;

	/*   res = u * m    */
	multMatrix(upd, m, res);

	/* resx = res * px,   y e z  */
	multMatrix(res, pX, resX);
	multMatrix(res, pY, resY);
	multMatrix(res, pZ, resZ);

	/* tx = resx * mt, y e z */
	multMatrix(resX, mT, tX);
	multMatrix(resY, mT, tY);
	multMatrix(resZ, mT, tZ);

	/*  x = transX * V       */
	multMatrix2(tX, vH, &x);
	multMatrix2(tY, vH, &y);
	multMatrix2(tZ, vH, &z);

	/*  Colocar os resultados no vetor a dar "return"   */
	r[0] = x;
	r[1] = y;
	r[2] = z;
}
//calcula o cross v*u e normaliza
void gerarNormalBezier(double u, double v, int a, double r[3]) {
	double vu[3];
	gerarUBezier(u, v, a, vu);
	double vv[3];
	gerarVBezier(u, v, a, vv);

	cross(vv, vu, r);

	normalize(r);

}







//itrea os patches e vai gravandos os pontos no file, com normais tb
void buildBezier(char* patch, int tess, char* outFile) {
	double uu, vv, uuu, vvv;
	double p1[3], p2[3], p3[3], p4[3];
	double n1[3], n2[3], n3[3], n4[3];
	double t1[3], t2[3], t3[3], t4[3];
	t1[0] = 0; t1[1] = 0; t1[2] = 0; t2[0] = 0; t2[1] = 0; t2[2] = 0; t3[0] = 0; t3[1] = 0; t3[2] = 0; t4[0] = 0; t4[1] = 0; t4[2] = 0;
	std::ofstream ofs(outFile, std::ofstream::out);

	for (int a = 0; a < patchDados.nrPatches; a++) {

		for (int u = 0 ; u < tess ; u++) {
			for (int v = 0 ; v < tess ; v++) {
				uu = (double)      u / tess;
				vv = (double)      v / tess;
				uuu = (double) (u+1) / tess;
				vvv = (double) (v+1) / tess;
				gerarPontoBezier(uu, vv, a, p1);
				gerarNormalBezier(uu, vv, a, n1);

				gerarPontoBezier(uu, vvv, a, p2);
				gerarNormalBezier(uu, vvv, a, n2);

				gerarPontoBezier(uuu, vvv, a, p3);
				gerarNormalBezier(uuu, vvv, a, n3);

				gerarPontoBezier(uuu, vv, a, p4);
				gerarNormalBezier(uuu, vv, a, n4);

				normalize(n1);
				normalize(n2);
				normalize(n3);
				normalize(n4);

				//1  2
				//4  3
				ofs << p1[0] << " " << p1[1] << " " << p1[2] << " " << p2[0] << " " << p2[1] << " " << p2[2] << " " << p3[0] << " " << p3[1] << " " << p3[2] << "\n"; //triangulo 123
				ofs << n1[0] << " " << n1[1] << " " << n1[2] << " " << n2[0] << " " << n2[1] << " " << n2[2] << " " << n3[0] << " " << n3[1] << " " << n3[2] << "\n"; //normal 123
				ofs << t1[0] << " " << t1[1] << " " << t1[2] << " " << t2[0] << " " << t2[1] << " " << t2[2] <<  "\n"; //tormal 123


				ofs << p1[0] << " " << p1[1] << " " << p1[2] << " " << p3[0] << " " << p3[1] << " " << p3[2] << " " << p4[0] << " " << p4[1] << " " << p4[2] << "\n"; //triangulo 134
				ofs << n1[0] << " " << n1[1] << " " << n1[2] << " " << n3[0] << " " << n3[1] << " " << n3[2] << " " << n4[0] << " " << n4[1] << " " << n4[2] << "\n"; //normal 134
				ofs << t1[0] << " " << t1[1] << " " << t1[2] << " " << t2[0] << " " << t2[1] << " " << t2[2] <<  "\n"; //tormal 123
			}
		}
	}
	ofs.close();
}

void cone(float radius, float alturaTotal, int slices, int stacks, string fileName) { //cone com base no plano XoZ, com vertice no eixo Y

	float alfainc = (2 * M_PI) / slices;
	float alturainc = alturaTotal / stacks;
	float alfa = 0.0;
	float altura = 0.0;
	float coneAngle = atan(radius / alturaTotal);
	char _ = ' ';
	char n = '\n';

	std::ofstream ofs(fileName, std::ofstream::out);
	//base
	for (int j = 0; j < slices; j++) {
		ofs << 0.0 << _ << 0.0 << _ << 0.0 << _;
		ofs << radius * sin(alfa + alfainc) << _ << 0.0 << _ << radius * cos(alfa + alfainc) << _;
		ofs << radius * sin(alfa) << _ << 0.0 << _ << radius * cos(alfa) << n;

		ofs << 0 << _ << -1 << _ << 0 << _;
		ofs << 0 << _ << -1 << _ << 0 << _;
		ofs << 0 << _ << -1 << _ << 0 << n;


		ofs << 0 << _ << 0 << _;
		ofs << 0 << _ << 0 << _;
		ofs << 0 << _ << 0 << n;

		alfa += alfainc;
	}
	//lateral (laterais?) do cone
	for (int i = 0; i < stacks; i++) { //stacks 
		alfa = 0;
		float topradius = (alturaTotal - altura - alturainc) / (alturaTotal / radius); //cada stack tem 2 circulos que a limitam, superior e inferior, com raios diferentes
		float bottomradius = (alturaTotal - altura) / (alturaTotal / radius);
		for (int j = 0; j < slices; j++) { //slices da stack AKA pseudo-quadradro AKA 2 triangulos

			//triangulo 1
			ofs << bottomradius * sin(alfa) << _ << altura << _ << bottomradius * cos(alfa) << _;
			ofs << bottomradius * sin(alfa + alfainc) << _ << altura << _ << bottomradius * cos(alfa + alfainc) << _;
			ofs << topradius * sin(alfa + alfainc) << _ << altura + alturainc << _ << topradius * cos(alfa + alfainc) << n;

			ofs << bottomradius * sin(alfa) << _ << altura << _ << bottomradius * cos(alfa) << _;                        //todo calcular estas normais, isto est?? mal
			ofs << bottomradius * sin(alfa + alfainc) << _ << altura << _ << bottomradius * cos(alfa + alfainc) << _;
			ofs << topradius * sin(alfa + alfainc) << _ << altura + alturainc << _ << topradius * cos(alfa + alfainc) << n;

			ofs << 0 << _ << 0 << _;
			ofs << 0 << _ << 0 << _;
			ofs << 0 << _ << 0 << n;

			//triangulo 2
			ofs << bottomradius * sin(alfa) << _ << altura << _ << bottomradius * cos(alfa) << _;
			ofs << topradius * sin(alfa + alfainc) << _ << altura + alturainc << _ << topradius * cos(alfa + alfainc) << _;
			ofs << topradius * sin(alfa) << _ << altura + alturainc << _ << topradius * cos(alfa) << n;

			ofs << bottomradius * sin(alfa) << _ << altura << _ << bottomradius * cos(alfa) << _;
			ofs << topradius * sin(alfa + alfainc) << _ << altura + alturainc << _ << topradius * cos(alfa + alfainc) << _;
			ofs << topradius * sin(alfa) << _ << altura + alturainc << _ << topradius * cos(alfa) << n;

			ofs << 0 << _ << 0 << _;
			ofs << 0 << _ << 0 << _;
			ofs << 0 << _ << 0 << n;


			alfa += alfainc;

		}
		altura += alturainc;
	}
	ofs.close();
}

void sphere(float radius, int slices, int stacks, string fileName) {  //esfera centrada na origem

	float alfainc = (2 * M_PI) / slices;
	float betainc = (M_PI) / stacks;
	float upinc = 1.0 / stacks;
	float sideinc = 1.0 / slices;

	float alfa = 0;
	float beta = -M_PI_2;
	float up = 0;
	float side = 0;


	char _ = ' ';
	char n = '\n';


	std::ofstream ofs(fileName, std::ofstream::out);

	for (int i = 0; i < stacks; i++) { //stacks 
		alfa = 0;
		side = 0;
		for (int j = 0; j < slices; j++) {//slices da stack AKA pseudo-quadradro AKA 2 triangulos - De baixo para cima

			ofs << radius * sin(alfa) * cos(beta) << _ << radius * sin(beta) << _ << radius * cos(alfa) * cos(beta) << _;                          //x y z
			ofs << radius * sin(alfa + alfainc) * cos(beta) << _ << radius * sin(beta) << _ << radius * cos(alfa + alfainc) * cos(beta) << _;
			ofs << radius * sin(alfa + alfainc) * cos(beta + betainc) << _ << radius * sin(beta + betainc) << _ << radius * cos(alfa + alfainc) * cos(beta + betainc) << n;

			ofs << sin(alfa) * cos(beta) << _ << sin(beta) << _ << cos(alfa) * cos(beta) << _;                                             //normais
			ofs << sin(alfa + alfainc) * cos(beta) << _ << sin(beta) << _ << cos(alfa + alfainc) * cos(beta) << _;
			ofs << sin(alfa + alfainc) * cos(beta + betainc) << _ << sin(beta + betainc) << _ << cos(alfa + alfainc) * cos(beta + betainc) << n;

			ofs << side << _ << up << _;
			ofs << side + sideinc << _ << up << _;
			ofs << side + sideinc << _ << up + upinc << n;





			ofs << radius * sin(alfa) * cos(beta) << _ << radius * sin(beta) << _ << radius * cos(alfa) * cos(beta) << _;                         //x y z
			ofs << radius * sin(alfa + alfainc) * cos(beta + betainc) << _ << radius * sin(beta + betainc) << _ << radius * cos(alfa + alfainc) * cos(beta + betainc) << _;
			ofs << radius * sin(alfa) * cos(beta + betainc) << _ << radius * sin(beta + betainc) << _ << radius * cos(alfa) * cos(beta + betainc) << n;

			ofs << sin(alfa) * cos(beta) << _ << sin(beta) << _ << cos(alfa) * cos(beta) << _;                                                      //normais
			ofs << sin(alfa + alfainc) * cos(beta + betainc) << _ << sin(beta + betainc) << _ << cos(alfa + alfainc) * cos(beta + betainc) << _;
			ofs << sin(alfa) * cos(beta + betainc) << _ << sin(beta + betainc) << _ << cos(alfa) * cos(beta + betainc) << n;

			ofs << side << _ << up << _;
			ofs << side + sideinc << _ << up + upinc << _;
			ofs << side << _ << up + upinc << n;


			alfa += alfainc;
			side += sideinc;
		}
		beta += betainc;
		up += upinc;
	}


	ofs.close();
}

void plane(float comp, float largura, string fileName) {  //plano no plano XoZ, centrado na origem

	float x = largura / 2; //dividir para centrar, depois usar x e -x
	float y = 0.0;
	float z = comp / 2;
	char _ = ' ';
	char n = '\n';


	std::ofstream ofs(fileName, std::ofstream::out);

	//se for pretendido desenhar o plano dos dois lados  descomentar o seguinte
	//triangulo 3  
	//ofs << x << _ << y << _ << -z << _;ofs << x << _ << y << _ << z << _ ;ofs << -x << _ << y << _ << z << n;
	//triangulo 4 
	//ofs << -x << _ << y << _ << z << _;ofs << -x << _ << y << _ << -z << _;ofs << x << _ << y << _ << -z << n;



	//triangulo 1  
	ofs << -x << _ << y << _ << z << _;
	ofs << x << _ << y << _ << z << _;
	ofs << x << _ << y << _ << -z << n;

	ofs << 0 << _ << 1 << _ << 0 << _;
	ofs << 0 << _ << 1 << _ << 0 << _;
	ofs << 0 << _ << 1 << _ << 0 << n;

	ofs << 0 << _ << 0 << _;
	ofs << 0 << _ << 0 << _;
	ofs << 0 << _ << 0 << n;

	//triangulo 2  
	ofs << x << _ << y << _ << -z << _;
	ofs << -x << _ << y << _ << -z << _;
	ofs << -x << _ << y << _ << z << n;

	ofs << 0 << _ << 1 << _ << 0 << _;
	ofs << 0 << _ << 1 << _ << 0 << _;
	ofs << 0 << _ << 1 << _ << 0 << n;

	ofs << 0 << _ << 0 << _;
	ofs << 0 << _ << 0 << _;
	ofs << 0 << _ << 0 << n;






	ofs.close();
}

void box(float compr, float largura, float altura, int divs, string fileName) { //caixa centrada na origem
	//  4 - - - - -3                1 ofs << -x << _ << y << _ << z << _;
	//  | \       | \               2 ofs << x << _ << y << _ << z << _;
	//  |   1- - - - -2				3 ofs << x << _ << y << _ << -z << _;
	//  |   |     |   |				4 ofs << -x << _ << y << _ << -z << _;
	//  8 - | - - 7   |				5 ofs << -x << _ << -y << _ << z << _;	
	//    \ |       \ |				6 ofs << x << _ << -y << _ << z << _;
	//  	5- - - - -6				7 ofs << x << _ << -y << _ << -z << _;
	//								8 ofs << -x << _ << -y << _ << -z << _;
	float x = largura / divs; //dividir para centrar, depois usar x e -x      
	float y = altura / divs;
	float z = compr / divs;
	float xi = largura / 2;
	float yi = altura / 2;
	float zi = compr / 2;
	char _ = ' ';
	char n = '\n';

	std::ofstream ofs(fileName, std::ofstream::out);

	for (int i = 0; i < divs; i++) {
		for (int j = 0; j < divs; j++) {
			//face de cima
			ofs << -xi + (j * x) << _ << yi << _ << zi - (i * z) << _;
			ofs << -xi + x + (j * x) << _ << yi << _ << zi - (i * z) << _;
			ofs << -xi + x + (j * x) << _ << yi << _ << zi - z - (i * z) << n;

			ofs << 0 << _ << 1 << _ << 0 << _;
			ofs << 0 << _ << 1 << _ << 0 << _;
			ofs << 0 << _ << 1 << _ << 0 << n;

			ofs << 0 << _ << 0 << _;
			ofs << 0 << _ << 0 << _;
			ofs << 0 << _ << 0 << n;

			ofs << -xi + (j * x) << _ << yi << _ << zi - (i * z) << _;
			ofs << -xi + x + (j * x) << _ << yi << _ << zi - z - (i * z) << _;
			ofs << -xi + (j * x) << _ << yi << _ << zi - z - (i * z) << n;

			ofs << 0 << _ << 1 << _ << 0 << _;
			ofs << 0 << _ << 1 << _ << 0 << _;
			ofs << 0 << _ << 1 << _ << 0 << n;

			ofs << 0 << _ << 0 << _;
			ofs << 0 << _ << 0 << _;
			ofs << 0 << _ << 0 << n;

			//face de baixo

			ofs << -xi + x + (j * x) << _ << -yi << _ << zi - z - (i * z) << _;
			ofs << -xi + x + (j * x) << _ << -yi << _ << zi - (i * z) << _;
			ofs << -xi + (j * x) << _ << -yi << _ << zi - (i * z) << n;

			ofs << 0 << _ << -1 << _ << 0 << _;
			ofs << 0 << _ << -1 << _ << 0 << _;
			ofs << 0 << _ << -1 << _ << 0 << n;

			ofs << 0 << _ << 0 << _;
			ofs << 0 << _ << 0 << _;
			ofs << 0 << _ << 0 << n;

			ofs << -xi + (j * x) << _ << -yi << _ << zi - z - (i * z) << _;
			ofs << -xi + x + (j * x) << _ << -yi << _ << zi - z - (i * z) << _;
			ofs << -xi + (j * x) << _ << -yi << _ << zi - (i * z) << n;

			ofs << 0 << _ << -1 << _ << 0 << _;
			ofs << 0 << _ << -1 << _ << 0 << _;
			ofs << 0 << _ << -1 << _ << 0 << n;

			ofs << 0 << _ << 0 << _;
			ofs << 0 << _ << 0 << _;
			ofs << 0 << _ << 0 << n;


			//face da frente
			ofs << -xi + (j * x) << _ << -yi + (i * y) << _ << zi << _;
			ofs << -xi + x + (j * x) << _ << -yi + (i * y) << _ << zi << _;
			ofs << -xi + x + (j * x) << _ << -yi + y + (i * y) << _ << zi << n;

			ofs << 0 << _ << 0 << _ << 1 << _;
			ofs << 0 << _ << 0 << _ << 1 << _;
			ofs << 0 << _ << 0 << _ << 1 << n;

			ofs << 0 << _ << 0 << _;
			ofs << 0 << _ << 0 << _;
			ofs << 0 << _ << 0 << n;

			ofs << -xi + (j * x) << _ << -yi + (i * y) << _ << zi << _;
			ofs << -xi + x + (j * x) << _ << -yi + y + (i * y) << _ << zi << _;
			ofs << -xi + (j * x) << _ << -yi + y + (i * y) << _ << zi << n;

			ofs << 0 << _ << 0 << _ << 1 << _;
			ofs << 0 << _ << 0 << _ << 1 << _;
			ofs << 0 << _ << 0 << _ << 1 << n;

			ofs << 0 << _ << 0 << _;
			ofs << 0 << _ << 0 << _;
			ofs << 0 << _ << 0 << n;


			//face de tras
			ofs << -xi + x + (j * x) << _ << -yi + y + (i * y) << _ << -zi << _;
			ofs << -xi + x + (j * x) << _ << -yi + (i * y) << _ << -zi << _;
			ofs << -xi + (j * x) << _ << -yi + (i * y) << _ << -zi << n;

			ofs << 0 << _ << 0 << _ << -1 << _;
			ofs << 0 << _ << 0 << _ << -1 << _;
			ofs << 0 << _ << 0 << _ << -1 << n;

			ofs << 0 << _ << 0 << _;
			ofs << 0 << _ << 0 << _;
			ofs << 0 << _ << 0 << n;

			ofs << -xi + (j * x) << _ << -yi + y + (i * y) << _ << -zi << _;
			ofs << -xi + x + (j * x) << _ << -yi + y + (i * y) << _ << -zi << _;
			ofs << -xi + (j * x) << _ << -yi + (i * y) << _ << -zi << n;

			ofs << 0 << _ << 0 << _ << -1 << _;
			ofs << 0 << _ << 0 << _ << -1 << _;
			ofs << 0 << _ << 0 << _ << -1 << n;

			ofs << 0 << _ << 0 << _;
			ofs << 0 << _ << 0 << _;
			ofs << 0 << _ << 0 << n;

			//face direita
			ofs << xi << _ << -yi + (i * y) << _ << zi - (z * j) << _;
			ofs << xi << _ << -yi + (i * y) << _ << zi - z - (z * j) << _;
			ofs << xi << _ << -yi + y + (i * y) << _ << zi - z - (z * j) << n;

			ofs << 1 << _ << 0 << _ << 0 << _;
			ofs << 1 << _ << 0 << _ << 0 << _;
			ofs << 1 << _ << 0 << _ << 0 << n;

			ofs << 0 << _ << 0 << _;
			ofs << 0 << _ << 0 << _;
			ofs << 0 << _ << 0 << n;

			ofs << xi << _ << -yi + (i * y) << _ << zi - (z * j) << _;
			ofs << xi << _ << -yi + y + (i * y) << _ << zi - z - (z * j) << _;
			ofs << xi << _ << -yi + y + (i * y) << _ << zi - (z * j) << n;

			ofs << 1 << _ << 0 << _ << 0 << _;
			ofs << 1 << _ << 0 << _ << 0 << _;
			ofs << 1 << _ << 0 << _ << 0 << n;

			ofs << 0 << _ << 0 << _;
			ofs << 0 << _ << 0 << _;
			ofs << 0 << _ << 0 << n;

			//face esquerda
			ofs << -xi << _ << -yi + y + (i * y) << _ << zi - z - (z * j) << _;
			ofs << -xi << _ << -yi + (i * y) << _ << zi - z - (z * j) << _;
			ofs << -xi << _ << -yi + (i * y) << _ << zi - (z * j) << n;


			ofs << -1 << _ << 0 << _ << 0 << _;
			ofs << -1 << _ << 0 << _ << 0 << _;
			ofs << -1 << _ << 0 << _ << 0 << n;

			ofs << 0 << _ << 0 << _;
			ofs << 0 << _ << 0 << _;
			ofs << 0 << _ << 0 << n;

			ofs << -xi << _ << -yi + y + (i * y) << _ << zi - (z * j) << _;
			ofs << -xi << _ << -yi + y + (i * y) << _ << zi - z - (z * j) << _;
			ofs << -xi << _ << -yi + (i * y) << _ << zi - (z * j) << n;


			ofs << -1 << _ << 0 << _ << 0 << _;
			ofs << -1 << _ << 0 << _ << 0 << _;
			ofs << -1 << _ << 0 << _ << 0 << n;

			ofs << 0 << _ << 0 << _;
			ofs << 0 << _ << 0 << _;
			ofs << 0 << _ << 0 << n;

		}
	}

	ofs.close();
}







int main(int argc, char** argv) {

	if (argc < 1) return -1;

	if (strcmp(argv[1],"plane")==0) {
		plane(atof(argv[2]), atof(argv[3]), argv[4]);
	}

	else if (!strcmp(argv[1], "box")) {
		if (argc == 6) { 
			box(atof(argv[2]), atof(argv[3]), atof(argv[4]), 1, argv[5]);
		}
		else{ 
			box(atof(argv[2]), atof(argv[3]), atof(argv[4]), atoi(argv[5]), argv[6]); 
		}
	}

	else if (!strcmp(argv[1], "sphere")) {
		sphere(atof(argv[2]), atof(argv[3]), atof(argv[4]), argv[5]);
	}

	else if (!strcmp(argv[1], "cone")) { 
		cone(atof(argv[2]), atof(argv[3]), atof(argv[4]), atof(argv[5]), argv[6]);
	}

	else if (!strcmp(argv[1], "cone")) {
		cone(atof(argv[2]), atof(argv[3]), atof(argv[4]), atof(argv[5]), argv[6]);
	}

	else if (strcmp(argv[1], "bezier") == 0) {  //   ./Generator bezier teapot.patch 20 bule.3d
		loadPatch(argv[2]);
		buildBezier(argv[2], atoi(argv[3]), argv[4]);
	}

	else printf("\nWrong comand!!\n\n");

	return 2147483647;
}
