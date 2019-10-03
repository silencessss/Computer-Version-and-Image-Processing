#include <cstdio>
#include <opencv2/opencv.hpp>
#include <vector>
#include <opencv2/ml.hpp>
#include <opencv2/core.hpp>
#include <opencv2/imgproc.hpp>
#include "opencv2/imgcodecs.hpp"
#include <opencv2/highgui.hpp>
using namespace cv;
using namespace cv::ml;

int main() {
	int width = 300;
	int height = 300;
	Mat I = Mat::zeros(height, width, CV_8UC3);
	Mat trainData(100, 2, CV_32FC1);
	Mat labels(100, 1, CV_32FC1);

	//設100個隨機點
	RNG rng;
	for (int i = 0; i<50; i++) {
		labels.at<float>(i, 0) = 1.0;
		int tempY = rng.uniform(0, 299);
		int tempX = rng.uniform(0, 170);
		trainData.at<float>(i, 0) = tempX;
		trainData.at<float>(i, 1) = tempY;
	}
	for (int i = 50; i<99; i++) {
		labels.at<float>(i, 0) = -1.0;
		int tempY = rng.uniform(0, 299);
		int tempX = rng.uniform(130, 299);
		trainData.at<float>(i, 0) = tempX;
		trainData.at<float>(i, 1) = tempY;
	}

	//ml::SVM::Params params;
	//CvSVMParams params;
	//ml::SVM.Params params;
	SVM::ParamTypes params;
	params.svmType = SVM::C_SVC;
	params.C = 0.1;
	params.kernel_type = SVM::LINEAR;
	params.term_crit = TermCriteria(CV_TERMCRIT_ITER, (int)1e7, 1e-6);
	CvSVM svm;
	svm.train(trainData, labels, Mat(), Mat(), params);

	Vec3b green(0, 100, 0), blue(100, 0, 0);
	for (int i = 0; i < I.rows; ++i) {
		for (int j = 0; j < I.cols; ++j) {
			Mat sampleMat = (Mat_<float>(1, 2) << i, j);
			float response = svm.predict(sampleMat);
			if (response == 1) {
				I.at<Vec3b>(j, i) = green;
			}
			else if (response == 2) {
				I.at<Vec3b>(j, i) = blue;
			}
		}
	}

	float px, py;
	for (int i = 0; i<50; ++i) {
		px = trainData.at<float>(i, 0);
		py = trainData.at<float>(i, 1);
		circle(I, Point((int)px, (int)py), 3, Scalar(0, 0, 255));
	}
	for (int i = 50; i<100; ++i) {
		px = trainData.at<float>(i, 0);
		py = trainData.at<float>(i, 1);
		circle(I, Point((int)px, (int)py), 3, Scalar(255, 255, 0));
	}

	imshow("SVM示範", I);
	waitKey(0);
	return 0;
}