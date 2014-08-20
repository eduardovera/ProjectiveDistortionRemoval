/**
	Projective Distortion Removal
	@author Eduardo Vera Sousa
*/

#include "stdafx.h"
#include <highgui.h>
#include <Eigen\Dense>
#include <opencv2\opencv.hpp>
#include <iostream>
#include <array>

using namespace std;
using namespace cv;
using namespace Eigen;

void buildMatrix(Mat1f &matrix, const array<Vec3i, 4> &pontosOriginais, const array<Vec3i, 4> &pontosReorganizados);

int main() {
	array<Vec3i, 4> pontosOriginais;
	array<Vec3i, 4> pontosReorganizados;

	pontosReorganizados[0] = Vec3i(645, 92, 1);
	pontosReorganizados[1] = Vec3i(650, 715, 1);
	pontosReorganizados[2] = Vec3i(1001, 263, 1);
	pontosReorganizados[3] = Vec3i(1020, 697, 1);

	pontosOriginais[0] = Vec3i(0, 0, 1);
	pontosOriginais[1] = Vec3i(0, 540, 1);
	pontosOriginais[2] = Vec3i(500, 0, 1);
	pontosOriginais[3] = Vec3i(500, 540, 1);

	Mat1f matrix(8, 9);
	buildMatrix(matrix, pontosReorganizados, pontosOriginais);

	Mat1f H(9, 1);
	SVD::solveZ(matrix, H);
	H = H.reshape(1, 3);

	Mat3f imageInput = imread("C:\\Users\\Eduardo\\Pictures\\sample_building.jpg", CV_64FC3);

	Mat3i imageOutput(540, 500);
	Mat1i output;
	Mat1f points(3, 1, 1.0);

	for (int j = 0; j < imageOutput.rows; j++) {
		for (int i = 0; i < imageOutput.cols; i++) {
			points(0, 0) = i;
			points(1, 0) = j;
			output = H.inv() * points;
			output(0, 0) = output(0, 0) / output(2, 0);
			output(1, 0) = output(1, 0) / output(2, 0);
			output(2, 0) = output(2, 0) / output(2, 0);
			imageOutput(j, i) = imageInput(floor(output(1, 0)), floor(output(0, 0)));
		}
	}
	
	imwrite("C:\\Users\\Eduardo\\Pictures\\sample_building_output.jpg", imageOutput);
	system("pause");
}

void buildMatrix(Mat1f &matrix, const array<Vec3i, 4> &pontosOriginais, const array<Vec3i, 4> &pontosReorganizados) {
	for (int i = 0; i < matrix.rows; i = i + 2) {
		matrix(i, 0) = -pontosOriginais[i/2][0];
		matrix(i, 1) = -pontosOriginais[i/2][1];
		matrix(i, 2) = -pontosOriginais[i/2][2];
		matrix(i, 3) = 0;
		matrix(i, 4) = 0;
		matrix(i, 5) = 0;
		matrix(i, 6) = pontosReorganizados[i / 2][0] * pontosOriginais[i / 2][0];
		matrix(i, 7) = pontosReorganizados[i / 2][0] * pontosOriginais[i / 2][1];
		matrix(i, 8) = pontosReorganizados[i / 2][0] * pontosOriginais[i / 2][2];

		matrix(i + 1, 0) = 0;
		matrix(i + 1, 1) = 0;
		matrix(i + 1, 2) = 0;
		matrix(i + 1, 3) = -pontosOriginais[i/2][0];
		matrix(i + 1, 4) = -pontosOriginais[i/2][1];
		matrix(i + 1, 5) = -pontosOriginais[i/2][2];
		matrix(i + 1, 6) = pontosReorganizados[i / 2][1] * pontosOriginais[i / 2][0];
		matrix(i + 1, 7) = pontosReorganizados[i / 2][1] * pontosOriginais[i / 2][1];
		matrix(i + 1, 8) = pontosReorganizados[i / 2][1] * pontosOriginais[i / 2][2];
	}
}
