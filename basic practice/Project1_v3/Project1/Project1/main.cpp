#include "Params.h"
#include <iostream>//std::cout
#include <opencv2/opencv.hpp>
#include <opencv2/core/core.hpp>//cv::Mat
#include <opencv2/highgui/highgui.hpp>//cv::imread()
#include <opencv2/imgproc/imgproc.hpp>//cv::Canny()
//#include "Ch3.h"
//#include "Ch4.h"
//#include "Ch5.h"
//#include "Ch7.h"
//#include "Ch8.h"
#include "Ch10.h"
using namespace cv;
using namespace std;
int main() {
	Mat img = imread("test2.jpg", CV_LOAD_IMAGE_GRAYSCALE);
	Mat ImOut(Y_SIZE,X_SIZE,CV_8UC3, Scalar(0,0,0));
	uchar m[Y_SIZE][X_SIZE];
	uchar m2[Y_SIZE][X_SIZE];
	//uchar m3[3][Y_SIZE][X_SIZE];
	//uchar m4[3][Y_SIZE][X_SIZE];
	//unsigned char med[] = { 1,2 };
	//long h[256];
	//char *buf = "a";
	/*if (!img.data) {
		std::cout << "not" << std::endl;
		return -1;
	}*/
	ConvertToUchar(img, m);
	
	//ConvertToUchar2(img, m3);
	//threshold(m,m2,120,2);
	//histgram(m,h);
	//histprint(h, buf);
	//histimage(h,m);
	//prewitt(m,m2,5);
	//thinning(m,m2);
	//laplacian(m, m2, 5, 2);
	//zero_cross(m, m2);
	//smooth(m, m2, 8);
	//noise_rand(m, m2, 100);
	//median_value(med[1]);//problem
	//noise_spike(m, m2, 20, 50);//problem
	//erosion(m, m2);
	//amplify(m, m2, 6, -50);
	//dither_ordered(m, m2);
	//dither_minimized_multi(m, m2, 8);
	//quantize(m, m2, 16);
	//labeling(m, m2, &cnt, &cnt2);//problem
	//colorbar(m3, 5);
	//b_to_ysh(m3, m4);
	//scale_near(m,m2,2,2);
	//scale(m, m2, 1, 1.5);
	//shift(m, m2, 50, -20);
	//rotation(m, m2, -60);
	//scale_rotate_shift(m, m2, 1.5, 1, 30, -100, 200);
	//affine(m, m2, 1, 1, 1, 3, 4, 20);
	//affine_coef(0, 0, 10, 10, 0, 20, 10, 30, 10, 0, 30, 20, 2.0, 2, 2, 3, 3, 3);
	//lattice(m);
	img = ConvertToMat(img,m);
	imshow("imageshow",img);
	waitKey(0);
	system("pause");
	return 0;
}