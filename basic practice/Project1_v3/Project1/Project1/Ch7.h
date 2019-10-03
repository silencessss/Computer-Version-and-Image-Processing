#pragma once
#include "Params.h"
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
//list7.1�i��v�����Ҥ�
void labelset(unsigned char image[Y_SIZE][X_SIZE], int xs, int ys, int label);
/*���v�����[���Ҹ��X_image_label��X�v���}�C(�g���Ҥƪ��v��)_cnf���Ҫ��Ӽ�_buf�w�İ�*/
int labeling(unsigned char image_in[Y_SIZE][X_SIZE], unsigned char image_label[Y_SIZE][X_SIZE], int *cnt, char *buf) {
	int i, j, label;
	for (i = 0; i < Y_SIZE; i++) {
		for (j = 0; j < X_SIZE; j++) {
			image_label[i][j] = image_in[i][j];
		}
	}
	label = L_BASE;
	for (i = 0; i < Y_SIZE; i++) {
		for (j = 0; j < X_SIZE; j++) {
			if (image_label[i][j] == HIGH) {
				if (label >= HIGH) {
					sprintf(buf, "Error!");
					return -1;
				}
				labelset(image_label, j, i, label); label++;
			}
		}
		*cnt = label - L_BASE;
		sprintf(buf, "number of labels:%d", *cnt);
		return 0;
	}
}
/*��۳s�����������[���Ҹ��X_xs,ys�_�l��m_label���ҧǸ�*/
void labelset(unsigned char image[Y_SIZE][X_SIZE], int xs, int ys, int label) {
	int i, j, cnt, im, ip, jm, jp;
	image[ys][xs] = label;
	for (;;) {
		cnt = 0;
		for (i = 0; i < Y_SIZE; i++) {
			for (j = 0; j < X_SIZE; j++) {
				if (image[i][j] == label) {
					im = i - 1; ip = i + 1; jm = j - 1; jp = j + 1;
					if (im < 0) im = 0; if (ip >= Y_SIZE) ip = Y_SIZE - 1;
					if (jm < 0) jm = 0; if (jp >= X_SIZE) jp = X_SIZE - 1;
					if (image[i][jp] == HIGH) {
						image[i][jp] = label; cnt++;
					}
					if (image[im][jp] == HIGH) {
						image[im][jp] = label; cnt++;
					}
					if (image[im][j] == HIGH) {
						image[im][j] = label; cnt++;
					}
					if (image[im][jm] == HIGH) {
						image[im][jm] = label; cnt++;
					}
					if (image[i][jm] == HIGH) {
						image[i][jm] = label; cnt++;
					}
					if (image[ip][jm] == HIGH) {
						image[ip][jm] = label; cnt++;
					}
					if (image[ip][j] == HIGH) {
						image[ip][j] = label; cnt++;
					}
					if (image[ip][jp] == HIGH) {
						image[ip][jp] = label; cnt++;
					}
				}
				if (cnt == 0) break;
			}
		}
	}
}
//list7.2�ˬd�S�x�Ѽ�
double calc_size(unsigned char image_label[Y_SIZE][X_SIZE], int label, int *cx, int *cy);
double calc_length(unsigned char image_label[Y_SIZE][X_SIZE], int label);
double trace(unsigned char iamge_label[Y_SIZE][X_SIZE], int xs, int ys);
void featrues(unsigned char image_label_in[Y_SIZE][X_SIZE], unsigned char image_label_out[Y_SIZE][X_SIZE], int cnt, double size[], double ratio[], char *buf) {
	int i, j, center_x, center_y;
	double l;
	int posi, m;
	posi = 0;
	for (i = 0; i < Y_SIZE; i++) {
		for (j = 0; j < X_SIZE; j++) {
			image_label_out[i][j] = image_label_in[i][j];
		}
	}
	m = sprintf(buf, "no area circum round grav(x,y)\n");
	posi += m;
	for (i = 0; i < cnt; i++) {
		size[i] = calc_size(image_label_out, i + L_BASE, &center_x, &center_y);
		l = calc_length(image_label_out, i + L_BASE);
		ratio[i] = (4 * PI*size[i] )/ (l*l);
		image_label_out[center_y][center_x] = HIGH;
		m = sprintf(&buf[posi], "%3d %6d %8.2f %8.4f (%3d,%3d)\n", i, (int)size[i], l, ratio[i], center_x, center_y);
		posi += m;
	}
}
/*�D���n�B���ߦ�m�Alabel���ҧǸ��Acx,cy���ߦ�m*/
double calc_size(unsigned char image_label[Y_SIZE][X_SIZE], int label, int *cx, int *cy) {
	int i, j;
	double tx, ty, total;
	tx = 0; ty = 0; total = 0;
	for (i = 0; i < Y_SIZE; i++) {
		for (j = 0; j < X_SIZE; j++) {
			if (image_label[i][j] == label) {
				tx += j; ty += i; total++;
			}
		}
	}
	if (total == 0.0) return 0.0;
	*cx = (int)(tx / total); *cy = (int)(ty / total);
	return total;
}
/*�D�P��*/
/*label���ҧǸ�*/
double calc_length(unsigned char image_label[Y_SIZE][X_SIZE], int label) {
	int i, j;
	double trace();
	for (i = 0; i < Y_SIZE; i++) {
		for (j = 0; j < X_SIZE; j++) {
			if (image_label[i][j] == label) {
				return trace(image_label, j - 1, i);
			}
		}
	}
	return 0;
}
/*�l�ܽ����u�Axs,ys�_�l��m*/
double trace(unsigned char image_label[Y_SIZE][X_SIZE], int xs, int ys) {
	int x, y, no, vec;
	double l;
	l = 0; x = xs; y = ys; no = image_label[y][x + 1]; vec = 5;
	for (;;) {
		if (x == xs && y == ys&&l != 0) return l;
		image_label[y][x] = HIGH;
		switch (vec) {
		case 3:
			if (image_label[y][x + 1] != no&&image_label[y - 1][x + 1] == no) {
				x = x + 1; y = y; l++; vec = 0; continue;
			}
		case 4:
			if (image_label[y-1][x + 1] != no&&image_label[y - 1][x + 1] == no) {
				x = x + 1; y = y - 1; l += ROOT2; vec = 1; continue;
			}
		case 5:
			if (image_label[y-1][x ] != no&&image_label[y - 1][x-1 ] == no) {
				x = x ; y = y-1; l++; vec = 2; continue;
			}
		case 6:
			if (image_label[y-1][x -1] != no&&image_label[y ][x - 1] == no) {
				x = x - 1; y = y-1; l+=ROOT2; vec = 3; continue;
			}
		case 7:
			if (image_label[y][x - 1] != no&&image_label[y + 1][x - 1] == no) {
				x = x - 1; y = y; l++; vec = 4; continue;
			}
		case 0:
			if (image_label[y+1][x - 1] != no&&image_label[y + 1][x ] == no) {
				x = x - 1; y = y+1; l+=ROOT2; vec = 5; continue;
			}
		case 1:
			if (image_label[y+1][x] != no&&image_label[y + 1][x + 1] == no) {
				x = x; y = y+1; l++; vec = 6; continue;
			}
		case 2:
			if (image_label[y+1][x + 1] != no&&image_label[y ][x + 1] == no) {
				x = x + 1; y = y+1; l+=ROOT2; vec = 7; continue;
			}
			vec = 3;
		}
	}
}

