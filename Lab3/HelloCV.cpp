#include <stdio.h>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/imgproc.hpp>
using namespace cv;
Mat bwImage;
Mat thresh;
int value = 128;
static void onChange(int val, void*) {
	threshold(bwImage, thresh, getTrackbarPos("Threshold trackbar", "Image Thresholding"), 255, THRESH_OTSU);
	imshow("Image Thresholding", thresh);
}
int main(int argc, char* argv[])
{
	//Print the OpenCV version
	printf("OpenCV version: %d.%d\n", CV_MAJOR_VERSION,CV_MINOR_VERSION);
	if (argc < 2) {
		printf("InputError: Image path needed!!!\n");
		return 0;
	}

	// Load an image
	cv::Mat image = imread(argv[1], IMREAD_UNCHANGED);
	if (image.empty()) {
		printf("Can not load image!\n");
		return -1;
	}

	// Create a display window to display it in
	namedWindow("Image Thresholding", WINDOW_AUTOSIZE);
	// Load the image into that window
	imshow("Image Thresholding", image);
	// First convert it to an 8 - bit grey scale image
	cvtColor(image, bwImage, COLOR_BGR2GRAY, 0);
	createTrackbar("Threshold trackbar", "Image Thresholding", &value, 255, onChange);
	waitKey(0);

	// Save the threshold image 
	imwrite("out.png", thresh);

	return 0;
}