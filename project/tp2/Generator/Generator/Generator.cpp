#define _USE_MATH_DEFINES

#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <fstream>
#include <cmath>
#include <math.h>

using namespace std;

void cone(float radius, float alturaTotal, int slices, int stacks, string fileName) { //cone com base no plano XoZ, com vertice no eixo Y
	
	float alfainc = (2 * M_PI) / slices;
	float alturainc = alturaTotal / stacks;
	float alfa = 0.0;
	float altura = 0.0;

	char _ = ' ';
	char n = '\n';

	std::ofstream ofs(fileName, std::ofstream::out);
	//base
	for (int j = 0; j < slices; j++) {
		ofs <<  0.0 << _ << 0.0 << _ << 0.0 << _ ;
		ofs << radius * sin(alfa + alfainc)<< _ << 0.0 << _ << radius * cos(alfa + alfainc) << _;
		ofs << radius * sin(alfa) << _ << 0.0 << _ << radius * cos(alfa) << n;
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
			ofs << bottomradius * sin(alfa + alfainc)  << _ << altura << _ << bottomradius * cos(alfa + alfainc) << _;
			ofs << topradius * sin(alfa + alfainc)<< _ << altura + alturainc << _ << topradius * cos(alfa + alfainc) << n;

			//triangulo 2
			ofs << bottomradius * sin(alfa)<< _ << altura << _ << bottomradius * cos(alfa)  << _;
			ofs << topradius * sin(alfa + alfainc)  << _ << altura + alturainc << _ << topradius * cos(alfa + alfainc) << _;
			ofs << topradius * sin(alfa)<< _ << altura + alturainc << _ << topradius * cos(alfa)<< n;

			alfa += alfainc;

		}
		altura += alturainc;
	}
	ofs.close();
}

