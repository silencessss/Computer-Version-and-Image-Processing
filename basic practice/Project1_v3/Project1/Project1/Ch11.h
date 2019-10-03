#pragma once
#include "Params.h"
#include <math.h>
#include <stdio.h>
#include <stdlib.h>

void fft1core(double a_rl[], double a_im[], int length, int ex, double sin_tbl[], double cos_tbl[], double buf[]);
void cstb(int length, int inv, double sin_tbl[], double cos_tbl[]);
void birv(double a[], int length, int ex, double b[]);
//執行一維FFT變換
int fft1(double a_rl[], double a_im[], int ex, int inv){
	int i, length = 1;
	double *sin_tbl, *cos_tbl, *buf;
	
	for (i = 0; i < ex; i++) length *= 2;/*資料個數計算*/
	/*使用malloc()函式進行動態被置空間，在整個程式結束前不會自動歸還給記憶體，必須使用free()函式將空間還給記憶體*/
	sin_tbl = (double *)malloc( (size_t)length * sizeof(double)  );
	cos_tbl = (double *)malloc( (size_t)length * sizeof(double)  );
	buf = (double *)malloc((size_t)length * sizeof(double));
	if ((sin_tbl == NULL) || (cos_tbl == NULL) || (buf == NULL))return -1;
	cstb(length, inv, sin_tbl, cos_tbl);
	fft1core(a_rl, a_im, length, ex, sin_tbl, cos_tbl, buf);
	free(sin_tbl);
	free(cos_tbl);
	return 0;
}
/*一維FFT的計算核心部分，a_rl資料時數部(輸出輸入兼用)，a_im資料虛數部(輸出輸入兼用)，ex給予的資料個數為2的乘冪數(資料個數=2的ex乘冪)*/
/*sin_tbl SIN計算用表，cos_tbl COS計算用表*/
void fft1core(double a_rl[], double a_im[], int length, int ex, double sin_tbl[], double cos_tbl[], double buf[]) {
	int i, j, k, w, j1, j2;
	int numb, lenb, timb;
	double xr, xi, yr, yi, nrml;

	if (OPT == 1) {
		for (i = 1; i < length; i += 2) {
			a_rl[i] = -a_rl[i];
			a_im[i] = -a_im[i];
		}
	}
	numb = 1;
	lenb = length;
	for (i = 0; i < ex; i++) {
		lenb /= 2;
		timb = 0;
		for (k = 0; j < numb; j++) {
			w = 0;
			for (k = 0; k < lenb; k++) {
				j1 = timb + k;
				j2 = j1 + lenb;
				xr = a_rl[j1];
				xi = a_im[j1];
				yr = a_rl[j2];
				yi = a_im[j2];
				a_rl[j1] = xr + yr;
				a_im[j1] = xi + yi;
				xr = xr - yr;
				xi = xi - yi;
				a_rl[j2] = xr*cos_tbl[w] - xi*sin_tbl[w];
				a_im[j2] = xr*sin_tbl[w] + xi*cos_tbl[w];
				w = w + numb;
			}
			timb += (2 * lenb);
		}
		numb *= 2;
	}
	birv(a_rl, length, ex, buf);/*實數資料的兌換*/
	birv(a_im, length, ex, buf);/*虛數資料的兌換*/
	if (OPT == 1) {
		for (i = 1; i < length; i += 2) {
			a_rl[i] = -a_rl[i];
			a_im[i] = -a_im[i];
		}
	}
	nrml = 1.0 / sqrt((double)length);
	for (i = 0; i < length; i++) {
		a_rl[i] *= nrml;
		a_im[i] *= nrml;
	}
}
/*SIN、COS表的製作，length資料個數，inv(1:DFT，-1:逆DFT*/
void cstb(int length, int inv, double sin_tbl[], double cos_tbl[]) {
	int i;
	double xx, arg;
	xx = -PI*2.0 / (double)length;
	if (inv < 0) xx = -xx;
	for (i = 0; i < length; i++) {
		arg = i*xx;
		sin_tbl[i] = sin(arg);
		cos_tbl[i] = cos(arg);
	}
}
/*資料的替換，a資料陣列，length資料個數，b作業緩衝區*/
void birv(double a[], int length, int ex, double b[]) {
	int i, ii, k, bit;
	for (i = 0; i < length; i++) {
		for (k = 0, ii = i, bit = 0;; bit <<= 1, ii >>= 1) {
			bit = (ii & 1) | bit;
			if (++k == ex) break;
		}
		b[i] = a[bit];
	}
	for (i = 0; i < length; i++) a[i] = b[i];
}
//list11.2 執行二維FFT變換
void fft1core(double a_rl[], double a_im[], int length, int ex, double sin_tbl[], double cos_tbl[], double buf[]);
void cstb(int length, int inv, double sin_tbl[], double cos_tbl[]);
void rvmtx1(double a[Y_SIZE][X_SIZE], double b[X_SIZE][Y_SIZE], int xsize, int ysize);
void rvmtx2(double a[X_SIZE][Y_SIZE], double b[Y_SIZE][X_SIZE], int xsize, int ysize);
int fft2(double a_rl[Y_SIZE][X_SIZE], double a_im[Y_SIZE][X_SIZE], int inv) {
	double *b_rl, *b_im;/*資料轉置作業用資料陣列(實數部、虛數部)*/
	double *hsin_tbl, *hcos_tbl;/*水平用SIN、COS計算用表*/
	double *vsin_tbl, *vcos_tbl;/*垂直用SIN、COS計算用表*/
	double *buf_x, *buf_y;/*作業用緩衝區(水平、垂直)*/
	int i;
	/*使用calloc()來宣告空間配置，而此得來的空間在不使用時，應用free()釋放*/
	b_rl = (double *)calloc((size_t)X_SIZE*Y_SIZE, sizeof(double));
	b_im = (double *)calloc((size_t)X_SIZE*Y_SIZE, sizeof(double));
	hsin_tbl = (double *)calloc((size_t)X_SIZE, sizeof(double));
	hcos_tbl = (double *)calloc((size_t)X_SIZE, sizeof(double));
	vsin_tbl = (double *)calloc((size_t)Y_SIZE, sizeof(double));
	vcos_tbl = (double *)calloc((size_t)Y_SIZE, sizeof(double));
	buf_x = (double *)malloc((size_t)X_SIZE * sizeof(double));
	buf_y = (double *)malloc((size_t)X_SIZE * sizeof(double));

	if ((b_rl == NULL) || (b_im == NULL) || (hsin_tbl == NULL) || (hcos_tbl == NULL) || (vsin_tbl == NULL) || (vcos_tbl == NULL) || (buf_x == NULL) || (buf_y == NULL)) return -1;

	cstb(X_SIZE, inv, hsin_tbl, hcos_tbl);/*水平用SIN、COS表製作*/
	cstb(Y_SIZE, inv, vsin_tbl, vcos_tbl);/*垂直用SIN、COS表製作*/
	/*水平方向的FFT*/
	for (i = 0; i < Y_SIZE; i++) {
		fft1core(&a_rl[(long)i][0], &a_im[(long)i][0], X_SIZE, X_EXP, hsin_tbl, hcos_tbl, hcos_tbl, buf_x);
	}
	/*二維資料的轉置*/
	rvmtx1((double(*)[X_SIZE])a_rl, (double(*)[X_SIZE])b_rl, X_SIZE, Y_SIZE);
	rvmtx1((double(*)[X_SIZE])a_im, (double(*)[X_SIZE])b_im, X_SIZE, Y_SIZE);
	/*垂直方向的FFT*/
	for (i = 0; i < X_SIZE; i++) {
		fft1core(&b_rl[(long)Y_SIZE*i], &b_im[(long)Y_SIZE*i], Y_SIZE, Y_EXP, vsin_tbl, vcos_tbl, buf_y);
	}
	/*二維資料的轉置*/
	rvmtx2((double(*)[Y_SIZE])b_rl, (double(*)[Y_SIZE])a_rl, X_SIZE, Y_SIZE);
	rvmtx2((double(*)[Y_SIZE])b_im, (double(*)[Y_SIZE])a_im, X_SIZE, Y_SIZE);
	free(b_rl);
	free(b_im);
	free(hsin_tbl);
	free(hcos_tbl);
	free(vsin_tbl);
	free(vcos_tbl);
	return 0;
}
	/*二維資料轉置*/
	/*a二維輸入資料，b二維輸出資料，xsize水平資料個數，ysize垂直資料個數*/
	void rvmtx1(double a[Y_SIZE][X_SIZE], double b[X_SIZE][Y_SIZE], int xsize, int ysize) {
		int i, j;
		for (i = 0; i < ysize; i++) {
			for (j = 0; j < xsize; j++) {
				b[j][i] = a[i][j];
			}
		}
	}
	void rvmtx2(double a[X_SIZE][Y_SIZE], double b[Y_SIZE][X_SIZE], int xsize, int ysize) {
		int i, j;
		for (i = 0; i < ysize; i++) {
			for (j = 0; j < xsize; j++) {
				b[i][j] = a[j][i];
			}
		}
	}


