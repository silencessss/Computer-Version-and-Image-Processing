#pragma once
#define X_SIZE	256
#define Y_SIZE	256
#define E_EXP	8
#define Y_EXP	8				
#define HIGH	255				/*�G��v����������*/
#define LOW		0				/*�G��v�����C���*/
#define OFFSET	128				/*�v�������v���*/
#define TEXT_BUT_LEN	10000	/*��r�C����*/
#define DIV 8
#define XS (X_SIZE/DIV)
#define YS (Y_SIZE/DIV)
#define DTH 0.7
#define TMP 128 /*�I���Ըɪ��@�׼ȩw��*/
#define PI 3.141592/*list4.6*/
#define DMAX 100/*list4.6*/
//#define RAND_MAX 32767
#define BUFF_MAX 10000 /*list6.4*/
/*list7.1_�s���������Ҫ��򥻭�*/
#define L_BASE 100 
#define ROOT2 (double)1.41421356
#define BIAS 128/*���������Ϫ�������*/
#define OPT 1/*OPT=1���Ǫ�DFT(���y�����b����)�AOPT=0�@�Z��DFT(���y�����b����)*/
#define B_VAL 128/*�Ϯإ~���ƭ�(label)*/
#define BYTESIZE 8/*1�Ӧ줸�ժ��줸��*/
#define LEN 4 /*�i�ܳ椸(2��4�줸)*/
#define DPCM dpcm1/*dpcm1���w���k�Adpcm2�����X���dpcm2���N*/
#define MAX_LENG X_SIZE*4/*�i�ܪ��ץN�X�νw�İϪ��̤j�줸�ռ�*/
#define IDPCM idpcm1/*dpcm1���w���k�Adpcm2�����X���dpcm2���N*/
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
	�@�몺�Ƕ��Ȭ��h��(�p�G�O8�줸�A�N�O256�ŦǶ�)(8^4)
--------------------------------------------------------------------------*/