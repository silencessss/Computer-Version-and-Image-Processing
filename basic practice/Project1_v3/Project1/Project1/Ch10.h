#pragma once
#include "Params.h"
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
//list10�X�j�Y�p
//zx��j���v(��V)�Azy��j���v(�a�V)
/*10.2�X�j�Y�p_�̪�F�k*/
void scale_near(unsigned char image_in[Y_SIZE][X_SIZE], unsigned char image_out[Y_SIZE][X_SIZE], double zx, double zy) {
	int i, j, m, n;
	for (i = 0; i < Y_SIZE; i++) {
		for (j = 0; j < X_SIZE; j++) {
			if (i > 0) m = (int)(i / zy + 0.5);
			else m = (int)(i / zy - 0.5);
			if (j > 0) n = (int)(j / zx + 0.5);
			else n = (int)(j / zx - 0.5);
			if ((m >= 0) && (m < Y_SIZE) && (n >= 0) && (n < X_SIZE))	image_out[i][j] = image_in[m][n];
			else image_out[i][j] = 0;
		}
	}
}
//list10.3�X�j�Y�p_�u�ʤ����k
void scale(unsigned char image_in[Y_SIZE][X_SIZE], unsigned char image_out[Y_SIZE][X_SIZE], double zx, double zy) {
	int i, j, m, n,d;
	double x, y, p, q;
	for (i = 0; i < Y_SIZE; i++) {
		for (j = 0; j < X_SIZE; j++) {
			y = i / zy;
			x = j / zx;
			if (y > 0) m = (int)y;
			else m = (int)(y - 1);
			if (x > 0) n = (int)x;
			else n = (int)(x - 1);
			q = y - m;
			p = x - n;
			if (q == 1) { q = 0; m = m + 1; }
			if (p == 1) { p = 0; n = n + 1; }
			if ((m >= 0) && (m < Y_SIZE) && (n >= 0) && (n < X_SIZE)) {
				d = (int)((1.0 - q)*(1.0 - p)*image_in[m][n] + p*image_in[m][n + 1]) + q*((1.0 - p)*image_in[m + 1][n] + p*image_in[m + 1][n + 1]);
			}
			else d = 0;
			if (d < 0) d = 0;
			if (d > 255) d = 255;
			image_out[i][j] = d;
		}
	}
}
//list10.4��m������_�u�ʤ����k
/*px,py���ʶq��V�B�a�V*/
void shift(unsigned char image_in[Y_SIZE][X_SIZE], unsigned char image_out[Y_SIZE][X_SIZE], double px, double py) {
	int i, j, m, n, d;
	double x, y, p, q;
	for (i = 0; i < Y_SIZE; i++) {
		for (j = 0; j < X_SIZE; j++) {
			y = i - py;
			x = j - px;
			if (y > 0) m = (int)y;
			else m = (int)(y - 1);
			if (x > 0) n = (int)x;
			else n = (int)(x - 1);
			q = y - m;
			p = x - n;
			if ((m >= 0) && (m < Y_SIZE) && (n >= 0) && (n < X_SIZE)) {
				d = (int)((1.0 - q)*((1.0 - p)*image_in[m][n] + p*image_in[m][n + 1]) + q*((1.0 - p)*image_in[m + 1][n] + p*image_in[m + 1][n + 1]));
			}
			else d = 0;
			if (d < 0) d = 0;
			if (d > 255) d = 255;
			image_out[i][j] = d;
		}
	}
}
//list10.5����_�u�ʤ����k
/*deg���ਤ��*/
void rotation(unsigned char image_in[Y_SIZE][X_SIZE], unsigned char image_out[Y_SIZE][X_SIZE], double deg) {
	int i, j, m, n, d;
	double x, y, p, q, r, c, s;
	r = deg*3.141592 / 180.0;
	c = cos(r);
	s = sin(r);
	for (i = 0; i < Y_SIZE; i++) {
		for (j = 0; j < X_SIZE; j++) {
			y = j*s + i*c;
			x = j*c - i*s;
			if (y > 0) m = (int)y;
			else m = (int)(y - 1);
			if (x > 0) n = (int)x;
			else n = (int)(x - 1);
			q = y - m;
			p = x - n;
			if ((m >= 0) && (m < Y_SIZE) && (n >= 0) && (n < X_SIZE)) {
				d = (int)((1.0 - q)*((1.0 - p)*image_in[m][n] + p*image_in[m][n + 1]) + q*((1.0 - p)*image_in[m + 1][n] + p*image_in[m + 1][n + 1]));
			}
			else d = 0;
			if (d < 0) d = 0;
			if (d > 255) d = 255;
			image_out[i][j] = d;
		}
	}
}
//list10.6��j�Y�p���ಾ��_�u�ʤ����k
/*zx,zy��j���v�Adeg���ਤ�סApx,py�����I*/
void scale_rotate_shift(unsigned char image_in[Y_SIZE][X_SIZE], unsigned char image_out[Y_SIZE][X_SIZE], double zx, double zy, double deg, double px, double py) {
	int i, j, m, n, d;
	double x, y, u, v, p, q, r, c, s;
	r = deg*PI / 180;
	c = cos(r);
	s = sin(r);
	for (i = 0; i < Y_SIZE; i++) {
		for (j = 0; j < X_SIZE; j++) {
			v = (double)(i - py);
			u = (double)(j - px);
			y = (u*s + v*c) / zy + py;
			x = (u*c - v*s) / zx + px;
			if (y > 0) m = (int)y;
			else m = (int)(y - 1);
			if (x > 0) n = (int)x;
			else n = (int)(x - 1);
			q = y - m;
			p = x - n;
			if ((m >= 0) && (m < Y_SIZE) && (n >= 0) && (n < X_SIZE)) {
				d = (int)((1.0 - q)*((1.0 - p)*image_in[m][n] + p*image_in[m][n + 1]) + q*((1.0 - p)*image_in[m + 1][n] + p*image_in[m + 1][n + 1]));
			}
			else d = 0;
			if (d < 0) d = 0;
			if (d > 255) d = 255;
			image_out[i][j] = d;
		}
	}
}
//list10.7����ഫ_�u�ʤ����k
/*a,b,c�ܴ��Y��X=ax+by+c�Ad,e,f�ܴ��Y��Y=dx+ey+f*/
void affine(unsigned char image_in[Y_SIZE][X_SIZE], unsigned char image_out[Y_SIZE][X_SIZE], double a, double b, double c, double d, double e, double f) {
	int i, j, m, n, dat;
	double x, y, p, q, aa, bb, cc, dd, ee, ff, gg;
	gg = 1 / (a*e - b*d);
	aa = e*gg;
	bb = -b*gg;
	cc = (b*f - c*e)*gg;
	dd = -d*gg;
	ee = a*gg;
	ff = (c*d - a*f)*gg;
	for (i = 0; i < Y_SIZE; i++) {
		for (j = 0; j < X_SIZE; j++) {
			x = aa*j + bb*i + cc;
			y = dd*j + ee*i + ff;
			if (y > 0) m = (int)y;
			else m = (int)(y - 1);
			if (x > 0) n = (int)x;
			else n = (int)(x - 1);
			q = y - m;
			p = x - n;
			if ((m >= 0) && (m < Y_SIZE) && (n >= 0) && (n < X_SIZE)) {
				dat = (int)((1.0 - q)*((1.0 - p)*image_in[m][n] + p*image_in[m][n + 1]) + q*((1.0 - p)*image_in[m + 1][n] + p*image_in[m + 1][n + 1]));
			}
			else dat = 0;
			if (dat < 0) dat = 0;
			if (dat > 255) dat = 255;
			image_out[i][j] = dat;
		}
	}
}
//list10.8�q3�ӹ����I�A�D����ഫ���ܴ��Y��
/*xn,yn�����In�b�ܴ��e��(x,y)�y�СAa,b,c�ܴ��Y��X=ax+by+c�Ad,e,f�ܴ��Y��Y=dx+ey+f*/
void affine_coef(int x1, int y1, int u1, int v1, int x2, int y2, int u2, int v2, int x3, int y3, int u3, int v3,
	double *a, double *b, double *c, double *d, double *e, double *f) {
	double g;
	g = (x1 - x2)*(y2 - y3) - (x2 - x3)*(y1 - y2);
	if (g != 0) {
		*a = ((u1 - u2)*(y2 - y3) - (u2 - u3)*(y1 - y2)) / g;
		*b = ((u2 - u3)*(x1 - x2) - (u1 - u2)*(x2 - x3)) / g;
		*c = u1 - *a*x1 - *b*y1;
		*d = ((v1 - v2)*(y2 - y3) - (v2 - v3)*(y1 - y2)) / g;
		*e = ((v2 - v3)*(x1 - x2) - (v1 - v3)*(x2 - x3)) / g;
		*f = v1 - *d*x1 - *e*y1;
	}
	else {
		*a = 1; *b = 0; *c = 0; *d = 0; *e = 1; *f = 0;
	}
}
//list10.12���v��������
void lattice(unsigned char image[Y_SIZE][X_SIZE]) {
	int i, j, width;
	width = X_SIZE / 8;
	for (i = 0; i < Y_SIZE; i++) {
		for (j = 0; j < X_SIZE; j++) {
			if (((i - Y_SIZE / 2) % 32 == 0) || (i == Y_SIZE - 1) || ((j - X_SIZE / 2) % 32 == 0) || (j == X_SIZE - 1)) image[i][j] = 255;
			else image[i][j] = 0;
		}
	}
}