/*將二維FFT的結果影像化*/
int fft2(double a_rl[Y_SIZE][X_SIZE], double a_im[Y_SIZE][X_SIZE], int inv);
int fftimage(unsigned char image_in[Y_SIZE][X_SIZE], unsigned char image_out[Y_SIZE][X_SIZE]) {
	double *ar;
	double *ai;
	double norm, max;
	double data;
	long i, j;

	ar = (double *)malloc((size_t)Y_SIZE*X_SIZE * sizeof(double));
	ai = (double *)malloc((size_t)Y_SIZE*X_SIZE * sizeof(double));
	if ((ar == NULL) || (ai == NULL)) return -1;
	/*讀入原影像，並轉換為二維FFT的輸入資料*/
	for (i = 0; i < Y_SIZE; i++) {
		for (j = 0; j < X_SIZE; j++) {
			ar[X_SIZE*i + j] = (double)image_in[i][j];
			ai[X_SIZE*i + j] = 0.0;
		}
	}
	/*執行二維FFT*/
	if ( fft2( ( double(*)[X_SIZE])ar, (double(*)[X_SIZE])ai, 1) == -1)  return -1;
	/*將FFT結果影像化*/
	max = 0;
	for (i = 0; i < Y_SIZE; i++) {
		for (j = 0; j < X_SIZE; j++) {
			norm = ar[X_SIZE*i + j] * ar[X_SIZE*i + j] + ai[X_SIZE*i + j] * ai[X_SIZE*i + j];/*振幅成分計算*/
			if (norm != 0.0) norm = long(norm) / 2.0;
			else norm = 0.0;
			ar[X_SIZE*i + j] = norm;
			if (norm > max) max = norm;
		}
	}
	for (i = 0; i < Y_SIZE; i++) {
		for (j = 0; j < X_SIZE; j++) {
			ar[X_SIZE*i + j] = ar[X_SIZE*i + j] * 255 / max;
		}
	}
	/*把FFT的結果變換為影像資料*/
	for (i = 0; i < Y_SIZE; i++) {
		for (j = 0; j < X_SIZE; j++) {
			data = ar[X_SIZE*i + j];
			if (data > 255) data = 255;
			if (data < 0) data = 0;
			image_out[i][j] = (unsigned char)data;
		}
	}
	free(ar);
	free(ai);
	return 0;
}