//list7.3���άY�d���Ҷ�׳s���������v��
/*cnt�s���������ӼơAratio�u��סAratio_min,ratio_max�̤p�ȳ̤j��*/
void extract_ratio(unsigned char image_label_in[Y_SIZE][X_SIZE], unsigned char image_label_out[Y_SIZE][X_SIZE], int cnt, double ratio[], double ratio_min, double ratio_max) {
	int i, j, x, y;
	int lno[256];
	for (i = 0, j = 0; i < cnt; i++) {
		if (ratio[i] >= ratio_min && ratio[i] <= ratio_max) {
			lno[j++] = L_BASE + i;
		}
	}
	for (y = 0; y < Y_SIZE; y++) {
		for (x = 0; x < X_SIZE; x++) {
			image_label_out[y][x] = 0;
			for (i = 0; i < j; i++) {
				if (image_label_out[y][x] = lno[i]) {
					image_label_out[y][x] = image_label_in[y][x];
				}
			}
		}
	}
}
//list7.4�u�ƻs�B�n�ϰ쪺�v�����
/*image_mask�B�n�v���}�C(�G�ȼv��)*/
void masking(unsigned char image_in[Y_SIZE][X_SIZE], unsigned char image_out[Y_SIZE][X_SIZE], unsigned char image_mask[Y_SIZE][X_SIZE]) {
	int i, j;
	for (i = 0; i < Y_SIZE; i++) {
		for (j = 0; j < X_SIZE; j++) {
			if (image_mask[i][j] == HIGH) {
				image_out[i][j] = image_in[i][j];
			}
			else {
				image_out[i][j] = 0;
			}
		}
	}
}
//list7.5���Φb�Y�ӽd�򤺭��n���s������
/*cnt�s���������ӼơAsize���n*/
void extract_size(unsigned char image_label_in[Y_SIZE][X_SIZE], unsigned char image_label_out[Y_SIZE][X_SIZE], int cnt, double size[], double size_min, double size_max) {
	int i, j, x, y;
	int lno[256];
	for (i = 0, j = 0; i < cnt; i++) {
		if (size[i] >= size_min && size[i] <= size_max) {
			lno[j++] = L_BASE;
		}
	}
	for (y = 0; y < Y_SIZE; y++) {
		for (x = 0; x < X_SIZE; x++) {
			image_label_out[y][x] = 0;
			for (i = 0; i < j; i++) {
				if (image_label_in[y][x] = lno[i]) {
					image_label_out[y][x] = image_label_in[y][x];
				}
			}
		}
	}
}