void sphere(float radius, int slices, int stacks, string fileName) {  //esfera centrada na origem

	float alfainc = (2 * M_PI) / slices;
	float betainc = (M_PI) / stacks;

	float alfa = 0;
	float beta = -M_PI_2;

	char _ = ' ';
	char n = '\n';


	std::ofstream ofs(fileName, std::ofstream::out);

	for (int i = 0; i < stacks; i++) { //stacks
		alfa = 0;
		for (int j = 0; j < slices; j++) {//slices da stack AKA pseudo-quadradro AKA 2 triangulos - De baixo para cima
			ofs << radius * sin(alfa)*cos(beta)<< _ << radius * sin(beta) << _ << radius * cos(alfa)*cos(beta) << _ ;
			ofs << radius * sin(alfa+alfainc)*cos(beta)<< _ << radius * sin(beta) << _ << radius * cos(alfa+alfainc) * cos(beta) << _;
			ofs << radius * sin(alfa + alfainc) * cos(beta+betainc) << _ << radius * sin(beta+betainc) << _ << radius * cos(alfa+alfainc) * cos(beta+betainc) << n;

			ofs << radius * sin(alfa) * cos(beta) << _ << radius * sin(beta) << _ << radius * cos(alfa) * cos(beta) << _;
			ofs << radius * sin(alfa + alfainc) * cos(beta + betainc) << _ << radius * sin(beta + betainc) << _ << radius * cos(alfa+alfainc) * cos(beta + betainc) <<_;
			ofs << radius * sin(alfa) * cos(beta+betainc) << _ << radius * sin(beta + betainc) << _ << radius * cos(alfa) * cos(beta + betainc) << n;

			alfa += alfainc;

		}
		beta += betainc;
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
	ofs << -x << _ << y << _ << z << _ ;
	ofs << x << _ << y << _ << z << _ ;
	ofs << x << _ << y << _ << -z << n;

	//triangulo 2  
	ofs << x << _ << y << _ << -z << _;
	ofs << -x << _ << y << _ << -z << _;
	ofs << -x << _ << y << _ << z;


	





	ofs.close();
}

void boxOldHardCoded(float compr, float largura, float altura, string fileName) { //caixa centrada na origem

	//  4 - - - - -3                1 ofs << -x << _ << y << _ << z << _;
	//  | \       | \               2 ofs << x << _ << y << _ << z << _;
	//  |   1- - - - -2				3 ofs << x << _ << y << _ << -z << _;
	//  |   |     |   |				4 ofs << -x << _ << y << _ << -z << _;
	//  8 - | - - 7   |				5 ofs << -x << _ << -y << _ << z << _;	
	//    \ |       \ |				6 ofs << x << _ << -y << _ << z << _;
	//  	5- - - - -6				7 ofs << x << _ << -y << _ << -z << _;
	//								8 ofs << -x << _ << -y << _ << -z << _;
	float x = largura / 2; //dividir para centrar, depois usar x e -x      
	float y = altura / 2;
	float z = compr / 2;
	char _ = ' ';
	char n = '\n';

	std::ofstream ofs(fileName, std::ofstream::out);

	//face de cima
	//123
	ofs << -x << _ << y << _ << z << _;
	ofs << x << _ << y << _ << z << _;
	ofs << x << _ << y << _ << -z << n;
	//341
	ofs << x << _ << y << _ << -z << _;
	ofs << -x << _ << y << _ << -z << _;
	ofs << -x << _ << y << _ << z << n;

	//face de baixo
	//576
	ofs << -x << _ << -y << _ << z << _;
	ofs << x << _ << -y << _ << -z << _;
	ofs << x << _ << -y << _ << z << n;
	//587
	ofs << -x << _ << -y << _ << z << _;
	ofs << -x << _ << -y << _ << -z << _;
	ofs << x << _ << -y << _ << -z << n;

	//face da frente
	//562
	ofs << -x << _ << -y << _ << z << _;
	ofs << x << _ << -y << _ << z << _;
	ofs << x << _ << y << _ << z << n;
	//521
	ofs << -x << _ << -y << _ << z << _;
	ofs << x << _ << y << _ << z << _;
	ofs << -x << _ << y << _ << z << n;


	//faca de tras
	//784
	ofs << x << _ << -y << _ << -z << _;
	ofs << -x << _ << -y << _ << -z << _;
	ofs << -x << _ << y << _ << -z << n;
	//743
	ofs << x << _ << -y << _ << -z << _;
	ofs << -x << _ << y << _ << -z << _;
	ofs << x << _ << y << _ << -z << n;

	//face esquerda
	//548
	ofs << -x << _ << -y << _ << z << _;
	ofs << -x << _ << y << _ << -z << _;
	ofs << -x << _ << -y << _ << -z << n;
	
	//541
	ofs << -x << _ << -y << _ << z << _;
	ofs << -x << _ << y << _ << z << _;
	ofs << -x << _ << y << _ << -z << n;
	

	//face direita
	//672
	ofs << x << _ << -y << _ << z << _;
	ofs << x << _ << -y << _ << -z << _;
	ofs << x << _ << y << _ << z << n;
	//732
	ofs << x << _ << -y << _ << -z << _;
	ofs << x << _ << y << _ << -z << _;
	ofs << x << _ << y << _ << z;
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
		for (int j = 0; j < divs; j++){
			//face de cima
			ofs << -xi + (j*x) << _ << yi << _ << zi -(i*z) << _;
			ofs << -xi + x + (j * x) << _ << yi << _ << zi - (i * z) << _;
			ofs << -xi + x + (j * x) << _ << yi << _ << zi - z - (i * z) << n;

			ofs << -xi + (j * x) << _ << yi << _ << zi - (i * z) << _;
			ofs << -xi + x + (j * x) << _ << yi << _ << zi - z - (i * z) << _;
			ofs << -xi + (j * x) << _ << yi << _ << zi - z - (i * z) << n;

			//face de baixo

			ofs << -xi + x + (j * x) << _ << -yi << _ << zi - z - (i * z) << _;
			ofs << -xi + x + (j * x) << _ << -yi << _ << zi - (i * z) << _;
			ofs << -xi + (j * x) << _ << -yi << _ << zi - (i * z) << n;

			ofs << -xi + (j * x) << _ << -yi << _ << zi - z - (i * z) << _;
			ofs << -xi + x + (j * x) << _ << -yi << _ << zi - z - (i * z) << _;
			ofs << -xi + (j * x) << _ << -yi << _ << zi - (i * z) << n;
			
			

			//face da frente
			ofs << -xi + (j * x) << _ << -yi + (i*y) << _ << zi  << _;
			ofs << -xi + x + (j * x) << _ << -yi + (i * y) << _ << zi << _;
			ofs << -xi + x + (j * x) << _ << -yi + y + (i * y) << _ << zi << n;

			ofs << -xi + (j * x) << _ << -yi + (i * y) << _ << zi << _;
			ofs << -xi + x + (j * x) << _ << -yi + y + (i * y) << _ << zi << _;
			ofs << -xi + (j * x) << _ << -yi +y+ (i * y) << _ << zi << n;

			//face de tras
			ofs << -xi + x + (j * x) << _ << -yi + y + (i * y) << _ << -zi << _;
			ofs << -xi + x + (j * x) << _ << -yi + (i * y) << _ << -zi << _;
			ofs << -xi + (j * x) << _ << -yi + (i * y) << _ << -zi << n;

			ofs << -xi + (j * x) << _ << -yi + y + (i * y) << _ << -zi << _;
			ofs << -xi + x + (j * x) << _ << -yi + y + (i * y) << _ << -zi << _;
			ofs << -xi + (j * x) << _ << -yi + (i * y) << _ << -zi << n;
			
			//face direita
			ofs << xi << _ << -yi + (i * y) << _ << zi - (z * j) << _;
			ofs << xi << _ << -yi + (i * y) << _ << zi -z- (z * j) << _;
			ofs << xi << _ << -yi + y + (i * y) << _ << zi - z -(z * j) << n;
			
			ofs << xi << _ << -yi + (i * y) << _ << zi - (z * j) << _;
			ofs << xi << _ << -yi + y + (i * y) << _ << zi - z - (z * j) << _;
			ofs << xi << _ << -yi + y + (i * y) << _ << zi - (z * j) << n;

			//face esquerda
			ofs << -xi << _ << -yi + y + (i * y) << _ << zi - z - (z * j) << _;
			ofs << -xi << _ << -yi + (i * y) << _ << zi - z - (z * j) << _;
			ofs << -xi << _ << -yi + (i * y) << _ << zi - (z * j) << n;

			

			ofs << -xi << _ << -yi + y + (i * y) << _ << zi - (z * j) << _;
			ofs << -xi << _ << -yi + y + (i * y) << _ << zi - z - (z * j) << _;
			ofs << -xi << _ << -yi + (i * y) << _ << zi - (z * j) << n;
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

	else printf("\nShape not recognized!\n\n");

	return 2147483647;
}