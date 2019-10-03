#pragma once
#include "Params.h"
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
//list5.1�h�����T(���ʥ����k)
/*type���Ƥƽd��(3:3x3�����A4:4x4�d��..)*/
void smooth(unsigned char image_in[Y_SIZE][X_SIZE],unsigned char image_out[Y_SIZE][X_SIZE],int type) {
	int i, j, m, n, k, x, y;
	double sum, num;
	k = type / 2;
	num = (double)type*type;
	for (i = 0; i < Y_SIZE; i++) {
		for (j = 0; j < X_SIZE; j++) {
			sum = 0.0;
			for (m = -k; m <= k; m++) {
				for (n = -k; n <= k; n++) {
					y = i + m;
					x = j + n;
					if (y < 0) y = 0;
					if (x < 0) x = 0;
					if (y > Y_SIZE) y = Y_SIZE;
					if (x > X_SIZE) x = X_SIZE;
					sum += image_in[y][x];
				}
			}
			sum = sum / num;
			if (sum < 0) sum = 0;
			if (sum > 255) sum = 255;
			image_out[i][j] = (unsigned char)sum;
		}
	}
}
//list5.2���[�H�����T
/*level���T�h�Ū��̤j��*/
void noise_rand(unsigned char image_in[Y_SIZE][X_SIZE],unsigned char image_out[Y_SIZE][X_SIZE],int level) {
	int i, j;
	int data, noise;
	for (i = 0; i < Y_SIZE; i++) {
		for (j = 0; j < X_SIZE; j++) {
			noise = (int)((rand() / (double)RAND_MAX - 0.5)*level*2.0);
			data = image_in[i][j] + noise;
			if (data > 255) image_out[i][j] = 255;
			else if (data < 0) image_out[i][j] = 0;
			else image_out[i][j] = data;
		}
	}
}
//list5.3 �[�v�Y�Ʋ��ʥ����k
/*type�[�v�Y������1.2.3*/
//list5.4�����o�i��
/*c����*/
int median_value(unsigned char c[9]);//�D�o�E�ӹ�����������(�����)
void median(unsigned char image_in[Y_SIZE][X_SIZE], unsigned char image_out[Y_SIZE][X_SIZE]) {
	int i, j;
	unsigned char c[9];
	for (i = 1; i < Y_SIZE - 1; i++) {
		for (j = 1; j < X_SIZE - 1; j++) {
			c[0] = image_in[i-1][j-1];
			c[1] = image_in[i-1][j];
			c[2] = image_in[i-1][j+1];
			c[3] = image_in[i][j-1];
			c[4] = image_in[i][j];
			c[5] = image_in[i][j+1];
			c[6] = image_in[i+1][j-1];
			c[7] = image_in[i+1][j];
			c[8] = image_in[i+1][j+1];
			image_out[i][j] = median_value(c);
		}
	}
}
int median_value(unsigned char c[9]) {
	int i, j, buf;
	for (j = 0; j < 8; j++) {
		for (i = 0; i < 8; i++) {
			if (c[i + 1] < c[i]) {
				buf = c[i + 1];
				c[i + 1] = c[i];
				c[i] = buf;
			}
		}
	}
	return c[4];
}
//list5.5���[��i���T
/*number��i���T���ƶq�Alevel��i���T���̤j��*/
void noise_spike(unsigned char image_in[Y_SIZE][X_SIZE], unsigned char image_out[Y_SIZE][X_SIZE],int number, int level) {
	int i, x, y;
	int data, noise;
	for (i = 0; i < number; i++) {
		x = (int)((rand() / (double)RAND_MAX)*X_SIZE);
		y = (int)((rand() / (double)RAND_MAX)*Y_SIZE);
		noise=(int)((rand() / (double)RAND_MAX-0.5)*level*2.0);
		data = image_in[y][x] + noise;
		if (data > 255) image_out[y][x] = 255;
		else if (data < 0) image_out[y][x] = 0;
		else image_out[y][x] = data;
	}
}


//list5.9���Y�B�z(�G�ȼv��)
void erosion(unsigned char image_in[Y_SIZE][X_SIZE],unsigned char image_out[Y_SIZE][X_SIZE]) {
	int i, j;
	for (i = 1; i < Y_SIZE - 1;i++) {
		for (j = 1; j < X_SIZE - 1;j++) {
			image_out[i][j] = image_in[i][j];
			if (image_in[i - 1][j - 1] == LOW) image_out[i][j] = LOW;
			if (image_in[i - 1][j] == LOW) image_out[i][j] = LOW;
			if (image_in[i - 1][j + 1] == LOW) image_out[i][j] = LOW;
			if (image_in[i ][j - 1] == LOW) image_out[i][j] = LOW;
			if (image_in[i ][j + 1] == LOW) image_out[i][j] = LOW;
			if (image_in[i + 1][j - 1] == LOW) image_out[i][j] = LOW;
			if (image_in[i + 1][j ] == LOW) image_out[i][j] = LOW;
			if (image_in[i + 1][j + 1] == LOW) image_out[i][j] = LOW;
		}
	}
}



