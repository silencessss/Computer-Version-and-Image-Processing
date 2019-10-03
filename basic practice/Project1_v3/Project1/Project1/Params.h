#pragma once
#define X_SIZE	256
#define Y_SIZE	256
#define E_EXP	8
#define Y_EXP	8				
#define HIGH	255				/*二質影像的高為準*/
#define LOW		0				/*二質影像的低位準*/
#define OFFSET	128				/*影像的補償位準*/
#define TEXT_BUT_LEN	10000	/*文字列長度*/
#define DIV 8
#define XS (X_SIZE/DIV)
#define YS (Y_SIZE/DIV)
#define DTH 0.7
#define TMP 128 /*背景候補的濃度暫定值*/
#define PI 3.141592/*list4.6*/
#define DMAX 100/*list4.6*/
//#define RAND_MAX 32767
#define BUFF_MAX 10000 /*list6.4*/
/*list7.1_連接成分標籤的基本值*/
#define L_BASE 100 
#define ROOT2 (double)1.41421356
#define BIAS 128/*分布直條圖的偏移值*/
#define OPT 1/*OPT=1光學的DFT(直流成分在中央)，OPT=0一班的DFT(直流成分在左端)*/
#define B_VAL 128/*圖框外的數值(label)*/
#define BYTESIZE 8/*1個位元組的位元數*/
#define LEN 4 /*可變單元(2或4位元)*/
#define DPCM dpcm1/*dpcm1的預測法，dpcm2的場合改用dpcm2取代*/
#define MAX_LENG X_SIZE*4/*可變長度代碼用緩衝區的最大位元組數*/
#define IDPCM idpcm1/*dpcm1的預測法，dpcm2的場合改用dpcm2取代*/
#define DXMAX X_SIZE/8
/*opencv*/
#include <opencv2/opencv.hpp>
using namespace cv;
Mat ConvertToMat(Mat img, uchar m[Y_SIZE][X_SIZE]) {
	for (int i = 0; i < img.rows;i++) {
		for (int j = 0; j < img.cols;j++) {
			img.at<uchar>(i, j) = m[i][j];
		}
	}
	return img;
}
void ConvertToUchar(Mat img, uchar m[Y_SIZE][X_SIZE]) {
	for (int i = 0; i < img.rows; i++) {
		for (int j = 0; j < img.cols; j++) {
			m[i][j] = img.at<uchar>(i, j);
		}
	}
}

/*
Mat CovertToMat2(Mat img, uchar m[3][Y_SIZE][X_SIZE]) {
	for (int i = 0; i < 3; i++) {
		for (int j = 0; j < img.rows; j++) {
			for (int k = 0; k < img.cols; k++) {
				img.at<uchar>(i, j, k) = m[i][j][k];
			}
		}
	}
}
void CovertToUchar2(Mat img, uchar m[3][Y_SIZE][X_SIZE]) {
	for (int i = 0; i < 3; i++) {
		for (int j = 0; j < img.rows; j++) {
			for (int k = 0; k < img.cols; k++) {
				m[i][j][k] = img.at<uchar>(i, j, k);
			}
		}
	}
}
*/
/*--------------------------------------------------------------------------
	一般的灰階值為多值(如果是8位元，就是256級灰階)(8^4)
--------------------------------------------------------------------------*/