#pragma once
#include "Params.h"
#include <math.h>
#include <stdio.h>
//list4.1
int cx_difference[9] = { 0,0,0,0,1,-1,0,0,0 };
int cy_difference[9] = { 0,0,0,0,1,0,0,-1,0 };
int cx_roberts[9] = { 0,0,0,0,1,0,0,0,-1 };
int cy_roberts[9] = { 0,0,0,0,0,1,0,-1,0 };
int cx_sobel[9] = { -1,0,1,-2,0,2,-1,0,1 };
int cy_sobel[9] = { -1,-2,-1,0,0,0,1,2,1 };
void gradient(unsigned char image_in[Y_SIZE][X_SIZE], unsigned char image_out[Y_SIZE][X_SIZE], double amp, int cx[9], int cy[9]);
/*通常的差分*/
void gradient_difference(unsigned char image_in[Y_SIZE][X_SIZE], unsigned char image_out[Y_SIZE][X_SIZE], double amp) {
	gradient(image_in, image_out, amp, cx_difference, cy_difference);
}
/*Roberts運算子*/
void gradient_roberts(unsigned char image_in[Y_SIZE][X_SIZE], unsigned char image_out[Y_SIZE][X_SIZE], double amp) {
	gradient(image_in, image_out, amp, cx_roberts, cy_roberts);
}
/*Sobel運算子*/
void gradient_sobel(unsigned char image_in[Y_SIZE][X_SIZE], unsigned char image_out[Y_SIZE][X_SIZE], double amp) {
	gradient(image_in, image_out, amp, cx_sobel, cy_sobel);
}
/*任意運算子*/
void gradient(unsigned char image_in[Y_SIZE][X_SIZE], unsigned char image_out[Y_SIZE][X_SIZE], double amp,int cx[9], int cy[9]) {
	int d[9];
	int i, j, dat;
	double xx, yy, zz;

	for (i = 1; i < Y_SIZE;i++) {
		for (j = 1; j < X_SIZE - 1;j++) {
			d[0] = image_in[i - 1][j - 1];
			d[1] = image_in[i-1][j];
			d[2] = image_in[i-1][j+1];
			d[3] = image_in[i][j-1];
			d[4] = image_in[i][j];
			d[5] = image_in[i][j+1];
			d[6] = image_in[i+1][j-1];
			d[7] = image_in[i+1][j];
			d[8] = image_in[i+1][j+1];
			xx = (double)(cx[0] * d[0] + cx[1] * d[1] + cx[2] * d[2] + cx[3] * d[3] + cx[4] * d[4] + cx[5] * d[5] + cx[6] * d[6] + cx[7] * d[7] + cx[8] * d[8]);
			yy = (double)(cy[0] * d[0] + cy[1] * d[1] + cy[2] * d[2] + cy[3] * d[3] + cy[4] * d[4] + cy[5] * d[5] + cy[6] * d[6] + cy[7] * d[7] + cy[8] * d[8]);
			zz = (double)(amp*sqrt(xx*xx + yy*yy));
			dat = (int)zz;
			if (dat > 255) dat = 255;
			image_out[i][j] = (char)dat;
		}
	}
}
//list4.2用Prewitt
/*image_in輸入影像陣列;image_out輸出影像陣列;amp輸出影像的增益*/
void prewitt(unsigned char image_in[Y_SIZE][X_SIZE], unsigned char image_out[Y_SIZE][X_SIZE], double amp) {
	int d0=0, d1=0, d2=0, d3=0, d4=0, d5=0, d6=0, d7=0, d8=0;
	int i, j, k, max, dat;
	int m[8];
	double zz;
	for (i = 1; i < Y_SIZE - 1;i++) {
		for (j = 1; j < X_SIZE - 1;j++) {
			d0 = image_in[i - 1][j - 1];
			d1 = image_in[i - 1][j];
			d2 = image_in[i - 1][j + 1];
			d3 = image_in[i][j - 1];
			d4 = image_in[i][j];
			d5 = image_in[i][j + 1];
			d6 = image_in[i + 1][j - 1];
			d7 = image_in[i + 1][j];
			d8 = image_in[i + 1][j + 1];
			m[0] = d0 + d1 + d2 + d3 - 2 * d4 + d5 - d6 - d7 - d8;
			m[1] = d0 + d1 + d2 + d3 - 2 * d4 - d5 + d6 - d7 - d8;
			m[2] = d0 + d1 - d2 + d3 - 2 * d4 - d5 + d6 + d7 - d8;
			m[3] = d0 - d1 - d2 + d3 - 2 * d4 - d5 + d6 + d7 + d8;
			m[4]= -d0 - d1 - d2 + d3 - 2 * d4 + d5 + d6 + d7 + d8;
			m[5]= -d0 - d1 + d2 - d3 - 2 * d4 + d5 + d6 + d7 + d8;
			m[6]= -d0 + d1 + d2 - d3 - 2 * d4 + d5 - d6 + d7 + d8;
			m[7] = d0 + d1 + d2 - d3 - 2 * d4 + d5 - d6 - d7 + d8;
			max = 0;
			for (k = 0; k < 8;k++) {
				if (max<m[k]) {
					max = m[k];
				}
			}
			zz = amp*(double)(max);
			dat = (int)(zz);
			if (dat > 255) dat = 255;
			image_out[i][j] = (char)dat;
		}
	}
}
//list4.3二值影像細線化
int ncon(int p[9]);
void thinning(unsigned char image_in[Y_SIZE][X_SIZE],unsigned char image_out[Y_SIZE][X_SIZE]) {
	int flg = 1;
	int i, j, k, n;
	int p[9];/*圖形:1背景:0，背景候補-1*/
	for (i = 0; i < Y_SIZE;i++) {
		for (j = 0; j < X_SIZE;j++) {
			image_out[i][j] = image_in[i][j];
		}
	}
	while (flg!=0) {
		flg = 0;
		for (i = 1; i < Y_SIZE - 1;i++) {
			for (j = 1; j < X_SIZE - 1;j++) {
				p[0] = image_out[i][j];
				p[1] = image_out[i][j+1];
				p[2] = image_out[i-1][j+1];
				p[3] = image_out[i-1][j];
				p[4] = image_out[i-1][j-1];
				p[5] = image_out[i][j-1];
				p[6] = image_out[i+1][j-1];
				p[7] = image_out[i+1][j];
				p[8] = image_out[i+1][j]+1;
				for (k = 0; k < 9;k++) {
					if (p[k] == HIGH) p[k] = 1;
					else if (p[k] == LOW) p[k] = 0;
					else p[k] = -1;
				}
				/*1.圖形的一部份*/
				if (p[0] != 1) continue;
				/*2.境界像素*/
				if (p[1] * p[3] * p[5] * p[7] != 0) continue;
				/*3.保留端點*/
				n = 0;
				for (k = 1; k < 9;k++) {
					if (p[k] != 0) n++;	
				}
				if (n < 2) continue;
				/*4.保留獨立點*/
				n = 0;
				for (k = 1; k < 9; k++) {
					if (p[k] != 1) n++;	
				}
				if (n < 1) continue;
				/*5.保留連結性*/
				if (ncon(p) != 1) continue;
				/*6.線條寬度為2時，只去除單邊*/
				n = 0;
				for (k = 1; k < 9;k++) {
					if (p[k] != -1) n++;
					else if (p[k] == -1) {
						p[k] = 0;
						if (ncon(p) == 1) n++;
						p[k] = -1;
					}
				}
				if (n < 8) continue;
				/*1.-6.全滿足時的消除對象*/
				image_out[i][j] = TMP;
				flg++;
			}
		}
		for (i = 1; i < Y_SIZE - 1;i++) {
			for (j = 1; j < X_SIZE - 1;j++) {
				if (image_out[i][j] == TMP) image_out[i][j] = LOW;
			}
		}
	}
}
/*--ncon--研究連結數(8個鄰近像素連結)，p:像素陣列3x3*/
int ncon(int p[9]) {
	int i, i1, i2;
	int q[9];
	int n = 0;
	for (i = 0; i < 9;i++) {
		if ((p[i] == 1) || (p[i] == -1)) q[i] = 0;
		else q[i] = 1;
	}
	for (i = 1; i < 9;i+=2) {
		i1 = i + 1;
		i2 = i + 2;
		if (i2 == 9) i2 = 1;
		n = n + q[i] - q[i] * q[i1] * q[i2];
	}
	return n;
}
//list4.4影像的二階微分化
/*amp輸出影像的增益，type係數類型(1.2.3)*/
void laplacian(unsigned char image_in[Y_SIZE][X_SIZE],unsigned char image_out[Y_SIZE][X_SIZE],double amp, int type) {
	int i, j;
	int d;
	int c[3][9] = { 0,-1,0,-1,4,-1,0,-1,0,-1,-1,-1,-1,8,-1,-1,-1,-1,1,-2,1,-2,4,-2,1,-2,1 };
	type = type - 1;
	if (type < 0) type = 0;
	if (type > 2) type = 2;
	for (i = 1; i < Y_SIZE - 1;i++) {
		for (j = 1; j < X_SIZE - 1;j++) {
			d = c[type][0] * image_in[i - 1][j - 1] +
				c[type][1] * image_in[i - 1][j] +
				c[type][2] * image_in[i - 1][j + 1] +
				c[type][3] * image_in[i][j - 1] +
				c[type][4] * image_in[i][j] +
				c[type][5] * image_in[i][j + 1] +
				c[type][6] * image_in[i + 1][j - 1] +
				c[type][7] * image_in[i + 1][j] +
				c[type][8] * image_in[i + 1][j + 1];
			d = (int)(d*amp) + OFFSET;
			if (d < 0) d = 0;
			if (d > 255) d = 255;
			image_out[i][j] = (unsigned char)d;
		}
	}
}
//list4.5利用零點交叉判定，擷取輪廓
void zero_cross(unsigned char image_in[Y_SIZE][X_SIZE],unsigned char image_out[Y_SIZE][X_SIZE]) {
	int i, j;
	for (i = 0; i < Y_SIZE;i++) {
		for (j = 0; j < X_SIZE;j++) {
			image_out[i][j] = LOW;
		}
	}

	for (i = 1; i < Y_SIZE - 1;i++) {
		for (j = 1; j < X_SIZE - 1;j++) {
			if ((int)image_in[i][j] == OFFSET ) {
				if (((int)image_in[i][j + 1] - OFFSET)*((int)image_in[i][j - 1] - OFFSET) < 0) image_out[i][j] = HIGH;
				if (((int)image_in[i + 1][j] - OFFSET)*((int)image_in[i - 1][j] - OFFSET) < 0) image_out[i][j] = HIGH;
				if (((int)image_in[i + 1][j + 1] - OFFSET)*((int)image_in[i - 1][j - 1] - OFFSET) < 0) image_out[i][j] = HIGH;
				if (((int)image_in[i + 1][j - 1] - OFFSET)*((int)image_in[i - 1][j + 1] - OFFSET) < 0) image_out[i][j] = HIGH;
			}
			else {
				if (((int)image_in[i][j] - OFFSET)*((int)image_in[i-1][j - 1] - OFFSET) < 0) image_out[i][j] = HIGH;
				if (((int)image_in[i][j] - OFFSET)*((int)image_in[i - 1][j] - OFFSET) < 0) image_out[i][j] = HIGH;
				if (((int)image_in[i][j] - OFFSET)*((int)image_in[i - 1][j + 1] - OFFSET) < 0) image_out[i][j] = HIGH;
				if (((int)image_in[i][j] - OFFSET)*((int)image_in[i][j - 1] - OFFSET) < 0) image_out[i][j] = HIGH;
			}
		}
	}
}
//list4.6利用Hough變換方法擷取直線
/*image_hough輸出影像陣列(Hough)，thresh臨界值，buf資料用緩衝記憶體*/
void hough_line(unsigned char image_in[Y_SIZE][X_SIZE], unsigned char image_out[Y_SIZE][X_SIZE], double rho, double theta);
void hough(unsigned char image_in[Y_SIZE][X_SIZE], unsigned char image_out[Y_SIZE][X_SIZE], unsigned char image_hough[Y_SIZE][X_SIZE], int thresh, char *buf) {
	int i, j, u, v, n;
	double rho, theta, d, a;
	double p[DMAX][2];
	int posi, m;

	d = PI / X_SIZE;
	a = Y_SIZE / 2 / sqrt(X_SIZE / 2 * X_SIZE / 2 + Y_SIZE / 2 * Y_SIZE / 2);
	for (i = 0; i < Y_SIZE; i++)
		for (j = 0; j < X_SIZE; j++)
			image_hough[i][j] = 0;
	for (i = 0; i < Y_SIZE; i++) {
		for (j = 0; j < X_SIZE; j++) {
			if (image_in[i][j] != HIGH) continue;
			for (u = 0; u < X_SIZE; u++) {
				theta = u*d;
			}
			rho = (j - X_SIZE / 2)*cos(theta) + (Y_SIZE / 2 - i)*sin(theta);
			v = (int)(rho*a + Y_SIZE / 2 + 0.5);
			if (v >= 0 && v < Y_SIZE)
				if (image_hough[v][u] < 255)
					image_hough[v][u] += 1;
		}
	}
	n = 0; posi = 0;
	for(u=0;u<X_SIZE;u++)
		for (v = 0; v < Y_SIZE; v++) {
			if (image_hough[v][u] < thresh) continue;
			if (u != 0 && v != 0 && image_hough[v][u] < image_hough[v - 1][u - 1]) continue;
			if (v != 0 && image_hough[v][u] < image_hough[v - 1][u]) continue;
			if (u != X_SIZE - 1 && v != 0 && image_hough[v - 1][u + 1] < image_hough[v - 1][u - 1]) continue;
			if (u != 0 && image_hough[v][u] < image_hough[v][u - 1]) continue;
			if (u != X_SIZE - 1 && image_hough[v][u] < image_hough[v][u + 1]) continue;
			if (u != 0 && v != Y_SIZE - 1 && image_hough[v][u] < image_hough[v + 1][u - 1]) continue;
			if (v != Y_SIZE && image_hough[v][u] < image_hough[v + 1][u]) continue;
			if (u != X_SIZE - 1 && v != Y_SIZE - 1 && image_hough[v][u] < image_hough[v + 1][u + 1]) continue;
			theta = u*d;
			rho = (v - Y_SIZE / 2) / a;
			p[n][0] = rho; p[n][1] = theta; n++;
			m = sprintf(&buf[posi], "theta=%10.3lf, rho=%10.3lf, value=%5d\n", theta * 180 / PI, rho, image_hough[v][u]);
			posi += m;
			if (n == DMAX) return;
		}
	for (i = 0; i < Y_SIZE; i++) {
		for (j = 0; j < X_SIZE; j++) {
			image_out[i][j] = image_in[i][j];
		}
	}
	for (i = 0; i < n; i++) {
		hough_line(image_in, image_out, p[i][0], p[i][1]);
	}
}
/*u,v直線的參數*/
void hough_line(unsigned char image_in[Y_SIZE][X_SIZE],unsigned char image_out[Y_SIZE][X_SIZE],double rho,double theta) {
	int i, j;
	if ((theta >= 0 && theta < PI / 4) || (theta >= 3 * PI / 4)) {
		for (i = 0; i < Y_SIZE; i++) {
			j = (int)((rho - (Y_SIZE / 2 - i)*sin(theta)) / cos(theta) + X_SIZE / 2 + 0.5);
			if (j >= 0 && j < X_SIZE) image_out[i][j] = 255;
		}
	}
	else {
		for (j = 0; j < X_SIZE; j++) {
			i = (int)(-rho + (j - X_SIZE / 2)*cos(theta)) / sin(theta) + Y_SIZE / 2 + 0.5);
			if (i >= 0 && i < Y_SIZE) image_out[i][j] = 255;
		}
	}
}

//list4.7求得兩直線的交叉點
void hough_cross(double theta1,double rho1,double theta2,doubl rho2,double *x,double *y) {
	double d, t1, t2;
	t1 = theta1*PI / 180.0;
	t2 = theta2*PI / 180.0;
	if (d == 0) return;
	*x = (rho2*sin(t1) - rho1*sin(t2)) / d;
	*y = (rho1*cos(t2) - rho2*cos(t1)) / d;
}




