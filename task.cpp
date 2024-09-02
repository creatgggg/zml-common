#include <opencv2/opencv.hpp>
#include <iostream>  

using namespace cv;
using namespace std;

int main() {
    Mat image = imread("Resources/mofang.png"); 
    Mat imghsv;
    cvtColor(image, imghsv, COLOR_BGR2HSV);
    Mat blueMask, yellowMask;
    inRange(imghsv, Scalar(100, 50, 50), Scalar(140, 255, 255), blueMask); 
    inRange(imghsv, Scalar(20, 100, 100), Scalar(40, 255, 255), yellowMask);    
    Mat kernel = getStructuringElement(MORPH_RECT, Size(3, 3));
    morphologyEx(blueMask, blueMask, MORPH_OPEN, kernel);
    morphologyEx(blueMask, blueMask, MORPH_CLOSE, kernel);

    morphologyEx(yellowMask, yellowMask, MORPH_OPEN, kernel);
    morphologyEx(yellowMask, yellowMask, MORPH_CLOSE, kernel);

    vector<vector<Point>> blueContours;
    findContours(blueMask, blueContours, RETR_EXTERNAL, CHAIN_APPROX_SIMPLE);
    for (const auto& contour : blueContours) {
        Rect rect = boundingRect(contour);
        rectangle(image, rect, Scalar(255,255, 255), 2); 
    }
    vector<vector<Point>> yellowContours;
    findContours(yellowMask, yellowContours, RETR_EXTERNAL, CHAIN_APPROX_SIMPLE);
    for (const auto& contour : yellowContours) {
        Rect rect = boundingRect(contour);
        rectangle(image, rect, Scalar(255,255,255), 2);  
    }

    imshow("Detected Colors", image);
    waitKey(0);

    return 0;
}