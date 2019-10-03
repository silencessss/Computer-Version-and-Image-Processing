#pragma once
#include "Params.h"
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
/****list14.1 影像之拼接****/
void mosaic_affine(unsigned char image_in1[Y_SIZE][X_SIZE], unsigned char image_in2[Y_SIZE][X_SIZE], unsigned char image_out[Y_SIZE * 2][X_SIZE * 2],
	double a, double b, double c, double d, double e, double f, int type);
void mosaic_key(unsigned char image_in[Y_SIZE * 2][X_SIZE * 2], unsigned char image_out[Y_SIZE * 2][X_SIZE * 2], int type);
/***影像之拼接****/
/*----------------------------------------------------------------
	image_in1輸入影像陣列1，image_in2輸入影像陣列2
	image_out輸出影像陣列(拼接合成之影像，影像大小為輸入影像的2X2倍)
	mx 相對於影像1的影像2之偏移量x
	my 相對於影像1的影像2之偏移量y
	zm zoom放大縮小量
	rt 旋轉角度
	type重複部分的處理(0:覆蓋，1:梯度)
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
/***影像之拼接_仿形***/
/*--------------------------------------------------------------------
	image_in1輸入影像陣列1，image_in2輸入影像陣列2
	image_out輸出影像陣列(拼接合成之影像，影像大小為輸入影像的2X2倍)
	a,b,c變換係數 X = ax+by+c
	d,e,f變換係數 Y = dx+dy+f
	type重複部分的處理(0:覆蓋，1:梯度)
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
/***拼接關鍵像素之產生***/
/*-------------------------------------------------------------------------------
	image_in輸入影像陣列(1:只含影像1，2:只含影像2，3:影像1與影像2重複
	image_out輸出影像陣列(合成關鍵影像)
	type重複部分的處理(0;覆蓋，1:梯度)
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
/****list14.2用方格匹配，求得影像拼接位置****/
/*----------------------------------------------------------------------
	image_in1輸入影像陣列1，image_in2輸入影像陣列2
	x1,y1 拼接影像1、2時，搜尋對應位置情況下，影像1的基準
	x2,y2 拼接影像1、2時，搜尋對應位置情況下，影像2的基準
	xd,yd 拼接影像1、2時，搜尋對應位置的範圍(+-xd,+-yd)
	bx,by 匹配區域
	mx,my 拼接位置
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
/****list14.3用方格匹配，求得影像拼接位置(用於彩色影像)****/
/*---------------------------------------------------------
	image_in1輸入影像陣列1(彩色)
	image_in2輸入影像陣列2(彩色)
	x1,y1 拼接影像1、2時，搜尋對應位置情況下，影像1的基準
	x2,y2 拼接影像1、2時，搜尋對應位置情況下，影像2的基準
	xd,yd 拼接影像1、2時，搜尋對應位置的範圍(+-xd,+-yd)
	bx,by 匹配區域
	mx,my 拼接位置
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


















