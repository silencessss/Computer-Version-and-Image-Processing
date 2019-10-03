#pragma once
#include "Params.h"
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
//list9.1�D�G�����������ϨñN��v����
/*image_in1,2�v�����X,Y�b�ΡAimage_hist�G������������*/
void hist2_image(unsigned char image_in1[Y_SIZE][X_SIZE], unsigned char image_in2[Y_SIZE][X_SIZE], unsigned char image_hist[Y_SIZE][X_SIZE]) {
	int i, j, kx, ky;
	int hx, hy, max, kk;
	for (i = 0; i < Y_SIZE; i++) {
		for (j = 0; j < X_SIZE; j++) {
			image_hist[i][j] = 0;
		}
	}
	max = 0;
	ky = 256 / Y_SIZE;
	kx = 256 / X_SIZE;
	/*�����*/
	for (i = 0; i < Y_SIZE; i++) {
		for (j = 0; j < X_SIZE; j++) {
			hy = (HIGH - (int)image_in2[i][j]) / ky;
			hx = ((int)image_in1[i][j]) / kx;
			if (image_hist[hy][hx] < HIGH) image_hist[hy][hx]++;
			if (max < image_hist[hy][hx]) max = image_hist[hy][hx];
		}
	}
	for (i = 0; i < Y_SIZE; i++) {
		for (j = 0; j < X_SIZE; j++) {
			if (image_hist[i][j] != 0) {
				kk = (long)image_hist[i][j] * HIGH / max + BIAS;
				if (kk > HIGH) image_hist[i][j] = HIGH;
				else image_hist[i][j] = kk;
			}
		}
	}
	for (i = 0; i < Y_SIZE; i++) image_hist[i][0] = HIGH;/*X�b*/
	for (j = 0; j < X_SIZE; j++) image_hist[Y_SIZE - 1][j] = HIGH;/*Y�b*/
}
//list9.2�Q��RGB�i���{�ɭȳB�z
/*rmin,rmax R���{�ɭȡAgmin,gmax G���{�ɭȡAbmin,bmax B���{�ɭ�*/
void thresh_rgb(unsigned char image_in_rgb[3][Y_SIZE][X_SIZE], unsigned char image_out[Y_SIZE][X_SIZE], int rmin, int rmax, int gmin, int gmax, int bmin, int bmax) {
	int i, j;
	for (i = 0; i < Y_SIZE; i++) {
		for (j = 0; j < X_SIZE; j++) {
			if (image_in_rgb[0][i][j] < rmin)	image_out[i][j] = LOW;
			else if (image_in_rgb[0][i][j] > rmax)	image_out[i][j] = LOW;
			else if (image_in_rgb[1][i][j] < gmin) image_out[i][j] = LOW;
			else if (image_in_rgb[1][i][j] > gmax)	image_out[i][j] = LOW;
			else if(image_in_rgb[2][i][j] < bmin) image_out[i][j] = LOW;
			else if (image_in_rgb[2][i][j] > bmax) image_out[i][j] = LOW;
			else image_out[i][j] = HIGH;
		}
	}
}
//list9.3�Q�ΫG�סB���M�סB��۶i���{�ɭȳB�z
/*image_in_ysh��J�v���}�C(YSH��m)�Aimage_out��X�v���}�C(key)
	ymin,ymax �G��Y���{�ɭȽd����w0-255
	smin,smax ���M��S���{�ɭȽd����w0-255
	hmin,hmax ���H���{�ɭȽd����w-180-180(hmax���׫��w�]�i��b180��)
*/
void thresh_ysh(int image_in_ysh[3][Y_SIZE][X_SIZE], unsigned char image_out[Y_SIZE][X_SIZE], int ymin, int ymax, int smin, int smax, int hmin, int hmax) {
	int i, j;
	if (hmax > 180) {
		hmax -= 360;
		for (i = 0; i < Y_SIZE; i++) {
			for (j = 0; j < X_SIZE; j++) {
				if (image_in_ysh[0][i][j] < ymin)	image_out[i][j] = LOW;
				else if (image_in_ysh[0][i][j] > ymax)	image_out[i][j] = LOW;
				else if (image_in_ysh[1][i][j] <smin)	image_out[i][j] = LOW;
				else if (image_in_ysh[1][i][j] > smax)	image_out[i][j] = LOW;
				else if (   (image_in_ysh[2][i][j] <hmin) && (image_in_ysh[2][i][j]>hmax) )	image_out[i][j] = LOW;
				else image_out[i][j] = HIGH;
			}
		}
	}
	else {
		for (i = 0; i < Y_SIZE; i++) {
			for (j = 0; j < X_SIZE; j++) {
				if (image_in_ysh[0][i][j] < ymin)	image_out[i][j] = LOW;
				else if (image_in_ysh[0][i][j] > ymax)	image_out[i][j] = LOW;
				else if (image_in_ysh[1][i][j] <smin)	image_out[i][j] = LOW;
				else if (image_in_ysh[1][i][j] > smax)	image_out[i][j] = LOW;
				else if ((image_in_ysh[2][i][j] <hmin) && (image_in_ysh[2][i][j]>hmax))	image_out[i][j] = LOW;
				else image_out[i][j] = HIGH;
			}
		}
	}
}
//list9.4�Q�Φ�t���{�ɭȳB�z
/*thresh�{�ɭȡAtype�{�ɭȳB�z��k(1:�H��������,2:�H�񬰤���,3�H�Ŭ�����*/
void thresh_color_difference(unsigned char image_in_rgb[3][Y_SIZE][X_SIZE], unsigned char image_out[Y_SIZE][X_SIZE], int thresh, int type) {
	int i, j, d;
	for (i = 0; i < Y_SIZE; i++) {
		for (j = 0; j < X_SIZE; j++) {
			switch (type) {
			case 1:
				d = ((int)u = image_in_rgb[1][i][j] + (int)image_in_rgb[2][i][j]) / 2 - (int)image_in_rgb[0][i][j];
				if (d >= thresh) image_out[i][j] = 255;
				else image_out[i][j] = 0;
				break;
			case 2:
				d = ((int)u = image_in_rgb[2][i][j] + (int)image_in_rgb[0][i][j]) / 2 - (int)image_in_rgb[1][i][j];
				if (d >= thresh) image_out[i][j] = 255;
				else image_out[i][j] = 0;
				break;
			case 3:
				d = ((int)u = image_in_rgb[0][i][j] + (int)image_in_rgb[1][i][j]) / 2 - (int)image_in_rgb[2][i][j];
				if (d >= thresh) image_out[i][j] = 255;
				else image_out[i][j] = 0;
				break;
			default:
				break;
			}
		}
	}
}


















