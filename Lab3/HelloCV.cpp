#include <stdio.h>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/imgproc.hpp>

using namespace cv;
using namespace std;

Mat bwImage;
Mat thresh_BINARY;
Mat thresh_OTSU;
int value = 128;
int threshold_value = 0;
char keyPress = 0;
bool saveImage = false;
string inputFilename;
string outputOTSUname;
string outputBINARYname;

static void Threshold_BINARY(int val, void*) {
	cv::threshold(bwImage, thresh_BINARY, threshold_value, 255, THRESH_BINARY);
	cv::imshow("Image Thresholding Binary", thresh_BINARY);
}

static void Threshold_OTSU(int val, void*) {
	cv::threshold(bwImage, thresh_OTSU, threshold_value, 255, THRESH_OTSU);
	cv:imshow("Image Thresholding OTSU", thresh_OTSU);
}

int main(int argc, char* argv[]){
	// Print the OpenCV version
	printf("OpenCV version: %d.%d\n", CV_MAJOR_VERSION,CV_MINOR_VERSION);
	// Make sure to check that the path has been provided
	if (argc < 2) {
		printf("InputError: Image path needed!!!\n");
		return -1;
	}

	// Load an image
	// Takes the path as the first argument and some flags that determine how the data is interpreted as the second
	cv::Mat image = imread(argv[1], IMREAD_UNCHANGED);
	if (image.empty()) {
		printf("Can not load the image!\n");
		return -1;
	}

	string inputFilename = argv[1];
	string outputOTSUname = inputFilename.substr(0, inputFilename.find_last_of(".")) + "_OTSU.png";
	string outputBINARYname = inputFilename.substr(0, inputFilename.find_last_of(".")) + "_BINARY.png";
	printf(outputBINARYname);

	// Create a display window to display it in
	cv::namedWindow("Image Thresholding Binary", WINDOW_AUTOSIZE);
	// Another window for OTSU
	cv::namedWindow("Image Thresholding OTSU", WINDOW_AUTOSIZE);

	// Load the image into those windows
	cv::imshow("Image Thresholding Binary", image);
	cv::imshow("Image Thresholding OTSU", image);

	// First convert it to an 8 - bit grey scale image
	cv::cvtColor(image, bwImage, COLOR_BGR2GRAY, 0);
	// Add the track bar for Binary 
	cv::createTrackbar("Threshold trackbar", "Image Thresholding Binary", &threshold_value, 255, Threshold_BINARY);

	// Implement OTSU for another one
	Threshold_OTSU(0,0);
	// Save the threshold image (OTSU)
	cv::imwrite(outputOTSUname, thresh_OTSU);
	cv::imwrite("bb.png", thresh_OTSU);

	// Wait for the user to press a key in order to move on with the code
	// cv::waitKey(0);

	// Change the thresholding manually and save the image
	while (keyPress != 'q') {
		// Check for key press
		keyPress = waitKey(1);

		// Check if 's' key is pressed
		if (keyPress == 's') {
			saveImage = true;
		}

		// Check if the image should be saved
		if (saveImage) {
			cv::imwrite(outputBINARYname, thresh_BINARY);
			cv::imwrite("jj.png", thresh_BINARY);
			printf("Image saved!\n");
			saveImage = false;
		}
	}

	return 0;
}
