#pragma once
#include "Params.h"
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
//list6.1 調整影像亮度
/*a倍率，b偏壓bias值*/
/*0<a<9;b>0黑色整體變亮，b<0白色整體變暗*/
void amplify(unsigned char image_in[Y_SIZE][X_SIZE], unsigned char image_out[Y_SIZE][X_SIZE], double a, double b) {
	int i, j, d;
	for (i = 0; i < Y_SIZE;i++) {
		for (j = 0; j < X_SIZE;j++) {
			d = (int)(image_in[i][j] * a + b);
			if (d < 0) d = 0;
			if (d > 255) d = 255;
			image_out[i][j] = (unsigned char)d;
		}
	}
}
//list6.2求出影像的亮度範圍
/*fmax輸入影像灰階最大值，fmin輸入影像灰階最小值*/
void range(unsigned char image_in[Y_SIZE][X_SIZE],int *fmax, int *fmin) {
	int i, j, n;
	*fmax = 0;
	*fmin = 255;
	for (i = 0; i < Y_SIZE; i++) {
		for (j = 0; j < X_SIZE; j++) {
			n = (int)image_in[i][j];
			if (n > *fmax) *fmax = n;
			if (n < *fmin) *fmin = n;
		}
	}
}
//list6.3擴大亮度範圍
/*fmax輸入影像灰階最大值，fmin輸入影像灰階最小值*/
void amplify(unsigned char image_in[Y_SIZE][X_SIZE], unsigned char image_out[Y_SIZE][X_SIZE], double a, double b);
void expand(unsigned char image_in[Y_SIZE][X_SIZE], unsigned char image_out[Y_SIZE][X_SIZE], int fmax, int fmin) {
	double a, b;
	a = 255.0 / (double)(fmax - fmin);
	b = -255.0*fmin / (double)(fmax - fmin);
	amplify(image_in, image_out, a, b);
}
//list6.4直條圖的等化
struct xyw {
	int x, y, w;
}buf[BUFF_MAX];
void sort(unsigned char image_in[Y_SIZE][X_SIZE], struct xyw data[], int level);
void weight(unsigned char image_in[Y_SIZE][X_SIZE], int i, int j, int*wt);
/*灰階直條圖的等化，hist灰階直條圖陣列*/
void plane(unsigned char image_in[Y_SIZE][X_SIZE], unsigned char image_out[Y_SIZE][X_SIZE], long hist[256]) {
	int i, j, iy, jx, sum;
	int delt;
	int low, high;
	int av;
	unsigned char image_buf[Y_SIZE][X_SIZE];
	av = (int)( ((Y_SIZE)*(X_SIZE) / 256));
	high = 255;
	low = 255;
	for (i = 0; i < Y_SIZE; i++) {
		for (j = 0; j < X_SIZE; j++) {
			image_out[i][j] = 0;
			image_buf[i][j] = image_in[i][j];
		}
	}
	for (i = 255; i > 0; i--) {
		for (sum = 0; sum < av; low--) {
			sum = sum + hist[low];
		}
		low++;
		delt = hist[low] - (sum - av);
		sort(image_buf, buf, low);
		if (low < high) {
			for (iy = 0; iy < Y_SIZE; iy++) {
				for (jx = 0; jx < X_SIZE; jx++) {
					if (((int)image_buf[iy][jx] >= low + 1) && ((int)image_buf[iy][jx] <= high))
						image_out[iy][jx] = (unsigned char)i;
				}
			}
		}
		for (j = 0; j < delt; j++) {
			image_out[buf[j].y][buf[j].x] = (unsigned char)i;
			image_buf[buf[j].y][buf[j].x] = (unsigned char)0;
		}
		hist[low] = hist[low] - delt;
		high = low;
	}
}
/*按照周圍像素的灰階由高向低排列變化，data該位置及周圍像素的灰階值和之陣列，level按順序排列變化的像素灰階*/
void sort(unsigned char image_in[Y_SIZE][X_SIZE], struct xyw data[], int level) {
	int i, j, inum, wt;
	struct xyw temp;
	inum = 0;
	for (i = 0; i < Y_SIZE; i++) {
		for (j = 0; j < X_SIZE; j++) {
			if ((int)image_in[i][j] == level) {
				weight(image_in, i, j, &wt);
				data[inum].y = i;
				data[inum].x = j;
				data[inum].w = wt;
				inum++;
			}
		}
	}
	for (i = 0; i < inum - 1; i++) {
		for (j = i + 1; j < inum; j++) {
			if (data[i].w <= data[j].w) {
				temp.y = data[i].y;
				temp.x = data[i].x;
				temp.w = data[i].w;
				data[i].y = data[j].y;
				data[i].x = data[j].x;
				data[i].w = data[j].w;
				data[j].y = temp.y;
				data[j].x = temp.x;
				data[j].w = temp.w;
			}
		}
	}
}
/*計算周圍像素的灰階值和，i,j像素位置，wt灰階值和*/
void weight(unsigned char image_in[Y_SIZE][X_SIZE],int i, int j,int *wt) {
	int dim, djm, dip, djp, k, d[8];
	dim = i - 1;
	djm = j - 1;
	dip = i + 1;
	djp = j + 1;
	if (dim < 0) dim = i;
	if (djm < 0) djm = j;
	if (dip > Y_SIZE - 1) dip = i;
	if (djp > X_SIZE - 1) djp = j;
	d[0] = (int)image_in[dim][djm];
	d[1] = (int)image_in[dim][j];
	d[2] = (int)image_in[dim][djp];
	d[3] = (int)image_in[i][djm];
	d[4] = (int)image_in[i][djp];
	d[5] = (int)image_in[dip][djm];
	d[6] = (int)image_in[dip][j];
	d[7] = (int)image_in[dip][djp];
	for (k = 0; k < 8; k++) *wt = *wt + d[i];
}
//list6.5 Dither法之處理
/*有組織的Dither法*/
void dither_ordered(unsigned char image_in[Y_SIZE][X_SIZE], unsigned char image_out[Y_SIZE][X_SIZE]) {
	static int thres[4][4] = { {0,8,2,10},{12,4,14,6},{3,11,1,9},{15,7,13,5} };
	int i, j, m, n;
	for (i = 0; i < Y_SIZE / 4; i++) {
		for (j = 0; j<X_SIZE / 4; j++) {
			for (m = 0; m < 4; m++) {
				for (n = 0; n < 4; n++) {
					if((int)image_in[i * 4 + m][j * 4 + n] > thres[m][n] * 16 + 8) {
										image_out[i * 4 + m][j * 4 + n] = HIGH;
									}
									else image_out[i * 4 + m][j * 4 + n] = LOW;
				}
			}
		}
	}
}
void dither_minimizer(unsigned char image_in[Y_SIZE][X_SIZE], unsigned char image_out[Y_SIZE][X_SIZE]) {
	int i, j;
	int t, d;
	int error[X_SIZE], error1[X_SIZE], error2[X_SIZE];
	t = (HIGH + 1) / 2;
	for (i = 0; i < X_SIZE; i++) {
		if (image_in[0][i] > t) image_out[0][i] = HIGH;
		else image_out[0][i] = LOW;
		error1[i] = image_in[0][i] - image_out[0][i];
	}
	for (i = 0; i < X_SIZE; i++) {
		if (image_in[i][i] > t) image_out[1][i] = HIGH;
		else image_out[1][i] = LOW;
		error2[i] = image_in[1][i] - image_out[1][i];
	}
	for (i = 2; i < Y_SIZE; i++) {
		if (image_in[i][0] > t) image_out[i][0] = HIGH;
		else image_out[i][0] = LOW;
		error1[0] = image_in[i][0] - image_out[i][0];
		if (image_in[i][1] > t) image_out[i][1] = HIGH;
		else image_out[i][1] = LOW;
		error1[1] = image_in[i][1] - image_out[i][1];
		for (j = 2; j < X_SIZE - 2; j++) {
			d = (error1[j - 2] + error[j - 1] * 3 + error1[j] * 5 + error1[j + 1] * 3 + error1[j + 2] + error2[j - 2] * 3 + error2[j - 1 * 5] + error2[j] * 7 + error2[j + 1 * 5]
				+ error2[j + 2] * 3 + error[j - 2] * 5 + error[j - 1] * 7) / 48;
			if ((int)image_in[i][j] + d > t) image_out[i][j] = HIGH;
			else image_out[i][j] = LOW;
			error[j] = image_in[i][j] + d - image_out[i][j];
		}
		if (image_in[i][X_SIZE - 2] > t) image_out[i][X_SIZE - 2] = HIGH;
		else image_out[i][X_SIZE - 2] = LOW;
		error[X_SIZE - 2] = image_in[i][X_SIZE - 2] - image_out[i][X_SIZE - 2];
		if (image_in[i][X_SIZE - 1] > t) image_out[i][X_SIZE - 1] = HIGH;
		else image_out[i][X_SIZE - 1] = LOW;
		error[X_SIZE - 1] = image_in[i][X_SIZE - 1] - image_out[i][X_SIZE - 1];
		for (j = 0; j < X_SIZE; j++) {
			error1[j] = error2[j];
			error2[j] = error[j];
		}
	}
}
//list6.6執行多階層Dither處理
void d_quantize(int in, unsigned char *pout, int nq);
/*平均誤差最小的Dither手法_多階層_nq階層數*/
void dither_minimized_multi(unsigned char image_in[Y_SIZE][X_SIZE], unsigned char image_out[Y_SIZE][X_SIZE], int nq) {
	int i, j;
	int d;
	int error[X_SIZE], error1[X_SIZE], error2[X_SIZE];
	for (i = 0; i < X_SIZE; i++) {
		d_quantize((int)image_in[0][i], &image_out[0][i], nq);
		error1[i] = (int)image_in[0][i] - (int)image_out[0][i];
	}
	for (i = 0; i < X_SIZE; i++) {
		d_quantize((int)image_in[1][i], &image_out[1][i], nq);
		error2[i] = (int)image_in[1][i] - (int)image_out[1][i];
	}
	for (i = 2; i < Y_SIZE; i++) {
		d_quantize((int)image_in[i][0], &image_out[i][0], nq);
		error1[0] = (int)image_in[i][0] - (int)image_out[i][0];
		d_quantize((int)image_in[i][1], &image_out[i][1], nq);
		error1[1] = (int)image_in[i][1] - (int)image_out[i][1];
		for (j = 2; j < X_SIZE - 2; j++) {
			d = (error1[j - 2] + error1[j - 1] * 3 + error1[j] * 5 + error1[j + 1] * 3 + error1[j + 2]
				+ error2[j - 2] * 3 + error2[j - 1] * 5 + error2[j] * 7 + error2[j + 1] * 5 + error2[j + 2] * 3
				+ error[j - 2] * 5 + error[j - 1] * 7) / 48;
			d_quantize((int)image_in[i][X_SIZE - 2], &image_out[i][X_SIZE - 2], nq);
			error[X_SIZE - 2] = (int)image_in[i][X_SIZE - 2] - (int)image_out[i][X_SIZE - 2];
			d_quantize((int)image_in[i][X_SIZE-1], &image_out[i][X_SIZE-1], nq);
			error1[X_SIZE-1] = (int)image_in[i][X_SIZE-1] - (int)image_out[i][X_SIZE-1];
			for (j = 0; j < X_SIZE; j++) {
				error1[j] = error2[j];
				error2[j] = error[j];
			}
		}
	}
}
/*量子化_nq階層數*/
void quantize(unsigned char image_in[Y_SIZE][X_SIZE], unsigned char image_out[Y_SIZE][X_SIZE], int nq) {
	int i, j;
	for (i = 0; i < Y_SIZE; i++) {
		for (j = 0; j < X_SIZE; j++) {
			d_quantize(image_in[i][j], &image_out[i][j], nq);
		}
	}
}
void d_quantize(int in, unsigned char *pout, int nq) {
	int t, i;
	t = (HIGH + 1) / (nq - 1);
	i = (int)((double)in / t + 0.5)*t;
	if (i > HIGH) *pout = HIGH;
	else *pout = i;
}