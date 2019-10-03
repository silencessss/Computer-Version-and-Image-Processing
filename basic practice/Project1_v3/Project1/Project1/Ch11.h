#pragma once
#include "Params.h"
#include <math.h>
#include <stdio.h>
#include <stdlib.h>

void fft1core(double a_rl[], double a_im[], int length, int ex, double sin_tbl[], double cos_tbl[], double buf[]);
void cstb(int length, int inv, double sin_tbl[], double cos_tbl[]);
void birv(double a[], int length, int ex, double b[]);
//����@��FFT�ܴ�
int fft1(double a_rl[], double a_im[], int ex, int inv){
	int i, length = 1;
	double *sin_tbl, *cos_tbl, *buf;
	
	for (i = 0; i < ex; i++) length *= 2;/*��ƭӼƭp��*/
	/*�ϥ�malloc()�禡�i��ʺA�Q�m�Ŷ��A�b��ӵ{�������e���|�۰��k�ٵ��O����A�����ϥ�free()�禡�N�Ŷ��ٵ��O����*/
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
/*�@��FFT���p��֤߳����Aa_rl��ƮɼƳ�(��X��J�ݥ�)�Aa_im��Ƶ�Ƴ�(��X��J�ݥ�)�Aex��������ƭӼƬ�2��������(��ƭӼ�=2��ex����)*/
/*sin_tbl SIN�p��Ϊ�Acos_tbl COS�p��Ϊ�*/
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
	birv(a_rl, length, ex, buf);/*��Ƹ�ƪ��I��*/
	birv(a_im, length, ex, buf);/*��Ƹ�ƪ��I��*/
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
/*SIN�BCOS���s�@�Alength��ƭӼơAinv(1:DFT�A-1:�fDFT*/
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
/*��ƪ������Aa��ư}�C�Alength��ƭӼơAb�@�~�w�İ�*/
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
//list11.2 ����G��FFT�ܴ�
void fft1core(double a_rl[], double a_im[], int length, int ex, double sin_tbl[], double cos_tbl[], double buf[]);
void cstb(int length, int inv, double sin_tbl[], double cos_tbl[]);
void rvmtx1(double a[Y_SIZE][X_SIZE], double b[X_SIZE][Y_SIZE], int xsize, int ysize);
void rvmtx2(double a[X_SIZE][Y_SIZE], double b[Y_SIZE][X_SIZE], int xsize, int ysize);
int fft2(double a_rl[Y_SIZE][X_SIZE], double a_im[Y_SIZE][X_SIZE], int inv) {
	double *b_rl, *b_im;/*�����m�@�~�θ�ư}�C(��Ƴ��B��Ƴ�)*/
	double *hsin_tbl, *hcos_tbl;/*������SIN�BCOS�p��Ϊ�*/
	double *vsin_tbl, *vcos_tbl;/*������SIN�BCOS�p��Ϊ�*/
	double *buf_x, *buf_y;/*�@�~�νw�İ�(�����B����)*/
	int i;
	/*�ϥ�calloc()�ӫŧi�Ŷ��t�m�A�Ӧ��o�Ӫ��Ŷ��b���ϥήɡA����free()����*/
	b_rl = (double *)calloc((size_t)X_SIZE*Y_SIZE, sizeof(double));
	b_im = (double *)calloc((size_t)X_SIZE*Y_SIZE, sizeof(double));
	hsin_tbl = (double *)calloc((size_t)X_SIZE, sizeof(double));
	hcos_tbl = (double *)calloc((size_t)X_SIZE, sizeof(double));
	vsin_tbl = (double *)calloc((size_t)Y_SIZE, sizeof(double));
	vcos_tbl = (double *)calloc((size_t)Y_SIZE, sizeof(double));
	buf_x = (double *)malloc((size_t)X_SIZE * sizeof(double));
	buf_y = (double *)malloc((size_t)X_SIZE * sizeof(double));

	if ((b_rl == NULL) || (b_im == NULL) || (hsin_tbl == NULL) || (hcos_tbl == NULL) || (vsin_tbl == NULL) || (vcos_tbl == NULL) || (buf_x == NULL) || (buf_y == NULL)) return -1;

	cstb(X_SIZE, inv, hsin_tbl, hcos_tbl);/*������SIN�BCOS��s�@*/
	cstb(Y_SIZE, inv, vsin_tbl, vcos_tbl);/*������SIN�BCOS��s�@*/
	/*������V��FFT*/
	for (i = 0; i < Y_SIZE; i++) {
		fft1core(&a_rl[(long)i][0], &a_im[(long)i][0], X_SIZE, X_EXP, hsin_tbl, hcos_tbl, hcos_tbl, buf_x);
	}
	/*�G����ƪ���m*/
	rvmtx1((double(*)[X_SIZE])a_rl, (double(*)[X_SIZE])b_rl, X_SIZE, Y_SIZE);
	rvmtx1((double(*)[X_SIZE])a_im, (double(*)[X_SIZE])b_im, X_SIZE, Y_SIZE);
	/*������V��FFT*/
	for (i = 0; i < X_SIZE; i++) {
		fft1core(&b_rl[(long)Y_SIZE*i], &b_im[(long)Y_SIZE*i], Y_SIZE, Y_EXP, vsin_tbl, vcos_tbl, buf_y);
	}
	/*�G����ƪ���m*/
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
	/*�G�������m*/
	/*a�G����J��ơAb�G����X��ơAxsize������ƭӼơAysize������ƭӼ�*/
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


/*�N�G��FFT�����G�v����*/
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
	/*Ū�J��v���A���ഫ���G��FFT����J���*/
	for (i = 0; i < Y_SIZE; i++) {
		for (j = 0; j < X_SIZE; j++) {
			ar[X_SIZE*i + j] = (double)image_in[i][j];
			ai[X_SIZE*i + j] = 0.0;
		}
	}
	/*����G��FFT*/
	if ( fft2( ( double(*)[X_SIZE])ar, (double(*)[X_SIZE])ai, 1) == -1)  return -1;
	/*�NFFT���G�v����*/
	max = 0;
	for (i = 0; i < Y_SIZE; i++) {
		for (j = 0; j < X_SIZE; j++) {
			norm = ar[X_SIZE*i + j] * ar[X_SIZE*i + j] + ai[X_SIZE*i + j] * ai[X_SIZE*i + j];/*���T�����p��*/
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
	/*��FFT�����G�ܴ����v�����*/
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

//list11.4�Q�ΤG��FFT�����o�i�B�z
int fft2(double a_rl[Y_SIZE][X_SIZE], double a_im[Y_SIZE][X_SIZE], int inv);
int fftfilter(unsigned char image_in[Y_SIZE][X_SIZE], unsigned char image_out[Y_SIZE][X_SIZE], int a, int b) {
	double *ar; /*��ƹ�Ƴ�(��J��X)*/
	double *ai;/*��Ƶ�Ƴ�(��J��X)*/
	double *ff;/*�o�i�����Ŷ��W�v�S��*/
	double data;
	long i, j, circ;

	ar = (double *)malloc((size_t)Y_SIZE*X_SIZE * sizeof(double));
	ai = (double *)malloc((size_t)Y_SIZE*X_SIZE * sizeof(double));
	ff = (double *)malloc((size_t)Y_SIZE*X_SIZE * sizeof(double));
	if ((ar == NULL) || (ai == NULL) || (ff == NULL)) return -1;
	/*Ū�J��v���ɡA�ܴ����G��FFT����J���*/
	for (i = 0; i < Y_SIZE; i++) {
		for (j = 0; j < X_SIZE; j++) {
			ar[X_SIZE*i + j] = (double)image_in[i][j];
			ai[X_SIZE*i + j] = 0.0;
		}
	}
	/*����FFT�A���v���ܴ����W�v����*/
	if (fft2((double(*)[X_SIZE])ar, (double(*)[X_SIZE])ai, 1) == -1) return -1;
	/*�s�@�X�u���W�v��a�H�Wb�H�U�������q�L���o�i��*/
	for (i = 0; i < Y_SIZE; i++) {
		for (j = 0; j < X_SIZE; j++) {
			data = (double)((j - X_SIZE / 2)*(j - X_SIZE / 2) + (i - Y_SIZE / 2)*(i - Y_SIZE / 2));
			circ = (long)sqrt(data);
			if ((circ >= a) && (circ <= b)) ff[X_SIZE*i + j] = 1.0;
			else ff[X_SIZE*i + j] = 0.0;
		}
	}
	/*��v�����W�v��������v�i�B�z*/
	for (i = 0; i < Y_SIZE; i++) {
		for (j = 0; j < X_SIZE; j++) {
			ar[X_SIZE*i + j] *= ff[X_SIZE*i + j];
			ai[X_SIZE*i + j] *= ff[X_SIZE*i + j];
		}
	}
	/*����f�VFFT�ܴ��A����i�B�z�L���W�v�����^�_���v��*/
	if (fft2((double(*)[X_SIZE])ar, (double(*)[X_SIZE])ai, -1) == -1) return -1;
	/*�N���G�ܬ��v�����*/
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






















