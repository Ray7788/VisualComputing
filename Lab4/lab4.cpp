#include <stdio.h>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <cmath>
#include<iostream>
#include <math.h>

using namespace cv;
using namespace std;

//Polynomial regression function
vector<double> fitPoly(vector<Point> points, int n)
{
    //Number of points
    int nPoints = points.size();

    //Vectors for all the points' xs and ys
    vector<float> xValues = vector<float>();
    vector<float> yValues = vector<float>();

    //Split the points into two vectors for x and y values
    for (int i = 0; i < nPoints; i++)
    {
        xValues.push_back(points[i].x);
        yValues.push_back(points[i].y);
    }

    //Augmented matrix
    // Dynamic memory allocation for: double matrixSystem[n + 1][n + 2];
    double** matrixSystem = new double* [n + 1];
    for (int i = 0; i < n + 1; i++) {
        matrixSystem[i] = new double[n + 2];
    }

    for (int row = 0; row < n + 1; row++)
    {
        for (int col = 0; col < n + 1; col++)
        {
            matrixSystem[row][col] = 0;
            for (int i = 0; i < nPoints; i++)
                matrixSystem[row][col] += pow(xValues[i], row + col);
        }

        matrixSystem[row][n + 1] = 0;
        for (int i = 0; i < nPoints; i++)
            matrixSystem[row][n + 1] += pow(xValues[i], row) * yValues[i];
    }

    //Array that holds all the coefficients
    // Dynamic memory allocation for: double coeffVec[n + 2] = {};  
    // the "= {}" is needed in visual studio, but not in Linux
    double* coeffVec = new double[n + 2]();
    //Gauss reduction
    for (int i = 0; i <= n - 1; i++)
        for (int k = i + 1; k <= n; k++)
        {
            double t = matrixSystem[k][i] / matrixSystem[i][i];

            for (int j = 0; j <= n + 1; j++)
                matrixSystem[k][j] = matrixSystem[k][j] - t * matrixSystem[i][j];

        }

    //Back-substitution
    for (int i = n; i >= 0; i--)
    {
        coeffVec[i] = matrixSystem[i][n + 1];
        for (int j = 0; j <= n + 1; j++)
            if (j != i)
                coeffVec[i] = coeffVec[i] - matrixSystem[i][j] * coeffVec[j];

        coeffVec[i] = coeffVec[i] / matrixSystem[i][i];
    }

    //Construct the cv vector and return it
    vector<double> result = vector<double>();
    for (int i = 0; i < n + 1; i++)
        result.push_back(coeffVec[i]);

    // memory fninalised
    for (int i = 0; i < n + 1; i++) {
        delete[] matrixSystem[i];
    }
    delete[] matrixSystem;
    delete[] coeffVec;

    return result;
}

//Returns the point for the equation determined
//by a vector of coefficents, at a certain x location
Point pointAtX(vector<double> coeff, double x)
{
    double y = 0;
    for (int i = 0; i < coeff.size(); i++)
        y += pow(x, i) * coeff[i];
    return Point(x, y);
}


// initial image
/*
*/
Mat image, image_gray, image_blur, dst, houghLine, canny, short_lines_removed, filtering_vertical, horizon;
//const char* window_name = "horizon Image";

