#include "Thinning.h"

Mat Thinning::doThinning(Mat img){

    // Fast thinning algorithm implmented from https://github.com/bsdnoobz/zhang-suen-thinning
	img /= 255;
	Mat prev = Mat::zeros(img.size(), CV_8UC1);
	Mat diff;

	do{
		thinningIteration(img, 0);
		thinningIteration(img, 1);
		absdiff(img, prev, diff);
		img.copyTo(prev);
	} while (countNonZero(diff) > 0);

	img *= 255;

	return img;
}

void Thinning::thinningIteration(Mat &img, int iter){
	Mat marker = cv::Mat::zeros(img.size(), CV_8UC1);

	for (int i = 1; i < img.rows - 1; i++){
		for (int j = 1; j < img.cols - 1; j++){

			uchar p2 = img.at<uchar>(i - 1, j);
			uchar p3 = img.at<uchar>(i - 1, j + 1);
			uchar p4 = img.at<uchar>(i, j + 1);
			uchar p5 = img.at<uchar>(i + 1, j + 1);
			uchar p6 = img.at<uchar>(i + 1, j);
			uchar p7 = img.at<uchar>(i + 1, j - 1);
			uchar p8 = img.at<uchar>(i, j - 1);
			uchar p9 = img.at<uchar>(i - 1, j - 1);

			int A = (p2 == 0 && p3 == 1) + (p3 == 0 && p4 == 1) +
				    (p4 == 0 && p5 == 1) + (p5 == 0 && p6 == 1) +
				    (p6 == 0 && p7 == 1) + (p7 == 0 && p8 == 1) +
				    (p8 == 0 && p9 == 1) + (p9 == 0 && p2 == 1);

			int B = p2 + p3 + p4 + p5 + p6 + p7 + p8 + p9;
			int m1 = iter == 0 ? (p2 * p4 * p6) : (p2 * p4 * p8);
			int m2 = iter == 0 ? (p4 * p6 * p8) : (p2 * p6 * p8);

			if (A == 1 && (B >= 2 && B <= 6) && m1 == 0 && m2 == 0)
				marker.at<uchar>(i, j) = 1;
		}
	}

	img &= ~marker;
}