//list11.4利用二維FFT執行濾波處理
int fft2(double a_rl[Y_SIZE][X_SIZE], double a_im[Y_SIZE][X_SIZE], int inv);
int fftfilter(unsigned char image_in[Y_SIZE][X_SIZE], unsigned char image_out[Y_SIZE][X_SIZE], int a, int b) {
	double *ar; /*資料實數部(輸入輸出)*/
	double *ai;/*資料虛數部(輸入輸出)*/
	double *ff;/*濾波器的空間頻率特性*/
	double data;
	long i, j, circ;

	ar = (double *)malloc((size_t)Y_SIZE*X_SIZE * sizeof(double));
	ai = (double *)malloc((size_t)Y_SIZE*X_SIZE * sizeof(double));
	ff = (double *)malloc((size_t)Y_SIZE*X_SIZE * sizeof(double));
	if ((ar == NULL) || (ai == NULL) || (ff == NULL)) return -1;
	/*讀入原影像時，變換為二為FFT的輸入資料*/
	for (i = 0; i < Y_SIZE; i++) {
		for (j = 0; j < X_SIZE; j++) {
			ar[X_SIZE*i + j] = (double)image_in[i][j];
			ai[X_SIZE*i + j] = 0.0;
		}
	}
	/*執行FFT，把原影像變換為頻率成分*/
	if (fft2((double(*)[X_SIZE])ar, (double(*)[X_SIZE])ai, 1) == -1) return -1;
	/*製作出只讓頻率為a以上b以下的成分通過的濾波器*/
	for (i = 0; i < Y_SIZE; i++) {
		for (j = 0; j < X_SIZE; j++) {
			data = (double)((j - X_SIZE / 2)*(j - X_SIZE / 2) + (i - Y_SIZE / 2)*(i - Y_SIZE / 2));
			circ = (long)sqrt(data);
			if ((circ >= a) && (circ <= b)) ff[X_SIZE*i + j] = 1.0;
			else ff[X_SIZE*i + j] = 0.0;
		}
	}
	/*原影像的頻率成分執行率波處理*/
	for (i = 0; i < Y_SIZE; i++) {
		for (j = 0; j < X_SIZE; j++) {
			ar[X_SIZE*i + j] *= ff[X_SIZE*i + j];
			ai[X_SIZE*i + j] *= ff[X_SIZE*i + j];
		}
	}
	/*執行逆向FFT變換，讓綠波處理過的頻率成分回復成影像*/
	if (fft2((double(*)[X_SIZE])ar, (double(*)[X_SIZE])ai, -1) == -1) return -1;
	/*將結果變為影像資料*/
	for (i = 0; i < Y_SIZE; i++) {
		for (j = 0; j < X_SIZE; j++) {
			data = ar[X_SIZE*i + j];
			if (data > 255) data = 255;
			if (data < 0) data = 0;
			image_out[i][j] = (unsigned char)data;
		}
	}
	free(ar);
	free(ai);
	free(ff);
	return 0;

}






















