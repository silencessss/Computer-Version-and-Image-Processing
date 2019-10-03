#pragma once
#include "Params.h"
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
void calc_ave_devi(int *pdata, int n, double *pave, double *pdevi);
void calc_corre(int *pdata1, double ave1, double devi1, int *pdata2, double ave2, double devi2, int n, double *pcorre);
/*image_l左邊的影像，image_r右邊的影像，image_d視差的影像，bsize方格大小*/
void stereo_diff(unsigned char image_l[Y_SIZE][X_SIZE], unsigned char image_r[Y_SIZE][X_SIZE], unsigned char image_d[Y_SIZE][X_SIZE], int bsize) {
	int i, j, dx, ddx;
	int x, y, xl, yl, xr, yr;
	double d, min;
	for (i = 0; i < Y_SIZE; i++) {
		for (j = 0; j < X_SIZE; j++) {
			for (dx = 0; dx <= DXMAX; dx++) {
				d = 0;
				for (y = -bsize / 2; y <= bsize / 2; y++) {
					yl = i + y; yr = yl;
					if (yl < 0) {
						yl = 0;
					}
					if (yl > Y_SIZE - 1) {
						yl = Y_SIZE - 1;
					}
					for (x = -bsize / 2; x <= bsize / 2; x++) {
						xl = j + x;
						if (xl < 0) {
							xl = 0;
						}
						if (xl > X_SIZE - 1) {
							xl = X_SIZE - 1;
						}
						xr = xl - dx;
						if (xr < 0) {
							xr = 0;
						}
						if (xr > X_SIZE - 1) {
							xr = X_SIZE - 1;
						}
						d += abs(image_r[yr][xr] - image_l[yl][xl]);
					}
				}
				if (dx == 0) {
					min = d;
					ddx = dx;
				}
				if (d < min) {
					min = d;
					ddx = dx;
				}
			}
			image_d[i][j] = ddx;
		}
	}
}

void calc_ave_devi(int *pdata, int n, double *pave, double *pdevi) {
	int i;
	int *p;
	double sum;
	sum = 0; p = pdata;
	for (i = 0; i < n; i++) {
		sum += *p++;
	}
	*pave = sum / n;
	sum = 0; p = pdata;
	for (i = 0; i < n; i++) {
		sum += (*p - *pave) * (*p - *pave);
		p++;
	}
	*pdevi = sqrt(sum);
}
/*pdata輸入資料，ave平均值，devi離散值，n輸入資料數，pcorre相關係數*/
void calc_corre(int *pdata1, double ave1, double devi1, int *pdata2, double ave2, double devi2, int n, double *pcorre) {
	int i;
	int *p1, *p2;
	double sum;
	sum = 0;
	p1 = pdata1; p2 = pdata2;
	for (i = 0; i < n; i++) {
		sum += (*p1++ - ave1) * (*p2++ - ave2);
	}
	*pcorre = sum / (devi1 * devi2);
	if (*pcorre < 0) *pcorre = -*pcorre;
}

