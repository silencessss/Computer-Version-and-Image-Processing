#pragma once
#include "Params.h"
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
/****list14.1 �v��������****/
void mosaic_affine(unsigned char image_in1[Y_SIZE][X_SIZE], unsigned char image_in2[Y_SIZE][X_SIZE], unsigned char image_out[Y_SIZE * 2][X_SIZE * 2],
	double a, double b, double c, double d, double e, double f, int type);
void mosaic_key(unsigned char image_in[Y_SIZE * 2][X_SIZE * 2], unsigned char image_out[Y_SIZE * 2][X_SIZE * 2], int type);
/***�v��������****/
/*----------------------------------------------------------------
	image_in1��J�v���}�C1�Aimage_in2��J�v���}�C2
	image_out��X�v���}�C(�����X�����v���A�v���j�p����J�v����2X2��)
	mx �۹��v��1���v��2�������qx
	my �۹��v��1���v��2�������qy
	zm zoom��j�Y�p�q
	rt ���ਤ��
	type���Ƴ������B�z(0:�л\�A1:���)
-----------------------------------------------------------------*/

void mosaic(unsigned char image_in1[Y_SIZE][X_SIZE], unsigned char image_in2[Y_SIZE][X_SIZE],
	unsigned char image_out[Y_SIZE * 2][X_SIZE * 2], double mx, double my, double zm, double rt, int type) {
	double a, b, c, d, e, f, cs, sn, x0, y0;
	double r;

	r = rt*PI / 180.0;
	cs = (double)cos(r);
	sn = (double)sin(r);
	x0 = X_SIZE / 2;
	y0 = Y_SIZE / 2;
	a = zm*cs;
	b = zm*sn;
	c = x0 + mx - zm*x0*cs - zm*y0*sn;
	d = -zm*sn;
	e = zm*cs;
	f = y0 + my + zm*x0*sn - zm*y0*cs;
	mosaic_affine(image_in1, image_in2, image_out, a, b, c, d, e, f, type);
}
/***�v��������_���***/
/*--------------------------------------------------------------------
	image_in1��J�v���}�C1�Aimage_in2��J�v���}�C2
	image_out��X�v���}�C(�����X�����v���A�v���j�p����J�v����2X2��)
	a,b,c�ܴ��Y�� X = ax+by+c
	d,e,f�ܴ��Y�� Y = dx+dy+f
	type���Ƴ������B�z(0:�л\�A1:���)
----------------------------------------------------------------------*/
void mosaic_affine(unsigned char image_in1[Y_SIZE][X_SIZE], unsigned char image_in2[Y_SIZE][X_SIZE],
	unsigned char image_out[Y_SIZE * 2][X_SIZE * 2],
	double a, double b, double c, double d, double e, double f, int type) {
	int i, j, m, n;
	int xs, ys;
	double x, y, p, q;
	double aa, bb, cc, dd, ee, ff, gg;
	double dat, k;
	unsigned char image_wk1[Y_SIZE * 2][X_SIZE * 2];
	unsigned char image_wk2[Y_SIZE * 2][X_SIZE * 2];
	unsigned char image_wk3[Y_SIZE * 2][X_SIZE * 2];
	xs = (X_SIZE - (int)c) / 2;
	ys = (Y_SIZE - (int)f) / 2;
	for (i = 0; i < Y_SIZE * 2; i++) {
		for (j = 0; j < X_SIZE * 2; j++) {
			image_wk1[i][j] = 0;
			image_wk2[i][j] = 0;
			image_wk3[i][j] = 0;
		}
	}
	for (i = 0; i < Y_SIZE * 2; i++) {
		for (j = 0; j < X_SIZE * 2; j++) {
			n = j - xs;
			m = i - ys;
			if ((m >= 0) && (m < Y_SIZE) && (n >= 0) && (n < X_SIZE)) {
				image_wk1[i][j] = image_in1[m][n];
				image_wk3[i][j] = 1;
			}
		}
	}
	gg = 1 / (a*e - b*d);
	aa = e*gg;
	bb = -b*gg;
	cc = (b*f - c*e)*gg;
	dd = -d*gg;
	ee = a*gg;
	ff = (c*d - a*f)*gg;
	for (i = 0; i < Y_SIZE * 2; i++) {
		for (j = 0; j < X_SIZE * 2; j++) {
			x = aa*j + bb*i + cc - xs;
			y = dd*j + ee*i + ff - ys;
			if (y > 0) m = (int)y;
			else m = (int)(y - 1);
			if (x > 0) n = (int)x;
			else n = (int)(x - 1);
			q = y - m;
			p = x - n;
			if ((m >= 0) && (m < Y_SIZE) && (n >= 0) && (n < X_SIZE)) {
				dat = (int)((1.0 - q)*((1.0 - p)*image_in2[m][n] + p*image_in2[m][n + 1]) + q*((1.0 - p)*image_in2[m + 1][n] + p*image_in2[m + 1][n + 1]));
				if (dat < 0) dat = 0;
				if (dat > 255) dat = 255;
				image_wk2[i][j] = (unsigned char)dat;
				if (image_wk3[i][j] == 1) image_wk3[i][j] = 3;
				else image_wk3[i][j] = 2;
			}
		}
	}
	mosaic_key(image_wk3, image_out, type);
	for (i = 0; i < Y_SIZE * 2; i++) {
		for (j = 0; j < X_SIZE * 2; j++) {
			k = image_out[i][j] / 255.0;
			dat = image_wk1[i][j] * (1.0 - k) + image_wk2[i][j] * k;
			if (dat < 0) d = 0;
			if (dat > 255) d = 255;
			image_out[i][j] = (unsigned char)dat;
		}
	}
}
/***�������乳��������***/
/*-------------------------------------------------------------------------------
	image_in��J�v���}�C(1:�u�t�v��1�A2:�u�t�v��2�A3:�v��1�P�v��2����
	image_out��X�v���}�C(�X������v��)
	type���Ƴ������B�z(0;�л\�A1:���)
--------------------------------------------------------------------------------*/
void mosaic_key(unsigned char image_in[Y_SIZE * 2][X_SIZE * 2], unsigned char image_out[Y_SIZE * 2][X_SIZE * 2], int type) {
	int i, j, m, n, m1, m2, n1, n2, mm, nn;
	double c, d;
	m1 = X_SIZE * 2;
	m2 = 0;
	n1 = Y_SIZE * 2;
	n2 = 0;
	for (i = 0; i < Y_SIZE * 2; i++) {
		for (j = 0; j < X_SIZE * 2; j++) {
			if (image_in[i][j] == 3) {
				if (i < m1) m1 = i;
				if (i > m2) m2 = i;
				if (j < n1) n1 = j;
				if (j > n2) n2 = j;
			}
		}
	}
	mm = (m2 - m1) / 2;
	nn = (n2 - n1) / 2;
	for (i = 0; i < Y_SIZE * 2; i++) {
		for (j = 0; j < X_SIZE * 2; j++) {
			if (image_in[i][j] == 2) image_out[i][j] = 255;
			else if (image_in[i][j] == 3) {
				if (type == 0) image_out[i][j] = 255;
				else {
					c = 0; d = 0;
					for (m = -mm; m <= mm; m++) {
						for (n = -nn; n <= nn; n++) {
							if (image_in[i + m][j + n] == 1) c++;
							if (image_in[i + m][j + n] == 2) { c++; d += 255; }
							if (image_in[i + m][j + n] == 3) { c++; d += 128; }
						}
					}
					image_out[i][j] = (unsigned char)(d / c);
				}
			}
			else image_out[i][j] = 0;
		}
	}
}
/****list14.2�Τ��ǰt�A�D�o�v��������m****/
/*----------------------------------------------------------------------
	image_in1��J�v���}�C1�Aimage_in2��J�v���}�C2
	x1,y1 �����v��1�B2�ɡA�j�M������m���p�U�A�v��1�����
	x2,y2 �����v��1�B2�ɡA�j�M������m���p�U�A�v��2�����
	xd,yd �����v��1�B2�ɡA�j�M������m���d��(+-xd,+-yd)
	bx,by �ǰt�ϰ�
	mx,my ������m
-----------------------------------------------------------------------*/
void mosaic_coef_blockmatch(unsigned char image_in1[Y_SIZE][X_SIZE], unsigned char image_in2[Y_SIZE][X_SIZE],
	int x1, int y1, int x2, int y2, int xd, int yd, int bx, int by, int *mx, int *my) {
	int i, j, m, n;
	int xx1, yy1, xx2, yy2;
	double d, min;
	for (i = -yd; i <= yd; i++) {
		for (j = -xd; i <= xd; j++) {
			d = 0;
			for (m = -by / 2; m < by / 2; m++) {
				yy1 = y1 + m;
				yy2 = y2 + i + m;
				for (n = -bx / 2; n < bx / 2; n++) {
					xx1 = x1 + n;
					xx2 = x2 + j + n;
					d += abs(image_in1[yy1][xx1] - image_in2[yy2][xx2]);
				}
			}
			if (j == -xd && i == -yd) {
				min = d;
				*mx = j + x1 - x2;
				*my = i + y1 - y2;
			}
		}
	}
}
/****list14.3�Τ��ǰt�A�D�o�v��������m(�Ω�m��v��)****/
/*---------------------------------------------------------
	image_in1��J�v���}�C1(�m��)
	image_in2��J�v���}�C2(�m��)
	x1,y1 �����v��1�B2�ɡA�j�M������m���p�U�A�v��1�����
	x2,y2 �����v��1�B2�ɡA�j�M������m���p�U�A�v��2�����
	xd,yd �����v��1�B2�ɡA�j�M������m���d��(+-xd,+-yd)
	bx,by �ǰt�ϰ�
	mx,my ������m
----------------------------------------------------------*/
void mosaic__coef_blockmatch_rgb(unsigned char image_in1[3][Y_SIZE][X_SIZE], unsigned char image_in2[3][Y_SIZE][X_SIZE],
	int x1, int y1, int x2, int y2, int xd, int yd, int bx, int by, int *mx, int *my) {
	int i, j, n, m;
	int xx1, yy1, xx2, yy2;
	double d, min;
	for (i = -yd; i <= yd; i++) {
		for (j = -xd; j <= xd; j++) {
			d = 0;
			for (m = -by / 2; m < by / 2; m++) {
				yy1 = y1 + m;
				yy2 = y2 + i + m;
				for (n = -bx / 2; n < bx / 2; n++) {
					xx1 = x1 + n;
					xx2 = x2 + j + n;
					d += abs(image_in1[0][yy1][xx1] - image_in2[0][yy2][xx2])
						+ abs(image_in1[1][yy1][xx1] - image_in2[1][yy2][xx2])
						+ abs(image_in1[2][yy1][xx1] - image_in2[2][yy2][xx2]);
				}
			}
			if (i == -xd && i == -yd) {
				min = d;
				*mx = j + x1 - x2;
				*my = i + y1 - y2;
			}
		}
	}
}

















