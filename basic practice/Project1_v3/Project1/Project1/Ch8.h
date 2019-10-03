#pragma once
#include "Params.h"
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
//list8.1�s�@��m��
/*image_rgbRGB��m�v���}�C�Alevel�Ƕ��@�׭�*/
void colorbar(unsigned char image_rgb[3][Y_SIZE][X_SIZE], int level) {
	int i, j, width;
	width = X_SIZE / 8;
	for (i = 0; i < Y_SIZE; i++) {
		for (j = 0; j < X_SIZE; j++) {
			if (((j >= 0) && (j < 2 * width)) || ((j >= 4 * width) && (j < 6 * width))) {
				image_rgb[0][i][j] = level;
			}
			else image_rgb[0][i][j] = 0;
			if ( (j >= 0) && (j < 4 * width))  {
				image_rgb[1][i][j] = level;
			}
			else image_rgb[0][i][j] = 0;
			if (  ((j >= 0) && (j < width)) || 
				  ((j >= 2 * width) && (j < 3 * width)) || 
				  ((j >= 4 * width) && (j < 5 * width)) || 
				  ((j >= 6 * width) && (j < 7 * width)) ){
				image_rgb[2][i][j] = level;
			}
			else image_rgb[2][i][j] = 0;
		}
	}
}
//list8.2RGB�P�G��Y�A���M��S�A���H
/*rgb_to_ysh*/
void rgb_to_ysh(unsigned char image_in_rgb[3][Y_SIZE][X_SIZE], int image_out_ysh[3][Y_SIZE][X_SIZE]) {
	int i, j;
	double r, g, b, y, cb, cr, s, h;
	for (i = 0; i < Y_SIZE; i++) {
		for (j = 0; j < X_SIZE; j++) {
			r = (double)image_in_rgb[0][i][j];
			g = (double)image_in_rgb[1][i][j];
			b = (double)image_in_rgb[2][i][j];
			y = 0.2126*r + 0.7152*g + 0.0722*b;
			cb = (b - y) / 1.8556;
			cr = (r - y) / 1.5748;
			s = sqrt(cb*cb + cr*cr);
			if (s != 0) h = atan2(cr, cb)*180.0 / PI;
			else h = 0;
			image_out_ysh[0][i][j] = (int)y;
			image_out_ysh[1][i][j] = (int)s;
			image_out_ysh[2][i][j] = (int)h;
		}
	}
}
/*ysh_to_rgb*/
void ysh_to_rgb(int image_in_ysh[3][Y_SIZE][X_SIZE], unsigned char image_out_rgb[3][Y_SIZE][X_SIZE]) {
	int i, j;
	double r, g, b, cb, cr, y, rad;
	for (i = 0; i < Y_SIZE; i++) {
		for (j = 0; j < X_SIZE; j++) {
			y = image_in_ysh[0][i][j];
			rad = (double)(PI*image_in_ysh[2][i][j] / 180.0);
			cb = image_in_ysh[1][i][j] * cos(rad);
			cr = image_in_ysh[1][i][j] * sin(rad);
			r = y + 1.5748*cr;
			b = y + 1.8556*cb;
			g = (y - 0.2126*r - 0.0722*b) / 0.7152;
			if (r < 0) r = 0;
			if (r > 255) r = 255;
			if (g < 0) g = 0;
			if (g > 255) g = 255;
			if (b < 0) b = 0;
			if (b > 255) b = 255;
			image_out_rgb[0][i][j] = (unsigned char)r;
			image_out_rgb[1][i][j] = (unsigned char)g;
			image_out_rgb[2][i][j] = (unsigned char)b;
		}
	}
}
//list8.3�G�׹��M�צ�۸���ܴ����Ƕ��@�׼v��
/*image_in_y�G�ת��ƾڰ}�C*/
void y_image(int image_in_y[Y_SIZE][X_SIZE], unsigned char image_out[Y_SIZE][X_SIZE]) {
	int i, j, d;
	for (i = 0; i < Y_SIZE; i++) {
		for (j = 0; j < X_SIZE; j++) {
			d = image_in_y[i][j];
			if (d < 0) d = 0;
			if (d > 255) d = 255;
			image_out[i][j] = (unsigned char)d;
		}
	}
}
void sat_image(int image_in_sat[Y_SIZE][X_SIZE], unsigned char image_out[Y_SIZE][X_SIZE]) {
	int i, j, d;
	for (i = 0; i < Y_SIZE; i++) {
		for (j = 0; j < X_SIZE; j++) {
			d = image_in_sat[i][j];
			if (d < 0) d = 0;
			if (d > 255) d = 255;
			image_out[i][j] = (unsigned char)d;
		}
	}
}
/*����ܴ����Ƕ��@�׼v���Aiage_in_sat���M�׼ƾڰ}�C�Aimage_in_hue��ۼƾڰ}�C�Aorg��Ǧ��(��)*/
void hue_image(int image_in_sat[Y_SIZE][X_SIZE], int image_in_hue[Y_SIZE][X_SIZE], unsigned char image_out[Y_SIZE][X_SIZE], int org) {
	int i, j;
	double d;
	for (i = 0; i < Y_SIZE; i++) {
		for (j = 0; j < X_SIZE; j++) {
			if (image_in_sat[i][j] > 0) {
				d = image_in_hue[i][j];
				d = d - org;
				if (d < 0) d = -d;
				if (d > 180) d = 360.0 - d;
				d = 255.0 - d*255.0 / 180.0;
				if (d < 0) d = 0;
				if (d > 255) d = 255;
				image_out[i][j] = (unsigned char)d;
			}
			else image_out[i][j] = 0;
		}
	}
}
//list8.4�ܧ�G�׹��M�צ��
/*ya�G�׭��v�Ayb�G�װ���(Bias)�Asa���M�׭��v�Asb���M�װ����Ahb��۰���*/
void tran_ysh(int image_in_ysh[3][Y_SIZE][X_SIZE], int image_out_ysh[3][Y_SIZE][X_SIZE], double ya, double yb, double sa, double sb, double hb) {
	int i, j;
	for (i = 0; i < Y_SIZE; i++) {
		for (j = 0; j < X_SIZE; j++) {
			image_out_ysh[0][i][j] = (int)(image_in_ysh[0][i][j] * ya + yb);
			image_out_ysh[1][i][j] = (int)(image_in_ysh[1][i][j] * sa + sb);
			image_out_ysh[2][i][j] = (int)(image_in_ysh[2][i][j] + hb);
			if (image_out_ysh[2][i][j] > 180) image_out_ysh[2][i][j] -= 360;
			if (image_out_ysh[2][i][j] < -180) image_out_ysh[2][i][j] += 360;
		}
	}
}
//listRGB�X�j��0-255�d��
/*type�X�j����k(0:RGB�W�ߡA1:RGB�@�q*/
void range(unsigned char image_in[Y_SIZE][X_SIZE], int *fmax, int *fmin);
void expand(unsigned char image_in[Y_SIZE][X_SIZE], unsigned char image_out[Y_SIZE][X_SIZE], int fmax, int fmin);
void expand_rgb(unsigned char image_in_rgb[3][Y_SIZE][X_SIZE], unsigned char image_out_rgb[3][Y_SIZE][X_SIZE], int type) {
	int fmax[3], fmin[3], gmax, gmin;
	switch (type) {
	case 1:
		range(image_in_rgb[0], &fmax[0], &fmin[0]);
		range(image_in_rgb[1], &fmax[1], &fmin[1]);
		range(image_in_rgb[2], &fmax[2], &fmin[2]);
		gmax = fmax[0];
		if (fmax[1] > gmax) gmax = fmax[1];
		if (fmax[2] > gmax) gmax = fmax[2];
		gmin = fmin[0];
		if (fmin[1] < gmin) gmin = fmin[1];
		if (fmin[2] < gmin) gmin = fmin[2];
		expand(image_in_rgb[0], image_out_rgb[0], gmax, gmin);
		expand(image_in_rgb[1], image_out_rgb[1], gmax, gmin);
		expand(image_in_rgb[2], image_out_rgb[2], gmax, gmin);
		break;
	}
}
//list8.6�N�G�סB���M���X�j��0-255�d��
/*type�X�j��k(0:Y�A1:YS����*/
void tran_ysh(int image_in_ysh[3][Y_SIZE][X_SIZE], int image_out_ysh[3][Y_SIZE][X_SIZE], double ya, double yb, double sa, double sb, double hb);
void range_ys(int image_in_ysh[3][Y_SIZE][X_SIZE], int *ymax, int *ymin, int *smax, int *smin);
void expand_ysh(int image_in_ysh[3][Y_SIZE][X_SIZE], int image_out_ysh[3][Y_SIZE][X_SIZE], int type) {
	int ymax, ymin, smax, smin;
	double ya, yb, sa;
	switch (type) {
	case 1:
		range_ys(image_in_ysh, &ymax, &ymin, &smax, &smin);
		ya = 255.0 / (double)(ymax - ymin);
		yb = -255.0*ymin / (double)(ymax - ymin);
		sa = 255.0 / (double)smax;
		tran_ysh(image_in_ysh, image_in_ysh, ya, yb, sa, 0.0, 0.0);
		break;
	default:
		range_ys(image_in_ysh, &ymax, &ymin, &smax, &smin);
		ya = 255.0 / (double)(ymax - ymin);
		yb = -255.0*ymin / (double)(ymax - ymin);
		tran_ysh(image_in_ysh, image_in_ysh, ya, yb, 1.0, 0.0, 0.0);
		break;
	}
}
/*ymax,ymin�G�ת��̤j�̤p�ȡAsmax,smin���M�ת��̤j�̤p��*/
void range_ys(int image_in_ysh[3][Y_SIZE][X_SIZE], int *ymax, int *ymin, int *smax, int *smin) {
	int i, j, n;
	*ymax = 0;
	*ymin = 255;
	for (i = 0; i < Y_SIZE; i++) {
		for (j = 0; j < X_SIZE; j++) {
			n = image_in_ysh[0][i][j];
			if (n > *ymax) *ymax = n;
			if (n < *ymin) *ymin = n;
		}
	}
}
//list8.7������m��ܪk
/*type�ܴ����A(1,2)*/
void pseudo_color(unsigned char image_in_m[Y_SIZE][X_SIZE], unsigned char image_out_r[Y_SIZE][X_SIZE], unsigned char image_out_g[Y_SIZE][X_SIZE], unsigned char image_out_b[Y_SIZE][X_SIZE], int type) {
	int i, j;
	unsigned char lutr[256], lutg[256], lutb[256];
	switch (type) {
	case 1:
		for (i = 0; i < 256; i++)	lutr[i] = i;
		for (i = 0; i < 256; i++)	lutg[i] = i;
		for (i = 0; i < 256; i++)	lutb[i] = i;
		break;
	case 2:
		for (i = 0; i < 128; i++)	lutr[i] = 0;
		for (i = 128; i < 256; i++)	lutr[i] = 2*i-255;
		for (i = 0; i < 128; i++)	lutg[i] = 2*i;
		for (i = 128; i < 256; i++)	lutg[i] = 510-2*i;
		for (i = 0; i < 128; i++)	lutb[i] = 255-2*i;
		for (i = 128; i < 256; i++)	lutb[i] = 0;
	default:
		break;
	}
	for (i = 0; i < Y_SIZE; i++) {
		for (j = 0; j < X_SIZE; j++) {
			image_out_r[i][j] = lutr[image_in_m[i][j]];
			image_out_g[i][j] = lutg[image_in_m[i][j]];
			image_out_b[i][j] = lutb[image_in_m[i][j]];
		}
	}
}