int main(int argc, char** argv) {
    //Binary threshold variable
    int threshold = 70;

    String imageName("horizon0.png"); // by default image
    if (argc > 1){
        imageName = argv[1]; //get data path of image
    } else if (argc < 2) {
        printf("Image path required\n");
        return -1;
    }

    image = imread(samples::findFile(imageName), IMREAD_GRAYSCALE); // Load an image
    if (image.empty()){
        // if there's no image, exit
        printf("Cannot read the image!\n");
        return -1;
    }

    //waitKey(0);
    // namedWindow( window_name, WINDOW_AUTOSIZE ); // Create a window to display results

    GaussianBlur(image, image_blur, Size(3, 3), 0);     // Blur
    Canny(image_blur, canny, 70, 123, 3);   // Apply a Canny filter on the image
    cvtColor(canny, image_gray, COLOR_GRAY2BGR, 0);     // Convert the image to Gray

    // Display canny edge detected image
    string outputCANNYname = imageName.substr(0, imageName.find_last_of(".")) + "_canny.png";
    imshow("1 Canny edge image", canny);
    imwrite(outputCANNYname, canny);
    waitKey(0);


    int maximum = 0, minimum = 0, average_distance = 0, total_distance = 0, difference = 0;
    /////////////////////////////////////////////////////////////////////////////////////////
    // 2: The Probabilistic Hough Line Transform
    houghLine = image_gray.clone();
    vector<Vec4i> linesP; // will hold the results of the detection
    HoughLinesP(canny, linesP, 1, CV_PI / 180, 50, 0, 3); // runs the actual detection
    string outputHoughLinename = imageName.substr(0, imageName.find_last_of(".")) + "HoughLine.png";

    // Draw the lines
    for (size_t i = 0; i < linesP.size(); i++){
        Vec4i l = linesP[i];
        //defining the startand end coordinates for line segments.
        line(houghLine, Point(l[0], l[1]), Point(l[2], l[3]), Scalar(0,0,255), 3, LINE_AA);

        // For part3
        int length = sqrt(((l[2] - l[0]) * (l[2] - l[0])) + ((l[3] - l[1]) * (l[3] - l[1])));
        if (maximum < length) {
            maximum = length;
        }
        if (minimum > length) {
            minimum = length;
        }
        total_distance += length;
    }
    imshow("2 Probabilistic Line Transform: Detected Lines (in red)", houghLine);
    imwrite(outputHoughLinename, houghLine);
    waitKey(0);


    /////////////////////////////////////////////////////////////////////////////////////////
    // 3: Filter out short lines: an image with the short lines removed
    short_lines_removed = image_gray.clone();
    average_distance = total_distance / linesP.size();
    difference = maximum - minimum;
    string outputSHORTname = imageName.substr(0, imageName.find_last_of(".")) + "_no_short.png";

    vector<Vec4i> removed_shortlines; // For part4

    for (size_t i = 0; i < linesP.size(); i++) {
        Vec4i l = linesP[i];
        int length = sqrt(((l[2] - l[0]) * (l[2] - l[0])) + ((l[3] - l[1]) * (l[3] - l[1])));

        if (difference >= 130) {
            if (length >= average_distance + 80) {
                removed_shortlines.push_back((l));
                line(short_lines_removed, Point(l[0], l[1]), Point(l[2], l[3]), Scalar(0, 0, 255), 1, LINE_AA);
            }
        }else {
            if (length >= average_distance + 30) {
                removed_shortlines.push_back((l));
                line(short_lines_removed, Point(l[0], l[1]), Point(l[2], l[3]), Scalar(0, 0, 255), 1, LINE_AA);
            }
        }
    }
    imshow("3 short lines removed", short_lines_removed);
    imwrite(outputSHORTname, short_lines_removed);
    waitKey(0);

    /////////////////////////////////////////////////////////////////////////////////////////
    // 4: Filter out vertical lines
    filtering_vertical = image_gray.clone();
    vector<Vec4i> filtered_vertical_lines;
    vector<Point> points;
    string outputVERTICALname = imageName.substr(0, imageName.find_last_of(".")) + "_no_vertical.png";

    for (size_t i = 0; i < removed_shortlines.size(); i++) {
        Vec4i l = removed_shortlines[i];
        int horizontal = l[2] - l[0];
        int vertical = l[3] - l[1];
        double tan;

        if (horizontal == 0) {
            tan = 10;
        }else if (vertical == 0) {
            tan = 0;
        }else {
            // inverse tangent line
            tan = atan2(l[3] - l[1], l[2] - l[0]);
        }

        if (tan < 0.2 && tan > -0.2) {
            line(filtering_vertical, Point(l[0], l[1]), Point(l[2], l[3]), Scalar(0, 0, 255), 1, LINE_AA);
            filtered_vertical_lines.push_back(l);
            points.push_back(Point(l[0], l[1]));
            points.push_back(Point(l[2], l[3]));
        }
    }
    imshow("4 filtering vertical", filtering_vertical);
    imwrite(outputVERTICALname, filtering_vertical);
    waitKey(0);


    /////////////////////////////////////////////////////////////////////////////////////////
    // 5: horizon
    cvtColor(image, horizon, COLOR_GRAY2BGR, 0);
    vector<double> coefficients = fitPoly(points, 2);   //  returns a vector of coefficients for the polynomial
    string outputHORIZONname = imageName.substr(0, imageName.find_last_of(".")) + "_horizon.png";

    for (int i = 0; i < 3000; i++) {
        Point p1 = pointAtX(coefficients, i);
        Point p2 = pointAtX(coefficients, i + 1);
        line(horizon, p1, p2, Scalar(0, 0, 255), 1, LINE_AA);   // Use red color
    }
    imshow("5 horizon", horizon);
    imwrite(outputHORIZONname, horizon);
    waitKey(0);

    return 0;
}