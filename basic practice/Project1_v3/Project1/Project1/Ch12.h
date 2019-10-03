#pragma once
#include "Params.h"
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
//list12.1預測編碼
/*image_in影像資料，line行號，data_out單行的DPCM資料*/
void dpcm1(unsigned char image_in[Y_SIZE][X_SIZE], int line, short data_out[X_SIZE]) {
	int pred, j;
	for (j = 0; j < X_SIZE; j++) {
		if (j == 0) pred = B_VAL;
		else pred = (int)image_in[line][j - 1];
		data_out[j] = (int)image_in[line][j] - pred;
	}
}

void dpcm2(unsigned char image_in[Y_SIZE][X_SIZE], int line, short data_out[X_SIZE]) {
	int pred, j;
	/*第一行*/
	if (line == 0) {
		for (j = 0; j < X_SIZE; j++) {
			if (j == 0) pred = B_VAL;
			else pred = (image_in[line][j] - pred);
		}
	}
	/*其他行*/
	else {
		for (j = 0; j < X_SIZE; j++) {
			if (j == 0) pred = (B_VAL + image_in[line - 1][j]) / 2;
			else pred = (image_in[line][j - 1] + image_in[line - 1][j]) / 2;
			data_out[j] = image_in[line][j] - pred;
		}
	}
}
//list12.2預測編碼的解碼
void idpcm1(short data_in[X_SIZE], int line, unsigned char image_out[Y_SIZE][X_SIZE]) {
	int pred, j;/*pred預測值*/
	for (j - 0; j < X_SIZE; j++) {
		if (j == 0) pred = B_VAL;
		else pred = (int)image_out[line][j - 1];
		image_out[line][j] = (unsigned char)(pred + (int)data_in[j]);
	}
}
void idpcm2(short data_in[X_SIZE], int line, unsigned char image_out[Y_SIZE][X_SIZE]) {
	int pred, j;
	/*第一行*/
	if (line == 0) {
		for (j = 0; j < X_SIZE; j++) {
			if (j == 0) pred = B_VAL;
			else pred = (image_out[line][j] = pred + data_in[j]);
		}
	}
	/*其他行*/
	else {
		for (j = 0; j < X_SIZE; j++) {
			if (j == 0) pred = (B_VAL + image_out[line - 1][j]) / 2;
			else pred = (image_out[line][j - 1] + image_out[line - 1][j]) / 2;
			image_out[line][j] = pred + data_in[j];
		}
	}
}
//list12.3可變長度編碼
/*data_in原資料，no資料長，vlc_out可變長度代碼*/
int vlcode(short int data_in[], int no, char vlc_out[]) {
	int i, st = 0, num = 0;/*num代碼長(位元組)*/
	int dl = BYTESIZE / LEN - 1;
	int mask = (1 << LEN) - 1;
	int dt, ms;

	vlc_out[num] = '\n';
	for (i = 0; i < no; i++) {
		dt = data_in[i];
		do {
			ms = dt >= mask ? mask : dt;
			vlc_out[num] |= (ms << (LEN*(dl - st)));
			dt -= mask;
			st++;
			if (st > dl) {
				st = 0; num++; vlc_out[num] = '\0';
			}
		} while (dt >= 0);
	}
	/*將最後位元組中剩餘的bit置為1*/
	if (st != 0) {
		ms = mask;
		for (i = (dl - st); i >= 0; i--) {
			vlc_out[num] |= ms;
			ms <<= LEN;
		}
		num++;
	}
	return num;
}
//list12.4可變長度編碼的解碼
void ivlcode(char vlc_in[], int no, short int data_out[]) {
	int i, j, k;
	int ino = 0;/*代碼長(位元組)*/
	int num = 0;
	int dl = BYTESIZE / LEN - 1;
	int mask = (1 << LEN) - 1;
	for (i = 0; i < no; i++) data_out[i] = 0;
	do {
		for (j = dl; j >= 0; j--) {
			k = vlc_in[ino] & (mask << (LEN*j));
			k >>= (LEN*j);
			data_out[num] += k;
			if (k != mask) num++;
			if (num >= 0) break;
		}
		ino++;
	} while (num < no);
}
//list12.5可變長度編碼與DPCM資料的轉換
/*dt:DPCM資料*/
int event(short dt) {
	int ev;
	if (dt <= 0) ev = -2 * dt;
	else ev = 2 * dt - 1;
	return ev;
}
int ievent(short ev) {
	int dt;
	if (ev % 2 == 0) dt = -ev / 2;
	else dt = (ev + 1) / 2;
	return dt;
}

