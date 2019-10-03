#pragma once
#include "Params.h"
/*--------------------------------------------------------------------------------------------------
	�D�Q�Φh�ȸ�T�h�B�z�A�ӬO�Q���{�ɭȡA��h��v���ܴ����u�����C�G��(HIGH�PLOW)���G�ȼv���i��B�z�C
	�G�ȼv���B�z�k:�Q�ΰ��C�G�ȳB�z�v���C
---------------------------------------------------------------------------------------------------*/
//list3.1�{�ɭȳB�z
void threshold(unsigned char image_in[Y_SIZE][X_SIZE], unsigned char image_out[Y_SIZE][X_SIZE], int thresh, int type)
{
	int i, j;
	for (i = 0; i < Y_SIZE; i++) {
		for (j = 0; j < X_SIZE; j++) {
			switch (type)
			{
			case 2:
				if ((int)image_in[i][j] <= thresh) {
					image_out[i][j] = HIGH;
				}
				else {
					image_out[i][j] = LOW;
				}
				break;
			default:
				if ((int)image_in[i][j] >= thresh) {
					image_out[i][j] = HIGH;
				}
				else {
					image_out[i][j] = LOW;
				}
				break;
			}
		}
	}
}
//list3.2�D������
void histgram(unsigned char image_in[Y_SIZE][X_SIZE], long hist[256]) {
	int i, j, n;
	for (n = 0; n < 256;n++) {
		hist[n] = 0;
	}
	for (i = 0; i < Y_SIZE;i++) {
		for (j = 0; j < X_SIZE;j++) {
			n = image_in[i][j];
			hist[n]++;
		}
	}
}
//list3.3�H�ƭȻP�r���⪽���ϦC�L�X��
void histprint(long hist[256], char *buf) {
	int i, j, k;
	double p, q, max;
	int posi, m;
	posi = 0;
	p = X_SIZE * Y_SIZE;
	max = 0;
	for (i = 0; i < 256;i++) {
		if (hist[i] > max) {
			max = hist[i];
		}
	}
	for (i = 0; i < 256;i++) {
		q = hist[i] / p*100.0;
		//m = sprintf(&buf[posi], "%3d:%5.1f%%|",i,q);
		m = sprintf_s(buf, sizeof(buf), "%3d:%5.1f%%|",i,q);
		posi += m;
		k = (int)(hist[i] / max *60.0);
		for (j = 0; j < k; j++) {
			//m = sprintf(&buf[posi],"*");
			m = sprintf_s(buf, sizeof(buf), "*");
			posi += m;
		}
		//m = sprintf(&buf[posi], "\n");
		m = sprintf_s(buf,sizeof(buf),"\n");
		posi += m;
	}
}
//list3.4�⪽���ϼv����
void histimage(long hist[256], unsigned char image_hist[Y_SIZE][X_SIZE]) {
	int i, j, k, max, range;
	double d;
	long n;
	range = Y_SIZE - 5;
	for (i = 0; i < Y_SIZE;i++) {
		for (j = 0; j < X_SIZE;j++) {
			image_hist[i][j] = LOW;
		}
	}
	if (X_SIZE>=256) {
		max = 0;
		for (i = 0; i < 256;i++) {
			n = hist[i];
			if (n > max) max = n;
		}
		for (i = 0; i < 256;i++) {
			d = hist[i];
			n = (long)(d / (double)max * range );
			for (j = 0; j <= n;j++) {
				image_hist[range - j][i] = HIGH;
			}
		}
		for (i = 0; i <= 4; i++) {
			k = 64 * i;
			if (k >= X_SIZE) {
				k = X_SIZE - 1;
			}
			for (j = range; j < Y_SIZE;j++) {
				image_hist[j][k] = HIGH;
			}
		}
	}
	else if (X_SIZE >= 128) {
		max = 0;
		for (i = 0; i < 128;i++) {
			n = hist[2 * i] + hist[2 * i + 1];
			if (n > max) max = n;
		}
		for (i = 0; i < 128; i++) {
			d = hist[2 * i] + hist[2 * i + 1];
			n = (long)(d / (double)max * range);
			for (j = 0; j <= n;j++) {
				image_hist[range - j][i] = HIGH;
			}
		}
		for (i = 0; i <= 4;i++) {
			k = 32 * i;
			if (k>=X_SIZE) {
				k = X_SIZE - 1;
			}
			for (j = range; j < Y_SIZE;j++) {
				image_hist[j][k] = HIGH;
			}
		}
	}
	else if (X_SIZE >= 64) {
		max = 0;
		for (i = 0; i < 64; i++) {
			n = hist[4 * i] + hist[4 * i + 1] + hist[4*i+2] + hist[4*i+3];
			if (n > max) max = n;
		}
		for (i = 0; i < 64; i++) {
			d = hist[4 * i] + hist[4 * i + 1] + hist[4 * i + 2] + hist[4 * i + 3];
			n = (long)(d / (double)max * range);
			for (j = 0; j <= n; j++) {
				image_hist[range - j][i] = HIGH;
			}
		}
		for (i = 0; i <= 4; i++) {
			k = 16 * i;
			if (k >= X_SIZE) {
				k = X_SIZE - 1;
			}
			for (j = range; j < Y_SIZE; j++) {
				image_hist[j][k] = HIGH;
			}
		}
	}
}
//list3.5�����ϥ��Ƥ�
void histsmooth(long hist_in[256], long hist_out[256]) {
	int m, n, i;
	long sum;
	for (n = 0; n < 256;n++) {
		sum = 0;
		for (m = -2; m <= 2;m++) {
			i = n + m;
			if ( i< 0 ) { i = 0; }
			if (i > 255) { i = 255; }
			sum += hist_in[i];
		}
		hist_out[n] = (long)((double)sum / 5.0 + 5.0);
	}
}
//list3.6
int threshmode(long hist[256]) {
	int m, n;
	long max, min;
	max = 0;
	for (m = 0; m < 256;) {
		if (max <= hist[m]) {
			max = hist[m];
		}
		else break;
	}
	min = max;
	for (n = m; n < 256;n++) {
		if (min>=hist[n]) {
			min = hist[n];
		}
		else break;
	}
	return n - 1;
}
//list3.7
int threshdiscrim(long hist[256], double disparity) {
	int i, k;
	double n0, n1, n2, m0, m1, m2;
	double v[256], vmax, v0;

	n0 = 0.0;
	m0 = 0.0;
	for (i = 0; i < 256;i++) {
		n0 += hist[i];
		m0 += i*hist[i];
	}
	if (n0 == 0.0) { m0 = 0.0; }
	else m0 /= n0;
	v0 = 0.0;
	for (i = 0; i < 256;i++) {
		v0 += hist[i] * (i - m0)*(i - m0) / n0;
	}
	for (k = 0; k < 256;k++) {
		n1 = 0.0;
		m1 = 0.0;
		for (i = 0; i < k;i++) {
			n1 += hist[i];
			m1 += i*hist[i];
		}
		if (n1==0.0) {
			m1 = 0.0;
		}
		else m1 /= n1;
		n2 = 0.0;
		m2 = 0.0;
		for (i = k; i < 256;i++) {
			n2 += hist[i];
			m2 += i*hist[i];
		}
		if (n2==0.0) {
			m2 = 0.0;
		}
		else m2 /= n2;
		v[k] = (n1*(m1 - m0)*(m1 - m0) + n2*(m2 - m0)*(m2 - m0)) / n0;
	}
	vmax = 0.0;
	for (i = 0; i < 256;i++) {
		if (vmax<=v[i]) {
			vmax = v[i];
			k = i;
		}
	}
	if (v0==0) {
		return 0;
	}
	if ((vmax / v0) >= disparity) {
		return k;
	}
	else return 0;
}
//list3.8�ʺA�{�ɭȤ�k
/*-----------------------------------------------------------------------------
	�N��i�v������DIV�Ӧ�C(��V��XS�����A�a�V��YS�����A�@�p��DIVxDIV�Ӥ��C
------------------------------------------------------------------------------*/
void threshold_dynamic(unsigned char image_in[Y_SIZE][X_SIZE], unsigned char image_out[Y_SIZE][X_SIZE], int type) {
	int i, j, k, m, n, m1, m2, n1, n2, s, t;
	int thm[DIV + 1][DIV + 1];
	long hist[256];
	int thresh;
	double p, q;
	//�����
	for (i = 0; i <= DIV;i++) {
		for (j = 0; j <= DIV;j++) {
			thm[i][j] = 0;
		}
	}
	//�M�w���I���{�ɭ�
	for (i = 0; i <= DIV;i++) {
		for (j = 0; j <= DIV;j++) {
			for (k = 0; k < 256; k++) { hist[k] = 0; }
			if (i != 0) m1 = -YS;
			else m1 = 0;
			if (i != DIV) m2 = YS;
			else m2 = 0;
			if (j != 0) n1 = -XS;
			else n1 = 0;
			if (j != DIV) n2 = XS;
			else n2 = 0;
			for (m = m1; m < m2;m++) {
				for (n = n1; n < n2;n++) {
					k = image_in[i*YS + m][j*XS + n];
					hist[k]++;
				}
			}
			thm[i][j] = threshdiscrim(hist, DTH);
		}
	}
	//�����D�o�{�ɭȤ�����w�{�ɭ�
	for (i = 0; i <= DIV;i++) {
		for (j = 0; j <= DIV;j++) {
			if (thm[i][j] <= 0) {
				for (k = 1; k < DIV;k++) {
					s = 0;
					t = 0;
					m1 = i - k;
					m2 = i + k;
					n1 = j - k;
					n2 = j + k;
					if (m1 < 0) m1 = 0;
					if (m2 > DIV) m2 = DIV;
					if (n1 < 0) n1 = 0;
					if (n2 > DIV) n2 = DIV;
					for (m = m1; m <= m2;m++) {
						for (n = n1; n <= n2; n++) {
							if (thm[m][n] > 0) {
								s += 1 / k;
								t += thm[m][n] / k;
							}
						}
					}
					if (s >= 4) {
						thm[i][j] = t / s;
						break;
					}
				}
			}
		}
	}
	//�C�@�ӹ�����w�{�ɭ�
	for (i = 0; i < Y_SIZE;i++) {
		for (j = 0; j < X_SIZE;j++) {
			m = i / YS;
			n = j / XS;
			q = (double)(i%YS) / YS;
			p = (double)(j%YS) / XS;
			//thresh = (int)(1.0 - q) * ((1.0 - p) *thm[m][n] + p*thm[m][n + 1]) + q*((1.0 - p)*thm[m + 1][n] + p*thm[m + 1][n + 1]);
			switch (type) {
			case 2:
				if ((int)image_in[i][j] <= thresh) image_out[i][j] = HIGH;
				else image_out[i][j] = LOW;
				break;
			default:
				if ((int)image_in[i][j] >= thresh) image_out[i][j] = HIGH;
				else image_out[i][j] = LOW;
				break;
			}
		}
	}
}