//list12.6 DPCM+可變長度編碼
/*image_in輸入影像陣列，image_buf輸出影像陣列(代碼序列)*/
int vlcode(short int data_in[], int no, char vlc_out[]);
void dpcm1(unsigned char image_in[Y_SIZE][X_SIZE], int line, short data_out[X_SIZE]);
void dpcm2(unsigned char image_in[Y_SIZE][X_SIZE], int line, short data_out[X_SIZE]);
int event(short dt);
int dpcm_vlcode(unsigned char image_in[Y_SIZE][X_SIZE], unsigned char image_buf[]) {
	int i, j, leng;
	long ptr, size;
	char vlc[MAX_LENG];/*可變長度代碼資料*/
	short int data[X_SIZE];/*單行的DPCM資料*/

	size = (long)X_SIZE*Y_SIZE;
	for (i = 0; i < Y_SIZE; i++) {
		image_buf[i] = 0;
	}
	ptr = 0;
	/*逐行進行編碼*/
	for (i = 0; i < Y_SIZE; i++) {
		DPCM(image_in, i, data);/*預測代碼*/
		for (j = 0; j < X_SIZE; j++) {
			data[j] = event(data[j]);/*變為VLC資料*/
		}
		leng = vlcode(data, X_SIZE, vlc);/*可變長度編碼*/
		image_buf[ptr] = (unsigned char)((leng >> 8) & 0x00ff);
		ptr++;
		if (ptr > size) return -1;/*當編碼資料比原影像大時，傳回錯誤碼-1*/
		image_buf[ptr] = (unsigned char)(leng & 0x00ff);
		ptr++;
		if (ptr > size) return -1;/*當編碼資料比原影像大時，傳回錯誤碼-1*/
		for (j = 0; j < leng; j++) {
			image_buf[ptr] = vlc[j];
			ptr++;
			if (ptr > size) return -1;/*當編碼資料比原影像大時，傳回錯誤碼-1*/
		}
	}
	return 0;
}
//list12.7 DPCM+可變長度編碼的解碼
void idpcm1(short data_in[X_SIZE], int line, unsigned char image_out[Y_SIZE][X_SIZE]);
void idpcm2(short data_in[X_SIZE, int line, unsigned char image_out[Y_SIZE][X_SIZE]]);
void ivlcode(char image_out[Y_SIZE][X_SIZE]);
int ievent(short ev);
int idpcm_vlcode(unsigned char image_buf[], unsigned char image_out[Y_SIZE][X_SIZE]) {
	int i, j, leng;
	long ptr, size;
	char vlc[MAX_LENG];/*可變長度代碼*/
	short int data[X_SIZE];/*單行的DPCM資料*/

	size = (long)X_SIZE*Y_SIZE;
	ptr = 0;
	/*逐行進行解碼*/
	for (i = 0; i < Y_SIZE; i++) {
		leng = (int)image_buf[ptr];
		ptr++;
		if (ptr > size) return -1;/*當編碼資料比原影像大時，傳回錯誤碼-1*/
		leng = (leng << 8) | image_buf[ptr];
		ptr++;
		for (j = 0; j < leng; j++) {
			vlc[j] = image_buf[ptr];
			ptr++;
			if (ptr > size) return -1;/*當編碼資料比原影像大時，傳回錯誤碼-1*/
		}
		ivlcode(vlc, X_SIZE, data);/*將可變長度代碼進行解碼*/
		for (j = 0; j < X_SIZE; j++) {
			data[j] = ievent(data[j]);/*把VLC資料變回差分資料*/
		}
		IDPCM(data, i, image_out);/*從差分資料復原影像*/
	}
	return 0;
}








